#include "statscontroller.h"

#include "event.h"
#include "submissionevent.h"
#include "judgingevent.h"

#include <QSet>

namespace DJ {
namespace Controller {
StatsController::StatsController(Model::Scoreboard *scoreboard, Model::Events *events, QObject *parent) : QObject(parent) {
	this->scoreboard = scoreboard;
	this->events = events;
}

int StatsController::getTotalSubmissions() {
	int num = 0;
	for (int i = 0; i < this->events->getNumEvents(); i++) {
		if (this->events->getEvent(i)->inTime(this->scoreboard)
				&& this->events->getEvent(i)->getType() == Model::SUBMISSIONEVENT) {
			num++;
		}
	}
	return num;
}

int StatsController::getTotalCorrect() {
	int num = 0;
	// This is to make sure that when a team submits two correct solutions for a problem, only one gets countet
	QSet<QString> done;
	for (int i = 0; i < this->events->getNumEvents(); i++) {
		if (this->events->getEvent(i)->inTime(this->scoreboard)
				&& this->events->getEvent(i)->getType() == Model::JUDGINGEVENT) {
			Model::JudgingEvent *judgingEvent = (Model::JudgingEvent *)this->events->getEvent(i);
			Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)judgingEvent->getSubmissionEvent();
			if (judgingEvent->isCorrect()
					&& !done.contains(submissionEvent->getSubmissionId())) {
				num++;
				done.insert(submissionEvent->getSubmissionId());
			}
		}
	}
	return num;
}

int StatsController::getNumSubmissionsOfProblem(QString problemid) {
	int num = 0;
	for (int i = 0; i < this->events->getNumEvents(); i++) {
		if (this->events->getEvent(i)->inTime(this->scoreboard)
				&& this->events->getEvent(i)->getType() == Model::SUBMISSIONEVENT
				&& ((Model::SubmissionEvent *)this->events->getEvent(i))->getProblem()->getId() == problemid) {
			num++;
		}
	}
	return num;
}

int StatsController::getNumCorrectofProblem(QString problemid) {
	int num = 0;
	// This is to make sure that when a team submits two correct solutions for a problem, only one gets countet
	QSet<QString> done;
	for (int i = 0; i < this->events->getNumEvents(); i++) {
		if (this->events->getEvent(i)->inTime(this->scoreboard)
				&& this->events->getEvent(i)->getType() == Model::JUDGINGEVENT) {
			Model::JudgingEvent *judgingEvent = (Model::JudgingEvent *)this->events->getEvent(i);
			Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)judgingEvent->getSubmissionEvent();
			if (submissionEvent->getProblem()->getId() == problemid
					&& judgingEvent->isCorrect()
					&& !done.contains(submissionEvent->getTeam()->getId())) {
				num++;
				done.insert(submissionEvent->getTeam()->getId());
			}
		}
	}
	return num;
}

bool StatsController::problemIsSolved(QString problemid) {
	for (int i = 0; i < this->events->getNumEvents(); i++) {
		if (this->events->getEvent(i)->inTime(this->scoreboard)
				&& this->events->getEvent(i)->getType() == Model::JUDGINGEVENT) {
			Model::JudgingEvent *judgingEvent = (Model::JudgingEvent *)this->events->getEvent(i);
			Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)judgingEvent->getSubmissionEvent();
			if (submissionEvent->getProblem()->getId() == problemid
					&& judgingEvent->isCorrect()) {
				return true;
			}
		}
	}
	return false;
}

QDateTime StatsController::getFirstSolved(QString problemid) {
	for (int i = 0; i < this->events->getNumEvents(); i++) {
		if (this->events->getEvent(i)->inTime(this->scoreboard)
				&& this->events->getEvent(i)->getType() == Model::JUDGINGEVENT) {
			Model::JudgingEvent *judgingEvent = (Model::JudgingEvent *)this->events->getEvent(i);
			Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)judgingEvent->getSubmissionEvent();
			if (submissionEvent->getProblem()->getId() == problemid
					&& judgingEvent->isCorrect()) {
				return submissionEvent->getDateTime();
			}
		}
	}
	return QDateTime();
}

QDateTime StatsController::getLastSubmission(QString problemid) {
	for (int i = this->events->getNumEvents() - 1; i >= 0; i--) {
		if (this->events->getEvent(i)->inTime(this->scoreboard)
				&& this->events->getEvent(i)->getType() == Model::JUDGINGEVENT) {
			Model::JudgingEvent *judgingEvent = (Model::JudgingEvent *)this->events->getEvent(i);
			Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)judgingEvent->getSubmissionEvent();
			if (submissionEvent->getProblem()->getId() == problemid) {
				return submissionEvent->getDateTime();
			}
		}
	}
	return QDateTime();
}

} // namespace Model
} // namespace Controller
