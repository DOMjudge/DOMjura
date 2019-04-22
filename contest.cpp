#include "contest.h"

#include <QDebug>

namespace DJ {
namespace Model {
Contest::Contest(QJsonObject contest, QObject *parent) : QObject(parent) {
    this->id = contest.value("id").toString();
    this->name = contest.value("name").toString("Unknown");
    this->penaltyMinutes = contest.value("penalty_time").toInt();

    this->start_time = QDateTime::fromString(contest.value("start_time").toString(), Qt::DateFormat::ISODate);
    this->end_time = QDateTime::fromString(contest.value("end_time").toString(), Qt::DateFormat::ISODate);
    this->freeze_time = QDateTime(this->end_time);

    QString freeze_duration = contest.value("scoreboard_freeze_duration").toString();
    freeze_duration = freeze_duration.size() < 12 ? "0" + freeze_duration : freeze_duration;
    QTime freeze = QTime::fromString(freeze_duration, Qt::ISODateWithMs);
    this->freeze_time.setTime(QTime(this->freeze_time.time().hour()-freeze.hour(),
                                    this->freeze_time.time().minute()-freeze.minute(),
                                    this->freeze_time.time().second()-freeze.second()));
}

Contest::~Contest() {

}

QString Contest::getId() {
    return this->id;
}

void Contest::setName(QString name) {
    this->name = name;
}

QDateTime Contest::getStart() {
    return this->start_time;
}

QDateTime Contest::getFreeze() {
    return this->freeze_time;
}

QDateTime Contest::getEnd() {
    return this->end_time;
}

QString Contest::getName() {
    return this->name;
}

int Contest::getPenaltyMinutes() {
    return this->penaltyMinutes;
}

QString Contest::toString() {
    QString s;
    s += "  id       = " + this->id + "\n";
    s += "  start    = " + this->start_time.toString("yyyy-MM-dd hh:mm:ss") + "\n";
    s += "  freeze   = " + this->freeze_time.toString("yyyy-MM-dd hh:mm:ss") + "\n";
    s += "  end      = " + this->end_time.toString("yyyy-MM-dd hh:mm:ss") + "\n";
    s += "  penalty  = " + QString::number(this->penaltyMinutes) + "\n";
    s += "  name     = " + this->name + "\n";
    return s;
}
}
}
