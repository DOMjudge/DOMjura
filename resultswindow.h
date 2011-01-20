#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "headergraphicsitem.h"
#include "teamgraphicsitem.h"
#include "legendagraphicsitem.h"

#include "defines.h"

namespace DJ {
namespace View {

class ResultsWindow : public QWidget {
	Q_OBJECT
public:
	explicit ResultsWindow(QWidget *parent = 0);
	void setBrandingImageFile(QString filename);
	void setTeams(QList<ResultTeam> teams);
	void reload();

protected:
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);

private slots:
	void hideLegenda();
	void animationDone();

private:
	void hideLegendAfterTimeout();
	void doNextStep();
	void resizeImage();

	QGraphicsPixmapItem *pixmap;
	QGraphicsProxyWidget *proxyImage;
	QGraphicsProxyWidget *legendaProxy;
	QGraphicsView *view;
	QGraphicsScene *scene;
	HeaderGraphicsItem *headerItem;
	LegendaGraphicsItem *legendaItem;
	QList<TeamGraphicsItem *> teamItems;
	QList<ResultTeam> teams;
	QTimer *legendaTimer;
	bool started;

	QParallelAnimationGroup *scrollToBottomAnim;
	QPropertyAnimation *legendaAnim;
};

} // namespace View
} // namespace DJ

#endif // RESULTSWINDOW_H
