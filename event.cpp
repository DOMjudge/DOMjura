#include "event.h"

namespace DJ {
	namespace Model {
		Event::Event(QString id, QDateTime datetime, QObject *parent) : QObject(parent) {
			this->id = id;
			this->datetime = datetime;
		}

		EventType Event::getType() {
			return UNKNOWN;
		}

		QString Event::getId() {
			return this->id;
		}

		QDateTime Event::getDateTime() {
			return this->datetime;
		}

	} // namespace Model
} // namespace DJ
