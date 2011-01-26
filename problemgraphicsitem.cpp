#include "problemgraphicsitem.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QLinearGradient>
#include <QGraphicsSimpleTextItem>
#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

namespace DJ {
namespace View {
ProblemGraphicsItem::ProblemGraphicsItem(double height, double width,
										 QGraphicsItem *parent)
	: QObject(), QGraphicsItem(parent) {
	this->height = height;
	this->width = width;
	this->state = NOTSUBMITTED;
	this->setCacheMode(DeviceCoordinateCache);
	this->time = 0;
	this->numTries = 0;
	this->highlighted = false;

	textItem = new QGraphicsSimpleTextItem(this);
	QFont font("Courier new", 24);
	font.setBold(true);
	textItem->setFont(font);
}

QRectF ProblemGraphicsItem::boundingRect() const {
	return QRectF(0, 0, width, height);
}

void ProblemGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setClipRect(option->exposedRect);
	QPen pen;
	if (this->highlighted) {
		pen.setColor(QColor(255, 255, 0));
		pen.setWidth(2);
	} else {
		pen.setColor(QColor(192, 192, 192));
		pen.setWidth(1);
	}
	switch (this->state) {
	case NOTSUBMITTED: {
		QBrush brush;
		painter->setPen(pen);
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(QColor(64, 64, 64));
		painter->setBrush(brush);

		textItem->setBrush(QBrush(QColor(192, 192, 192)));
		textItem->setPen(QPen(Qt::black));
		textItem->setText(this->problemId);
		QFontMetrics fm(textItem->font());
		int w = fm.width(textItem->text());
		int h = fm.height();
		textItem->setPos(width - w - 2, height-h+4);

		if (painter->pen().width() == 2) {
			painter->drawRoundedRect(2, 2, width-4, height-4, 5, 5);
		} else {
			painter->drawRoundedRect(1, 1, width-2, height-2, 5, 5);
		}
		break;
	}
	case SOLVED: {
		QLinearGradient gradient(0, 0, 0, height-10);
		gradient.setColorAt(0, QColor(0, 230, 0));
		gradient.setColorAt(1, QColor(0, 128, 0));
		QBrush brush(gradient);
		painter->setPen(pen);
		brush.setStyle(Qt::LinearGradientPattern);
		painter->setBrush(brush);

		textItem->setBrush(QBrush(Qt::white));
		textItem->setPen(QPen(Qt::black));
		textItem->setText(QString::number(this->numTries) + "-" + QString::number(this->time));
		QFontMetrics fm(textItem->font());
		int w = fm.width(textItem->text());
		int h = fm.height();
		textItem->setPos(width/2 - w/2, height/2-h/2);

		if (painter->pen().width() == 2) {
			painter->drawRoundedRect(2, 2, width-4, height-4, 5, 5);
		} else {
			painter->drawRoundedRect(1, 1, width-2, height-2, 5, 5);
		}
		break;
	}
	case FAILED: {
		QLinearGradient gradient(0, 0, 0, height);
		gradient.setColorAt(0, QColor(240, 0, 0));
		gradient.setColorAt(1, QColor(133, 0, 0));
		QBrush brush(gradient);
		painter->setPen(pen);
		brush.setStyle(Qt::LinearGradientPattern);
		painter->setBrush(brush);

		textItem->setBrush(QBrush(Qt::white));
		textItem->setPen(QPen(Qt::black));
		textItem->setText(QString::number(this->numTries) + "-" + QString::number(this->time));
		QFontMetrics fm(textItem->font());
		int w = fm.width(textItem->text());
		int h = fm.height();
		textItem->setPos(width/2 - w/2, height/2-h/2);

		if (painter->pen().width() == 2) {
			painter->drawRoundedRect(2, 2, width-4, height-4, 5, 5);
		} else {
			painter->drawRoundedRect(1, 1, width-2, height-2, 5, 5);
		}
		break;
	}
	case PENDING_FAILED:
	case PENDING_SOLVED:{
		QLinearGradient gradient(0, 0, 0, height-10);

		if (this->highlighted) {
			gradient.setColorAt(0, this->finalColor);
			gradient.setColorAt(1, this->highlightColor);
		} else {
			gradient.setColorAt(0, QColor(255, 223, 54));
			gradient.setColorAt(1, QColor(143, 124, 29));
		}
		QBrush brush(gradient);
		painter->setPen(pen);
		brush.setStyle(Qt::LinearGradientPattern);
		painter->setBrush(brush);

		textItem->setBrush(QBrush(Qt::white));
		textItem->setPen(QPen(Qt::black));
		textItem->setText(QString::number(this->numTries) + "-" + QString::number(this->time));
		QFontMetrics fm(textItem->font());
		int w = fm.width(textItem->text());
		int h = fm.height();
		textItem->setPos(width/2 - w/2, height/2-h/2);

		if (painter->pen().width() == 2) {
			painter->drawRoundedRect(2, 2, width-4, height-4, 5, 5);
		} else {
			painter->drawRoundedRect(1, 1, width-2, height-2, 5, 5);
		}
		break;
	}
	}
}

void ProblemGraphicsItem::setHeight(double height) {
	this->height = height;
	update();
}

void ProblemGraphicsItem::setWidth(double width) {
	this->width = width;
	update();
}

double ProblemGraphicsItem::getWidth() {
	return this->width;
}

void ProblemGraphicsItem::setState(ProblemState state) {
	this->state = state;
	update();
}

void ProblemGraphicsItem::setNumTries(int numTries) {
	this->numTries = numTries;
}

void ProblemGraphicsItem::setTime(int time) {
	this->time = time;
}

void ProblemGraphicsItem::setHighlighted(bool highlighted) {
	this->highlighted = highlighted;
	update();
}

void ProblemGraphicsItem::setProblemId(QString problemId) {
	this->problemId = problemId;
}

void ProblemGraphicsItem::setHighlightColor(QColor color) {
	this->highlightColor = color;
	update();
}

QColor ProblemGraphicsItem::getHighlightColor() {
	return this->highlightColor;
}

void ProblemGraphicsItem::setFinalColor(QColor color) {
	this->finalColor = color;
	update();
}

QColor ProblemGraphicsItem::getFinalColor() {
	return this->finalColor;
}

bool ProblemGraphicsItem::isSolved() {
	return (this->state == SOLVED || this->state == PENDING_SOLVED);
}
}
}
