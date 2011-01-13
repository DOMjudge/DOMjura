#ifndef HEADERGRAPHICSITEM_H
#define HEADERGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>

namespace DJ {
namespace View {

class HeaderGraphicsItem : public QGraphicsItem
{
public:
	explicit HeaderGraphicsItem(double screenWidth, QGraphicsItem *parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

private:
	QGraphicsSimpleTextItem *rankTextItem;
	QGraphicsSimpleTextItem *nameTextItem;
	QGraphicsSimpleTextItem *solvedTextItem;
	QGraphicsSimpleTextItem *timeTextItem;
	QGraphicsLineItem *lineItem;

	int screenWidth;

};

} // namespace View
} // namespace DJ

#endif // HEADERGRAPHICSITEM_H
