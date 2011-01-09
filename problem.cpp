#include "problem.h"

namespace DJ {
namespace Model {
Problem::Problem(QString id, QColor color, QObject *parent) : QObject(parent) {
	this->id = id;
	this->color = color;
}

void Problem::setName(QString name) {
	this->name = name;
}

QString Problem::getId() {
	return this->id;
}

QString Problem::toString() {
	QString s;
	s += "    id = " + this->id + "\n";
	s += "    color = " + this->color.name() + "\n";
	s += "    name  = " + this->name + "\n";
	return s;
}
}
}
