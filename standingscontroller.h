#ifndef STANDINGSCONTROLLER_H
#define STANDINGSCONTROLLER_H

#include <QObject>

#include "scoreboard.h"
#include "events.h"
#include "rankedteam.h"

namespace DJ {
	namespace Controller {
		class StandingsController : public QObject {
			Q_OBJECT
		public:
			explicit StandingsController(Model::Scoreboard *scoreboard, Model::Events *events, QObject *parent = 0);
			void initStandings();
			void nextStanding();

		private:
			Model::Scoreboard *scoreboard;
			Model::Events *events;
			QList<Model::RankedTeam *> currentRanking;
		};
	} // namespace Controller
} // namespace DJ

#endif // STANDINGSCONTROLLER_H
