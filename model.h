#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QXmlDefaultHandler>

#include "scoreboard.h"

#include <QNetworkAccessManager>

namespace DJ {
	namespace Model {
		class Model : public QObject {
			Q_OBJECT
		public:
			explicit Model(QString url, QString username = "", QString password = "", QObject *parent = 0);
			~Model();

			void setUrl(QString url);
			void setUsername(QString username);
			void setPassword(QString password);
			void refresh();
			Scoreboard *getScoreboard();

		private:
			// Class for reading XML data
			class ScoreboardParser : public QXmlDefaultHandler {
			public:
				ScoreboardParser();
				~ScoreboardParser();

				bool startDocument();
				bool endElement(const QString &, const QString &, const QString &qName);
				bool startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &atts);
				bool characters(const QString &ch);
				Scoreboard *getScoreboard();

			private:
				Scoreboard *scoreboard;

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
					CATEGORY
				};

				Parsestate parseState;
				QObject *currentItem;
			};

			void readData();

			QString url;
			QString username;
			QString password;
			bool read;

			QNetworkAccessManager *manager;

			Scoreboard *scoreboard;

		private slots:
			void finish(QNetworkReply *reply);

		};
	}
}

#endif // MODEL_H
