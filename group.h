/** \file group.h
  * \brief Contains a class for a group.
  */

#ifndef GROUP_H
#define GROUP_H

#include <QObject>
#include <QHash>

namespace DJ {
namespace Model {

class Team;

/** A group (from the API).
  */
class Group : public QObject
{
    Q_OBJECT
public:
    /** Constructs a new group.
          * \param group The team as returned from the DOMjudge API
          * \param parent The parent of this object
          */
        explicit Group(QJsonObject group, QObject *parent = nullptr);
        /** Returns the name of this group.
          * \return The name of this group.
          */
        QString getName();
        /** Returns the ID of this group.
          * \return The ID of this group.
          */
        QString getId();
        /** Returns the color of this group.
          * \return The color of this group.
          */
        QString getColor();
        /** Returns a string representing this group.
          * \return A string representation of this group.
          * Useful for debug printing.
          */
        QString toString();

        /**
         * Add a team to this group
         * \param team The team to add
         */
        void addTeam(Team *team);

        /**
         * @brief Returns the number of teams in this group
         * @return The number of teams in this group
         */
        int numTeams();

    private:
        QString id;
        QString name;
        QString color;
        QHash<QString, Team *> teams;
};
}
}

#endif // GROUP_H
