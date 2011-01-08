#include "maincontroller.h"

#include <QDebug>

namespace DJ {
	namespace Controller {
		MainController::MainController(QObject *parent) : QObject(parent) {
		}

		void MainController::go() {
			this->readDataController = new ReadDataController("http://localhost/~nicky/", "plugin", "plugin", this);
			connect(this->readDataController, SIGNAL(dataRead()), this, SLOT(dataRead()));
			this->readDataController->refresh();
		}

		void MainController::dataRead() {
			this->statsController = new StatsController(this->readDataController->getScoreboard(), this->readDataController->getEvents(), this);
			qDebug() << "num submissions = " << this->statsController->getTotalSubmissions();
		}
	} // namespace Controller
} // namespace DJ
