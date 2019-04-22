/** \file legendagraphicsitem.h
  * \brief Contains a class for the legenda item.
  */

#ifndef LEGENDAGRAPHICSITEM_H
#define LEGENDAGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QObject>

namespace DJ {
namespace View {
/** The legenda graphics item.
  */
class LegendaGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    /** The opacity of this item.
      */
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_INTERFACES(QGraphicsItem)
public:
    /** Constructs a new legenda item.
      * \param parent The parent of this object.
      */
    LegendaGraphicsItem(QGraphicsItem *parent = NULL);
    /** Returns the bounding rect of this item.
      * \return The bounding rect of this item.
      */
    QRectF boundingRect() const;
    /** Paints this item on the scene.
      * \param painter The painter to use.
      * \param option The current option information.
      * \param widget The widget to draw on.
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
    QGraphicsSimpleTextItem *solvedText;
    QGraphicsSimpleTextItem *rejectedText;
    QGraphicsSimpleTextItem *pendingText;
};

} // namespace View
} // namespace DJ

#endif // LEGENDAGRAPHICSITEM_H
