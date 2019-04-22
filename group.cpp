#include "group.h"

#include <QJsonObject>

#include "team.h"

namespace DJ {
namespace Model {
Group::Group(QJsonObject group, QObject *parent) : QObject(parent) {
    this->id = group.value("id").toString();
    this->name = group.value("name").toString("UNKNOWN");
    this->color = group.value("color").toString("");
}

QString Group::getName() {
    return this->name;
}

QString Group::getId() {
    return this->id;
}

QString Group::getColor() {
    return this->color;
}

void Group::addTeam(Team *team) {
    this->teams[team->getId()] = team;
}

int Group::numTeams() {
    return this->teams.size();
}

QString Group::toString() {
    QString s;
    s += "    id    = " + this->id + "\n";
    s += "    color = " + this->color + "\n";
    s += "    name  = " + this->name + "\n";
    return s;
}

}
}
