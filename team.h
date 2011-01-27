/** \file team.h
  * \brief Contains the class for a team.
  */

#ifndef TEAM_H
#define TEAM_H

#include <QObject>

namespace DJ {
namespace Model {
class Scoreboard;
class Category;
class Affiliation;

/** A team (from the scoreboard).
  */
class Team : public QObject {
	Q_OBJECT
public:
	/** Constructs a new team.
	  * \param id The unique ID of this team.
	  * \param categoryid The ID of the category.
	  * \param affiliationid The ID of the affiliation.
	  * \param parent The parent of this object.
	  */
	explicit Team(QString id, QString categoryid, QString affiliationid, QObject *parent = 0);
	/** Updates the references (affiliation and category) of this team.
	  * \param scoreboard The scoreboard to fetch the information from.
	  */
	void updateIds(Scoreboard *scoreboard);

	/** Sets the name of this team.
	  * \param name The name to set.
	  */
	void setName(QString name);
	/** Returns the name of this team.
	  * \return The name of this team.
	  */
	QString getName();
	/** Returns the ID of this team.
	  * \return The ID of this team.
	  */
	QString getId();
	/** Returns the category of this team.
	  * \return The category of this team.
	  */
	Category *getCategory();
	/** Returns the affiliation of this team.
	  * \return The affiliation of this team.
	  */
	Affiliation *getAffiliation();

	/** Returns a string representing this team.
	  * \return A string representation of this team.
	  * Useful for debug printing.
	  */
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
