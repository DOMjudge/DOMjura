/** \file problemgraphicsitem.h
  * \brief Contains the class for the problem items.
  */

#ifndef PROBLEMGRAPHICSITEM_H
#define PROBLEMGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QColor>

#include "defines.h"

class QGraphicsSimpleTextItem;

namespace DJ {
namespace View {
/** A problem graphics item.
  */
class ProblemGraphicsItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    /** The color during highlighting.
      */
    Q_PROPERTY(QColor highlightColor READ getHighlightColor WRITE setHighlightColor)
    /** The color to resolve to.
      */
    Q_PROPERTY(QColor finalColor READ getFinalColor WRITE setFinalColor)
    Q_INTERFACES(QGraphicsItem)
public:
    /** Constructs a new problem item.
      * \param height The height of this item.
      * \param width The widt of this problem.
      * \param parent The parent of this item.
      */
    ProblemGraphicsItem(double height = 40.0, double width = 90.0, QGraphicsItem *parent = NULL);
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
    /** Sets the height of this item.
      * \param height The new height.
      */
    void setHeight(double height);
    /** Sets the width of this item.
      * \param width The new width.
      */
    void setWidth(double width);
    /** Returns the width of this item.
      * \return The width of this item.
      */
    double getWidth();
    /** Whether this problem is solved.
      * \return Whether this problem is solved.
      */
    bool isSolved();
    /** Sets the state of this problem.
      * \param state The new state.
      */
    void setState(ProblemState state);
    /** Sets the problem ID of this problem.
      * \param problemId The problem ID to set.
      */
    void setProblemId(QString problemId);
    /** Sets the number of tries for this problem.
      * \param numTries The number of tries for this problem.
      */
    void setNumTries(int numTries);
    /** Sets the time taken for this problem.
      * \param time The time taken for this problem.
      */
    void setTime(int time);
    /** Sets whether this item should be highlighted.
      * \param Whether this item should be highlighted.
      */
    void setHighlighted(bool highlighted);
    /** Sets the highlight color of this item.
      * \param color The highlight color to set.
      */
    void setHighlightColor(QColor color);
    /** Returns the highlight color of this item.
      * \return The highlight color of this item.
      */
    QColor getHighlightColor();
    /** Sets the final color of this item.
      * \param color The final color to set.
      */
    void setFinalColor(QColor color);
    /** Returns the final color of this item.
      * \return The final color of this item.
      */
    QColor getFinalColor();

private:
    bool highlighted;
    double height;
    double width;
    ProblemState state;
    QString problemId;
    int numTries;
    int time;
    QColor highlightColor;
    QColor finalColor;
    QGraphicsSimpleTextItem *textItem;

};
}
}

#endif // PROBLEMGRAPHICSITEM_H
