/** \file submissionevent.h
  * \brief Contains the submission event class.
  */

#ifndef SUBMISSIONEVENT_H
#define SUBMISSIONEVENT_H

#include "event.h"
#include "team.h"
#include "problem.h"
#include "language.h"

namespace DJ {
namespace Model {
/** A submission event.
  */
class SubmissionEvent : public Event {
public:
	/** Constructs a new submission event.
	  * \param id The ID of the event.
	  * \param datetime The date and time that this event happened.
	  * \param submissionId The ID of the submission.
	  * \param inFreeze Whether this submission was in the freeze time.
	  * \param parent The parent of this object.
	  */
	SubmissionEvent(QString id, QDateTime datetime, QString submissionId, bool inFreeze, QObject *parent = NULL);
	/** Sets the team for this event.
	  * \param team The team to set.
	  */
	void setTeam(Team *team);
	/** Sets the problem for this event.
	  * \param problem The problem to set.
	  */
	void setProblem(Problem *problem);
	/** Sets the language for this event.
	  * \param language The language to set.
	  */
	void setLanguage(Language *language);
	/** Returns the submission ID of this event.
	  * \return The submission ID of this event.
	  */
	QString getSubmissionId();
	/** Returns a string representing this event.
	  * \return A string representation of this event.
	  * Useful for debug printing.
	  */
	QString toString();
	EventType getType();
	/** Returns the problem of this event.
	  * \return The problem of this event.
	  */
	Problem *getProblem();
	/** Returns the team of this event.
	  * \return The team of this event.
	  */
	Team *getTeam();
	/** Returns the language of this event.
	  * \return The language of this event.
	  */
	Language *getLangugage();
	/** Determines whether this event was in the freeze time.
	  * \return true if and only this event was in the freeze time.
	  */
	bool isInFreeze();

private:
	bool inFreeze;
	QString submissionId;
	Team *team;
	Problem *problem;
	Language *language;
};
} // namespace Model
} // namespace DJ

#endif // SUBMISSIONEVENT_H
