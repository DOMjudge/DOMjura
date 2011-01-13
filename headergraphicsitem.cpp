#include "headergraphicsitem.h"

#include <QBrush>
#include <QPainter>

namespace DJ {
namespace View {

HeaderGraphicsItem::HeaderGraphicsItem(double screenWidth, QGraphicsItem *parent)
	: QGraphicsItem(parent) {
	this->screenWidth = screenWidth;
	this->lineItem = new QGraphicsLineItem(0, 30, screenWidth, 30, this);
	this->lineItem->setPen(QPen(Qt::white));
}

QRectF HeaderGraphicsItem::boundingRect() const {
	return QRectF(0, 0, screenWidth, 30);
}

void HeaderGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
}

} // namespace View
} // namespace DJ
