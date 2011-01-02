#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>

class QGraphicsScene;

namespace Ui {
	class MainWindow;
}

namespace BAPC {
	class ProblemGraphicsItem;

	class MainWindow : public QWidget
	{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:
		Ui::MainWindow *ui;
		QGraphicsScene *scene;

		QList<ProblemGraphicsItem *> items;
	};
}

#endif // MAINWINDOW_H
