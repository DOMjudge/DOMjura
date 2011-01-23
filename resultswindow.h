#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

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

class ResultsWindow : public QGraphicsView {
	Q_OBJECT
public:
	explicit ResultsWindow(QWidget *parent = 0);
	void setBrandingImageFile(QString filename);
	void setTeams(QList<ResultTeam> teams, bool animated = false, int lastResolvedTeam = -1,
				  int lastResolvedProblem = -1, int currentTeam = -1);
	void stopAnimations();
	void reload();
	int getCurrentResolvIndex();
	ResultTeam getResultTeam(int i);

signals:
	void newStandingNeeded();

protected:
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);

private slots:
	void hideLegenda();
	void animationDone();
	void timerDone();

private:
	void hideLegendAfterTimeout();
	void doNextStep();
	void resizeImage();

	QGraphicsPixmapItem *pixmap;
	QGraphicsProxyWidget *proxyImage;
	QGraphicsProxyWidget *legendaProxy;
	QGraphicsScene *scene;
	HeaderGraphicsItem *headerItem;
	LegendaGraphicsItem *legendaItem;
	QList<TeamGraphicsItem *> teamItems;
	QList<ResultTeam> teams;
	bool started;
	bool canDoNextStep;
	int currentResolvIndex;
	QList<ResultTeam> teamsToSet;
	int lastResolvTeam;
	int lastResolvProblem;
	qreal offset;

	QList<QAbstractAnimation *> runningAnimations;
	QList<QTimer *> runningTimers;
};

} // namespace View
} // namespace DJ

#endif // RESULTSWINDOW_H
