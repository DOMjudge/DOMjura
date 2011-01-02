#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include "problemgraphicsitem.h"

namespace BAPC {
	MainWindow::MainWindow(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::MainWindow) {
		ui->setupUi(this);

		this->scene = new QGraphicsScene(this);

		this->ui->graphicsView->setScene(this->scene);

		QList<QString> problems;
		problems.append("A");
		problems.append("B");
		problems.append("C");
		problems.append("D");

		for (int i = 0; i < 4; i++) {
			ProblemGraphicsItem *item = new ProblemGraphicsItem();
			item->setState((ProblemState)i);
			item->setPos(i * 100, 0);
			item->setNumTries(i*4);
			item->setTime(i * 60);
			item->setProblemId(problems.at(i));
			this->items.append(item);
			this->scene->addItem(item);
		}
	}

	MainWindow::~MainWindow() {
		delete ui;
	}
}
