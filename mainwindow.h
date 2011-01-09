#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>

class QGraphicsScene;
class QGraphicsView;

namespace DJ {
namespace View {
class ProblemGraphicsItem;

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	QGraphicsScene *scene;
	QGraphicsView *view;

	QList<ProblemGraphicsItem *> items;
};
}
}

#endif // MAINWINDOW_H
