#ifndef RANKEDTEAM_H
#define RANKEDTEAM_H

#include <QObject>
#include <QHash>

#include "defines.h"

namespace DJ {
namespace Model {
struct RankedProblem {
	QString id;
	ProblemState problemState;
	int tries;
	int timeLastTry;

	RankedProblem *copy() {
		RankedProblem *c = new RankedProblem();
		c->id = this->id;
		c->problemState = this->problemState;
		c->tries = this->tries;
		c->timeLastTry = this->timeLastTry;
		return c;
	}
};

class RankedTeam : public QObject {
	Q_OBJECT
public:
	explicit RankedTeam(QString id, QString name, QObject *parent = 0);
	void setProblem(QString id, RankedProblem *problem);

	int getNumSolved();
	int getTotalTime();
	QString getName();
	QString getId();
	int getNumProblems();
	RankedProblem *getProblem(int i);
	RankedProblem *getProblemById(QString id);

private:
	void recalculateData();

	QString name;
	QString id;
	QList<RankedProblem *> problems;
	QHash<QString, int> problemsHash;
	int numSolved;
	int totalTime;
};
} // namespace Model
} // namespace DJ

#endif // RANKEDTEAM_H
