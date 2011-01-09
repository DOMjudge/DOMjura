#ifndef TEAM_H
#define TEAM_H

#include <QObject>

namespace DJ {
namespace Model {
class Scoreboard;
class Category;
class Affiliation;

class Team : public QObject {
	Q_OBJECT
public:
	explicit Team(QString id, QString categoryid, QString affiliationid, QObject *parent = 0);
	void updateIds(Scoreboard *scoreboard);

	void setName(QString name);
	QString getName();
	QString getId();
	Category *getCategory();

	QString toString();

private:
	QString id;
	QString categoryid;
	Category *category;
	QString affiliationid;
	Affiliation* affiliation;
	QString name;

};
}
}

#endif // TEAM_H
