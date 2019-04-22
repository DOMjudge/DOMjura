/** \file main.cpp
    \brief The main file for the DOMjura application
*/

/** \namespace DJ
  * \brief The namespace the whole DOMjura application is in.
  */

/** \namespace DJ::View
  * \brief The namespace for all the view related classes.
  */

/** \namespace DJ::View::Ui
  * \brief The namespace for all Qt Designer generated widget headers.
  */

/** \namespace DJ::Model
  * \brief The namespace for all the model related classes.
  */

/** \namespace DJ::Controller
  * \brief The namespace where all controllers reside in.
  */

#include <QApplication>
#include <QPixmapCache>

#include "maincontroller.h"

/** Sets the pixmap cache to 1 GB and then starts the application.
  * \param argc The number of arguments.
  * \param argv The arguments.
  * \return 0 on success, another value otherwise.
  */
int main(int argc, char *argv[]) {
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
