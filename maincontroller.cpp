#include "maincontroller.h"

#include <QDebug>
#include <QMessageBox>

namespace DJ {
namespace Controller {

MainController::MainController(QObject *parent) : QObject(parent) {
	this->readDataController = new ReadDataController(this);

	this->mainDialog = new View::MainDialog;
	this->aboutDialog = new View::AboutDialog(this->mainDialog);
	this->statsDialog = new View::StatsDialog(this->mainDialog);
	this->resultsWindow = new View::ResultsWindow;
	this->standingsController = NULL;

	connect(this->mainDialog, SIGNAL(URLChanged(QString)), this, SLOT(updateURL(QString)));
	connect(this->mainDialog, SIGNAL(usernameChanged(QString)), this, SLOT(updateUsername(QString)));
	connect(this->mainDialog, SIGNAL(passwordChanged(QString)), this, SLOT(updatePassword(QString)));
	connect(this->mainDialog, SIGNAL(switchedToDir()), this, SLOT(switchToDir()));
	connect(this->mainDialog, SIGNAL(switchedToURL()), this, SLOT(switchToURL()));
	connect(this->mainDialog, SIGNAL(loadDataClicked()), this, SLOT(loadData()));
	connect(this->mainDialog, SIGNAL(saveXMLclicked(QString)), this, SLOT(saveXML(QString)));
	connect(this->mainDialog, SIGNAL(dirChanged(QDir)), this, SLOT(updateDir(QDir)));
	connect(this->mainDialog, SIGNAL(resultsClicked()), this, SLOT(showResults()));
	connect(this->mainDialog, SIGNAL(brandingImageSelected(QString)), this, SLOT(setBrandingImage(QString)));

	connect(this->mainDialog, SIGNAL(aboutClicked()), this->aboutDialog, SLOT(exec()));
	connect(this->mainDialog, SIGNAL(statsClicked()), this, SLOT(showStats()));

	connect(this->readDataController, SIGNAL(dataRead()), this, SLOT(enableSave()));
	connect(this->readDataController, SIGNAL(dataRead()), this, SLOT(enableActions()));

	connect(this->resultsWindow, SIGNAL(newStandingNeeded()), this, SLOT(updateStanding()));
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

void MainController::updateURL(QString url) {
	this->readDataController->setUrl(url);
}

void MainController::updateUsername(QString username) {
	this->readDataController->setUsername(username);
}

void MainController::updatePassword(QString password) {
	this->readDataController->setPassword(password);
}

void MainController::switchToDir() {
	this->readDataController->setOfDir(true);
	this->mainDialog->setSaveXMLEnabled(false);
	this->mainDialog->setActionsEnabled(false);
}

void MainController::switchToURL() {
	this->readDataController->setOfDir(false);
	this->mainDialog->setSaveXMLEnabled(false);
	this->mainDialog->setActionsEnabled(false);
}

void MainController::loadData() {
	this->readDataController->refresh();
}

void MainController::enableSave() {
	this->mainDialog->setSaveXMLEnabled(true);
}

void MainController::enableActions() {
	QList<QString> categories;
	Model::Scoreboard *scoreboard = this->readDataController->getScoreboard();
	for (int i = 0; i < scoreboard->getNumCategories(); i++) {
		categories.append(scoreboard->getCategory(i)->getName());
	}
	this->mainDialog->setCategories(categories);
	this->mainDialog->setActionsEnabled(true);
}

void MainController::saveXML(QString dir) {
	this->readDataController->saveXML(dir);
}

void MainController::updateDir(QDir dir) {
	this->readDataController->setDir(dir);
}

void MainController::showStats() {
	QString stats;

	StatsController *statsController = new StatsController(this->readDataController->getScoreboard(), this->readDataController->getEvents());

	stats += "Total number of submissions: " + QString::number(statsController->getTotalSubmissions()) + "\n";
	stats += "Total number of correct submissions: " + QString::number(statsController->getTotalCorrect()) + "\n";
	for (int i = 0; i < this->readDataController->getScoreboard()->getNumProblems(); i++) {
		QString problemid = this->readDataController->getScoreboard()->getProblem(i)->getId();
		stats += "\n== Problem " + problemid + " ==\n";
		stats += "Number of submissions: " + QString::number(statsController->getNumSubmissionsOfProblem(problemid)) + "\n";
		if (statsController->getNumSubmissionsOfProblem(problemid) > 0) {
			stats += "Last submission time: " + statsController->getLastSubmission(problemid).toString("dd-MM-yyyy hh:mm:ss") + "\n";
		}
		stats += "Is solved? " + QString(statsController->problemIsSolved(problemid) ? "Yes" : "No") + "\n";
		if (statsController->problemIsSolved(problemid)) {
			stats += "First solve time: " + statsController->getFirstSolved(problemid).toString("dd-MM-yyyy hh:mm:ss") + "\n";
			stats += "Number of correct submissions: " + QString::number(statsController->getNumCorrectofProblem(problemid)) + "\n";
		}
	}

	delete statsController;

	this->statsDialog->setStats(stats);
	this->statsDialog->exec();
}

void MainController::showResults() {
	if (this->standingsController) {
		delete this->standingsController;
		this->standingsController = NULL;
	}
	this->standingsController = new StandingsController(this->readDataController->getScoreboard(),
														this->readDataController->getEvents());
	this->standingsController->initStandings(this->mainDialog->getSelectedCategory());
	int curRank = 1;
	QList<ResultTeam> teams;
	QList<Model::RankedTeam *> ranking = this->standingsController->getCurrentRanking();
	for (int i = 0; i < ranking.size(); i++) {
		ResultTeam team;
		Model::RankedTeam *rankedTeam = ranking.at(i);
		team.name = rankedTeam->getName();
		team.id = rankedTeam->getId();
		team.solved = rankedTeam->getNumSolved();
		if (i > 0) {
			Model::RankedTeam *prevTeam = ranking.at(i - 1);
			if (rankedTeam->getNumSolved() == prevTeam->getNumSolved()
					&& rankedTeam->getTotalTime() == prevTeam->getTotalTime()) {
				team.rank = curRank;
			} else {
				curRank = i + 1;
				team.rank = curRank;
			}
		} else {
			team.rank = 1;
			curRank = 1;
		}
		team.time = rankedTeam->getTotalTime();
		QList<ResultProblem> problems;
		for (int j = 0; j < rankedTeam->getNumProblems(); j++) {
			Model::RankedProblem *rankedProblem = rankedTeam->getProblem(j);
			ResultProblem problem;
			problem.numTries = rankedProblem->tries;
			problem.state = rankedProblem->problemState;
			problem.problemId = rankedProblem->id;
			problem.time = rankedProblem->timeLastTry;
			problems.append(problem);
		}
		team.problems = problems;
		teams.append(team);
	}
	this->resultsWindow->reload();
	this->resultsWindow->setTeams(teams);
	this->resultsWindow->showFullScreen();
}

void MainController::setBrandingImage(QString image) {
	this->resultsWindow->setBrandingImageFile(image);
}

void MainController::updateStanding() {
	if (this->standingsController->nextStanding()) {
		QList<Model::RankedTeam *> ranking = this->standingsController->getCurrentRanking();
		QList<ResultTeam> teams;
		int curRank = 1;
		for (int i = 0; i < ranking.size(); i++) {
			ResultTeam team;
			Model::RankedTeam *rankedTeam = ranking.at(i);
			team.name = rankedTeam->getName();
			team.id = rankedTeam->getId();
			team.solved = rankedTeam->getNumSolved();
			if (i > 0) {
				Model::RankedTeam *prevTeam = ranking.at(i - 1);
				if (rankedTeam->getNumSolved() == prevTeam->getNumSolved()
						&& rankedTeam->getTotalTime() == prevTeam->getTotalTime()) {
					team.rank = curRank;
				} else {
					curRank = i + 1;
					team.rank = curRank;
				}
			} else {
				team.rank = 1;
				curRank = 1;
			}
			team.time = rankedTeam->getTotalTime();
			QList<ResultProblem> problems;
			for (int j = 0; j < rankedTeam->getNumProblems(); j++) {
				Model::RankedProblem *rankedProblem = rankedTeam->getProblem(j);
				ResultProblem problem;
				problem.numTries = rankedProblem->tries;
				problem.state = rankedProblem->problemState;
				problem.problemId = rankedProblem->id;
				problem.time = rankedProblem->timeLastTry;
				problems.append(problem);
			}
			team.problems = problems;
			teams.append(team);
		}
		this->resultsWindow->setTeams(teams, true,
									  this->standingsController->getLastResolvedTeam(),
									  this->standingsController->getLastResolvedProblem(),
									  this->standingsController->getCurrentPos());
	}
}

} // namespace Controller
} // namespace DJ
