/** \file language.h
  * \brief Contains a class for a language.
  */

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>

namespace DJ {
namespace Model {
/** A language (from the scoreboard).
  */
class Language : public QObject {
	Q_OBJECT
public:
	/** Constructs a new language.
	  * \param id The unique ID of this language.
	  * \param parent The parent of this object.
	  */
	explicit Language(QString id, QObject *parent = 0);
	/** Sets the name of this language.
	  * \param name The name to set.
	  */
	void setName(QString name);
	/** Returns the ID of this language.
	  * \return The ID of this language.
	  */
	QString getId();
	/** Returns the name of this language.
	  * \return The name of this language.
	  */
	QString getName();
	/** Returns a string representing this language.
	  * \return A string representation of this language.
	  * Useful for debug printing.
	  */
	QString toString();

private:
	QString id;
	QString name;
};
}
}

#endif // LANGUAGE_H
