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
    this->reAddItems();
}

QRectF WinnerGraphicsItem::boundingRect() const {
    QRect screenSize = QApplication::desktop()->screenGeometry();
    return QRectF(0, 0, screenSize.width(), screenSize.height());
}

void WinnerGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                               QWidget *) {
    QLinearGradient gradient(0, 0, 0, this->boundingRect().height());
    gradient.setColorAt(0, QColor(111, 81, 19));
    gradient.setColorAt(0.5, QColor(251, 247, 200));
    gradient.setColorAt(1, QColor(114, 84, 22));
    QBrush brush(gradient);
    brush.setStyle(Qt::LinearGradientPattern);
    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->drawRect(this->boundingRect());
}

void WinnerGraphicsItem::setWinner(QString text) {
    update();
    this->winner = text;
    this->reAddItems();
}

void WinnerGraphicsItem::setContestName(QString text) {
    update();
    this->contestName = text;
    this->reAddItems();
}

void WinnerGraphicsItem::reAddItems() {
    int centerHeight = this->boundingRect().height() / 2;
    int centerWidth = this->boundingRect().width() / 2;

    QFont font("DejaVu Sans Mono");
    font.setPixelSize(75);
    font.setBold(true);
    QFontMetrics fm(font);

    QString startingText = "Winner " + this->contestName;
    QString currentLine = "";
    int myWidth = this->boundingRect().width();
    QStringList words = startingText.split(" ");
    QStringList lines;
    foreach (QString word, words) {
        QString proposedLine = currentLine;
        if (!proposedLine.isEmpty()) {
            proposedLine += " ";
        }
        proposedLine += word;
        int newWidth = fm.width(proposedLine);
        if (newWidth <= myWidth) {
            currentLine = proposedLine;
        } else {
            lines.append(currentLine);
            currentLine = word;
        }
    }

    if (!currentLine.isEmpty()) {
        lines.append(currentLine);
    }

    lines.append("");
    lines.append(this->winner);

    qDeleteAll(this->textItems);
    this->textItems.clear();
    this->setCacheMode(DeviceCoordinateCache);

    int i = 0;
    int total = lines.size();
    foreach (QString line, lines) {
        QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem(this);
        textItem->setFont(font);
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(3);
        textItem->setPen(pen);
        textItem->setBrush(QBrush(Qt::white));

        textItem->setText(line);

        int offsetFromCenter = -((total * fm.height()) / 2) + (i * fm.height());

        textItem->setPos(centerWidth - fm.width(textItem->text()) / 2, centerHeight + offsetFromCenter);

        this->textItems.append(textItem);
        ++i;
    }
}

} // namespace View
} // namespace DJ
