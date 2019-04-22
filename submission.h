#ifndef DJ_MODEL_SUBMISSION_H
#define DJ_MODEL_SUBMISSION_H

#include <QObject>
#include <QDateTime>

namespace DJ {
namespace Model {
class Problem;
class Team;
class Contest;

class Submission : public QObject
{
    Q_OBJECT
public:
    explicit Submission(QJsonObject submission,
                        QHash<QString, Team *> teams,
                        QHash<QString, Problem *> problems,
                        QObject *parent = nullptr);

    QString getId();
    Problem *getProblem();
    Team *getTeam();
    QDateTime getTime();
    bool inTime(Contest *contest);
    bool inFreeze(Contest *contest);

signals:

public slots:

private:
    QString id;
    Problem *problem;
    Team *team;
    QDateTime time;

};

} // namespace Model
} // namespace DJ

#endif // DJ_MODEL_SUBMISSION_H
