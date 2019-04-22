/** \file teamgraphicsitem.h
  * \brief Contains the class for a team item.
  */

#ifndef TEAMGRAPHICSITEM_H
#define TEAMGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>

#include "problemgraphicsitem.h"

namespace DJ {
namespace View {

/** A team graphics item.
  */
class TeamGraphicsItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    /** The position of this item.
      */
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    /** The opacity of this item.
      */
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_INTERFACES(QGraphicsItem)
public:
    /** Constructs a new team item.
      * \param problemItems The problem items for this team.
      * \param parent The parent of this item.
      */
    TeamGraphicsItem(QList<ProblemGraphicsItem *> problemItems, QGraphicsItem *parent = NULL);
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

    /** Sets the rank of this team.
      * \param rank The rank to set.
      */
    void setRank(int rank);
    /** Sets the name of this team.
      * \param name The name to set.
      */
    void setName(QString name);
    /** Sets the solved state of this team.
      * \param solved The solved stte to set.
      */
    void setSolved(int solved);
    /** Sets the total time of this team.
      * \param time The total time to set.
      */
    void setTime(int time);
    /** Sets whether this row is even.
      * \param even Whether this row is even or not.
      */
    void setEven(bool even);
    /** Sets whether this row is highlighted.
      * \param even Whether this row is highlighted or not.
      */
    void setHighlighted(bool highlighted);
    /** Sets the medal of this team.
      * \param medal The medal to set.
      */
    void setMedal(Medal medal);
    /** Returns the i-th problem item.
      * \param i The index to use.
      * \return The i-th problem item.
      */
    ProblemGraphicsItem *getProblemGraphicsItem(int i);

private:
    bool highlighted;
    int screenWidth;
    bool even;
    Medal medal;

    QGraphicsSimpleTextItem *rankItem;
    QGraphicsSimpleTextItem *nameItem;
    QGraphicsSimpleTextItem *solvedItem;
    QGraphicsSimpleTextItem *timeItem;
    QList<ProblemGraphicsItem *> problemItems;
};

} // namespace View
} // namespace DJ

#endif // TEAMGRAPHICSITEM_H
