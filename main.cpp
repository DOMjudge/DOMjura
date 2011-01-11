#include <QtGui/QApplication>

#include "maincontroller.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DJ::Controller::MainController mc;
	mc.showMainWindow();

	return a.exec();
}
