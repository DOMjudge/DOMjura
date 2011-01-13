#include "teamgraphicsitem.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLinearGradient>

#include "defines.h"

#include <QPen>
#include <QBrush>
#include <QPainter>

namespace DJ {
namespace View {

TeamGraphicsItem::TeamGraphicsItem(QList<ProblemGraphicsItem *> problemItems, QGraphicsItem *parent)
	: QGraphicsItem(parent) {
	this->screenWidth = QApplication::desktop()->screenGeometry().width();
	this->problemItems = problemItems;

	QFont font("Courier new", 24);
	font.setBold(true);

	QFont italicFont("Courier new", 24);
	italicFont.setItalic(true);
	italicFont.setBold(true);

	QFontMetrics fm(font);
	QFontMetrics fmi(italicFont);

	for (int i = 0; i < this->problemItems.size(); i++) {
		this->problemItems.at(i)->setParentItem(this);
		this->problemItems.at(i)->setPos(LEFT_MARGIN + RANK_WIDTH +
										 (i * PROB_MARGIN) +
										 (i * this->problemItems.at(0)->getWidth()), fm.height() + NAME_PROBS_MARGIN);
		this->problemItems.at(i)->setHeight(TEAMITEM_HEIGHT - qMax(fm.height(), fmi.height()) - NAME_PROBS_MARGIN - PROBS_BELOW_MARGIN);
	}

	this->rankItem = new QGraphicsSimpleTextItem(this);
	this->rankItem->setPos(LEFT_MARGIN, 0);
	this->rankItem->setPen(QPen(Qt::black));
	this->rankItem->setBrush(QBrush(Qt::white));
	this->rankItem->setFont(italicFont);

	this->nameItem = new QGraphicsSimpleTextItem(this);
	this->nameItem->setPos(LEFT_MARGIN + RANK_WIDTH, 0);
	this->nameItem->setPen(QPen(Qt::black));
	this->nameItem->setBrush(QBrush(Qt::white));
	this->nameItem->setFont(font);

	this->solvedItem = new QGraphicsSimpleTextItem(this);
	this->solvedItem->setPos(LEFT_MARGIN + RANK_WIDTH + NAME_WIDTH, 0);
	this->solvedItem->setPen(QPen(Qt::black));
	this->solvedItem->setBrush(QBrush(Qt::white));
	this->solvedItem->setFont(italicFont);

	this->timeItem = new QGraphicsSimpleTextItem(this);
	this->timeItem->setPos(LEFT_MARGIN + RANK_WIDTH + NAME_WIDTH + SOLVED_WIDTH, 0);
	this->timeItem->setPen(QPen(Qt::black));
	this->timeItem->setBrush(QBrush(Qt::white));
	this->timeItem->setFont(font);
}

QRectF TeamGraphicsItem::boundingRect() const {
	return QRectF(0, 0, screenWidth, TEAMITEM_HEIGHT - 1);
}

void TeamGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
							 QWidget *widget) {
	if (even) {
		QLinearGradient gradient(0, 0, screenWidth, 0);
		gradient.setColorAt(0, QColor(30, 30, 30));
		gradient.setColorAt(0.5, QColor(86, 86, 86));
		gradient.setColorAt(1, QColor(30, 30, 30));
		QBrush brush(gradient);
		brush.setStyle(Qt::LinearGradientPattern);
		painter->setBrush(brush);
		painter->setPen(Qt::NoPen);
		painter->drawRect(0, 0, screenWidth, TEAMITEM_HEIGHT);
	}
}

void TeamGraphicsItem::setEven(bool even) {
	this->even = even;
}

void TeamGraphicsItem::setRank(int rank) {
	if (rank <= 0) {
		this->rankItem->setText("");
	} else {
		this->rankItem->setText(QString::number(rank));
	}
}

void TeamGraphicsItem::setName(QString name) {
	this->nameItem->setText(name);
}

void TeamGraphicsItem::setSolved(int solved) {
	QString txt = QString::number(solved);
	QFontMetrics fm(this->solvedItem->font());
	int fw = fm.width(txt);
	if (solved < 0) {
		this->solvedItem->setText("");
	} else {
		this->solvedItem->setText(txt);
	}
	this->solvedItem->setPos(LEFT_MARGIN + RANK_WIDTH + NAME_WIDTH + SOLVED_WIDTH - fw, 0);
}

void TeamGraphicsItem::setTime(int time) {
	QString txt = QString::number(time);
	QFontMetrics fm(this->timeItem->font());
	int fw = fm.width(txt);
	if (time < 0) {
		this->timeItem->setText("");
	} else {
		this->timeItem->setText(txt);
	}
	this->timeItem->setPos(LEFT_MARGIN + RANK_WIDTH + NAME_WIDTH + SOLVED_WIDTH + TIME_WIDTH - fw, 0);
}

} // namespace View
} // namespace DJ
