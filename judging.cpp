#include "judging.h"

#include <QJsonObject>

#include "submission.h"

namespace DJ {
namespace Model {

Judging::Judging(QJsonObject judging,
                 QHash<QString, Submission *> submissions,
                 QObject *parent) : QObject(parent)
{
    this->id = judging.value("id").toInt();
    QString submissionId = judging.value("submission_id").toString();
    if (submissions.contains(submissionId)) {
        this->submission = submissions[submissionId];
    } else {
        this->submission = nullptr;
    }

    QString outcome = judging.value("judgement_type_id").toString("?");
    this->correct = outcome == "AC";
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
