#ifndef JUDGINGEVENT_H
#define JUDGINGEVENT_H

#include "event.h"

namespace DJ {
	namespace Model {
		class JudgingEvent : public Event {
		public:
			JudgingEvent(QString id, QDateTime datetime, QString judgingId, Event *submissionEvent, QObject *parent = NULL);
			void setCorrect(bool correct);
			QString toString();
			EventType getType();
			Event *getSubmissionEvent();

		private:
			QString judgingId;
			Event *submissionEvent;
			bool correct;
		};
	} // namespace Model
} // namespace DJ

#endif // JUDGINGEVENT_H
