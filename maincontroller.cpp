#include "maincontroller.h"

#include <QMessageBox>
#include <QDebug>

#include <QJsonArray>

#include "domjudgeapimanager.h"
#include "contest.h"
#include "category.h"
#include "team.h"
#include "problem.h"
#include "submission.h"
#include "judging.h"

namespace DJ {
namespace Controller {

MainController::MainController(QObject *parent) : QObject(parent) {

	this->mainDialog = new View::MainDialog;
	this->aboutDialog = new View::AboutDialog(this->mainDialog);
	this->settingsDialog = new View::SettingsDialog(this->mainDialog);
	this->resultsWindow = new View::ResultsWindow;
	this->standingsController = NULL;
	this->contest = NULL;

	connect(this->mainDialog, &View::MainDialog::aboutClicked, this->aboutDialog, &View::AboutDialog::exec);
	connect(this->mainDialog, &View::MainDialog::connectClicked, this, &MainController::connectToServer);
	connect(this->mainDialog, &View::MainDialog::settingsClicked, this->settingsDialog, &View::SettingsDialog::exec);
	connect(this->mainDialog, &View::MainDialog::startClicked, this, &MainController::showResults);

	Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
	connect(apiManager, &Shared::DomjudgeApiManager::userLoaded, this, &MainController::processUser);
	connect(apiManager, &Shared::DomjudgeApiManager::userDataFailedLoading, this, &MainController::processContestLoadError);
	connect(apiManager, &Shared::DomjudgeApiManager::contestDataLoaded, this, &MainController::processContestData);
	connect(apiManager, &Shared::DomjudgeApiManager::contestDataFailedLoading, this, &MainController::processContestLoadError);
	connect(apiManager, &Shared::DomjudgeApiManager::categoriesDataLoaded, this, &MainController::processCategoriesData);
	connect(apiManager, &Shared::DomjudgeApiManager::categoriesDataFailedLoading, this, &MainController::processContestLoadError);
	connect(apiManager, &Shared::DomjudgeApiManager::teamsDataLoaded, this, &MainController::processTeamData);
	connect(apiManager, &Shared::DomjudgeApiManager::teamsDataFailedLoading, this, &MainController::processContestLoadError);
	connect(apiManager, &Shared::DomjudgeApiManager::problemsDataLoaded, this, &MainController::processProblemData);
	connect(apiManager, &Shared::DomjudgeApiManager::problemsDataFailedLoading, this, &MainController::processContestLoadError);

	connect(apiManager, &Shared::DomjudgeApiManager::submissionsDataLoaded, this, &MainController::processSubmissionData);
	connect(apiManager, &Shared::DomjudgeApiManager::submissionsDataFailedLoading, this, &MainController::processEventLoadError);
	connect(apiManager, &Shared::DomjudgeApiManager::judgingsDataLoaded, this, &MainController::processJudgingData);
	connect(apiManager, &Shared::DomjudgeApiManager::judgingsDataFailedLoading, this, &MainController::processEventLoadError);

	connect(this->resultsWindow, &View::ResultsWindow::newStandingNeeded, this, &MainController::updateStanding);
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

void MainController::connectToServer() {
	if (this->contest) {
		delete this->contest;
		this->contest = NULL;
	}

	if (!this->teams.empty()) {
		qDeleteAll(this->teams);
		this->teams.clear();
	}

	if (!this->categories.empty()) {
		qDeleteAll(this->categories);
		this->categories.clear();
	}

	if (!this->problems.empty()) {
		qDeleteAll(this->problems);
		this->problems.clear();
	}

	if (!this->judgings.empty()) {
		qDeleteAll(this->judgings);
		this->judgings.clear();
	}

	if (!this->submissions.empty()) {
		qDeleteAll(this->submissions);
		this->submissions.clear();
	}

	Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
	apiManager->setConnectionInfo(this->mainDialog->getProtocol(), this->mainDialog->getURL(), this->mainDialog->getUsername(), this->mainDialog->getPassword());
	apiManager->loadUserData();
}

void MainController::processUser(QJsonDocument userData) {
	QJsonObject user = userData.object();
	QJsonArray roles = user.value("roles").toArray();
	if (!roles.contains(QString("admin")) && !roles.contains(QString("jury"))) {
		this->processContestLoadError("You need at least the jury or admin role. Did you forget to enter a username / password?");
	} else {
		Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
		apiManager->loadContestData();
	}
}

void MainController::processContestData(QJsonDocument contestData) {
	this->contest = new Model::Contest(contestData.object());
	Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
	apiManager->loadCategoriesData();
}

void MainController::processContestLoadError(QString error) {
	QMessageBox::warning(this->mainDialog, "DOMjura failed loading data", "Failed loading contest data: " + error);

	this->mainDialog->hideContest();
}

void MainController::processCategoriesData(QJsonDocument categoriesData) {
	foreach (auto categoryValue, categoriesData.array()) {
		auto categoryObject = categoryValue.toObject();
		Model::Category *category = new Model::Category(categoryObject);
		this->categories[category->getId()] = category;
	}

	Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
	apiManager->loadTeamData();
}

void MainController::processTeamData(QJsonDocument teamData) {
	foreach (auto teamValue, teamData.array()) {
		auto teamObject = teamValue.toObject();
		Model::Team *team = new Model::Team(teamObject, this->categories);
		if (team->getCategory()) {
			// Only teams in a category are useful
			this->teams[team->getId()] = team;
		} else {
			delete team;
		}
	}

	Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
	apiManager->loadProblemData(this->contest->getId());
}

void MainController::processProblemData(QJsonDocument problemData) {
	foreach (auto problemValue, problemData.array()) {
		auto problemObject = problemValue.toObject();
		Model::Problem *problem = new Model::Problem(problemObject);
		this->problems[problem->getId()] = problem;
	}

	this->mainDialog->displayContest(this->contest, this->categories);
}

void MainController::processEventLoadError(QString error) {
	QMessageBox::warning(this->mainDialog, "DOMjura failed loading data", "Failed loading submissions and/or judgings: " + error);
}

void MainController::processSubmissionData(QJsonDocument submissionData) {
	foreach (QJsonValue submissionValue, submissionData.array()) {
		QJsonObject submisionObject = submissionValue.toObject();
		Model::Submission *submission = new Model::Submission(submisionObject,
															  this->teams,
															  this->problems);
		if (submission->getProblem() && submission->getTeam()) {
			this->submissions[submission->getId()] = submission;
		} else {
			delete submission;
		}
	}

	Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
	apiManager->loadJudgings();
}

void MainController::processJudgingData(QJsonDocument judgingData) {
	foreach (QJsonValue judgingValue, judgingData.array()) {
		QJsonObject judgingObject = judgingValue.toObject();
		Model::Judging *judging = new Model::Judging(judgingObject,
													 this->submissions);
		if (judging->getSubmission()) {
			this->judgings.append(judging);
		} else {
			delete judging;
		}
	}

	if (this->standingsController) {
		delete this->standingsController;
		this->standingsController = NULL;
	}

	QHash<int, Model::Team *> selectedTeams;
	QList<int> selectedCategories = this->mainDialog->selectedCategories().keys();
	foreach (Model::Team *team, this->teams) {
		if (selectedCategories.contains(team->getCategory()->getId())) {
			selectedTeams[team->getId()] = team;
		}
	}

	this->standingsController = new StandingsController(this->contest,
														selectedTeams,
														this->problems,
														this->judgings);
	this->standingsController->initStandings();

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
			problem.problemId = rankedProblem->shortname;
			problem.time = rankedProblem->timeLastTry;
			problems.append(problem);
		}
		team.problems = problems;
		teams.append(team);
	}

	this->resultsWindow->setContest(this->contest);
	this->resultsWindow->stopAnimations();
	this->resultsWindow->reload();
	this->resultsWindow->setTeams(teams);
	this->resultsWindow->showFullScreen();
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

void MainController::showResults() {
	if (!this->judgings.empty()) {
		qDeleteAll(this->judgings);
		this->judgings.clear();
	}

	if (!this->submissions.empty()) {
		qDeleteAll(this->submissions);
		this->submissions.clear();
	}

	Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
	apiManager->loadSubmissions();
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
				problem.problemId = rankedProblem->shortname;
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
	} else {
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
				problem.problemId = rankedProblem->shortname;
				problem.time = rankedProblem->timeLastTry;
				problems.append(problem);
			}
			team.problems = problems;
			teams.append(team);
		}
		this->resultsWindow->setTeams(teams, false);
		this->resultsWindow->setResolvDone();
	}
}

} // namespace Controller
} // namespace DJ
