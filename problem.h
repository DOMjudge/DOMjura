/** \file problem.h
  * \brief Contains the class for a problem.
  */

#ifndef PROBLEM_H
#define PROBLEM_H

#include <QObject>
#include <QColor>

namespace DJ {
namespace Model {
/** A problem (from the scoreboard).
  */
class Problem : public QObject {
	Q_OBJECT
public:
	/** Constructs a new problem.
	  * \param id The unique ID of this problem.
	  * \param color The color for this problem.
	  * \param parent The parent of this object.
	  */
	explicit Problem(QString id, QColor color, QObject *parent = 0);
	/** Sets the name of this problem.
	  * \param name The name to set.
	  */
	void setName(QString name);
	/** Returns the ID of this problem.
	  * \return The ID of this problem.
	  */
	QString getId();
	/** Returns the name of this problem.
	  * \return The name of this problem.
	  */
	QString getName();
	/** Returns the color of this problem.
	  * \return The color of this problem.
	  */
	QColor getColor();
	/** Returns a string representing this problem.
	  * \return A string representation of this problem.
	  * Useful for debug printing.
	  */
	QString toString();

private:
	QString id;
	QColor color;
	QString name;
};
}
}

#endif // PROBLEM_H
