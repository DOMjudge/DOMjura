#ifndef PROBLEMGRAPHICSITEM_H
#define PROBLEMGRAPHICSITEM_H

#include <QGraphicsItem>

#include "defines.h"

class QGraphicsSimpleTextItem;

namespace DJ {
namespace View {
class ProblemGraphicsItem : public QGraphicsItem {
public:
	ProblemGraphicsItem(double height = 40.0, double width = 90.0, QGraphicsItem *parent = NULL);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
				   QWidget *widget);

	void setHeight(double height);
	void setWidth(double width);
	double getWidth();
	void setState(ProblemState state);
	void setProblemId(QString problemId);
	void setNumTries(int numTries);
	void setTime(int time);
	void setHighlighted(bool highlighted);

signals:

public slots:

private:
	bool highlighted;
	double height;
	double width;
	ProblemState state;
	QString problemId;
	int numTries;
	int time;
	QGraphicsSimpleTextItem *textItem;

};
}
}

#endif // PROBLEMGRAPHICSITEM_H
