#include "standingscontroller.h"

#include <QSet>
#include <QDebug>

#include "judging.h"
#include "submission.h"

namespace DJ {
namespace Controller {
StandingsController::StandingsController(Model::Contest *contest,
                                         QHash<QString, Model::Team *> teams,
                                         QHash<QString, Model::Problem *> problems,
                                         QList<Model::Judging *> judgings,
                                         QObject *parent) : QObject(parent) {
    this->contest = contest;
    this->teams = teams;
    this->problems = problems;
    this->judgings = judgings;
}

void StandingsController::initStandings() {
    // First, clear
    for (int i = 0; i < this->currentRanking.size(); i++) {
        delete this->currentRanking[i];
    }
    this->currentRanking.clear();
    // Then, add all the teams
    QList<Model::Problem *> problems = this->problems.values();
    qSort(problems.begin(), problems.end(), problemLessThan);
    foreach (Model::Team *team, this->teams) {
        Model::RankedTeam *rankedTeam = new Model::RankedTeam(team->getId(), team->getName(), this);
        // For each team, add the problems
        foreach (Model::Problem *problem, problems) {
            Model::RankedProblem *rankedProblem = new Model::RankedProblem();
            rankedProblem->id = problem->getId();
            rankedProblem->shortname = problem->getShortName();
            rankedProblem->problemState = NOTSUBMITTED;
            rankedProblem->tries = 0;
            rankedProblem->total_tries = 0;
            rankedProblem->timeLastTry = 0;
            rankedProblem->timeFirstCorrectTry = 0;
            rankedTeam->setProblem(rankedProblem->id, rankedProblem, this->contest);
        }
        this->currentRanking.append(rankedTeam);
    }

    // Then, walk through the events
    // To keep track of the processed submissions
    QSet<QString> processedSubmissions;
    for (int i = 0; i < this->judgings.size(); i++) {
        Model::Judging *judging = this->judgings.at(i);
        Model::Submission *submission = judging->getSubmission();
        // Ignore submission events and events that are too late (because we can use the judging events if we want to)
        if (submission->inTime(this->contest)) {
            QString submissionId = submission->getId();
            QString problemId = submission->getProblem()->getId();
            if (processedSubmissions.contains(submissionId)) {
                // Already processed this, so the only thing that can happen is that it is now correct (or pending correct)
                Model::RankedTeam *team = this->getTeamById(submission->getTeam()->getId());
                Model::RankedProblem *problem = team->getProblemById(problemId)->copy();
                if (submission->inFreeze(contest)) {
                    if (problem->problemState != PENDING_SOLVED && judging->isCorrect()) {
                        problem->problemState = PENDING_SOLVED;
                    }
                } else {
                    if (problem->problemState != SOLVED && judging->isCorrect()) {
                        problem->problemState = SOLVED;
                    }
                }
                team->setProblem(problemId, problem, this->contest);
            } else {
                // We always need to consider this event
                Model::RankedTeam *team = this->getTeamById(submission->getTeam()->getId());
                if (!team) {
                    continue;
                }
                Model::RankedProblem *problem = team->getProblemById(problemId)->copy();
                problem->total_tries++;
                problem->timeLastTry = (this->contest->getStart().secsTo(submission->getTime()) - 0) / 60;
                if (!(problem->problemState == SOLVED || problem->problemState == PENDING_SOLVED)) {
                    if (submission->inFreeze(this->contest)) {
                        if (judging->isCorrect()) {
                            problem->problemState = PENDING_SOLVED;
                            problem->tries++;
                            problem->timeFirstCorrectTry = (this->contest->getStart().secsTo(submission->getTime()) - 0) / 60;
                        } else {
                            problem->problemState = PENDING_FAILED;
                            problem->tries++;
                        }
                    } else {
                        if (judging->isCorrect()) {
                            problem->problemState = SOLVED;
                            problem->tries++;
                            problem->timeFirstCorrectTry = (this->contest->getStart().secsTo(submission->getTime()) - 0) / 60;
                        } else {
                            problem->problemState = FAILED;
                            problem->tries++;
                        }
                    }
                }
                processedSubmissions.insert(submissionId);
                team->setProblem(problemId, problem, this->contest);
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
            team->setProblem(problem->id, problem, this->contest);
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
    int curRank = 0;
    QString s;
    s += "Current standings:\n";
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
            s += problem->shortname + ": " + QString::number(problem->tries) + " - " + QString::number(problem->timeLastTry) + " ";
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
    return nullptr;
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

bool problemLessThan(Model::Problem *problem1, Model::Problem *problem2) {
    return problem1->getShortName() < problem2->getShortName();
}

} // namespace Controller
} // namespace DJ
