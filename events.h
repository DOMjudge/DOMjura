/** \file events.h
  * \brief Contains events related data.
  */

#ifndef EVENTS_H
#define EVENTS_H

#include <QObject>

#include "event.h"

namespace DJ {
namespace Model {

/** An event-list.
  */
class Events : public QObject {
	Q_OBJECT
public:
	/** Constructs an empty event-list.
	  * \param parent The parent of this object.
	  */
	explicit Events(QObject *parent = 0);
	/** Adds an event to the event-list.
	  * \param event The event to add.
	  */
	void addEvent(Event *event);
	/** Returns a submission event by its ID.
	  * \param submitId The submit ID to search for.
	  * \return The event corresponding to this submit ID if found,
	  * NULL otherwise.
	  */
	Event *eventBySubmitId(QString submitId);
	/** Returns the number of events in the event-list.
	  * \return The number of events.
	  */
	int getNumEvents();
	/** Sorts the event-list by submission time.
	  */
	void sort();
	/** Returns the i-th event.
	  * \param i The index of the event to return.
	  * \return The i-th event in the event-list.
	  */
	Event *getEvent(int i);
	/** Returns the event for the given ID.
	  * \param id The ID to search for.
	  * \return The event corresponding to the provided ID if found,
	  * NULL otherwise.
	  */
	Event *getEventById(QString id);
	/** Returns a string representing this event-list.
	  * \return A string representation of this event-list.
	  * Useful for debug printing.
	  */
	QString toString();
private:
	QList<Event *> events;
	bool sorted;
};

/** A function used for sorting events by submission time.
  * \param event1 The first event.
  * \param event2 The second event.
  * \return true if and only if the first event happened before
  * second event based on submission times.
  */
bool SortBySubTime(Model::Event *event1, Model::Event *event2);

}
}

#endif // EVENTS_H
