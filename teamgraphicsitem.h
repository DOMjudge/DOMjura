#ifndef TEAMGRAPHICSITEM_H
#define TEAMGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>

#include "problemgraphicsitem.h"

namespace DJ {
namespace View {

class TeamGraphicsItem : public QGraphicsItem
{
public:
	TeamGraphicsItem(QList<ProblemGraphicsItem *> problemItems, QGraphicsItem *parent = NULL);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

	void setRank(int rank);
	void setName(QString name);
	void setSolved(int solved);
	void setTime(int time);
	void setEven(bool even);
	void setHighlighted(bool highlighted);

private:
	bool highlighted;
	int screenWidth;
	bool even;

	QGraphicsSimpleTextItem *rankItem;
	QGraphicsSimpleTextItem *nameItem;
	QGraphicsSimpleTextItem *solvedItem;
	QGraphicsSimpleTextItem *timeItem;
	QList<ProblemGraphicsItem *> problemItems;
};

} // namespace View
} // namespace DJ

#endif // TEAMGRAPHICSITEM_H
