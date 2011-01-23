#include "winnergraphicsitem.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QLinearGradient>
#include <QFont>

#include "defines.h"

namespace DJ {
namespace View {

WinnerGraphicsItem::WinnerGraphicsItem(QGraphicsItem *parent) :
	QObject(), QGraphicsItem(parent) {
	this->winner = "n2dg";
	this->textItem1 = new QGraphicsSimpleTextItem(this);
	this->textItem2 = new QGraphicsSimpleTextItem(this);
	QFont font("Courier new", 75);
	font.setBold(true);
	this->textItem1->setFont(font);
	this->textItem2->setFont(font);
	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(3);
	this->textItem1->setPen(pen);
	this->textItem1->setBrush(QBrush(Qt::white));
	this->textItem2->setPen(pen);
	this->textItem2->setBrush(QBrush(Qt::white));
}

QRectF WinnerGraphicsItem::boundingRect() const {
	QRect screenSize = QApplication::desktop()->screenGeometry();
	return QRectF(0, 0, screenSize.width(), screenSize.height());
}

void WinnerGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
							   QWidget *widget) {
	QLinearGradient gradient(0, 0, 0, this->boundingRect().height());
	gradient.setColorAt(0, QColor(111, 81, 19));
	gradient.setColorAt(0.5, QColor(251, 247, 200));
	gradient.setColorAt(1, QColor(114, 84, 22));
	QBrush brush(gradient);
	brush.setStyle(Qt::LinearGradientPattern);
	painter->setBrush(brush);
	painter->setPen(Qt::NoPen);
	painter->drawRect(this->boundingRect());
	this->textItem1->setText(WINNER_TEXT);
	this->textItem2->setText(this->winner);
	QFontMetrics fm(this->textItem1->font());
	int centerHeight = this->boundingRect().height() / 2;
	int centerWidth = this->boundingRect().width() / 2;
	this->textItem1->setPos(centerWidth - fm.width(this->textItem1->text()) / 2, centerHeight - fm.height());
	this->textItem2->setPos(centerWidth - fm.width(this->textItem2->text()) / 2, centerHeight);
}

void WinnerGraphicsItem::setWinner(QString text) {
	this->winner = text;
}

} // namespace View
} // namespace DJ
