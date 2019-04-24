/** \file rankedteam.h
  * \brief Contains the class for a ranked team and problem.
  */

#ifndef RANKEDTEAM_H
#define RANKEDTEAM_H

#include <QObject>
#include <QHash>

#include "defines.h"
#include "contest.h"

namespace DJ {
namespace Model {
/** A ranked problem.
  */
struct RankedProblem {
    QString id; /**< The ID of this problem. */
    QString shortname; /**< The short name of this problem. */
    ProblemState problemState; /**< The state of this problem. */
    int tries; /**< The number of tries for this problem until correct one. */
    int total_tries; /**< The number of total tries for this problem. */
    int timeLastTry; /**< The time of the last try for this problem. */
    int timeFirstCorrectTry; /**< The time of the last (propably) correct try for this problem. */

    /** Makes a copy of a ranked problem.
      */
    RankedProblem *copy() {
        RankedProblem *c = new RankedProblem();
        c->id = this->id;
        c->shortname = this->shortname;
        c->problemState = this->problemState;
        c->tries = this->tries;
        c->total_tries = this->total_tries;
        c->timeLastTry = this->timeLastTry;
        c->timeFirstCorrectTry = this->timeFirstCorrectTry;
        return c;
    }
};

/** A ranked team.
  */
class RankedTeam : public QObject {
    Q_OBJECT
public:
    /** Constructs a new ranked team.
      * \param id The unique ID for this ranked team.
      * \param name The name of this team.
      * \param parent The parent of this object.
      */
    explicit RankedTeam(QString id, QString name, QObject *parent = 0);
    /** Adds / updates a problem.
      * \param id The ID of the problem to add / update.
      * \param problem The new problem.
      */
    void setProblem(QString id, RankedProblem *problem, Contest *contest);

    /** Returns the number of solved problems.
      * \return the number of solved problems.
      */
    int getNumSolved();
    /** Returns the total time for thist eam.
      * \return The total time for thist eam.
      */
    int getTotalTime();
    /** Returns the name of this team.
      * \return The name of this team.
      */
    QString getName();
    /** Returns the ID of this team.
      * \return The ID of this team.
      */
    QString getId();
    /** Returns the number of problems.
      * \return The number of problems.
      */
    int getNumProblems();
    /** Returns the i-th problem for this team.
      * \param i The index to use.
      * \return The i-th problem for this team.
      */
    RankedProblem *getProblem(int i);
    /** Returns the problem for the given ID.
      * \param id The ID to search.
      * \pre The problem with the specified ID exists.
      * \return The problem for the given ID.
      */
    RankedProblem *getProblemById(QString id);

private:
    void recalculateData(Contest *contest);

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
