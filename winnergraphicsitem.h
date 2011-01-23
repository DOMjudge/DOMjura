#ifndef WINNERGRAPHICSITEM_H
#define WINNERGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>

namespace DJ {
namespace View {

class WinnerGraphicsItem : public QObject, public QGraphicsItem {
	Q_OBJECT
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	Q_INTERFACES(QGraphicsItem)
public:
	explicit WinnerGraphicsItem(QGraphicsItem *parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);
	void setWinner(QString text);

private:
	QGraphicsSimpleTextItem *textItem1;
	QGraphicsSimpleTextItem *textItem2;
	QString winner;

};

} // namespace View
} // namespace DJ

#endif // WINNERGRAPHICSITEM_H
