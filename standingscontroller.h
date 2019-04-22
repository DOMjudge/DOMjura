/** \file standingscontroller.h
  * \brief Contains the class for the standings controller.
  */

#ifndef STANDINGSCONTROLLER_H
#define STANDINGSCONTROLLER_H

#include <QObject>

#include "rankedteam.h"
#include "team.h"
#include "problem.h"
#include "judging.h"
#include "contest.h"

namespace DJ {
namespace Controller {
/** The controller that computes the standings.
  */
class StandingsController : public QObject {
    Q_OBJECT
public:
    /** Constructs a new standings controller.
      * \param teams The teams to use
      * \param problems The problems to use
      * \param parent The parent of this object.
      */
    explicit StandingsController(Model::Contest *contest,
                                 QHash<QString, Model::Team *> teams,
                                 QHash<QString, Model::Problem *> problems,
                                 QList<Model::Judging *> judgings,
                                 QObject *parent = nullptr);
    /** Initializes the standings.
      */
    void initStandings();
    /** Calculates the next standing.
      * \return true if and only if a next standing could be calculated.
      * If this value is false, there is no next standing anymore and thus
      * the resolving is done.
      */
    bool nextStanding();
    /** Returns a string representing the current standing.
      * \return A string representation of the current standing.
      * Useful for debug printing.
      */
    QString toString();
    /** Returns the position that is currently being resolved.
      * \return The position that is currently being resolved.
      */
    int getCurrentPos();
    /** Returns the last resolved team.
      * \return The last resolved team.
      */
    int getLastResolvedTeam();
    /** Returns the last resolved problem.
      * \return The last resolved problem.
      */
    int getLastResolvedProblem();
    /** Returns the team with the given ID.
      * \param id The ID to search for.
      * \return The team with the given ID, or NULL if not found.
      */
    Model::RankedTeam *getTeamById(QString id);
    /** Returns the current ranking.
      * \return The current ranking.
      */
    QList<Model::RankedTeam *> getCurrentRanking();

private:
    Model::Contest *contest;
    QHash<QString, Model::Team *> teams;
    QHash<QString, Model::Problem *> problems;
    QList<Model::Judging *> judgings;
    QList<Model::RankedTeam *> currentRanking;
    // Helper variable to speed up the searching for the next change
    int currentPos;
    int currentProblem;
    int lastResolvedTeam;
    int lastResolvedProblem;
};

/** Used for sorting the ranking.
  * \param team1 The first team.
  * \param team2 The second team.
  * \return true if and only if team1 is ranked higher than team2.
  */
bool rankedTeamLessThan(Model::RankedTeam *team1, Model::RankedTeam *team2);

/** Used for sorting the problems.
  * \param problem1 The first problem.
  * \param problem2 The second problem.
  * \return true if and only if problem1 is less than problem2.
  */
bool problemLessThan(Model::Problem *problem1, Model::Problem *problem2);
} // namespace Controller
} // namespace DJ

#endif // STANDINGSCONTROLLER_H
