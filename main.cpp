#include <QtGui/QApplication>

#include "datacontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	DJ::Controller::DataController m("http://dj.nicky-en-anne.nl/", "plugin", "plugin");
	m.refresh();

    return a.exec();
}
