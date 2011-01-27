/** \file contest.h
  * \brief Contains a class for a contest.
  */

#ifndef CONTEST_H
#define CONTEST_H

#include <QObject>
#include <QDateTime>

namespace DJ {
namespace Model {
/** A contest (form the scorebaord).
  */
class Contest : public QObject {
	Q_OBJECT
public:
	/** Constructs a new contest.
	  * \param start The start time of this contest.
	  * \param start The end time of this contest.
	  * \param start The freeze time of this contest.
	  * \param id The ID of this contest (must be unique).
	  * \param parent The parent of this object.
	  */
	explicit Contest(QDateTime start, QDateTime end, QDateTime freeze, int id, QObject *parent = 0);
	/** Destructs this contest.
	  */
	~Contest();

	/** Sets the name for this contest.
	  * \param name The name to set.
	  */
	void setName(QString name);

	/** Returns the start time of the contest.
	  * \return The start time of the contest.
	  */
	QDateTime getStart();
	/** Returns the freeze time of the contest.
	  * \return The freeze time of the contest.
	  */
	QDateTime getFreeze();
	/** Returns the end time of the contest.
	  * \return The end time of the contest.
	  */
	QDateTime getEnd();
	/** Returns the ID of this contest.
	  * \return The ID of this contest.
	  */
	int getId();
	/** Returns the name of this contest.
	  * \return The name of this contest.
	  */
	QString getName();

	/** Returns a string representing this contest.
	  * \return A string representation of this contest.
	  * Useful for debug printing.
	  */
	QString toString();

private:
	QString name;
	QDateTime start, end, freeze;
	int id;
};
}
}
#endif // CONTEST_H
