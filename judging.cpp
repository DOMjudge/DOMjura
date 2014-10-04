#include "judging.h"

#include <QJsonObject>

#include "submission.h"

namespace DJ {
namespace Model {

Judging::Judging(QJsonObject judging,
				 QHash<int, Submission *> submissions,
				 QObject *parent) : QObject(parent)
{
	this->id = judging.value("id").toString("0").toInt();
	int submissionId = judging.value("submission").toString("0").toInt();
	if (submissions.contains(submissionId)) {
		this->submission = submissions[submissionId];
	} else {
		this->submission = NULL;
	}

	QString outcome = judging.value("outcome").toString("?");
	this->correct = outcome == "correct";
}

int Judging::getId() {
	return this->id;
}

Submission *Judging::getSubmission() {
	return this->submission;
}

bool Judging::isCorrect() {
	return this->correct;
}

} // namespace Model
} // namespace DJ
