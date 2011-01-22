#ifndef PROBLEMGRAPHICSITEM_H
#define PROBLEMGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QColor>

#include "defines.h"

class QGraphicsSimpleTextItem;

namespace DJ {
namespace View {
class ProblemGraphicsItem : public QObject, public QGraphicsItem {
	Q_OBJECT
	Q_PROPERTY(QColor highlightColor READ getHighlightColor WRITE setHighlightColor)
	Q_PROPERTY(QColor finalColor READ getFinalColor WRITE setFinalColor)
	Q_INTERFACES(QGraphicsItem)
public:
	ProblemGraphicsItem(double height = 40.0, double width = 90.0, QGraphicsItem *parent = NULL);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
				   QWidget *widget);

	void setHeight(double height);
	void setWidth(double width);
	double getWidth();
	bool isSolved();
	void setState(ProblemState state);
	void setProblemId(QString problemId);
	void setNumTries(int numTries);
	void setTime(int time);
	void setHighlighted(bool highlighted);
	void setHighlightColor(QColor color);
	QColor getHighlightColor();
	void setFinalColor(QColor color);
	QColor getFinalColor();

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
	QColor highlightColor;
	QColor finalColor;
	QGraphicsSimpleTextItem *textItem;

};
}
}

#endif // PROBLEMGRAPHICSITEM_H
