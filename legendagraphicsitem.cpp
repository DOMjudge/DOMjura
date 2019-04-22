#include "legendagraphicsitem.h"

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QStyleOptionGraphicsItem>

namespace DJ {
namespace View {

LegendaGraphicsItem::LegendaGraphicsItem(QGraphicsItem *parent)
    : QObject(),
      QGraphicsItem(parent) {
    QFont font("Courier new");
    font.setPixelSize(26);
    font.setBold(true);

    this->setCacheMode(DeviceCoordinateCache);

    this->solvedText = new QGraphicsSimpleTextItem("Solved Problem", this);
    this->solvedText->setPos(55, 15);
    this->solvedText->setPen(QPen(Qt::black));
    this->solvedText->setBrush(QBrush(Qt::white));
    this->solvedText->setFont(font);

    this->rejectedText = new QGraphicsSimpleTextItem("Rejected Run(s)", this);
    this->rejectedText->setPos(55, 55);
    this->rejectedText->setPen(QPen(Qt::black));
    this->rejectedText->setBrush(QBrush(Qt::white));
    this->rejectedText->setFont(font);

    this->pendingText = new QGraphicsSimpleTextItem("Pending Submission", this);
    this->pendingText->setPos(55, 95);
    this->pendingText->setPen(QPen(Qt::black));
    this->pendingText->setBrush(QBrush(Qt::white));
    this->pendingText->setFont(font);
}

QRectF LegendaGraphicsItem::boundingRect() const {
    return QRectF(0, 0, 380, 135);
}

void LegendaGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *) {
    painter->setClipRect(option->exposedRect);
    painter->setPen(Qt::white);
    painter->setBrush(QColor(0, 0, 0, 180));
    painter->drawRoundedRect(1, 1, 378, 133, 10, 10);

    painter->setPen(Qt::NoPen);

    QColor green(97, 235, 0);
    painter->setBrush(green);
    painter->drawRoundedRect(15, 15, 25, 25, 6, 6);

    QColor red(221, 5, 10);
    painter->setBrush(red);
    painter->drawRoundedRect(15, 55, 25, 25, 6, 6);

    QColor yellow(248, 228, 0);
    painter->setBrush(yellow);
    painter->drawRoundedRect(15, 95, 25, 25, 6, 6);
}

} // namespace View
} // namespace DJ
