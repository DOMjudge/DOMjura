#ifndef EVENTS_H
#define EVENTS_H

#include <QObject>

#include "event.h"

namespace DJ {
namespace Model {

class Events : public QObject {
	Q_OBJECT
public:
	explicit Events(QObject *parent = 0);
	void addEvent(Event *event);
	Event *eventBySubmitId(QString submitId);
	int getNumEvents();
	Event *getEvent(int i);
	Event *getEventById(QString id);

	QString toString();
private:
	QList<Event *> events;
};
}
}

#endif // EVENTS_H
