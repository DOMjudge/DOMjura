#include "problem.h"

#include <QJsonObject>

namespace DJ {
namespace Model {
Problem::Problem(QJsonObject problem, QObject *parent) : QObject(parent) {
    this->id = problem.value("id").toString();
    this->name = problem.value("name").toString("UNKNOWN");
    this->shortname = problem.value("short_name").toString("?");
    this->color = problem.value("color").toString();
    this->rgb = problem.value("rgb").toString();
}

QString Problem::getId() {
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

QString Problem::getRGB() {
    return this->rgb;
}

QString Problem::toString() {
    QString s;
    s += "       id  = " + this->id + "\n";
    s += "     name  = " + this->name + "\n";
    s += "shortname  = " + this->shortname + "\n";
    s += "    color  = " + this->color + "\n";
    s += "    rgb    = " + this->rgb + "\n";
    return s;
}
}
}
