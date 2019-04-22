#include "problemgraphicsitem.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QLinearGradient>
#include <QGraphicsSimpleTextItem>
#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

#include "gradientcache.h"

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
    QFont font("Courier new");
    font.setPixelSize(24);
    font.setBold(true);
    textItem->setFont(font);
}

QRectF ProblemGraphicsItem::boundingRect() const {
    return QRectF(0, 0, width, height);
}

void ProblemGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
    painter->setClipRect(option->exposedRect);
    switch (this->state) {
    case NOTSUBMITTED: {
        textItem->setBrush(QBrush(QColor(192, 192, 192)));
        textItem->setPen(QPen(Qt::black));
        textItem->setText(this->problemId);
        QFontMetrics fm(textItem->font());
        int w = fm.width(textItem->text());
        int h = fm.height();
        textItem->setPos(width - w - 2, height-h+4);

        painter->drawPixmap(0, 0, width, height, GradientCache::getInstance()->getColorGradient(QColor(64, 64, 64), QColor(64, 64, 64)));
        break;
    }
    case SOLVED: {
        textItem->setBrush(QBrush(Qt::white));
        textItem->setPen(QPen(Qt::black));
        textItem->setText(QString::number(this->numTries) + "-" + QString::number(this->time));
        QFontMetrics fm(textItem->font());
        int w = fm.width(textItem->text());
        int h = fm.height();
        textItem->setPos(width/2 - w/2, height/2-h/2);
        if (this->highlighted) {
            painter->drawPixmap(0, 0, width, height, GradientCache::getInstance()->getColorGradientHighlighted(QColor(0, 230, 0), QColor(0, 128, 0)));
        } else {
            painter->drawPixmap(0, 0, width, height, GradientCache::getInstance()->getColorGradient(QColor(0, 230, 0), QColor(0, 128, 0)));
        }
        break;
    }
    case FAILED: {
        textItem->setBrush(QBrush(Qt::white));
        textItem->setPen(QPen(Qt::black));
        textItem->setText(QString::number(this->numTries) + "-" + QString::number(this->time));
        QFontMetrics fm(textItem->font());
        int w = fm.width(textItem->text());
        int h = fm.height();
        textItem->setPos(width/2 - w/2, height/2-h/2);
        if (this->highlighted) {
            painter->drawPixmap(0, 0, width, height, GradientCache::getInstance()->getColorGradientHighlighted(QColor(240, 0, 0), QColor(133, 0, 0)));
        } else {
            painter->drawPixmap(0, 0, width, height, GradientCache::getInstance()->getColorGradient(QColor(240, 0, 0), QColor(133, 0, 0)));
        }
        break;
    }
    case PENDING_FAILED:
    case PENDING_SOLVED:{
        textItem->setBrush(QBrush(Qt::white));
        textItem->setPen(QPen(Qt::black));
        textItem->setText(QString::number(this->numTries) + "-" + QString::number(this->time));
        QFontMetrics fm(textItem->font());
        int w = fm.width(textItem->text());
        int h = fm.height();
        textItem->setPos(width/2 - w/2, height/2-h/2);
        if (this->highlighted) {
            painter->drawPixmap(0, 0, width, height, GradientCache::getInstance()->getColorGradientHighlighted(this->finalColor, this->highlightColor));
        } else {
            painter->drawPixmap(0, 0, width, height, GradientCache::getInstance()->getColorGradient(QColor(255, 223, 54), QColor(143, 124, 29)));
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
