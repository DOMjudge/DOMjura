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
			this->statsController = new StatsController(this->readDataController->getScoreboard(), this->readDataController->getEvents(), this);
			qDebug() << "num submissions = " << this->statsController->getTotalSubmissions();
			for (int i = 0; i < this->readDataController->getScoreboard()->getNumProblems(); i++) {
				qDebug() << "submissions for problem" << this->readDataController->getScoreboard()->getProblem(i)->getId() << "=" <<
							this->statsController->getNumSubmissionsOfProblem(this->readDataController->getScoreboard()->getProblem(i)->getId());
			}
		}
	} // namespace Controller
} // namespace DJ
