#ifndef LEGENDAGRAPHICSITEM_H
#define LEGENDAGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QObject>

namespace DJ {
namespace View {

class LegendaGraphicsItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	Q_INTERFACES(QGraphicsItem)
public:
	LegendaGraphicsItem(QGraphicsItem *parent = NULL);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

private:
	QGraphicsSimpleTextItem *solvedText;
	QGraphicsSimpleTextItem *rejectedText;
	QGraphicsSimpleTextItem *pendingText;
};

} // namespace View
} // namespace DJ

#endif // LEGENDAGRAPHICSITEM_H
