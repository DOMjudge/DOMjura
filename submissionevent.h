#ifndef SUBMISSIONEVENT_H
#define SUBMISSIONEVENT_H

#include "event.h"
#include "team.h"
#include "problem.h"
#include "language.h"

namespace DJ {
	namespace Model {
		class SubmissionEvent : public Event {
		public:
			SubmissionEvent(QString id, QDateTime datetime, QString submissionId, bool inFreeze, QObject *parent = NULL);
			void setTeam(Team *team);
			void setProblem(Problem *problem);
			void setLanguage(Language *language);
			QString getSubmissionId();
			QString toString();
			EventType getType();
			Problem *getProblem();
			Team *getTeam();
			bool isValid();

		private:
			bool valid;
			bool inFreeze;
			QString submissionId;
			Team *team;
			Problem *problem;
			Language *language;
		};
	} // namespace Model
} // namespace DJ

#endif // SUBMISSIONEVENT_H
