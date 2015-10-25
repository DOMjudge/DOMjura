#include "problem.h"

#include <QJsonObject>

namespace DJ {
namespace Model {
Problem::Problem(QJsonObject problem, QObject *parent) : QObject(parent) {
	this->id = problem.value("id").toInt();
	this->color = problem.value("color").toString();
	this->name = problem.value("name").toString("UNKNOWN");
	this->shortname = problem.value("shortname").toString("?");
}

int Problem::getId() {
	return this->id;
}

QString Problem::getName() {
	return this->name;
}

QString Problem::getShortName() {
	return this->shortname;
}

QString Problem::getColor() {
	return this->color;
}

QString Problem::toString() {
	QString s;
	s += "       id  = " + QString::number(this->id) + "\n";
	s += "    color  = " + this->color + "\n";
	s += "     name  = " + this->name + "\n";
	s += "shortname  = " + this->shortname + "\n";
	return s;
}
}
}
