/** \file problem.h
  * \brief Contains the class for a problem.
  */

#ifndef PROBLEM_H
#define PROBLEM_H

#include <QObject>

namespace DJ {
namespace Model {
/** A problem (from the API).
  */
class Problem : public QObject {
    Q_OBJECT
public:
    /** Constructs a new problem.
      * \param problem The problem as returned from the DOMjudge API
      * \param parent The parent of this object.
      */
    explicit Problem(QJsonObject problem, QObject *parent = 0);
    /** Returns the ID of this problem.
      * \return The ID of this problem.
      */
    QString getId();
    /** Returns the name of this problem.
      * \return The name of this problem.
      */
    QString getName();
    /** Returns the short name of this problem.
      * \return The short name of this problem.
      */
    QString getShortName();
    /** Returns the color of this problem.
      * \return The color of this problem.
      */
    QString getColor();
    /** Returns the rgb of this problem.
      * \return The rgb of this problem.
      */
    QString getRGB();
    /** Returns a string representing this problem.
      * \return A string representation of this problem.
      * Useful for debug printing.
      */
    QString toString();

private:
    QString id;
    QString name;
    QString shortname;
    QString color;
    QString rgb;
};
}
}

#endif // PROBLEM_H
