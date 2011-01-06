#include <QtGui/QApplication>

#include "datacontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	DJ::Controller::DataController m("http://localhost/~nicky/", "", "");
	m.refresh();

    return a.exec();
}
