#include "submissionevent.h"

namespace DJ {
	namespace Model {
		SubmissionEvent::SubmissionEvent(QString id, QDateTime datetime, QString submissionId, bool inFreeze, QObject *parent) : Event(id, datetime, parent) {
			this->submissionId = submissionId;
			this->inFreeze = inFreeze;
		}

		void SubmissionEvent::setTeam(Team *team) {
			this->team = team;
		}

		void SubmissionEvent::setProblem(Problem *problem) {
			this->problem = problem;
		}

		void SubmissionEvent::setLanguage(Language *language) {
			this->language = language;
		}

		QString SubmissionEvent::getSubmissionId() {
			return this->submissionId;
		}

		QString SubmissionEvent::toString() {
			QString s;
			s += "    type         = submission\n";
			s += "    id           = " + this->id + "\n";
			s += "    time         = " + this->datetime.toString("yyyy-MM-dd hh:mm:ss") + "\n";
			s += "    submissionid = " + this->submissionId + "\n";
			s += "    team         = " + this->team->getId() + "\n";
			s += "    problem      = " + this->problem->getId() + "\n";
			s += "    language     = " + this->language->getId() + "\n";
			return s;
		}

		EventType SubmissionEvent::getType() {
			return SUBMISSIONEVENT;
		}

		Problem *SubmissionEvent::getProblem() {
			return this->problem;
		}

		Team *SubmissionEvent::getTeam() {
			return this->team;
		}
	} // namespace Model
} // namespace DJ
