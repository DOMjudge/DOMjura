/** \file team.h
  * \brief Contains the class for a team.
  */

#ifndef TEAM_H
#define TEAM_H

#include <QObject>

namespace DJ {
namespace Model {
class Category;

/** A team (from the API).
  */
class Team : public QObject {
	Q_OBJECT
public:
	/** Constructs a new team.
	  * \param contest The team as returned from the DOMjudge API
	  * \param parent The parent of this object.
	  */
	explicit Team(QJsonObject team, QHash<int, Category *> categories, QObject *parent = 0);

	/** Returns the name of this team.
	  * \return The name of this team.
	  */
	QString getName();
	/** Returns the ID of this team.
	  * \return The ID of this team.
	  */
	int getId();
	/** Returns the category of this team.
	  * \return The category of this team.
	  */
	Category *getCategory();

	/** Returns a string representing this team.
	  * \return A string representation of this team.
	  * Useful for debug printing.
	  */
	QString toString();

private:
	int id;
	Category *category;
	QString name;

};
}
}

#endif // TEAM_H
