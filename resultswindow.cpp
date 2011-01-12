#include "resultswindow.h"

#include <QKeyEvent>

#include <QDebug>

namespace DJ {
namespace View {

ResultsWindow::ResultsWindow(QWidget *parent) : QWidget(parent) {
	setStyleSheet("background-color: black;");
	this->imageLabel = new QLabel(this);
}

void ResultsWindow::setBrandingImageFile(QString filename) {
	if (!filename.isEmpty()) {
		QPixmap pixmap(filename);
		this->imageLabel->setPixmap(pixmap);
	} else {
		this->imageLabel->clear();
	}
}

void ResultsWindow::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Escape:
	case Qt::Key_Q:
	case Qt::Key_X:
		close();
	}
}

void ResultsWindow::showFullScreen() {
	QWidget::showFullScreen();
	updateBrandingRect();
}

void ResultsWindow::updateBrandingRect() {
	QSize size;
	if (this->imageLabel->pixmap()) {
		size = this->imageLabel->pixmap()->size();
	} else {
		size = QSize(0, 0);
	}
	QRect screenSize = this->geometry();
	QRect labelRect;
	int height;
	int width;
	labelRect.setLeft(screenSize.width() - size.width());
	labelRect.setTop(screenSize.height() - size.height());
	labelRect.setWidth(size.width());
	labelRect.setHeight(size.width());
	this->imageLabel->setMinimumSize(labelRect.size());
	this->imageLabel->setMaximumSize(labelRect.size());
	this->imageLabel->setGeometry(labelRect);

	qDebug() << "sizes";
	qDebug() << screenSize;
	qDebug() << labelRect;
}

} // namespace View
} // namespace DJ
