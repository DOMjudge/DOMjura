#include "teamgraphicsitem.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLinearGradient>
#include <QStyleOptionGraphicsItem>

#include "gradientcache.h"
#include "defines.h"

#include <QPen>
#include <QBrush>
#include <QPainter>

namespace DJ {
namespace View {

TeamGraphicsItem::TeamGraphicsItem(QList<ProblemGraphicsItem *> problemItems, QGraphicsItem *parent)
	: QObject(), QGraphicsItem(parent) {
	this->screenWidth = QApplication::desktop()->screenGeometry().width();
	this->problemItems = problemItems;
	this->highlighted = false;
	this->setCacheMode(DeviceCoordinateCache);

	QFont font("DejaVu Sans Mono");
	font.setPixelSize(24);
	font.setBold(true);

	QFont italicFont("DejaVu Sans Mono");
	italicFont.setPixelSize(24);
	italicFont.setItalic(true);
	italicFont.setBold(true);

	QFontMetrics fm(font);
	QFontMetrics fmi(italicFont);

	for (int i = 0; i < this->problemItems.size(); i++) {
		this->problemItems.at(i)->setParentItem(this);
		this->problemItems.at(i)->setPos(LEFT_MARGIN + RANK_WIDTH +
										 (i * PROB_MARGIN) +
										 (i * this->problemItems.at(0)->getWidth()), fm.height() + NAME_PROBS_MARGIN);
		GradientCache::getInstance()->setHeight(TEAMITEM_HEIGHT - qMax(fm.height(), fmi.height()) - NAME_PROBS_MARGIN - PROBS_BELOW_MARGIN);
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
	return QRectF(0, 0, screenWidth, TEAMITEM_HEIGHT);
}

ProblemGraphicsItem *TeamGraphicsItem::getProblemGraphicsItem(int i) {
	return this->problemItems.at(i);
}

void TeamGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
							 QWidget *) {
	painter->setClipRect(option->exposedRect);
	if (this->highlighted) {
		painter->drawPixmap(0, 0, screenWidth, TEAMITEM_HEIGHT,
							GradientCache::getInstance()->getOddEvenHighlightedGradient(2));
	} else if (this->medal == NO_MEDAL) {
		painter->drawPixmap(0, 0, screenWidth, TEAMITEM_HEIGHT,
							GradientCache::getInstance()->getOddEvenHighlightedGradient(this->even ? 0 : 1));
	} else {
		painter->drawPixmap(0, 0, screenWidth, TEAMITEM_HEIGHT,
							GradientCache::getInstance()->getMedalGradient(this->medal));
	}
}

void TeamGraphicsItem::setEven(bool even) {
	update();
	this->even = even;
}

void TeamGraphicsItem::setHighlighted(bool highlighted) {
	update();
	this->highlighted = highlighted;
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

void TeamGraphicsItem::setMedal(Medal medal) {
	update();
	this->medal = medal;
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
