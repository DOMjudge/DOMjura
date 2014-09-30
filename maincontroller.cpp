#include "maincontroller.h"

#include <QMessageBox>
#include <QDebug>

#include "domjudgeapimanager.h"

namespace DJ {
namespace Controller {

MainController::MainController(QObject *parent) : QObject(parent) {

	this->mainDialog = new View::MainDialog;
	this->aboutDialog = new View::AboutDialog(this->mainDialog);
	this->statsDialog = new View::StatsDialog(this->mainDialog);
	this->settingsDialog = new View::SettingsDialog(this->mainDialog);
	this->resultsWindow = new View::ResultsWindow;
	this->standingsController = NULL;

	connect(this->mainDialog, SIGNAL(aboutClicked()), this->aboutDialog, SLOT(exec()));
	connect(this->mainDialog, SIGNAL(testConnectionClicked()), this, SLOT(testConnection()));
	connect(this->mainDialog, SIGNAL(settingsClicked()), this->settingsDialog, SLOT(exec()));

	Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
	connect(apiManager, SIGNAL(contestDataLoaded(QJsonObject)), this, SLOT(processContestData(QJsonObject)));
}

MainController::~MainController() {
	delete this->resultsWindow;
	delete this->mainDialog;
	if (this->standingsController) {
		delete this->standingsController;
		this->standingsController = NULL;
	}
}

void MainController::showMainWindow() {
	this->mainDialog->show();
}

void MainController::testConnection() {
	Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
	apiManager->setConnectionInfo(this->mainDialog->getProtocol(), this->mainDialog->getURL(), this->mainDialog->getUsername(), this->mainDialog->getPassword());
	apiManager->loadContestData();
}

void MainController::processContestData(QJsonObject contestData) {
	Model::Contest *contest = new Model::Contest(contestData);
	qDebug() << contest->toString();
}

void MainController::processContestLoadError() {

}

//void MainController::showStats() {
//	QString stats;

//	StatsController *statsController = new StatsController(this->readDataController->getScoreboard(), this->readDataController->getEvents());

//	stats += "Total number of submissions: " + QString::number(statsController->getTotalSubmissions()) + "\n";
//	stats += "Total number of correct submissions: " + QString::number(statsController->getTotalCorrect()) + "\n";
//	for (int i = 0; i < this->readDataController->getScoreboard()->getNumProblems(); i++) {
//		QString problemid = this->readDataController->getScoreboard()->getProblem(i)->getId();
//		stats += "\n== Problem " + problemid + " ==\n";
//		stats += "Number of submissions: " + QString::number(statsController->getNumSubmissionsOfProblem(problemid)) + "\n";
//		if (statsController->getNumSubmissionsOfProblem(problemid) > 0) {
//			stats += "Last submission time: " + statsController->getLastSubmission(problemid).toString("dd-MM-yyyy hh:mm:ss") + "\n";
//		}
//		stats += "Is solved? " + QString(statsController->problemIsSolved(problemid) ? "Yes" : "No") + "\n";
//		if (statsController->problemIsSolved(problemid)) {
//			stats += "First solve time: " + statsController->getFirstSolved(problemid).toString("dd-MM-yyyy hh:mm:ss") + "\n";
//			stats += "Number of correct submissions: " + QString::number(statsController->getNumCorrectofProblem(problemid)) + "\n";
//		}
//	}

//	delete statsController;

//	this->statsDialog->setStats(stats);
//	this->statsDialog->exec();
//}

//void MainController::showResults() {
//	if (this->standingsController) {
//		delete this->standingsController;
//		this->standingsController = NULL;
//	}
//	this->standingsController = new StandingsController(this->readDataController->getScoreboard(),
//														this->readDataController->getEvents());
//	this->standingsController->initStandings(this->mainDialog->getSelectedCategory());
//	int curRank = 1;
//	QList<ResultTeam> teams;
//	QList<Model::RankedTeam *> ranking = this->standingsController->getCurrentRanking();
//	for (int i = 0; i < ranking.size(); i++) {
//		ResultTeam team;
//		Model::RankedTeam *rankedTeam = ranking.at(i);
//		team.name = rankedTeam->getName();
//		team.id = rankedTeam->getId();
//		team.solved = rankedTeam->getNumSolved();
//		if (i > 0) {
//			Model::RankedTeam *prevTeam = ranking.at(i - 1);
//			if (rankedTeam->getNumSolved() == prevTeam->getNumSolved()
//					&& rankedTeam->getTotalTime() == prevTeam->getTotalTime()) {
//				team.rank = curRank;
//			} else {
//				curRank = i + 1;
//				team.rank = curRank;
//			}
//		} else {
//			team.rank = 1;
//			curRank = 1;
//		}
//		team.time = rankedTeam->getTotalTime();
//		QList<ResultProblem> problems;
//		for (int j = 0; j < rankedTeam->getNumProblems(); j++) {
//			Model::RankedProblem *rankedProblem = rankedTeam->getProblem(j);
//			ResultProblem problem;
//			problem.numTries = rankedProblem->tries;
//			problem.state = rankedProblem->problemState;
//			problem.problemId = rankedProblem->id;
//			problem.time = rankedProblem->timeLastTry;
//			problems.append(problem);
//		}
//		team.problems = problems;
//		teams.append(team);
//	}
//	this->resultsWindow->stopAnimations();
//	this->resultsWindow->reload();
//	this->resultsWindow->setTeams(teams);
//	this->resultsWindow->showFullScreen();
//}

//void MainController::updateStanding() {
//	if (this->standingsController->nextStanding()) {
//		QList<Model::RankedTeam *> ranking = this->standingsController->getCurrentRanking();
//		QList<ResultTeam> teams;
//		int curRank = 1;
//		for (int i = 0; i < ranking.size(); i++) {
//			ResultTeam team;
//			Model::RankedTeam *rankedTeam = ranking.at(i);
//			team.name = rankedTeam->getName();
//			team.id = rankedTeam->getId();
//			team.solved = rankedTeam->getNumSolved();
//			if (i > 0) {
//				Model::RankedTeam *prevTeam = ranking.at(i - 1);
//				if (rankedTeam->getNumSolved() == prevTeam->getNumSolved()
//						&& rankedTeam->getTotalTime() == prevTeam->getTotalTime()) {
//					team.rank = curRank;
//				} else {
//					curRank = i + 1;
//					team.rank = curRank;
//				}
//			} else {
//				team.rank = 1;
//				curRank = 1;
//			}
//			team.time = rankedTeam->getTotalTime();
//			QList<ResultProblem> problems;
//			for (int j = 0; j < rankedTeam->getNumProblems(); j++) {
//				Model::RankedProblem *rankedProblem = rankedTeam->getProblem(j);
//				ResultProblem problem;
//				problem.numTries = rankedProblem->tries;
//				problem.state = rankedProblem->problemState;
//				problem.problemId = rankedProblem->id;
//				problem.time = rankedProblem->timeLastTry;
//				problems.append(problem);
//			}
//			team.problems = problems;
//			teams.append(team);
//		}
//		this->resultsWindow->setTeams(teams, true,
//									  this->standingsController->getLastResolvedTeam(),
//									  this->standingsController->getLastResolvedProblem(),
//									  this->standingsController->getCurrentPos());
//	} else {
//		QList<Model::RankedTeam *> ranking = this->standingsController->getCurrentRanking();
//		QList<ResultTeam> teams;
//		int curRank = 1;
//		for (int i = 0; i < ranking.size(); i++) {
//			ResultTeam team;
//			Model::RankedTeam *rankedTeam = ranking.at(i);
//			team.name = rankedTeam->getName();
//			team.id = rankedTeam->getId();
//			team.solved = rankedTeam->getNumSolved();
//			if (i > 0) {
//				Model::RankedTeam *prevTeam = ranking.at(i - 1);
//				if (rankedTeam->getNumSolved() == prevTeam->getNumSolved()
//						&& rankedTeam->getTotalTime() == prevTeam->getTotalTime()) {
//					team.rank = curRank;
//				} else {
//					curRank = i + 1;
//					team.rank = curRank;
//				}
//			} else {
//				team.rank = 1;
//				curRank = 1;
//			}
//			team.time = rankedTeam->getTotalTime();
//			QList<ResultProblem> problems;
//			for (int j = 0; j < rankedTeam->getNumProblems(); j++) {
//				Model::RankedProblem *rankedProblem = rankedTeam->getProblem(j);
//				ResultProblem problem;
//				problem.numTries = rankedProblem->tries;
//				problem.state = rankedProblem->problemState;
//				problem.problemId = rankedProblem->id;
//				problem.time = rankedProblem->timeLastTry;
//				problems.append(problem);
//			}
//			team.problems = problems;
//			teams.append(team);
//		}
//		this->resultsWindow->setTeams(teams, false);
//		this->resultsWindow->setResolvDone();
//	}
//}

} // namespace Controller
} // namespace DJ
