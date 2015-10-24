#include "contest.h"

#include <QDebug>

namespace DJ {
namespace Model {
Contest::Contest(QJsonObject contest, QObject *parent) : QObject(parent) {
	this->id = contest.value("id").toInt(0);
	this->name = contest.value("name").toString("Unknown");
	this->penaltyMinutes = contest.value("penalty").toInt(0) / 60;

	this->start = QDateTime::fromTime_t(qRound(contest.value("start").toDouble(0)));
	this->freeze = QDateTime::fromTime_t(qRound(contest.value("freeze").toDouble(0)));
	this->end = QDateTime::fromTime_t(qRound(contest.value("end").toDouble(0)));
	this->unfreeze = QDateTime::fromTime_t(qRound(contest.value("unfreeze").toDouble(0)));
}

Contest::~Contest() {

}

void Contest::setName(QString name) {
	this->name = name;
}

QDateTime Contest::getStart() {
	return this->start;
}

QDateTime Contest::getFreeze() {
	return this->freeze;
}

QDateTime Contest::getUnfreeze() {
	return this->unfreeze;
}

QDateTime Contest::getEnd() {
	return this->end;
}

int Contest::getId() {
	return this->id;
}

QString Contest::getName() {
	return this->name;
}

int Contest::getPenaltyMinutes() {
	return this->penaltyMinutes;
}

QString Contest::toString() {
	QString s;
	s += "  id       = " + QString::number(this->id) + "\n";
	s += "  start    = " + this->start.toString("yyyy-MM-dd hh:mm:ss") + "\n";
	s += "  freeze   = " + this->freeze.toString("yyyy-MM-dd hh:mm:ss") + "\n";
	s += "  end      = " + this->end.toString("yyyy-MM-dd hh:mm:ss") + "\n";
	s += "  unfreeze = " + this->unfreeze.toString("yyyy-MM-dd hh:mm:ss") + "\n";
	s += "  penalty  = " + QString::number(this->penaltyMinutes) + "\n";
	s += "  name     = " + this->name + "\n";
	return s;
}
}
}
