#include "submission.h"

#include <QJsonObject>

#include "team.h"
#include "problem.h"
#include "contest.h"

namespace DJ {
namespace Model {

Submission::Submission(QJsonObject submission,
                       QHash<QString, Team *> teams,
                       QHash<QString, Problem *> problems,
                       QObject *parent) : QObject(parent) {
    this->id = submission.value("id").toString();
    QString problemId = submission.value("problem_id").toString();
    if (problems.contains(problemId)) {
        this->problem = problems[problemId];
    } else {
        this->problem = nullptr;
    }

    QString teamId = submission.value("team_id").toString();
    if (teams.contains(teamId)) {
        this->team = teams[teamId];
    } else {
        this->team = nullptr;
    }

    this->time = QDateTime::fromString(submission.value("time").toString(), Qt::DateFormat::ISODate);
}

QString Submission::getId() {
    return this->id;
}

Problem *Submission::getProblem() {
    return this->problem;
}

Team *Submission::getTeam() {
    return this->team;
}

QDateTime Submission::getTime() {
    return this->time;
}

bool Submission::inTime(Contest *contest) {
    if (!contest->getEnd().isValid()) {
        return true;
    }
    return (this->getTime() >= contest->getStart() && this->getTime() <= contest->getEnd());
}

bool Submission::inFreeze(Contest *contest) {
    if (!contest->getFreeze().isValid()) {
        return false;
    }
    return (this->getTime() > contest->getFreeze());
}

} // namespace Model
} // namespace DJ
