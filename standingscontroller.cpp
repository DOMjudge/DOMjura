#include "standingscontroller.h"

#include <QSet>

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
		if (team->getCategory() && team->getCategory()->getName() == category) {
			Model::RankedTeam *rankedTeam = new Model::RankedTeam(team->getId(), team->getName(), this);
			// For each team, add the problems
			for (int j = 0; j < this->scoreboard->getNumProblems(); j++) {
				Model::Problem *problem = this->scoreboard->getProblem(j);
				Model::RankedProblem *rankedProblem = new Model::RankedProblem();
				rankedProblem->id = problem->getId();
				rankedProblem->problemState = NOTSUBMITTED;
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
						if (problem->problemState != PENDING_SOLVED && judgingEvent->isCorrect()) {
							problem->problemState = PENDING_SOLVED;
						}
					} else {
						if (problem->problemState != SOLVED && judgingEvent->isCorrect()) {
							problem->problemState = SOLVED;
						}
					}
					team->setProblem(problemId, problem);
				} else {
					// We always need to consider this event
					Model::RankedTeam *team = this->getTeamById(submissionEvent->getTeam()->getId());
					if (!team) {
						// team is of wrong category, skip
						continue;
					}
					Model::RankedProblem *problem = team->getProblemById(problemId)->copy();
					if (!(problem->problemState == SOLVED || problem->problemState == PENDING_SOLVED)) {
						if (submissionEvent->isInFreeze()) {
							if (judgingEvent->isCorrect()) {
								problem->problemState = PENDING_SOLVED;
								problem->tries++;
								problem->timeLastTry = (this->scoreboard->getContest()->getStart().secsTo(submissionEvent->getDateTime()) - 0) / 60;
							} else {
								problem->problemState = PENDING_FAILED;
								problem->tries++;
								problem->timeLastTry = (this->scoreboard->getContest()->getStart().secsTo(submissionEvent->getDateTime()) - 0) / 60;
							}
						} else {
							if (judgingEvent->isCorrect()) {
								problem->problemState = SOLVED;
								problem->tries++;
								problem->timeLastTry = (this->scoreboard->getContest()->getStart().secsTo(submissionEvent->getDateTime()) - 0) / 60;
							} else {
								problem->problemState = FAILED;
								problem->tries++;
								problem->timeLastTry = (this->scoreboard->getContest()->getStart().secsTo(submissionEvent->getDateTime()) - 0) / 60;
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
	while (!(this->currentPos == -1 && this->currentProblem == 0)) {
		Model::RankedTeam *team = this->currentRanking.at(this->currentPos);
		Model::RankedProblem *problem = team->getProblem(this->currentProblem)->copy();
		bool doneSomething = false;
		if (problem->problemState == PENDING_FAILED) {
			problem->problemState = FAILED;
			doneSomething = true;
		} else if (problem->problemState == PENDING_SOLVED) {
			problem->problemState = SOLVED;
			doneSomething = true;
		}
		if (doneSomething) {
			this->lastResolvedTeam = this->currentPos;
			this->lastResolvedProblem = this->currentProblem;
			// Update problem
			team->setProblem(problem->id, problem);
			qSort(this->currentRanking.begin(), this->currentRanking.end(), rankedTeamLessThan);
			this->currentProblem = 0;
			return true;
		} else {
			if (this->currentProblem == team->getNumProblems() - 1) {
				this->lastResolvedTeam = this->currentPos;
				this->lastResolvedProblem = -1;
				this->currentPos--;
				this->currentProblem = 0;
				return true;
			} else {
				this->currentProblem++;
			}
		}
	}
	return false;
}

int StandingsController::getLastResolvedTeam() {
	return this->lastResolvedTeam;
}

int StandingsController::getLastResolvedProblem() {
	return this->lastResolvedProblem;
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
			case NOTSUBMITTED:
				s += "_";
				break;
			case FAILED:
				s += "F";
				break;
			case SOLVED:
				s += "S";
				break;
			case PENDING_FAILED:
			case PENDING_SOLVED:
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

QList<Model::RankedTeam *> StandingsController::getCurrentRanking() {
	return this->currentRanking;
}

int StandingsController::getCurrentPos() {
	return this->currentPos;
}

bool rankedTeamLessThan(Model::RankedTeam *team1, Model::RankedTeam *team2) {
	if (team1->getNumSolved() == team2->getNumSolved()) {
		if (team1->getTotalTime() == team2->getTotalTime()) {
			return team1->getName() < team2->getName();
		} else {
			return team1->getTotalTime() < team2->getTotalTime();
		}
	} else {
		return team1->getNumSolved() > team2->getNumSolved();
	}
}

} // namespace Controller
} // namespace DJ
