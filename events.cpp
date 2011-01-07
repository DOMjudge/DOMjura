#include "events.h"

#include <QDebug>

#include "submissionevent.h"

namespace DJ {
	namespace Model {
		Events::Events(QObject *parent) : QObject(parent) {
		}

		void Events::addEvent(Event *event) {
			this->events.append(event);
		}

		Event *Events::eventBySubmitId(QString submitId) {
			for (int i = 0; i < this->events.size(); i++) {
				Event *event = this->events.at(i);
				if (event->getType() == SUBMISSIONEVENT) {
					SubmissionEvent *submissionEvent = (SubmissionEvent *)event;
					if (submissionEvent->getSubmissionId() == submitId) {
						return submissionEvent;
					}
				}
			}
			return NULL;
		}

		QString Events::toString() {
			QString s;
			s += "Events (" + QString::number(this->events.size()) + "):\n";
			for (int i = 0; i < this->events.size(); i++) {
				s += "  Event " + QString::number(i) + ":\n";
				s += this->events.at(i)->toString();
			}
			return s;
		}
	}
}
