#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "headergraphicsitem.h"

namespace DJ {
namespace View {

class ResultsWindow : public QWidget {
	Q_OBJECT
public:
	explicit ResultsWindow(QWidget *parent = 0);
	~ResultsWindow();
	void setBrandingImageFile(QString filename);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	void resizeImage();

	QGraphicsPixmapItem *pixmap;
	QGraphicsProxyWidget *proxyImage;
	QGraphicsView *view;
	QGraphicsScene *scene;
	HeaderGraphicsItem *headerItem;
};

} // namespace View
} // namespace DJ

#endif // RESULTSWINDOW_H
