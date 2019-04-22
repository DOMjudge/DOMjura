#include "team.h"

#include <QJsonObject>
#include <QJsonArray>

#include "group.h"

namespace DJ {
namespace Model {
Team::Team(QJsonObject team, QHash<QString, Group *> groups, QObject *parent) : QObject(parent) {
    this->id = team.value("id").toString();
    this->name = team.value("name").toString("UNKNOWN");
    this->affilation = team.value("affilation").toString("UNKNOWN");
    this->nationality = team.value("nationality").toString("UNKNOWN");
    QJsonArray groups_ids = team.value("group_ids").toArray();
    QString groupId = groups_ids[0].toString();
    if (groups.contains(groupId)) {
        this->group = groups[groupId];
        this->group->addTeam(this);
    } else {
        this->group = nullptr;
    }
}

QString Team::getId() {
    return this->id;
}

QString Team::getName() {
    return this->name;
}

Group *Team::getGroup() {
    return this->group;
}

QString Team::getAffilation() {
    return this->affilation;
}

QString Team::getNationality() {
    return this->nationality;
}

QString Team::toString() {
    QString s;
    s += "    id          = " + this->id + "\n";
    s += "    name        = " + this->name + "\n";
    s += "    affilation  = " + this->affilation + "\n";
    s += "    nationality = " + this->nationality + "\n";
    s += "    group       = " + this->group->getId() + " (" + this->group->getName() + ")\n";
    return s;
}
}
}
