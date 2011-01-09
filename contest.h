#ifndef CONTEST_H
#define CONTEST_H

#include <QObject>
#include <QDateTime>

namespace DJ {
namespace Model {
class Contest : public QObject {
	Q_OBJECT
public:
	explicit Contest(QDateTime start, QDateTime end, QDateTime freeze, int id, QObject *parent = 0);
	~Contest();

	void setName(QString name);

	QDateTime getStart();
	QDateTime getFreeze();
	QDateTime getEnd();

	QString toString();

private:
	QString name;
	QDateTime start, end, freeze;
	int id;
};
}
}
#endif // CONTEST_H
