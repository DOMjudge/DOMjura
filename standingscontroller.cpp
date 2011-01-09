#include "standingscontroller.h"

#include <QSet>

namespace DJ {
	namespace Controller {
		StandingsController::StandingsController(Model::Scoreboard *scoreboard, Model::Events *events, QObject *parent) : QObject(parent) {
			this->scoreboard = scoreboard;
			this->events = events;
		}

		void StandingsController::initStandings(QString category) {
			this->category = category;
			// First, clear
			for (int i = 0; i < this->currentRanking.size(); i++) {
				delete this->currentRanking[i];
			}
			this->currentRanking.clear();
			// Then, add all the teams
			for (int i = 0; i < this->scoreboard->getNumTeams(); i++) {
				Model::Team *team = this->scoreboard->getTeam(i);
				if (team->getCategory()->getName() == category) {
					Model::RankedTeam *rankedTeam = new Model::RankedTeam(team->getId(), team->getName(), this);
					// For each team, add the problems
					for (int j = 0; j < this->scoreboard->getNumProblems(); j++) {
						Model::Problem *problem = this->scoreboard->getProblem(j);
						Model::RankedProblem *rankedProblem = new Model::RankedProblem();
						rankedProblem->id = problem->getId();
						rankedProblem->problemState = Model::NOTSUBMITTED;
						rankedProblem->tries = 0;
						rankedProblem->timeLastTry = 0;
						rankedTeam->setProblem(rankedProblem->id, rankedProblem);
					}
					this->currentRanking.append(rankedTeam);
				}
			}

			// Then, walk through the events

			// To keep track of the processed submissions
			QSet<QString> processedSubmissions;
			for (int i = 0; i < this->events->getNumEvents(); i++) {
				Model::Event *event = this->events->getEvent(i);
				// Ignore submission events (because we can use the judging events if we want to)
			}

			qSort(this->currentRanking.begin(), this->currentRanking.end(), rankedTeamLessThan);
			this->currentPos = this->currentRanking.size() - 1;
		}

		void StandingsController::nextStanding() {

		}

		QString StandingsController::toString() {
			int curRank;
			QString s;
			s += "Current standings for " + this->category + ":\n";
			for (int i = 0; i < this->currentRanking.size(); i++) {
				Model::RankedTeam *team = this->currentRanking.at(i);
				if (i > 0) {
					Model::RankedTeam *prevTeam = this->currentRanking.at(i - 1);
					if (team->getNumSolved() == prevTeam->getNumSolved()
							&& team->getTotalTime() == prevTeam->getTotalTime()) {
						s += QString::number(curRank) + ". " + team->getName() + "\n";
					} else {
						curRank = i + 1;
						s += QString::number(curRank) + ". " + team->getName() + "\n";
					}
				} else {
					s += "1. " + team->getName() + "\n";

					curRank = 1;
				}
				for (int j = 0; j < team->getNumProblems(); j++) {
					Model::RankedProblem *problem = team->getProblem(j);
					s += problem->id + ": " + QString::number(problem->tries) + " - " + QString::number(problem->timeLastTry) + " ";
					switch(problem->problemState) {
					case Model::NOTSUBMITTED:
						s += "_";
						break;
					case Model::FAILED:
						s += "F";
						break;
					case Model::SOLVED:
						s += "S";
						break;
					case Model::PENDING_FAILED:
					case Model::PENDING_SOLVED:
						s += "?";
						break;
					}

					s += "\t";
				}
				s += "\n";
			}
			return s;
		}

		bool rankedTeamLessThan(Model::RankedTeam *team1, Model::RankedTeam *team2) {
			if (team1->getNumSolved() == team2->getNumSolved()) {
				return team1->getName() < team2->getName();
			} else {
				return team1->getNumSolved() < team2->getNumSolved();
			}
		}
	} // namespace Controller
} // namespace DJ
