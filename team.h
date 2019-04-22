/** \file team.h
  * \brief Contains the class for a team.
  */

#ifndef TEAM_H
#define TEAM_H

#include <QObject>

namespace DJ {
namespace Model {

/** A team (from the API).
  */
class Team : public QObject {
	Q_OBJECT
public:
	/** Constructs a new team.
	  * \param contest The team as returned from the DOMjudge API
	  * \param parent The parent of this object.
	  */
    explicit Team(QJsonObject team, QObject *parent = 0);

	/** Returns the name of this team.
	  * \return The name of this team.
	  */
	QString getName();
	/** Returns the ID of this team.
	  * \return The ID of this team.
	  */
    QString getId();

    /** Returns the nationality of this team.
      * \return The nationality of this team.
	  */
    QString getNationality();

    /** Returns the affilation of this team.
      * \return The affilation of this team.
      */
    QString getAffilation();

	/** Returns a string representing this team.
	  * \return A string representation of this team.
	  * Useful for debug printing.
	  */
	QString toString();

private:
    QString id;
	QString name;
    QString affilation;
    QString nationality;

};
}
}

#endif // TEAM_H
