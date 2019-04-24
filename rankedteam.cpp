#include "rankedteam.h"

namespace DJ {
namespace Model {
RankedTeam::RankedTeam(QString id, QString name, QObject *parent) : QObject(parent) {
    this->id = id;
    this->name = name;
}

void RankedTeam::setProblem(QString id, RankedProblem *problem, Contest *contest) {
    if (problemsHash.contains(id)) {
        // First remove old one
        int idx = problemsHash[id];
        delete problems[idx];
        // Now replace with new problem
        problems.replace(idx, problem);
    } else {
        problems.append(problem);
        problemsHash[id] = problems.size() - 1;
    }
    this->recalculateData(contest);
}

void RankedTeam::recalculateData(Contest *contest) {
    this->numSolved = 0;
    this->totalTime = 0;
    for (int i = 0; i < this->problems.size(); i++) {
        RankedProblem *problem = this->problems.at(i);
        if (problem->problemState == SOLVED) {
            this->numSolved++;
            this->totalTime += problem->timeFirstCorrectTry + (contest->getPenaltyMinutes() * (problem->tries - 1));
        }
    }
}

int RankedTeam::getNumSolved() {
    return this->numSolved;
}

int RankedTeam::getTotalTime() {
    return this->totalTime;
}

QString RankedTeam::getName() {
    return this->name;
}

QString RankedTeam::getId() {
    return this->id;
}

int RankedTeam::getNumProblems() {
    return this->problems.size();
}

RankedProblem *RankedTeam::getProblem(int i) {
    return this->problems.at(i);
}

RankedProblem *RankedTeam::getProblemById(QString id){
    return this->problems.at(this->problemsHash[id]);
}
} // namespace Model
} // namespace DJ
