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
			void initStandings(QString category);
			void nextStanding();
			QString toString();

		private:

			Model::Scoreboard *scoreboard;
			Model::Events *events;
			QList<Model::RankedTeam *> currentRanking;
			// Helper variable to speed up the searching for the next change
			int currentPos;
			QString category;
		};

		bool rankedTeamLessThan(Model::RankedTeam *team1, Model::RankedTeam *team2);
	} // namespace Controller
} // namespace DJ

#endif // STANDINGSCONTROLLER_H
