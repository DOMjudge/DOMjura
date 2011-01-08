#ifndef STATSCONTROLLER_H
#define STATSCONTROLLER_H

#include <QObject>

#include "scoreboard.h"
#include "events.h"

namespace DJ {
	namespace Controller {
		class StatsController : public QObject {
			Q_OBJECT
		public:
			explicit StatsController(Model::Scoreboard *scoreboard, Model::Events *events, QObject *parent = 0);

			int getTotalSubmissions();
			int getNumSubmissionsOfProblem(QString problemid);
			int getNumCorrectofProblem(QString problemid);
			bool problemIsSolved(QString problemid);
			QDateTime getFirstSolved(QString problemid);

		private:
			Model::Scoreboard *scoreboard;
			Model::Events *events;
		};
	} // namespace Model
} // namespace Controller

#endif // STATSCONTROLLER_H
