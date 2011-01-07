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

			QString toString();
		private:
			QList<Event *> events;
		};
	}
}

#endif // EVENTS_H
