/** \file category.h
  * \brief Contains a class for a category.
  */

#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QHash>

namespace DJ {
namespace Model {

class Team;

/** A category (from the API).
  */
class Category : public QObject
{
	Q_OBJECT
public:
	/** Constructs a new category.
	  * \param category The team as returned from the DOMjudge API
	  * \param parent The parent of this object
	  */
	explicit Category(QJsonObject category, QObject *parent = 0);
	/** Returns the name of this category.
	  * \return The name of this category.
	  */
	QString getName();
	/** Returns the ID of this category.
	  * \return The ID of this category.
	  */
	int getId();
	/** Returns the color of this category.
	  * \return The color of this category.
	  */
	QString getColor();
	/** Returns a string representing this category.
	  * \return A string representation of this category.
	  * Useful for debug printing.
	  */
	QString toString();

	/**
	 * Add a team to this category
	 * \param team The team to add
	 */
	void addTeam(Team *team);

	/**
	 * @brief Returns the number of teams in this category
	 * @return The number of teams in this category
	 */
	int numTeams();

private:
	int id;
	QString color;
	QString name;
	QHash<int, Team *> teams;
};
}
}

#endif // CATEGORY_H
