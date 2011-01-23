#include <QtGui/QApplication>

#include "maincontroller.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setOrganizationName("Nicky Gerritsen");
	QApplication::setOrganizationDomain("nickygerritsen.github.com");
	QApplication::setApplicationName("DOMjura");
	QApplication::setApplicationVersion("1.0");

	DJ::Controller::MainController mc;
	mc.showMainWindow();

	return a.exec();
}
