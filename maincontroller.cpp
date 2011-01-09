#include "maincontroller.h"

#include <QDebug>

namespace DJ {
	namespace Controller {
		MainController::MainController(QObject *parent) : QObject(parent) {
		}

		void MainController::go() {
			this->readDataController = new ReadDataController("http://dj.nicky-en-anne.nl", "plugin", "plugin", this);
			connect(this->readDataController, SIGNAL(dataRead()), this, SLOT(dataRead()));
			this->readDataController->refresh();
		}

		void MainController::dataRead() {
			this->standingsController = new StandingsController(this->readDataController->getScoreboard(), this->readDataController->getEvents(), this);
			this->standingsController->initStandings("Participants");
			qDebug() << this->standingsController->toString();
		}
	} // namespace Controller
} // namespace DJ
