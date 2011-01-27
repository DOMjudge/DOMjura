/** \file headergraphicsitem.h
  * \brief Contains a header graphics item class.
  */

#ifndef HEADERGRAPHICSITEM_H
#define HEADERGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>

namespace DJ {
namespace View {

/** A header graphics item containing the header texts.
  */
class HeaderGraphicsItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	/** The position of this item.
	  */
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	/** The opacity of this item.
	  */
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	Q_INTERFACES(QGraphicsItem)
public:
	/** Constructs a new header graphics item for the given width.
	  * \param screenWidth How wide this item will be.
	  * \param parent The parent of this item.
	  */
	explicit HeaderGraphicsItem(double screenWidth, QGraphicsItem *parent = 0);
	/** Returns the bounding rectangle for this item.
	  * \return The bounding rectangle for this item.
	  */
	QRectF boundingRect() const;
	/** Paints this item on the scene.
	  * \param painter The painter to use.
	  * \param option The current option information.
	  * \param widget The widget to draw on.
	  */
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

private:
	QGraphicsSimpleTextItem *rankTextItem;
	QGraphicsSimpleTextItem *nameTextItem;
	QGraphicsSimpleTextItem *solvedTextItem;
	QGraphicsSimpleTextItem *timeTextItem;

	int screenWidth;

};

} // namespace View
} // namespace DJ

#endif // HEADERGRAPHICSITEM_H
