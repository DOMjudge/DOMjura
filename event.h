/** \file event.h
  * \brief Contains event related data.
  */

#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QDateTime>

namespace DJ {
namespace Model {
class Scoreboard;
/** The kind of events that this application used.
  */
enum EventType {
	UNKNOWN, /**< An unknown event (thus not used by this application). */
	JUDGINGEVENT, /**< A judging event (i.e. a submission that is judged). */
	SUBMISSIONEVENT /**< A submission event (i.e. a submission to a problem by a team. */
};

/** An event (from the event-list).
  * \warning Don't create an instance of this class directly. Use its subclasses instead!
  */
class Event : public QObject {
	Q_OBJECT
public:
	/** Constructs an event.
	  * \param id The ID of this event (must be unique).
	  * \param datetime The date and time this event happened.
	  * \param parent The parent of this object.
	  */
	explicit Event(QString id, QDateTime datetime, QObject *parent = 0);
	/** Returns a string representing this event.
	  * \return A string representation of this event.
	  * Useful for debug printing.
	  */
	virtual QString toString() = 0;
	/** Returns the type of this event.
	  * \return The type of this event.
	  */
	virtual EventType getType();
	/** Returns the ID of this event.
	  * \return The ID of this event.
	  */
	QString getId();
	/** Returns the date and time on which this event happened.
	  * \return The date and time on which this event happened.
	  */
	QDateTime getDateTime();
	/** Determines whether this event was in time.
	  * \param scoreboard The scoreboard to check against.
	  * \return true if and only if this event happened before the contest ended.
	  * \note For judging events, this will return whether its corresponding submission
	  * event was in time.
	  */
	bool inTime(Scoreboard *scoreboard);

protected:
	QString id;
	QDateTime datetime;
};
} // namespace Model
} // namespace DJ

#endif // EVENT_H
