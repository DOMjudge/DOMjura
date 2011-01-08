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
			explicit StandingsController(QObject *parent = 0);

		private:
			QList<Model::RankedTeam *> currentRanking;
		};
	} // namespace Controller
} // namespace DJ

#endif // STANDINGSCONTROLLER_H
