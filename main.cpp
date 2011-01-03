#include <QtGui/QApplication>

#include "model.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	DJ::Model::Model m("http://dj.nicky-en-anne.nl/", "plugin", "plugin");
	m.refresh();

    return a.exec();
}
