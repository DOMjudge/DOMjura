/** \file readdatacontroller.h
  * \brief Contains the class to read data.
  */

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
/** Controls the reading of the data.
  */
class ReadDataController : public QObject {
	Q_OBJECT
public:
	/** Constructs a new data read controller.
	  * \param parent The parent of this object.
	  */
	explicit ReadDataController(QObject *parent = 0);
	/** Destructs the controller.
	  */
	~ReadDataController();

	/** Sets the URL to read from.
	  * \param url The URL to read from.
	  */
	void setUrl(QString url);
	/** Sets the username to use for the reading.
	  * \param username The username to use.
	  */
	void setUsername(QString username);
	/** Sets the password to use for the reading.
	  * \param password The password to use.
	  */
	void setPassword(QString password);
	/** Sets the directory to use for the reading.
	  * \param dir The directory to use.
	  */
	void setDir(QDir dir);
	/** Sets whether to use a directory for reading or not.
	  * \param ofDir If true, use directory for reading.
	  * If false, use a URL.
	  */
	void setOfDir(bool ofDir);
	/** Refreshes (rereads) the data.
	  */
	void refresh();
	/** Returns the scoreboard.
	  * \pre The data is read.
	  * \return The scoreboard.
	  */
	Model::Scoreboard *getScoreboard();
	/** Returns the events.
	  * \pre The data is read.
	  * \return The events.
	  */
	Model::Events *getEvents();
	/** Saves the XML of the data.
	  * \param dir The directory to save the XML to.
	  * \pre The data is read.
	  */
	void saveXML(QString dir);
	/** Sets a parent widget to use for the message boxes.
	  * \param widget The parent to use.
	  */
	void setParentOfMessages(QWidget *widget);

private:
	/** Reads scoreboard XML data.
	  */
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

	/** Reads event XML data.
	  */
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
	/** Emitted when the data is read.
	  */
	void dataRead();

};
}
}

#endif // DATACONTROLLER_H
