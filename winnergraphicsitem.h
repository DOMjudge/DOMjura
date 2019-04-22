/** \file winnergraphicsitem.h
  * \brief Contains a class for the winner item.
  */

#ifndef WINNERGRAPHICSITEM_H
#define WINNERGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>

namespace DJ {
namespace View {
/** A winner graphics item.
  */
class WinnerGraphicsItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    /** The opacity of the item.
      */
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_INTERFACES(QGraphicsItem)
public:
    /** Constructs a new graphics item.
      * \param parent The parent of this item.
      */
    explicit WinnerGraphicsItem(QGraphicsItem *parent = 0);
    /** Returns the bounding rectangle for this item.
      * \return The bounding rectangle for this item.
      */
    QRectF boundingRect() const;
    /** Paints this item on the scene.
      * \param painter The painter to use.
      * \param option The current option information.
      * \param widget The widget to draw on.
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    /** Sets the winner text to display.
      * \param text The text to display.
      */
    void setWinner(QString text);
    /** Sets the contest name to display.
      * \param text The contest name to display.
      */
    void setContestName(QString text);

private:
    void reAddItems();

    QList<QGraphicsSimpleTextItem *> textItems;
    QString winner;
    QString contestName;

};

} // namespace View
} // namespace DJ

#endif // WINNERGRAPHICSITEM_H
