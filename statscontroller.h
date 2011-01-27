/** \file statscontroller.h
  * \brief Contains the stats controller class.
  */

#ifndef STATSCONTROLLER_H
#define STATSCONTROLLER_H

#include <QObject>

#include "scoreboard.h"
#include "events.h"

namespace DJ {
namespace Controller {
/** The controller to calculate the stats.
  */
class StatsController : public QObject {
	Q_OBJECT
public:
	/** Constructs a new stats controller with the given scoreboard and events.
	  * \param scoreboard The scoreboard to use.
	  * \param events The events to use.
	  * \param parent The parent of this object.
	  */
	explicit StatsController(Model::Scoreboard *scoreboard, Model::Events *events, QObject *parent = 0);

	/** Returns the total number of submissions.
	  * \return The total number of submissions.
	  */
	int getTotalSubmissions();
	/** Returns the total number of correct submissions.
	  * \return The total number of correct submissions.
	  */
	int getTotalCorrect();
	/** Returns the number of submissions for the given problem.
	  * \param problemid The problem to use.
	  * \return The number of submissions for the given problem.
	  */
	int getNumSubmissionsOfProblem(QString problemid);
	/** Returns the number of correct submissions for the given problem.
	  * \param problemid The problem to use.
	  * \return The number of correct submissions for the given problem.
	  */
	int getNumCorrectofProblem(QString problemid);
	/** Returns whether the problem is solved.
	  * \param problemid The problem to use.
	  * \return Whether the problem is solved.
	  */
	bool problemIsSolved(QString problemid);
	/** Returns the date and time of the first correct solution of the given problem.
	  * \param problemid The problem to use.
	  * \return The date and time of the first correct solution of the given problem.
	  */
	QDateTime getFirstSolved(QString problemid);
	/** Returns the date and time of the last solution of the given problem.
	  * \param problemid The problem to use.
	  * \return The date and time of the last solution of the given problem.
	  */
	QDateTime getLastSubmission(QString problemid);

private:
	Model::Scoreboard *scoreboard;
	Model::Events *events;
};
} // namespace Model
} // namespace Controller

#endif // STATSCONTROLLER_H
