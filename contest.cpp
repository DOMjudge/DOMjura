#include "contest.h"

namespace DJ {
namespace Model {
Contest::Contest(QDateTime start, QDateTime end, QDateTime freeze, int id, QObject *parent) : QObject(parent) {
	this->start = start;
	this->end = end;
	this->freeze = freeze;
	this->id = id;
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

QDateTime Contest::getEnd() {
	return this->end;
}

QString Contest::toString() {
	QString s;
	s += "  id     = " + QString::number(this->id) + "\n";
	s += "  start  = " + this->start.toString("yyyy-MM-dd hh:mm:ss") + "\n";
	s += "  freeze = " + this->freeze.toString("yyyy-MM-dd hh:mm:ss") + "\n";
	s += "  end    = " + this->end.toString("yyyy-MM-dd hh:mm:ss") + "\n";
	s += "  name   = " + this->name + "\n";
	return s;
}
}
}
