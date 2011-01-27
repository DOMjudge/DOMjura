/** \file category.h
  * \brief Contains a class for a category.
  */

#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QColor>

namespace DJ {
namespace Model {
/** A category (from the scorebaord).
  */
class Category : public QObject
{
	Q_OBJECT
public:
	/** Constructs a new category.
	  * \param id The ID of this category (must be unique).
	  * \param color The color for this category.
	  * \param parent The parent of this object
	  */
	explicit Category(QString id, QColor color, QObject *parent = 0);
	/** Sets the name of this category.
	  * \param name The name to set.
	  */
	void setName(QString name);
	/** Returns the name of this category.
	  * \return The name of this category.
	  */
	QString getName();
	/** Returns the ID of this category.
	  * \return The ID of this category.
	  */
	QString getId();
	/** Returns the color of this category.
	  * \return The color of this category.
	  */
	QColor getColor();
	/** Returns a string representing this category.
	  * \return A string representation of this category.
	  * Useful for debug printing.
	  */
	QString toString();

private:
	QString id;
	QColor color;
	QString name;
};
}
}

#endif // CATEGORY_H
