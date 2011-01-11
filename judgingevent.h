#ifndef JUDGINGEVENT_H
#define JUDGINGEVENT_H

#include "event.h"

namespace DJ {
namespace Model {
class JudgingEvent : public Event {
public:
	JudgingEvent(QString id, QDateTime datetime, QString judgingId, Event *submissionEvent, QObject *parent = NULL);
	void setCorrect(bool correct);
	void setResult(QString result);
	QString toString();
	EventType getType();
	QString getResult();
	QString getJudgingId();
	Event *getSubmissionEvent();
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
