#include "standingscontroller.h"

#include <QSet>
#include <QDebug>

#include "judgingevent.h"
#include "submissionevent.h"

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
				// Ignore submission events and events that are too late (because we can use the judging events if we want to)
				if (event->getType() == Model::JUDGINGEVENT && event->inTime(this->scoreboard)) {
					Model::JudgingEvent *judgingEvent = (Model::JudgingEvent *)event;
					Model::SubmissionEvent *submissionEvent = (Model::SubmissionEvent *)judgingEvent->getSubmissionEvent();
					if (submissionEvent->isValid()) {
						QString submissionId = submissionEvent->getSubmissionId();
						QString problemId = submissionEvent->getProblem()->getId();
						if (processedSubmissions.contains(submissionId)) {
							// Already processed this, so the only thing that can happen is that it is now correct (or pending correct)
							Model::RankedTeam *team = this->getTeamById(submissionEvent->getTeam()->getId());
							Model::RankedProblem *problem = team->getProblemById(problemId)->copy();
							if (submissionEvent->isInFreeze()) {
								if (problem->problemState != Model::PENDING_SOLVED && judgingEvent->isCorrect()) {
									problem->problemState = Model::PENDING_SOLVED;
								}
							} else {
								if (problem->problemState != Model::SOLVED && judgingEvent->isCorrect()) {
									problem->problemState = Model::SOLVED;
								}
							}
							team->setProblem(problemId, problem);
						} else {
							// We always need to consider this event
							Model::RankedTeam *team = this->getTeamById(submissionEvent->getTeam()->getId());
							Model::RankedProblem *problem = team->getProblemById(problemId)->copy();
							if (!(problem->problemState == Model::SOLVED || problem->problemState == Model::PENDING_SOLVED)) {
								if (submissionEvent->isInFreeze()) {
									if (judgingEvent->isCorrect()) {
										problem->problemState = Model::PENDING_SOLVED;
										problem->tries++;
										problem->timeLastTry = this->scoreboard->getContest()->getStart().secsTo(submissionEvent->getDateTime()) / 60;;
									} else {
										problem->problemState = Model::PENDING_FAILED;
										problem->tries++;
										problem->timeLastTry = this->scoreboard->getContest()->getStart().secsTo(submissionEvent->getDateTime()) / 60;;
									}
								} else {
									if (judgingEvent->isCorrect()) {
										problem->problemState = Model::SOLVED;
										problem->tries++;
										problem->timeLastTry = this->scoreboard->getContest()->getStart().secsTo(submissionEvent->getDateTime()) / 60;;
									} else {
										problem->problemState = Model::FAILED;
										problem->tries++;
										problem->timeLastTry = this->scoreboard->getContest()->getStart().secsTo(submissionEvent->getDateTime()) / 60;;
									}
								}
							}
							processedSubmissions.insert(submissionId);
							team->setProblem(problemId, problem);
						}
					}
				}
			}

			qSort(this->currentRanking.begin(), this->currentRanking.end(), rankedTeamLessThan);
			this->currentPos = this->currentRanking.size() - 1;
			this->currentProblem = 0;
		}

		bool StandingsController::nextStanding() {
			while (!(this->currentPos == 0 && this->currentProblem == this->currentRanking.at(0)->getNumProblems() - 1)) {
				Model::RankedTeam *team = this->currentRanking.at(this->currentPos);
				Model::RankedProblem *problem = team->getProblem(this->currentProblem)->copy();
				bool doneSomething = false;
				if (problem->problemState == Model::PENDING_FAILED) {
					problem->problemState = Model::FAILED;
					doneSomething = true;
				} else if (problem->problemState == Model::PENDING_SOLVED) {
					problem->problemState = Model::SOLVED;
					doneSomething = true;
				}
				if (doneSomething) {
					// Update problem
					team->setProblem(problem->id, problem);
					qSort(this->currentRanking.begin(), this->currentRanking.end(), rankedTeamLessThan);
					this->currentProblem = 0;
					return true;
				} else {
					if (this->currentProblem == team->getNumProblems() - 1) {
						this->currentPos--;
						this->currentProblem = 0;
					} else {
						this->currentProblem++;
					}
				}
			}
			return false;
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
						s += QString::number(curRank) + ". " + team->getName() + " ";
					} else {
						curRank = i + 1;
						s += QString::number(curRank) + ". " + team->getName() + " ";
					}
				} else {
					s += "1. " + team->getName() + " ";
					curRank = 1;
				}
				s += QString::number(team->getNumSolved()) + " " + QString::number(team->getTotalTime()) + "\n";
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

		Model::RankedTeam *StandingsController::getTeamById(QString id) {
			for (int i = 0; i < this->currentRanking.size(); i++) {
				Model::RankedTeam *team = this->currentRanking.at(i);
				if (team->getId() == id) {
					return team;
				}
			}
			return NULL;
		}

		bool rankedTeamLessThan(Model::RankedTeam *team1, Model::RankedTeam *team2) {
			if (team1->getNumSolved() == team2->getNumSolved()) {
				return team1->getName() < team2->getName();
			} else {
				return team1->getNumSolved() > team2->getNumSolved();
			}
		}
	} // namespace Controller
} // namespace DJ
