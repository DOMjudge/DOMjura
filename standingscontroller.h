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
	bool nextStanding();
	QString toString();
	int getCurrentPos();
	Model::RankedTeam *getTeamById(QString id);
	QList<Model::RankedTeam *> getCurrentRanking();

private:

	Model::Scoreboard *scoreboard;
	Model::Events *events;
	QList<Model::RankedTeam *> currentRanking;
	// Helper variable to speed up the searching for the next change
	int currentPos;
	int currentProblem;
	QString category;
};

bool rankedTeamLessThan(Model::RankedTeam *team1, Model::RankedTeam *team2);
} // namespace Controller
} // namespace DJ

#endif // STANDINGSCONTROLLER_H
