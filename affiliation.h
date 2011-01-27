/** \file affiliation.h
  * \brief Contains a class for an affiliation.
  */

#ifndef AFFILIATION_H
#define AFFILIATION_H

#include <QObject>

namespace DJ {
namespace Model {
/** An affiliation (from the scoreboard).
  */
class Affiliation : public QObject {
	Q_OBJECT
public:
	/** Constructs a new affiliation.
	  * \param id The id of this affiliation (must be unique).
	  * \param country The country of this affiliation.
	  * \param parent The parent of this object.
	  */
	explicit Affiliation(QString id, QString country, QObject *parent = 0);
	/** Sets the name of this affiliation.
	  * \param name The name to set.
	  */
	void setName(QString name);
	/** Returns the name of this affiliation.
	  * \return The name of this affiliation.
	  */
	QString getName();
	/** Returns the ID of this affiliation.
	  * \return The ID of this affiliation.
	  */
	QString getId();
	/** Returns the country of this affiliation.
	  * \return The country of this affiliation.
	  */
	QString getCountry();
	/** Returns a string representing this affiliation.
	  * \return A string representation of this affiliation.
	  * Useful for debug printing.
	  */
	QString toString();

private:
	QString id;
	QString country;
	QString name;
};
}
}


#endif // AFFILIATION_H
