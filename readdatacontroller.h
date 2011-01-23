#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <QObject>
#include <QXmlDefaultHandler>
#include <QDir>

#include "scoreboard.h"
#include "events.h"

#include <QNetworkAccessManager>

namespace DJ {
namespace Controller {
class ReadDataController : public QObject {
	Q_OBJECT
public:
	explicit ReadDataController(QObject *parent = 0);
	~ReadDataController();

	void setUrl(QString url);
	void setUsername(QString username);
	void setPassword(QString password);
	void setDir(QDir dir);
	void setOfDir(bool ofDir);
	void refresh();
	Model::Scoreboard *getScoreboard();
	Model::Events *getEvents();
	void saveXML(QString dir);
	void setParentOfMessages(QWidget *widget);

private:
	// Class for reading scoreboard XML data
	class ScoreboardParser : public QXmlDefaultHandler {
	public:
		ScoreboardParser();
		~ScoreboardParser();

		bool startDocument();
		bool endElement(const QString &, const QString &, const QString &qName);
		bool startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts);
		bool characters(const QString &ch);
		Model::Scoreboard *getScoreboard();

	private:
		Model::Scoreboard *scoreboard;

		enum Parsestate {
			NOT_STARTED,
			OUTER_PART,
			CONTEST,
			PROBLEM_LEGEND,
			PROBLEM,
			LANGUAGE_LEGEND,
			LANGUAGE,
			AFFILIATION_LEGEND,
			AFFILIATION,
			CATEGORY_LEGEND,
			CATEGORY,
			ROWS,
			ROW,
			TEAM
		};

		Parsestate parseState;
		QObject *currentItem;
	};

	// Class for reading events XML data
	class EventsParser : public QXmlDefaultHandler {
	public:
		EventsParser(Model::Scoreboard *scoreboard);
		~EventsParser();

		bool startDocument();
		bool endElement(const QString &, const QString &, const QString &qName);
		bool startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts);
		bool characters(const QString &ch);
		Model::Events *getEvents();

	private:
		Model::Events *events;
		Model::Scoreboard *scoreboard;

		enum Parsestate {
			NOT_STARTED,
			OUTER_PART,
			EVENTS,
			EVENT,
			SUBMISSION,
			TEAM,
			PROBLEM,
			LANGUAGE,
			JUDGING
		};

		Parsestate parseState;
		QObject *currentItem;
		QString currentEventId;
		QDateTime currentEventTime;
	};

	void readData();

	QString url;
	QString username;
	QString password;
	QDir dir;
	bool read;
	bool ofDir;
	QWidget *parentOfMessages;

	QNetworkAccessManager *manager;

	Model::Scoreboard *scoreboard;
	Model::Events *events;

private slots:
	void finish(QNetworkReply *reply);

signals:
	void dataRead();

};
}
}

#endif // DATACONTROLLER_H
