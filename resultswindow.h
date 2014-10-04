/** \file resultswindow.h
  * \brief Contains the class for the results window (a graphics view).
  */
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
#include "winnergraphicsitem.h"

#include "defines.h"

namespace DJ {
namespace Model {
class Contest;
}

namespace View {

/** The result window. Note this is actually a QGraphicsView.
  * It (optionally) uses OpenGL to draw the scene.
  */
class ResultsWindow : public QGraphicsView {
	Q_OBJECT
public:
	/** Constructs a new result window.
	  * \param parent The parent of this widget.
	  */
	explicit ResultsWindow(QWidget *parent = 0);
	/** Sets the teams.
	  * \param teams The teams to set.
	  * \param animated If this parameter is false, the teams are set immediately.
	  * Otherwise, the function will calculate the differences and animate to the new state.
	  * \param lastResolvedTeam The team that was last resolved. Only used when animated = true.
	  * \param lastResolvedProblem The problem that was last resolved. Only used when animated = true.
	  * \param currentTeam The current team. Only used when animated = true.
	  */
	void setTeams(QList<ResultTeam> teams, bool animated = false, int lastResolvedTeam = -1,
				  int lastResolvedProblem = -1, int currentTeam = -1);
	void setContest(Model::Contest *contest);
	/** Stops all running animations and timers.
	  */
	void stopAnimations();
	/** Resets all data.
	  */
	void reload();
	/** Returns the current resolve index.
	  * \return The current resolve index.
	  */
	int getCurrentResolvIndex();
	/** If called, will indicate that resolving is done.
	  */
	void setResolvDone();
	/** Returns the i-th team.
	  * \param i The index to use.
	  * \return The i-th team.
	  */
	ResultTeam getResultTeam(int i);

signals:
	/** Emitted when a new standing is needed.
	  */
	void newStandingNeeded();

protected:
	/** Handles key press events (q, x, escape, space, enter and return).
	  */
	void keyPressEvent(QKeyEvent *event);
	/** Handles mouse click events (left mouse button).
	  */
	void mousePressEvent(QMouseEvent *event);

private slots:
	void hideLegenda();
	void animationDone();
	void timerDone();
	void timerMoveUpDone();

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
	WinnerGraphicsItem *winnerItem;
	QList<TeamGraphicsItem *> teamItems;
	QList<ResultTeam> teams;
	bool started;
	bool canDoNextStep;
	bool resolvDone;
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
