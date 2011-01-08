#include <QtGui/QApplication>

#include "readdatacontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	DJ::Controller::ReadDataController m("http://dj.nicky-en-anne.nl/", "plugin", "plugin");
	m.refresh();

    return a.exec();
}
