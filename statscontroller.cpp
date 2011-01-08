#include "statscontroller.h"

#include "event.h"
#include "submissionevent.h"
#include "judgingevent.h"

namespace DJ {
	namespace Controller {
		StatsController::StatsController(Model::Scoreboard *scoreboard, Model::Events *events, QObject *parent) : QObject(parent) {
			this->scoreboard = scoreboard;
			this->events = events;
		}

		int StatsController::getTotalSubmissions() {
			int num = 0;
			for (int i = 0; i < this->events->getNumEvents(); i++) {
				if (this->events->getEvent(i)->getType() == Model::SUBMISSIONEVENT) {
					num++;
				}
			}
			return num;
		}

		int StatsController::getNumSubmissionsOfProblem(QString problemid) {
			int num = 0;
			for (int i = 0; i < this->events->getNumEvents(); i++) {
				if (this->events->getEvent(i)->getType() == Model::SUBMISSIONEVENT &&
						((Model::SubmissionEvent *)(this->events->getEvent(i)))->getProblem()->getId() == problemid) {
					num++;
				}
			}
			return num;
		}
	} // namespace Model
} // namespace Controller
