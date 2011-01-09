#include "standingscontroller.h"

namespace DJ {
	namespace Controller {
		StandingsController::StandingsController(Model::Scoreboard *scoreboard, Model::Events *events, QObject *parent) : QObject(parent) {
			this->scoreboard = scoreboard;
			this->events = events;
		}

		void StandingsController::initStandings() {
			// Use the scoreboard and the events to initialize the standings

		}

		void StandingsController::nextStanding() {

		}
	} // namespace Controller
} // namespace DJ
