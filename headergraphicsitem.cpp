#include "headergraphicsitem.h"

#include <QBrush>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyleOptionGraphicsItem>

#include "defines.h"

namespace DJ {
namespace View {

HeaderGraphicsItem::HeaderGraphicsItem(double screenWidth, QGraphicsItem *parent)
	: QObject(), QGraphicsItem(parent){
	this->screenWidth = screenWidth;

	QFont font("Courier new");
	font.setPixelSize(16);
	font.setItalic(true);

	QFontMetrics fm(font);

	this->setCacheMode(DeviceCoordinateCache);

	this->rankTextItem = new QGraphicsSimpleTextItem("Rank", this);
	this->rankTextItem->setPos(LEFT_MARGIN, HEADER_HEIGHT - fm.height());
	this->rankTextItem->setPen(QPen(Qt::white));
	this->rankTextItem->setBrush(QBrush(Qt::white));
	this->rankTextItem->setFont(font);

	this->nameTextItem = new QGraphicsSimpleTextItem("Name", this);
	this->nameTextItem->setPos(LEFT_MARGIN + RANK_WIDTH, HEADER_HEIGHT - fm.height());
	this->nameTextItem->setPen(QPen(Qt::white));
	this->nameTextItem->setBrush(QBrush(Qt::white));
	this->nameTextItem->setFont(font);

	this->timeTextItem = new QGraphicsSimpleTextItem("Time", this);
	this->timeTextItem->setPos(QApplication::desktop()->screenGeometry().width() - RIGHT_MARGIN - fm.width("Time"), HEADER_HEIGHT - fm.height());
	this->timeTextItem->setPen(QPen(Qt::white));
	this->timeTextItem->setBrush(QBrush(Qt::white));
	this->timeTextItem->setFont(font);

	this->solvedTextItem = new QGraphicsSimpleTextItem("Solved", this);
	this->solvedTextItem->setPos(QApplication::desktop()->screenGeometry().width() - RIGHT_MARGIN - TIME_WIDTH - fm.width("Solved"), HEADER_HEIGHT - fm.height());
	this->solvedTextItem->setPen(QPen(Qt::white));
	this->solvedTextItem->setBrush(QBrush(Qt::white));
	this->solvedTextItem->setFont(font);
}

QRectF HeaderGraphicsItem::boundingRect() const {
	return QRectF(0, 0, screenWidth, HEADER_HEIGHT);
}

void HeaderGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setClipRect(option->exposedRect);
	QFont font("Courier new");
	font.setPixelSize(16);
	font.setItalic(true);

	QFontMetrics fm(font);

	this->rankTextItem->setPos(LEFT_MARGIN, HEADER_HEIGHT - fm.height());
	this->nameTextItem->setPos(LEFT_MARGIN + RANK_WIDTH, HEADER_HEIGHT - fm.height());
	this->timeTextItem->setPos(QApplication::desktop()->screenGeometry().width() - RIGHT_MARGIN - fm.width("Time"), HEADER_HEIGHT - fm.height());
	this->solvedTextItem->setPos(QApplication::desktop()->screenGeometry().width() - RIGHT_MARGIN - TIME_WIDTH - fm.width("Solved"), HEADER_HEIGHT - fm.height());

	QLinearGradient gradient(0, 0, screenWidth, 0);
	gradient.setColorAt(0, QColor(0, 0, 0));
	gradient.setColorAt(0.5, QColor(56, 56, 56));
	gradient.setColorAt(1, QColor(0, 0, 0));
	QBrush brush(gradient);
	brush.setStyle(Qt::LinearGradientPattern);
	painter->setBrush(brush);
	painter->setPen(Qt::NoPen);
	painter->drawRect(0, 0, screenWidth, HEADER_HEIGHT);
	painter->setPen(Qt::white);
	painter->drawLine(0, HEADER_HEIGHT-1, screenWidth, HEADER_HEIGHT-1);
}

} // namespace View
} // namespace DJ
