#include "events.h"

#include "submissionevent.h"
#include "judgingevent.h"

namespace DJ {
namespace Model {
Events::Events(QObject *parent) : QObject(parent) {
	this->sorted = false;
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

int Events::getNumEvents() {
	return this->events.size();
}

void Events::sort() {
	qSort(events.begin(), events.end(), SortBySubTime);
}

Event *Events::getEvent(int i) {
	return this->events.at(i);
}

Event *Events::getEventById(QString id) {
	for (int i = 0; i < this->events.size(); i++) {
		if (this->events.at(i)->getId() == id) {
			return this->events.at(i);
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

bool SortBySubTime(Model::Event *event1, Model::Event *event2) {
	if (event1->getType() == Model::SUBMISSIONEVENT && event2->getType() == Model::SUBMISSIONEVENT) {
		return event1->getDateTime() < event2->getDateTime();
	}
	if (event1->getType() == Model::JUDGINGEVENT && event2->getType() == Model::JUDGINGEVENT) {
		Model::SubmissionEvent *submissionEvent1 = (Model::SubmissionEvent *)(((Model::JudgingEvent *)event1)->getSubmissionEvent());
		Model::SubmissionEvent *submissionEvent2 = (Model::SubmissionEvent *)(((Model::JudgingEvent *)event2)->getSubmissionEvent());
		if (submissionEvent1->getDateTime() == submissionEvent2->getDateTime()) {
			return event1->getDateTime() < event2->getDateTime();
		} else {
			return submissionEvent1->getDateTime() < submissionEvent2->getDateTime();
		}
	}
	if (event1->getType() == Model::SUBMISSIONEVENT && event2->getType() == Model::JUDGINGEVENT) {
		Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)(((Model::JudgingEvent *)event2)->getSubmissionEvent());
		if (submissionEvent == event1) {
			return true;
		} else {
			return event1->getDateTime() < submissionEvent->getDateTime();
		}
	}
	if (event1->getType() == Model::JUDGINGEVENT && event2->getType() == Model::SUBMISSIONEVENT) {
		Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)(((Model::JudgingEvent *)event1)->getSubmissionEvent());
		if (submissionEvent == event2) {
			return false;
		} else {
			return submissionEvent->getDateTime() < event2->getDateTime();
		}
	}
	return true;
}
}
}
