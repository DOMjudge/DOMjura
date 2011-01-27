/** \file judgingevent.h
  * \brief Contains a class for a judging event.
  */

#ifndef JUDGINGEVENT_H
#define JUDGINGEVENT_H

#include "event.h"

namespace DJ {
namespace Model {
/** A judging event. This is an event where an judge
  * determines the correctness of a submission.
  */
class JudgingEvent : public Event {
public:
	/** Constructs a new judging event.
	  * \param id The (unique) ID of this event.
	  * \param datetime The date and time this event happened.
	  * \param judgingId The judging ID of this event.
	  * \param submissionEvent The submission event this event belongs to.
	  * \param parent The parent of this object.
	  */
	JudgingEvent(QString id, QDateTime datetime, QString judgingId, Event *submissionEvent, QObject *parent = NULL);
	/** Sets the correctness of this judging.
	  * \param correct Whether this judging is correct.
	  */
	void setCorrect(bool correct);
	/** Sets the result of this judging.
	  * \param result What the string representation of the result is.
	  */
	void setResult(QString result);
	QString toString();
	EventType getType();
	/** Returns the result of this judging.
	  * \return The string representation of the result of this judging.
	  */
	QString getResult();
	/** Returns the judging ID of this event.
	  * \return The judging ID of this event.
	  */
	QString getJudgingId();
	/** Returns the submission event this event belongs to.
	  * \return The submission event this event belongs to.
	  */
	Event *getSubmissionEvent();
	/** Returns whether this judging is correct.
	  * \return true if and only if this judging is true.
	 */
	bool isCorrect();

private:
	QString judgingId;
	QString result;
	Event *submissionEvent;
	bool correct;
};
} // namespace Model
} // namespace DJ

#endif // JUDGINGEVENT_H
