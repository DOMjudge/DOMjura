#include "event.h"

#include "scoreboard.h"
#include "judgingevent.h"

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

bool Event::inTime(Scoreboard *scoreboard) {
	if (this->getType() == SUBMISSIONEVENT) {
		return (this->getDateTime() >= scoreboard->getContest()->getStart() && this->getDateTime() <= scoreboard->getContest()->getEnd());
	} else if (this->getType() == JUDGINGEVENT) {
		JudgingEvent *judgingEvent = (JudgingEvent *)this;
		Event *submissionEvent = judgingEvent->getSubmissionEvent();
		return (submissionEvent->getDateTime() >= scoreboard->getContest()->getStart() && submissionEvent->getDateTime() <= scoreboard->getContest()->getEnd());
	} else {
		return false;
	}
}
} // namespace Model
} // namespace DJ
