#include "resultswindow.h"

#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>

namespace DJ {
namespace View {

ResultsWindow::ResultsWindow(QWidget *parent) : QWidget(parent) {
	this->view = new QGraphicsView(this);

	this->scene = new QGraphicsScene(this);
	this->scene->setSceneRect(QApplication::desktop()->screenGeometry());

	this->view->setScene(this->scene);
	this->view->setGeometry(QApplication::desktop()->screenGeometry());

	this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->view->setStyleSheet("background-color: black; border: none;");
	this->view->centerOn(0, 0);

	this->headerItem = new HeaderGraphicsItem(QApplication::desktop()->screenGeometry().width());
	this->headerItem->setPos(0, 0);

	this->pixmap = new QGraphicsPixmapItem;

	this->scene->addItem(this->pixmap);
	this->scene->addItem(this->headerItem);
}

ResultsWindow::~ResultsWindow() {
	delete this->headerItem;
}

void ResultsWindow::setBrandingImageFile(QString filename) {
	if (!filename.isEmpty()) {
		QPixmap pixmap(filename);
		this->pixmap->setPixmap(pixmap);
	} else {
		this->pixmap->setPixmap(QPixmap());
	}
	resizeImage();
}

void ResultsWindow::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Escape:
	case Qt::Key_Q:
	case Qt::Key_X:
		close();
	}
}

void ResultsWindow::resizeImage() {
	QSize size;
	if (!this->pixmap->pixmap().isNull()) {
		size = this->pixmap->pixmap().size();
	} else {
		size = QSize(0, 0);
	}
	QRect screenSize = QApplication::desktop()->screenGeometry();
	QPointF labelPos;
	labelPos.setX(screenSize.width() - size.width());
	labelPos.setY(screenSize.height() - size.height());
	this->pixmap->setPos(labelPos);
}

} // namespace View
} // namespace DJ
