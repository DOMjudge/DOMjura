#ifndef TEAM_H
#define TEAM_H

#include <QObject>

namespace DJ {
	namespace Model {
		class Category;
		class Affiliation;

		class Team : public QObject {
			Q_OBJECT
		public:
			explicit Team(QString id, Category *category, Affiliation *affiliation, QObject *parent = 0);
			void setName(QString name);
			QString getId();

			QString toString();

		private:
			QString id;
			Category *category;
			Affiliation* affiliation;
			QString name;

		};
	}
}

#endif // TEAM_H
