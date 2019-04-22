#ifndef DJ_MODEL_JUDGING_H
#define DJ_MODEL_JUDGING_H

#include <QObject>

namespace DJ {
namespace Model {

class Submission;

class Judging : public QObject
{
    Q_OBJECT
public:
    explicit Judging(QJsonObject judging,
                     QHash<QString, Submission *> submissions,
                     QObject *parent = nullptr);

    int getId();
    Submission *getSubmission();
    bool isCorrect();
signals:

public slots:

private:
    int id;
    Submission *submission;
    bool correct;
};

} // namespace Model
} // namespace DJ

#endif // DJ_MODEL_JUDGING_H
