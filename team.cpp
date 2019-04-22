#include "team.h"

#include <QJsonObject>

namespace DJ {
namespace Model {
Team::Team(QJsonObject team, QObject *parent) : QObject(parent) {
    this->id = team.value("id").toString();
    this->name = team.value("name").toString("UNKNOWN");
    this->affilation = team.value("affilation").toString("UNKNOWN");
    this->nationality = team.value("nationality").toString("UNKNOWN");
}

QString Team::getId() {
    return this->id;
}

QString Team::getName() {
    return this->name;
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
    return s;
}
}
}
