#include <QtGui/QApplication>
#include <QPixmapCache>

#include "maincontroller.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setOrganizationName("Nicky Gerritsen");
	QApplication::setOrganizationDomain("nickygerritsen.github.com");
	QApplication::setApplicationName("DOMjura");
	QApplication::setApplicationVersion("1.0");

	QPixmapCache::setCacheLimit(1024*1024);

	DJ::Controller::MainController mc;
	mc.showMainWindow();

	return a.exec();
}
