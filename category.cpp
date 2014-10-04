#include "category.h"

#include <QJsonObject>

#include "team.h"

namespace DJ {
namespace Model {
Category::Category(QJsonObject category, QObject *parent) : QObject(parent) {
	this->id = category.value("categoryid").toString("0").toInt();
	this->color = category.value("color").toString("");
	this->name = category.value("name").toString("UNKNOWN");
}

QString Category::getName() {
	return this->name;
}

int Category::getId() {
	return this->id;
}

QString Category::getColor() {
	return this->color;
}

void Category::addTeam(Team *team) {
	this->teams[team->getId()] = team;
}

int Category::numTeams() {
	return this->teams.size();
}

QString Category::toString() {
	QString s;
	s += "    id    = " + QString::number(this->id) + "\n";
	s += "    color = " + this->color + "\n";
	s += "    name  = " + this->name + "\n";
	return s;
}

}
}
