#include "mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsView>

#include "problemgraphicsitem.h"

namespace DJ {
namespace View {
MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
	this->setMinimumSize(QSize(1024, 768));
	this->setMaximumSize(QSize(1024, 768));
	this->setGeometry(QRect(0, 0, 1024, 768));
	this->setWindowTitle("DJ::MainWindow");

	this->view = new QGraphicsView(this);
	this->view->setGeometry(0, 0, 1024, 768);
	this->view->setStyleSheet("background-color: black");

	this->scene = new QGraphicsScene(this);

	this->view->setScene(this->scene);

	QList<QString> problems;
	problems.append("A");
	problems.append("B");
	problems.append("C");
	problems.append("D");

	for (int i = 0; i < 4; i++) {
		ProblemGraphicsItem *item = new ProblemGraphicsItem();
		item->setState((Model::ProblemState)i);
		item->setPos(i * 100, 0);
		item->setNumTries(i*4);
		item->setTime(i * 60);
		item->setProblemId(problems.at(i));
		this->items.append(item);
		this->scene->addItem(item);
	}
}

MainWindow::~MainWindow() {
}
}
}
