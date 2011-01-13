#include "readdatacontroller.h"
#include <QXmlInputSource>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QXmlStreamWriter>

#include "submissionevent.h"
#include "judgingevent.h"

namespace DJ {
namespace Controller {
ReadDataController::ReadDataController(QObject *parent) : QObject(parent) {
	this->read = false;
	this->manager = new QNetworkAccessManager(this);
	this->scoreboard = NULL;
	this->events = NULL;
	connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finish(QNetworkReply*)));
}

ReadDataController::~ReadDataController() {

}

void ReadDataController::setUrl(QString url) {
	this->url = url;
	this->ofDir = false;
	this->read = false;
}

void ReadDataController::setUsername(QString username) {
	this->username = username;
}

void ReadDataController::setPassword(QString password) {
	this->password = password;
}

void ReadDataController::setDir(QDir dir) {
	this->dir = dir;
}

void ReadDataController::setOfDir(bool ofDir) {
	this->ofDir = ofDir;
}

void ReadDataController::refresh() {
	this->read = false;
	this->readData();
}

void ReadDataController::readData() {
	if (this->scoreboard) {
		delete this->scoreboard;
		this->scoreboard = NULL;
	}
	if (this->events) {
		delete this->events;
		this->events = NULL;
	}

	if (this->ofDir) {
		ScoreboardParser parser;
		QFile scoreboardFile(dir.filePath("scoreboard.xml"));
		if (!scoreboardFile.open(QIODevice::ReadOnly)) {
			QMessageBox::warning(NULL, "Error!", QString("Error reading scoreboard.xml!\n\nError was:\n\%1\"").arg(scoreboardFile.errorString()));
			return;
		}
		QXmlInputSource source(&scoreboardFile);
		QXmlSimpleReader reader;
		reader.setContentHandler(&parser);
		reader.setErrorHandler(&parser);

		if (reader.parse(&source)) {
			this->scoreboard = parser.getScoreboard();
			this->scoreboard->updateTeamRefs();

			EventsParser eventsParser(this->scoreboard);
			QFile eventsFile(dir.filePath("event.xml"));
			if (!eventsFile.open(QIODevice::ReadOnly)) {
				QMessageBox::warning(NULL, "Error!", QString("Error reading event.xml!\n\nError was:\n\"%1\"").arg(eventsFile.errorString()));
				return;
			}
			QXmlInputSource eventsSource(&eventsFile);
			QXmlSimpleReader eventsReader;
			eventsReader.setContentHandler(&eventsParser);
			eventsReader.setErrorHandler(&eventsParser);

			if (eventsReader.parse(&eventsSource)) {
				this->read = true;
				this->events = eventsParser.getEvents();
				this->events->sort();
				emit dataRead();
			} else {
				QMessageBox::warning(NULL, "Error", QString("Can not parse event.xml!\nError was:\n\"%1\"").arg(parser.errorString()));
			}
		} else {
			QMessageBox::warning(NULL, "Error", QString("Can not parse scoreboard.xml!\nError was:\n\"%1\"").arg(parser.errorString()));
		}
	} else {
		QNetworkRequest request;
		request.setRawHeader("what", "scoreboard");
		QUrl url(this->url + "/plugin/scoreboard.php");
		if (!this->username.isEmpty()) {
			url.setUserName(this->username);
			url.setPassword(this->password);
		}
		request.setUrl(url);
		manager->get(request);
	}
}

void ReadDataController::finish(QNetworkReply *reply) {
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
			QNetworkRequest request;
			request.setRawHeader("what", "event");
			QUrl url(this->url + "/plugin/event.php");
			if (!this->username.isEmpty()) {
				url.setUserName(this->username);
				url.setPassword(this->password);
			}
			request.setUrl(url);
			manager->get(request);
		} else {
			QMessageBox::warning(NULL, "Error", QString("Can not parse scoreboard.php!\nError was:\n\"%1\"").arg(parser.errorString()));
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
			this->events->sort();
			this->read = true;
			emit dataRead();
		} else {
			QMessageBox::warning(NULL, "Error", QString("Can not parse event.xml!\nError was:\n\"%1\"").arg(parser.errorString()));
		}
	}
}

