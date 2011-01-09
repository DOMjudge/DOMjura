#include "rankedteam.h"

namespace DJ {
	namespace Model {
		RankedTeam::RankedTeam(QString id, QString name, QObject *parent) : QObject(parent) {
			this->id = id;
			this->name = name;
		}

		void RankedTeam::setProblem(QString id, RankedProblem *problem) {
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
			this->recalculateData();
		}

		void RankedTeam::recalculateData() {
			this->numSolved = 0;
			this->totalTime = 0;
			for (int i = 0; i < this->problems.size(); i++) {
				RankedProblem *problem = this->problems.at(i);
				if (problem->problemState == SOLVED) {
					this->numSolved++;
					this->totalTime += problem->timeLastTry + (PENALTY_TIME * (problem->tries - 1));
				}
			}
		}
	} // namespace Model
} // namespace DJ
