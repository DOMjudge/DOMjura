#include "problemgraphicsitem.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QLinearGradient>
#include <QGraphicsSimpleTextItem>
#include <QFontMetrics>

namespace BAPC {
	ProblemGraphicsItem::ProblemGraphicsItem(double height, double width) {
		this->height = height;
		this->width = width;
		this->state = NOTSUBMITTED;
		this->time = 0;
		this->numTries = 0;

		textItem = new QGraphicsSimpleTextItem(this);
		QFont font("Courier new", 24);
		font.setBold(true);
		textItem->setFont(font);
	}

	QRectF ProblemGraphicsItem::boundingRect() const {
		return QRectF(0, 0, width, height);
	}

	void ProblemGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
		switch (this->state) {
		case NOTSUBMITTED: {
			QBrush brush;
			painter->setPen(QColor(192, 192, 192));
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

			painter->drawRoundedRect(0, 0, width, height, 5, 5);
			break;
		}
		case SOLVED: {
			QLinearGradient gradient(0, 0, 0, height-10);
			gradient.setColorAt(0, QColor(0, 230, 0));
			gradient.setColorAt(1, QColor(0, 128, 0));
			QBrush brush(gradient);
			painter->setPen(QColor(192, 192, 192));
			brush.setStyle(Qt::LinearGradientPattern);
			painter->setBrush(brush);

			textItem->setBrush(QBrush(Qt::white));
			textItem->setPen(QPen(Qt::black));
			textItem->setText(QString::number(this->numTries) + "-" + QString::number(this->time));
			QFontMetrics fm(textItem->font());
			int w = fm.width(textItem->text());
			int h = fm.height();
			textItem->setPos(width/2 - w/2, height/2-h/2);

			painter->drawRoundedRect(0, 0, width, height, 5, 5);
			break;
		}
		case FAILED: {
			QLinearGradient gradient(0, 0, 0, height);
			gradient.setColorAt(0, QColor(240, 0, 0));
			gradient.setColorAt(1, QColor(133, 0, 0));
			QBrush brush(gradient);
			painter->setPen(QColor(192, 192, 192));
			brush.setStyle(Qt::LinearGradientPattern);
			painter->setBrush(brush);

			textItem->setBrush(QBrush(Qt::white));
			textItem->setPen(QPen(Qt::black));
			textItem->setText(QString::number(this->numTries) + "-" + QString::number(this->time));
			QFontMetrics fm(textItem->font());
			int w = fm.width(textItem->text());
			int h = fm.height();
			textItem->setPos(width/2 - w/2, height/2-h/2);

			painter->drawRoundedRect(0, 0, width, height, 5, 5);
			break;
		}
		case PENDING: {
			QLinearGradient gradient(0, 0, 0, height-10);
			gradient.setColorAt(0, QColor(255, 223, 54));
			gradient.setColorAt(1, QColor(143, 124, 29));
			QBrush brush(gradient);
			painter->setPen(QColor(192, 192, 192));
			brush.setStyle(Qt::LinearGradientPattern);
			painter->setBrush(brush);

			textItem->setBrush(QBrush(Qt::white));
			textItem->setPen(QPen(Qt::black));
			textItem->setText(QString::number(this->numTries) + "-" + QString::number(this->time));
			QFontMetrics fm(textItem->font());
			int w = fm.width(textItem->text());
			int h = fm.height();
			textItem->setPos(width/2 - w/2, height/2-h/2);

			painter->drawRoundedRect(0, 0, width, height, 5, 5);
			break;
		}
		}
	}

	void ProblemGraphicsItem::setHeight(double height) {
		this->height = height;
	}

	void ProblemGraphicsItem::setWidth(double width) {
		this->width = width;
	}

	void ProblemGraphicsItem::setState(ProblemState state) {
		this->state = state;
	}

	void ProblemGraphicsItem::setNumTries(int numTries) {
		this->numTries = numTries;
	}

	void ProblemGraphicsItem::setTime(int time) {
		this->time = time;
	}

	void ProblemGraphicsItem::setProblemId(QString problemId) {
		this->problemId = problemId;
	}
}