Model::Scoreboard *ReadDataController::getScoreboard() {
	if (!this->read) {
		this->readData();
		return NULL;
	}
	return this->scoreboard;
}

Model::Events *ReadDataController::getEvents() {
	if (!this->read) {
		this->readData();
		return NULL;
	}
	return this->events;
}

void ReadDataController::saveXML(QString dir) {
	// Save xml files to "dir"
	if (dir.at(dir.size()-1) != '/') {
		dir += "/";
	}
	QFile scorebaordXMLfile(dir + "scoreboard.xml");
	if (!scorebaordXMLfile.open(QIODevice::WriteOnly)) {
		QMessageBox::warning(NULL, "Error!", "Can not open scoreboard.xml for writing!");
	}
	QXmlStreamWriter scoreboardXMLWriter(&scorebaordXMLfile);
	scoreboardXMLWriter.setAutoFormatting(true);
	scoreboardXMLWriter.setAutoFormattingIndent(-1);

	scoreboardXMLWriter.writeStartDocument();
	scoreboardXMLWriter.writeStartElement("root");
	scoreboardXMLWriter.writeStartElement("scoreboard");
	scoreboardXMLWriter.writeStartElement("contest");
	scoreboardXMLWriter.writeAttribute("id", QString::number(this->scoreboard->getContest()->getId()));
	scoreboardXMLWriter.writeAttribute("start", this->scoreboard->getContest()->getStart().toString("yyyy-MM-dd hh:mm:ss"));
	scoreboardXMLWriter.writeAttribute("end", this->scoreboard->getContest()->getEnd().toString("yyyy-MM-dd hh:mm:ss"));
	scoreboardXMLWriter.writeAttribute("freeze", this->scoreboard->getContest()->getFreeze().toString("yyyy-MM-dd hh:mm:ss"));
	scoreboardXMLWriter.writeCharacters(this->scoreboard->getContest()->getName());
	scoreboardXMLWriter.writeEndElement(); // contest

	scoreboardXMLWriter.writeStartElement("rows");
	for (int i = 0; i < this->scoreboard->getNumTeams(); i++) {
		scoreboardXMLWriter.writeStartElement("row");
		// Rank attribute ignored
		scoreboardXMLWriter.writeStartElement("team");
		scoreboardXMLWriter.writeAttribute("id", this->scoreboard->getTeam(i)->getId());
		Model::Category *category = this->scoreboard->getTeam(i)->getCategory();
		scoreboardXMLWriter.writeAttribute("categoryid", category ? category->getId() : "");
		Model::Affiliation *affiliation = this->scoreboard->getTeam(i)->getAffiliation();
		scoreboardXMLWriter.writeAttribute("affillid", affiliation ? affiliation->getId() : "");
		// Country ignored (can be found using affiliation)
		scoreboardXMLWriter.writeCharacters(this->scoreboard->getTeam(i)->getName());
		scoreboardXMLWriter.writeEndElement(); // team
		// num_solved ignored
		// totaltime ignored
		// problems ignored
		scoreboardXMLWriter.writeEndElement(); // row
	}
	scoreboardXMLWriter.writeEndElement(); // rows
	// summary ignored
	scoreboardXMLWriter.writeStartElement("problem_legend");
	for (int i = 0; i < this->scoreboard->getNumProblems(); i++) {
		Model::Problem *problem = this->scoreboard->getProblem(i);
		scoreboardXMLWriter.writeStartElement("problem");
		scoreboardXMLWriter.writeAttribute("id", problem->getId());
		scoreboardXMLWriter.writeAttribute("color", problem->getColor().name());
		scoreboardXMLWriter.writeCharacters(problem->getName());
		scoreboardXMLWriter.writeEndElement(); // problem
	}
	scoreboardXMLWriter.writeEndElement(); // problem_legend

	scoreboardXMLWriter.writeStartElement("language_legend");
	for (int i = 0; i < this->scoreboard->getNumLanguages(); i++) {
		Model::Language *language = this->scoreboard->getLanguage(i);
		scoreboardXMLWriter.writeStartElement("language");
		scoreboardXMLWriter.writeAttribute("id", language->getId());
		scoreboardXMLWriter.writeCharacters(language->getName());
		scoreboardXMLWriter.writeEndElement(); // language
	}
	scoreboardXMLWriter.writeEndElement(); // language_legend

	scoreboardXMLWriter.writeStartElement("affiliation_legend");
	for (int i = 0; i < this->scoreboard->getNumAffiliations(); i++) {
		Model::Affiliation *affiliation = this->scoreboard->getAffiliation(i);
		scoreboardXMLWriter.writeStartElement("affiliation");
		scoreboardXMLWriter.writeAttribute("id", affiliation->getId());
		scoreboardXMLWriter.writeAttribute("country", affiliation->getCountry());
		scoreboardXMLWriter.writeCharacters(affiliation->getName());
		scoreboardXMLWriter.writeEndElement(); // affiliation
	}
	scoreboardXMLWriter.writeEndElement(); // affiliation_legend

	scoreboardXMLWriter.writeStartElement("category_legend");
	for (int i = 0; i < this->scoreboard->getNumCategories(); i++) {
		Model::Category *category = this->scoreboard->getCategory(i);
		scoreboardXMLWriter.writeStartElement("category");
		scoreboardXMLWriter.writeAttribute("id", category->getId());
		scoreboardXMLWriter.writeAttribute("color", category->getColor().name());
		scoreboardXMLWriter.writeCharacters(category->getName());
		scoreboardXMLWriter.writeEndElement(); // category
	}
	scoreboardXMLWriter.writeEndElement(); // category_legend

	scoreboardXMLWriter.writeEndElement(); // scorebaord
	scoreboardXMLWriter.writeEndElement(); // root
	scoreboardXMLWriter.writeEndDocument();

	scorebaordXMLfile.close();

	QFile eventXMLfile(dir + "event.xml");
	if (!eventXMLfile.open(QIODevice::WriteOnly)) {
		QMessageBox::warning(NULL, "Error!", "Can not open event.xml for writing!");
	}
	QXmlStreamWriter eventXMLWriter(&eventXMLfile);
	eventXMLWriter.setAutoFormatting(true);
	eventXMLWriter.setAutoFormattingIndent(-1);

	eventXMLWriter.writeStartDocument();

	eventXMLWriter.writeStartElement("root");
	eventXMLWriter.writeStartElement("events");
	for (int i = 0; i < this->events->getNumEvents(); i++) {
		Model::Event *event = this->events->getEvent(i);
		eventXMLWriter.writeStartElement("event");
		eventXMLWriter.writeAttribute("id", event->getId());
		eventXMLWriter.writeAttribute("time", event->getDateTime().toString("yyyy-MM-dd hh:mm:ss"));
		if (event->getType() == Model::SUBMISSIONEVENT) {
			Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)event;
			eventXMLWriter.writeStartElement("submission");
			eventXMLWriter.writeAttribute("id", submissionEvent->getSubmissionId());

			eventXMLWriter.writeStartElement("team");
			eventXMLWriter.writeAttribute("id", submissionEvent->getTeam()->getId());
			eventXMLWriter.writeCharacters(submissionEvent->getTeam()->getName());
			eventXMLWriter.writeEndElement(); // team

			eventXMLWriter.writeStartElement("problem");
			eventXMLWriter.writeAttribute("id", submissionEvent->getProblem()->getId());
			eventXMLWriter.writeCharacters(submissionEvent->getProblem()->getName());
			eventXMLWriter.writeEndElement(); // problem

			eventXMLWriter.writeStartElement("language");
			eventXMLWriter.writeAttribute("id", submissionEvent->getLangugage()->getId());
			eventXMLWriter.writeCharacters(submissionEvent->getLangugage()->getName());
			eventXMLWriter.writeEndElement(); // language

			eventXMLWriter.writeEndElement(); // submission
		} else if (event->getType() == Model::JUDGINGEVENT) {
			Model::JudgingEvent *judgingEvent = (Model::JudgingEvent *)event;
			Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)judgingEvent->getSubmissionEvent();
			eventXMLWriter.writeStartElement("judging");
			eventXMLWriter.writeAttribute("id", judgingEvent->getJudgingId());
			eventXMLWriter.writeAttribute("submitid", submissionEvent->getSubmissionId());

			eventXMLWriter.writeCharacters(judgingEvent->getResult());

			eventXMLWriter.writeEndElement(); // judging
		} // else ignore (other events)
		eventXMLWriter.writeEndElement(); // event
	}
	eventXMLWriter.writeEndElement(); // events
	eventXMLWriter.writeEndElement(); // root

	eventXMLWriter.writeEndDocument();

	eventXMLfile.close();

	QMessageBox::information(NULL, "Success!", "Files saved successfully");
}

