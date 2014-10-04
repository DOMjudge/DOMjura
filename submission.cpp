#include "submission.h"

#include <QJsonObject>

#include "team.h"
#include "problem.h"
#include "contest.h"

namespace DJ {
namespace Model {

Submission::Submission(QJsonObject submission,
					   QHash<int, Team *> teams,
					   QHash<int, Problem *> problems,
					   QObject *parent) : QObject(parent) {
	this->id = submission.value("id").toString("0").toInt();
	int problemId = submission.value("problem").toString("0").toInt();
	if (problems.contains(problemId)) {
		this->problem = problems[problemId];
	} else {
		this->problem = NULL;
	}

	int teamId = submission.value("team").toString("0").toInt();
	if (teams.contains(teamId)) {
		this->team = teams[teamId];
	} else {
		this->team = NULL;
	}

	this->time = QDateTime::fromTime_t(qRound(submission.value("time").toString().toDouble(0)));
}

int Submission::getId() {
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
