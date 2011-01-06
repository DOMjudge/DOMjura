#include "datacontroller.h"

#include <QDebug>
#include <QXmlInputSource>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>

namespace DJ {
	namespace Controller {
		DataController::DataController(QString url, QString username, QString password, QObject *parent) : QObject(parent) {
			this->url = url;
			this->username = username;
			this->password = password;
			this->read = false;
			this->scoreboard = NULL;
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

			QNetworkRequest request;
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
				this->read = true;

				qDebug() << this->scoreboard->toString();
			} else {
				QMessageBox::warning(NULL, "Error!", "scoreboard.xml could not be read correctly!");
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
			if (this->scoreboard) {
				delete this->scoreboard;
			}
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
			} else if (this->parseState == CATEGORY && qName == "category") {
				Model::Category *category = (Model::Category *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->addCategory(category);
				this->parseState = CATEGORY_LEGEND;
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
			}
			return true;
		}

		Model::Scoreboard *DataController::ScoreboardParser::getScoreboard() {
			return this->scoreboard;
		}

	}
}
