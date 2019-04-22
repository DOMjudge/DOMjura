/** \file contest.h
  * \brief Contains a class for a contest.
  */

#ifndef CONTEST_H
#define CONTEST_H

#include <QJsonObject>
#include <QObject>
#include <QDateTime>

namespace DJ {
namespace Model {
/** A contest (from the API).
  */
class Contest : public QObject {
    Q_OBJECT
public:
    /** Constructs a new contest.
      * \param contest The contest as returned from the DOMjudge API
      * \param parent The parent of this object.
      */
    explicit Contest(QJsonObject contest, QObject *parent = 0);
    /** Destructs this contest.
      */
    ~Contest();

    /** Sets the name for this contest.
      * \param name The name to set.
      */
    void setName(QString name);

    /** Returns the start time of the contest.
      * \return The start time of the contest.
      */
    QDateTime getStart();
    /** Returns the freeze time of the contest.
      * \return The freeze time of the contest.
      */
    QDateTime getFreeze();
    /** Returns the end time of the contest.
      * \return The end time of the contest.
      */
    QDateTime getEnd();
    /** Returns the ID of this contest.
      * \return The ID of this contest.
      */
    QString getId();
    /** Returns the name of this contest.
      * \return The name of this contest.
      */
    QString getName();
    /** Returns the penalty time in minutes.
      * \return The penalty time in minutes.
      */
    int getPenaltyMinutes();

    /** Returns a string representing this contest.
      * \return A string representation of this contest.
      * Useful for debug printing.
      */
    QString toString();

private:
    QString id;
    QString name;
    QDateTime start_time, end_time, freeze_time;
    int penaltyMinutes;
};
}
}
#endif // CONTEST_H
