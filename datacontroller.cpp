#include "datacontroller.h"

#include <QDebug>
#include <QXmlInputSource>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>

#include "submissionevent.h"
#include "judgingevent.h"

namespace DJ {
	namespace Controller {
		DataController::DataController(QString url, QString username, QString password, QObject *parent) : QObject(parent) {
			this->url = url;
			this->username = username;
			this->password = password;
			this->read = false;
			this->scoreboard = NULL;
			this->events = NULL;
			this->manager = new QNetworkAccessManager(this);
			connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finish(QNetworkReply*)));
		}

		DataController::~DataController() {

		}

		void DataController::setUrl(QString url) {
			this->url = url;
			this->read = false;
		}

		void DataController::setUsername(QString username) {
			this->username = username;
		}

		void DataController::setPassword(QString password) {
			this->password = password;
		}

		void DataController::refresh() {
			this->read = false;
			this->readData();
		}

		void DataController::readData() {
			if (this->scoreboard) {
				delete this->scoreboard;
				this->scoreboard = NULL;
			}
			if (this->events) {
				delete this->events;
				this->events = NULL;
			}

			QNetworkRequest request;
			request.setRawHeader("what", "scoreboard");
			QUrl url(this->url + "plugin/scoreboard.php");
			if (!this->username.isEmpty()) {
				url.setUserName(this->username);
				url.setPassword(this->password);
			}
			request.setUrl(url);
			manager->get(request);
		}

		void DataController::finish(QNetworkReply *reply) {
			reply->deleteLater();
			if (reply->request().rawHeader("what") == "scoreboard") {
				if (reply->error()) {
					QMessageBox::warning(NULL, "Error!", QString("Error reading url\n%1/plugin/scoreboard.php!\n\nError was:\n\"%2\"").arg(this->url).arg(reply->errorString()));
					return;
				}
				ScoreboardParser parser;

				QXmlInputSource source;
				source.setData(reply->readAll());
				QXmlSimpleReader reader;
				reader.setContentHandler(&parser);
				reader.setErrorHandler(&parser);

				if (reader.parse(&source)) {
					this->scoreboard = parser.getScoreboard();
					this->scoreboard->updateTeamRefs();

					qDebug() << this->scoreboard->toString();

					QNetworkRequest request;
					request.setRawHeader("what", "event");
					QUrl url(this->url + "plugin/event.php");
					if (!this->username.isEmpty()) {
						url.setUserName(this->username);
						url.setPassword(this->password);
					}
					request.setUrl(url);
					manager->get(request);
				} else {
					QMessageBox::warning(NULL, "Error!", "scoreboard.php could not be read correctly!");
				}
			} else if (reply->request().rawHeader("what") == "event") {
				if (reply->error()) {
					QMessageBox::warning(NULL, "Error!", QString("Error reading url\n%1/plugin/event.php!\n\nError was:\n\"%2\"").arg(this->url).arg(reply->errorString()));
					return;
				}
				EventsParser parser(this->scoreboard);

				QXmlInputSource source;
				source.setData(reply->readAll());
				QXmlSimpleReader reader;
				reader.setContentHandler(&parser);
				reader.setErrorHandler(&parser);

				if (reader.parse(&source)) {
					this->events = parser.getEvents();
					this->read = true;

					qDebug() << this->events->toString();
				} else {
					QMessageBox::warning(NULL, "Error!", "event.php could not be read correctly!");
				}
			}
		}

		Model::Scoreboard *DataController::getScoreboard() {
			if (!this->read) {
				this->readData();
			}
			return this->scoreboard;
		}

		DataController::ScoreboardParser::ScoreboardParser() {
			this->scoreboard = new Model::Scoreboard;
			this->parseState = NOT_STARTED;
		}

		DataController::ScoreboardParser::~ScoreboardParser() {
		}

		bool DataController::ScoreboardParser::startDocument() {
			return true;
		}

		bool DataController::ScoreboardParser::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts) {
			qDebug() << "start element:" << qName;
			if (qName == "contest") {
				this->parseState = CONTEST;
				QDateTime start, end, freeze;
				int id;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						id = atts.value(i).toInt();
					} else if (atts.localName(i) == "start") {
						start = QDateTime::fromString(atts.value(i), "yyyy-MM-dd hh:mm:ss");
					} else if (atts.localName(i) == "end") {
						end = QDateTime::fromString(atts.value(i), "yyyy-MM-dd hh:mm:ss");
					} else if (atts.localName(i) == "freeze") {
						freeze = QDateTime::fromString(atts.value(i), "yyyy-MM-dd hh:mm:ss");
					}
				}
				Model::Contest *contest = new Model::Contest(start, end, freeze, id, this->scoreboard);
				this->currentItem = (QObject *)contest;
			} else if (qName == "problem_legend") {
				this->parseState = PROBLEM_LEGEND;
			} else if (this->parseState == PROBLEM_LEGEND && qName == "problem") {
				this->parseState = PROBLEM;
				QString id;
				QColor color;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						id = atts.value(i);
					} else if (atts.localName(i) == "color") {
						color.setNamedColor(atts.value(i));
					}
				}
				Model::Problem *problem = new Model::Problem(id, color, this->scoreboard);
				this->currentItem = (QObject *)problem;
			} else if (qName == "language_legend") {
				this->parseState = LANGUAGE_LEGEND;
			} else if (this->parseState == LANGUAGE_LEGEND && qName == "language") {
				this->parseState = LANGUAGE;
				QString id;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						id = atts.value(i);
					}
				}
				Model::Language *language = new Model::Language(id, this->scoreboard);
				this->currentItem = (QObject *)language;
			} else if (qName == "affiliation_legend") {
				this->parseState = AFFILIATION_LEGEND;
			} else if (this->parseState == AFFILIATION_LEGEND && qName == "affiliation") {
				this->parseState = AFFILIATION;
				QString id, country;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						id = atts.value(i);
					} else if (atts.localName(i) == "country") {
						country = atts.value(i);
					}
				}
				Model::Affiliation *affiliation = new Model::Affiliation(id, country, this->scoreboard);
				this->currentItem = (QObject *)affiliation;
			} else if (qName == "category_legend") {
				this->parseState = CATEGORY_LEGEND;
			} else if (this->parseState == CATEGORY_LEGEND && qName == "category") {
				this->parseState = CATEGORY;
				QString id;
				QColor color;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						id = atts.value(i);
					} else if (atts.localName(i) == "color") {
						color.setNamedColor(atts.value(i).isEmpty() ? "white" : atts.value(i));
					}
				}
				Model::Category *category = new Model::Category(id, color, this->scoreboard);
				this->currentItem = (QObject *)category;
			} else if (qName == "rows") {
				this->parseState = ROWS;
			} else if (this->parseState == ROWS && qName == "row") {
				this->parseState = ROW;
			} else if (this->parseState == ROW && qName == "team") {
				this->parseState = TEAM;
				QString id;
				QString categoryid;
				QString affiliationid;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						id = atts.value(i);
					} else if (atts.localName(i) == "categoryid") {
						categoryid = atts.value(i);
					} else if (atts.localName(i) == "affilid") {
						affiliationid = atts.value(i);
					}
				}
				Model::Team *team = new Model::Team(id, categoryid, affiliationid, this->scoreboard);
				this->currentItem = (QObject *)team;
			}
			return true;
		}

		bool DataController::ScoreboardParser::endElement(const QString &, const QString &, const QString &qName) {
			qDebug() << "end element" << qName;
			if (qName == "contest") {
				Model::Contest *contest = (Model::Contest *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->setContest(contest);
				this->parseState = OUTER_PART;
			} else if (qName == "problem_legend") {
				this->parseState = OUTER_PART;
			} else if (this->parseState == PROBLEM && qName == "problem") {
				Model::Problem *problem = (Model::Problem *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->addProblem(problem);
				this->parseState = PROBLEM_LEGEND;
			} else if (qName == "language_legend") {
				this->parseState = OUTER_PART;
			} else if (this->parseState == LANGUAGE && qName == "language") {
				Model::Language *language = (Model::Language *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->addLanguage(language);
				this->parseState = LANGUAGE_LEGEND;
			} else if (qName == "affiliation_legend") {
				this->parseState = OUTER_PART;
			} else if (this->parseState == AFFILIATION && qName == "affiliation") {
				Model::Affiliation *affiliation = (Model::Affiliation *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->addAffiliation(affiliation);
				this->parseState = AFFILIATION_LEGEND;
			} else if (qName == "category_legend") {
				this->parseState = OUTER_PART;
			} else if (this->parseState == CATEGORY && qName == "category") {
				Model::Category *category = (Model::Category *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->addCategory(category);
				this->parseState = CATEGORY_LEGEND;
			} else if (qName == "rows") {
				this->parseState = OUTER_PART;
			} else if (this->parseState == ROW && qName == "row") {
				this->parseState = ROWS;
			} else if (this->parseState == TEAM && qName == "team") {
				Model::Team *team = (Model::Team *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->addTeam(team);
				this->parseState = ROW;
			}
			return true;
		}

		bool DataController::ScoreboardParser::characters(const QString &ch) {
			qDebug() << "Characters" << ch;
			if (this->parseState == CONTEST) {
				Model::Contest *contest = (Model::Contest *)this->currentItem;
				contest->setName(ch);
			} else if (this->parseState == PROBLEM) {
				Model::Problem *problem = (Model::Problem *)this->currentItem;
				problem->setName(ch);
			} else if (this->parseState == LANGUAGE) {
				Model::Language *language = (Model::Language *)this->currentItem;
				language->setName(ch);
			} else if (this->parseState == AFFILIATION) {
				Model::Affiliation *affiliation = (Model::Affiliation *)this->currentItem;
				affiliation->setName(ch);
			} else if (this->parseState == CATEGORY) {
				Model::Category *category = (Model::Category *)this->currentItem;
				category->setName(ch);
			} else if (this->parseState == TEAM) {
				Model::Team *team = (Model::Team *)this->currentItem;
				team->setName(ch);
			}
			return true;
		}

		Model::Scoreboard *DataController::ScoreboardParser::getScoreboard() {
			return this->scoreboard;
		}

		DataController::EventsParser::EventsParser(Model::Scoreboard *scoreboard) {
			this->scoreboard = scoreboard;
			this->events = new Model::Events;
			this->parseState = NOT_STARTED;
		}

		DataController::EventsParser::~EventsParser() {
		}

		bool DataController::EventsParser::startDocument() {
			return true;
		}

		bool DataController::EventsParser::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts) {
			qDebug() << "start element:" << qName;
			if (qName == "events") {
				this->parseState = EVENTS;
			} else if (qName == "event") {
				this->parseState = EVENT;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						this->currentEventId = atts.value(i);
					} else if (atts.localName(i) == "time") {
						this->currentEventTime = QDateTime::fromString(atts.value(i), "yyyy-MM-dd hh:mm:ss");
					}
				}
			} else if (qName == "submission") {
				this->parseState = SUBMISSION;
				QString submissionId;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						submissionId = atts.value(i);
					}
				}
				Model::Event *event = new Model::SubmissionEvent(this->currentEventId, this->currentEventTime, submissionId, this->events);
				this->currentItem = (QObject *)event;
			} else if (this->parseState == SUBMISSION && qName == "team") {
				this->parseState = TEAM;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						Model::Team *team = this->scoreboard->getTeamById(atts.value(i));
						Model::SubmissionEvent *event = (Model::SubmissionEvent *)this->currentItem;
						event->setTeam(team);
					}
				}
			} else if (this->parseState == SUBMISSION && qName == "problem") {
				this->parseState = PROBLEM;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						Model::Problem *problem = this->scoreboard->getProblemById(atts.value(i));
						Model::SubmissionEvent *event = (Model::SubmissionEvent *)this->currentItem;
						event->setProblem(problem);
					}
				}
			} else if (this->parseState == SUBMISSION && qName == "language") {
				this->parseState = LANGUAGE;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						Model::Language *language = this->scoreboard->getLanguageById(atts.value(i));
						qDebug() << language->getId();
						Model::SubmissionEvent *event = (Model::SubmissionEvent *)this->currentItem;
						event->setLanguage(language);
					}
				}
			} else if (this->parseState == EVENT && qName == "judging") {
				this->parseState = JUDGING;
				QString judgingId;
				Model::Event *event;
				for (int i = 0; i < atts.count(); i++) {
					if (atts.localName(i) == "id") {
						judgingId = atts.value(i);
					} else if (atts.localName(i) == "submitid") {
						event = this->events->eventBySubmitId(atts.value(i));
					}
				}
				Model::JudgingEvent *judgingEvent = new Model::JudgingEvent(this->currentEventId, this->currentEventTime, judgingId, event, this->events);
				this->currentItem = (QObject *)judgingEvent;
			}
			return true;
		}

		bool DataController::EventsParser::endElement(const QString &, const QString &, const QString &qName) {
			qDebug() << "end element" << qName;
			if (qName == "events") {
				this->parseState = OUTER_PART;
			} else if (qName == "event") {
				Model::Event *event = (Model::Event *)this->currentItem;
				this->currentItem = NULL;
				this->events->addEvent(event);
				this->parseState = EVENTS;
			} else if (this->parseState == SUBMISSION && qName == "submission") {
				this->parseState = EVENT;
			} else if (this->parseState == TEAM && qName == "team") {
				this->parseState = SUBMISSION;
			} else if (this->parseState == PROBLEM && qName == "problem") {
				this->parseState = SUBMISSION;
			} else if (this->parseState == LANGUAGE && qName == "language") {
				this->parseState = SUBMISSION;
			} else if (this->parseState == JUDGING && qName == "judging") {
				this->parseState = EVENT;
			}
			return true;
		}

		bool DataController::EventsParser::characters(const QString &ch) {
			qDebug() << "Characters" << ch;
			if (this->parseState == JUDGING) {
				Model::JudgingEvent *event = (Model::JudgingEvent *)this->currentItem;
				event->setCorrect(ch == "correct");
			}
			return true;
		}

		Model::Events *DataController::EventsParser::getEvents() {
			return this->events;
		}

	}
}
