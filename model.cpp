#include "model.h"

#include <QDebug>
#include <QXmlInputSource>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>

namespace DJ {
	namespace Model {
		Model::Model(QString url, QString username, QString password, QObject *parent) : QObject(parent) {
			this->url = url;
			this->username = username;
			this->password = password;
			this->read = false;
			this->scoreboard = NULL;
			this->manager = new QNetworkAccessManager(this);
			connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finish(QNetworkReply*)));
		}

		Model::~Model() {

		}

		void Model::setUrl(QString url) {
			this->url = url;
			this->read = false;
		}

		void Model::setUsername(QString username) {
			this->username = username;
		}

		void Model::setPassword(QString password) {
			this->password = password;
		}

		void Model::refresh() {
			this->read = false;
			this->readData();
		}

		void Model::readData() {
			if (this->scoreboard) {
				delete this->scoreboard;
				this->scoreboard = NULL;
			}

			QNetworkRequest request;
			QUrl url(this->url + "plugin/scoreboard.php");
			url.setUserName(this->username);
			url.setPassword(this->password);
			request.setUrl(url);
			manager->get(request);
		}

		void Model::finish(QNetworkReply *reply) {
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

		Scoreboard *Model::getScoreboard() {
			if (!this->read) {
				this->readData();
			}
			return this->scoreboard;
		}

		Model::ScoreboardParser::ScoreboardParser() {
			this->scoreboard = new Scoreboard;
			this->parseState = NOT_STARTED;
		}

		Model::ScoreboardParser::~ScoreboardParser() {
			if (this->scoreboard) {
				delete this->scoreboard;
			}
		}

		bool Model::ScoreboardParser::startDocument() {
			return true;
		}

		bool Model::ScoreboardParser::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts) {
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
				Contest *contest = new Contest(start, end, freeze, id, this->scoreboard);
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
				Problem *problem = new Problem(id, color, this->scoreboard);
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
				Language *language = new Language(id, this->scoreboard);
				this->currentItem = (QObject *)language;
			}
			return true;
		}

		bool Model::ScoreboardParser::endElement(const QString &, const QString &, const QString &qName) {
			qDebug() << "end element" << qName;
			if (qName == "contest") {
				Contest *contest = (Contest *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->setContest(contest);
				this->parseState = OUTER_PART;
			} else if (qName == "problem_legend") {
				this->parseState = OUTER_PART;
			} else if (this->parseState == PROBLEM && qName == "problem") {
				Problem *problem = (Problem *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->addProblem(problem);
				this->parseState = PROBLEM_LEGEND;
			} else if (qName == "language_legend") {
				this->parseState = OUTER_PART;
			} else if (this->parseState == LANGUAGE && qName == "language") {
				Language *language = (Language *)this->currentItem;
				this->currentItem = NULL;
				this->scoreboard->addLanguage(language);
				this->parseState = LANGUAGE_LEGEND;
			}
			return true;
		}

		bool Model::ScoreboardParser::characters(const QString &ch) {
			qDebug() << "Characters" << ch;
			if (this->parseState == CONTEST) {
				Contest *contest = (Contest *)this->currentItem;
				contest->setName(ch);
			} else if (this->parseState == PROBLEM) {
				Problem *problem = (Problem *)this->currentItem;
				problem->setName(ch);
			} else if (this->parseState == LANGUAGE) {
				Language *language = (Language *)this->currentItem;
				language->setName(ch);
			}
			return true;
		}

		Scoreboard *Model::ScoreboardParser::getScoreboard() {
			return this->scoreboard;
		}

	}
}
