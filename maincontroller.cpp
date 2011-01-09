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
			qDebug() << "total num submissions =" << this->statsController->getTotalSubmissions();
			qDebug() << "total num correct =" << this->statsController->getTotalCorrect();
			for (int i = 0; i < this->readDataController->getScoreboard()->getNumProblems(); i++) {
				QString problemid = this->readDataController->getScoreboard()->getProblem(i)->getId();
				qDebug() << "problem" << problemid;
				qDebug() << "num submissions =" << this->statsController->getNumSubmissionsOfProblem(problemid);
				qDebug() << "solved =" << this->statsController->problemIsSolved(problemid);
				qDebug() << "first solve time =" << this->statsController->getFirstSolved(problemid).toString("yyyy-MM-dd hh:mm:ss");
				qDebug() << "correct =" << this->statsController->getNumCorrectofProblem(problemid);
			}

			this->standingsController = new StandingsController(this->readDataController->getScoreboard(), this->readDataController->getEvents(), this);
			this->standingsController->initStandings("Participants");
			qDebug() << "Initial ranking...";
			qDebug() << this->standingsController->toString();
			int i = 1;
			while (this->standingsController->nextStanding()) {
				qDebug() << "Ranking after" << i << "times nextStanding()...";
				qDebug() << this->standingsController->toString();
				i++;
			}
			qDebug() << "Done!";
		}
	} // namespace Controller
} // namespace DJ
