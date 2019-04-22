#include "gradientcache.h"

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QBitmap>

namespace DJ {
namespace View {

GradientCache* GradientCache::instance = NULL;

GradientCache::GradientCache() : QObject() {
}

GradientCache *GradientCache::getInstance() {
    if (instance == NULL) {
        instance = new GradientCache;
    }
    return instance;
}

void GradientCache::clearCache() {
    this->problemGradient.clear();
    this->problemGradientHighlighted.clear();
    this->medalGradient.clear();
    this->oddEvenHighlightGradient.clear();
}

void GradientCache::setNumProbs(int n) {
    if (this->numProbs != n) {
        this->numProbs = n;
        this->problemGradient.clear();
        this->problemGradientHighlighted.clear();
    }
}

void GradientCache::setHeight(int height) {
    if (this->height != height) {
        this->height = height;
        this->problemGradient.clear();
        this->problemGradientHighlighted.clear();
    }
}

QPixmap GradientCache::getColorGradient(QColor color1, QColor color2) {
    ColorPair colorPair(color1, color2);
    if (!this->problemGradient.contains(colorPair)) {
        QPen pen;
        pen.setWidth(1);
        pen.setColor(QColor(192, 192, 192));
        int width = (NAME_WIDTH - (numProbs - 1) * PROB_MARGIN) / numProbs;
        QPixmap pm(width, height);
        QBitmap bm(width, height);
        bm.clear();
        pm.setMask(bm);
        QPainter *painter = new QPainter(&pm);

        QLinearGradient gradient(0, 0, 0, height-10);
        gradient.setColorAt(0, colorPair.color1);
        gradient.setColorAt(1, colorPair.color2);
        QBrush brush(gradient);
        painter->setPen(pen);
        brush.setStyle(Qt::LinearGradientPattern);
        painter->setBrush(brush);


        QRectF rect(0.5, 0.5, width-1, height-1);
        painter->drawRoundedRect(rect, 5, 5);
        delete painter;
        this->problemGradient[colorPair] = pm;
    }
    return this->problemGradient[colorPair];
}

QPixmap GradientCache::getColorGradientHighlighted(QColor color1, QColor color2) {
    ColorPair colorPair(color1, color2);
    if (!this->problemGradientHighlighted.contains(colorPair)) {
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(255, 255, 0));
        int width = (NAME_WIDTH - (numProbs - 1) * PROB_MARGIN) / numProbs;
        QPixmap pm(width, height);
        QBitmap bm(width, height);
        bm.clear();
        pm.setMask(bm);
        QPainter *painter = new QPainter(&pm);

        QLinearGradient gradient(0, 0, 0, height-10);
        gradient.setColorAt(0, colorPair.color1);
        gradient.setColorAt(1, colorPair.color2);
        QBrush brush(gradient);
        painter->setPen(pen);
        brush.setStyle(Qt::LinearGradientPattern);
        painter->setBrush(brush);


        QRectF rect(1, 1, width-2, height-2);
        painter->drawRoundedRect(rect, 5, 5);
        delete painter;
        this->problemGradientHighlighted[colorPair] = pm;
    }
    return this->problemGradientHighlighted[colorPair];
}

QPixmap GradientCache::getMedalGradient(Medal medal) {
    if (!this->medalGradient.contains(medal)) {
        int screenWidth = QApplication::desktop()->screenGeometry().width();
        QPixmap pm(screenWidth, TEAMITEM_HEIGHT);
        QPainter *painter = new QPainter(&pm);
        QLinearGradient gradient(0, 0, screenWidth, 0);
        switch (medal) {
        case GOLD_MEDAL:
            gradient.setColorAt(0, QColor(111, 81, 19));
            gradient.setColorAt(0.5, QColor(251, 247, 200));
            gradient.setColorAt(1, QColor(114, 84, 22));
            break;
        case SILVER_MEDAL:
            gradient.setColorAt(0, QColor(153, 153, 153));
            gradient.setColorAt(0.5, QColor(255, 255, 255));
            gradient.setColorAt(1, QColor(155, 155, 155));
            break;
        case BRONZE_MEDAL:
            gradient.setColorAt(0, QColor(97, 51, 2));
            gradient.setColorAt(0.5, QColor(255, 253, 230));
            gradient.setColorAt(1, QColor(101, 58, 5));
            break;
        default:
            break;
        }

        painter->setPen(Qt::NoPen);
        QBrush brush(gradient);
        brush.setStyle(Qt::LinearGradientPattern);
        painter->setBrush(brush);
        painter->drawRect(0, 0, screenWidth, TEAMITEM_HEIGHT);

        delete painter;
        this->medalGradient[medal] = pm;
    }
    return this->medalGradient[medal];
}

QPixmap GradientCache::getOddEvenHighlightedGradient(int oddEvenHighlighted) {
    if (!this->oddEvenHighlightGradient.contains(oddEvenHighlighted)) {
        int screenWidth = QApplication::desktop()->screenGeometry().width();
        QPixmap pm(screenWidth, TEAMITEM_HEIGHT);
        QPainter *painter = new QPainter(&pm);
        if (oddEvenHighlighted == 2) { // highlighted
            painter->setBrush(QColor(92, 138, 221));
        } else if (oddEvenHighlighted == 1) { // odd
            QLinearGradient gradient(0, 0, screenWidth, 0);
            gradient.setColorAt(0, QColor(0, 0, 0));
            gradient.setColorAt(0.5, QColor(56, 56, 56));
            gradient.setColorAt(1, QColor(0, 0, 0));
            QBrush brush(gradient);
            brush.setStyle(Qt::LinearGradientPattern);
            painter->setBrush(brush);
        } else if (oddEvenHighlighted == 0) { // even
            QLinearGradient gradient(0, 0, screenWidth, 0);
            gradient.setColorAt(0, QColor(30, 30, 30));
            gradient.setColorAt(0.5, QColor(86, 86, 86));
            gradient.setColorAt(1, QColor(30, 30, 30));
            QBrush brush(gradient);
            brush.setStyle(Qt::LinearGradientPattern);
            painter->setBrush(brush);
        }
        painter->setPen(Qt::NoPen);
        painter->drawRect(0, 0, screenWidth, TEAMITEM_HEIGHT);

        delete painter;
        this->oddEvenHighlightGradient[oddEvenHighlighted] = pm;
    }
    return this->oddEvenHighlightGradient[oddEvenHighlighted];
}

} // namespace View
} // namespace DJ