ReadDataController::ScoreboardParser::ScoreboardParser() {
	this->scoreboard = new Model::Scoreboard;
	this->parseState = NOT_STARTED;
}

ReadDataController::ScoreboardParser::~ScoreboardParser() {
}

bool ReadDataController::ScoreboardParser::startDocument() {
	return true;
}

bool ReadDataController::ScoreboardParser::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts) {
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

bool ReadDataController::ScoreboardParser::endElement(const QString &, const QString &, const QString &qName) {
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

bool ReadDataController::ScoreboardParser::characters(const QString &ch) {
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

Model::Scoreboard *ReadDataController::ScoreboardParser::getScoreboard() {
	return this->scoreboard;
}

ReadDataController::EventsParser::EventsParser(Model::Scoreboard *scoreboard) {
	this->scoreboard = scoreboard;
	this->events = new Model::Events;
	this->parseState = NOT_STARTED;
}

ReadDataController::EventsParser::~EventsParser() {
}

bool ReadDataController::EventsParser::startDocument() {
	return true;
}

bool ReadDataController::EventsParser::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts) {
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
		Model::Event *event = new Model::SubmissionEvent(this->currentEventId, this->currentEventTime, submissionId, this->scoreboard->getContest()->getFreeze().isValid() && this->currentEventTime >= this->scoreboard->getContest()->getFreeze(), this->events);
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

bool ReadDataController::EventsParser::endElement(const QString &, const QString &, const QString &qName) {
	if (qName == "events") {
		this->parseState = OUTER_PART;
	} else if (qName == "event") {
		Model::Event *event = (Model::Event *)this->currentItem;
		this->currentItem = NULL;
		this->parseState = EVENTS;
		if (event->getType() == Model::SUBMISSIONEVENT) {
			// Check if team exists
			Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)event;
			if (submissionEvent->getTeam()) {
				this->events->addEvent(event);
			}
		} else if (event->getType() == Model::JUDGINGEVENT) {
			// Check if team exists
			Model::JudgingEvent *judgingEvent = (Model::JudgingEvent *)event;
			Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)judgingEvent->getSubmissionEvent();
			if (submissionEvent && submissionEvent->getTeam()) {
				this->events->addEvent(event);
			}
		}
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

bool ReadDataController::EventsParser::characters(const QString &ch) {
	if (this->parseState == JUDGING) {
		Model::JudgingEvent *event = (Model::JudgingEvent *)this->currentItem;
		event->setCorrect(ch == "correct");
		event->setResult(ch);
	}
	return true;
}

Model::Events *ReadDataController::EventsParser::getEvents() {
	return this->events;
}

}
}
