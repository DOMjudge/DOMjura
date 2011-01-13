#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "headergraphicsitem.h"
#include "teamgraphicsitem.h"
#include "defines.h"

namespace DJ {
namespace View {

class ResultsWindow : public QWidget {
	Q_OBJECT
public:
	explicit ResultsWindow(QWidget *parent = 0);
	void setBrandingImageFile(QString filename);
	void setTeams(QList<ResultTeam> teams);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	void resizeImage();

	QGraphicsPixmapItem *pixmap;
	QGraphicsProxyWidget *proxyImage;
	QGraphicsView *view;
	QGraphicsScene *scene;
	HeaderGraphicsItem *headerItem;
	QList<TeamGraphicsItem *> teamItems;
	QList<ResultTeam> teams;
};

} // namespace View
} // namespace DJ

#endif // RESULTSWINDOW_H
