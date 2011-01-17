#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>

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
	void hideLegendAfterTimeout();

protected:
	void keyPressEvent(QKeyEvent *event);

private slots:
	void hideLegenda();

private:
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
};

} // namespace View
} // namespace DJ

#endif // RESULTSWINDOW_H
