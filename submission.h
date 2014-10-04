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
						QHash<int, Team *> teams,
						QHash<int, Problem *> problems,
						QObject *parent = 0);

	int getId();
	Problem *getProblem();
	Team *getTeam();
	QDateTime getTime();
	bool inTime(Contest *contest);
	bool inFreeze(Contest *contest);

signals:

public slots:

private:
	int id;
	Problem *problem;
	Team *team;
	QDateTime time;

};

} // namespace Model
} // namespace DJ

#endif // DJ_MODEL_SUBMISSION_H
