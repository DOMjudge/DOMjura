#include "maincontroller.h"

#include <QMessageBox>
#include <QDebug>

#include <QJsonArray>

#include "domjudgeapimanager.h"
#include "contest.h"
#include "team.h"
#include "group.h"
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
    this->standingsController = nullptr;
    this->contest = nullptr;

    connect(this->mainDialog, &View::MainDialog::aboutClicked, this->aboutDialog, &View::AboutDialog::exec);
    connect(this->mainDialog, &View::MainDialog::connectClicked, this, &MainController::connectToServer);
    connect(this->mainDialog, &View::MainDialog::settingsClicked, this->settingsDialog, &View::SettingsDialog::exec);
    connect(this->mainDialog, &View::MainDialog::startClicked, this, &MainController::showResults);
    connect(this->mainDialog, &View::MainDialog::loadContestsClicked, this, &MainController::loadContest);

    Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
    connect(apiManager, &Shared::DomjudgeApiManager::userLoaded, this, &MainController::processUser);
    connect(apiManager, &Shared::DomjudgeApiManager::userDataFailedLoading, this, &MainController::processUserLoadError);
    connect(apiManager, &Shared::DomjudgeApiManager::contestsDataLoaded, this, &MainController::processContestsData);
    connect(apiManager, &Shared::DomjudgeApiManager::contestsDataFailedLoading, this, &MainController::processContestsLoadError);
    connect(apiManager, &Shared::DomjudgeApiManager::teamsDataLoaded, this, &MainController::processTeamData);
    connect(apiManager, &Shared::DomjudgeApiManager::teamsDataFailedLoading, this, &MainController::processTeamsLoadError);
    connect(apiManager, &Shared::DomjudgeApiManager::groupsDataLoaded, this, &MainController::processGroupData);
    connect(apiManager, &Shared::DomjudgeApiManager::groupsDataFailedLoading, this, &MainController::processGroupsLoadError);
    connect(apiManager, &Shared::DomjudgeApiManager::problemsDataLoaded, this, &MainController::processProblemData);
    connect(apiManager, &Shared::DomjudgeApiManager::problemsDataFailedLoading, this, &MainController::processProblemsLoadError);

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
        this->standingsController = nullptr;
    }
}

void MainController::showMainWindow() {
    this->mainDialog->show();
}

void MainController::connectToServer() {
    this->contest = this->mainDialog->getContest();

    if (!this->groups.empty()) {
        qDeleteAll(this->groups);
        this->groups.clear();
    }

    if (!this->teams.empty()) {
        qDeleteAll(this->teams);
        this->teams.clear();
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

    this->mainDialog->hideContest();

    Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
    apiManager->setConnectionInfo(this->mainDialog->getProtocol(), this->mainDialog->getURL(), this->mainDialog->getUsername(), this->mainDialog->getPassword());
    apiManager->loadUserData();
}

void MainController::processUser(QJsonDocument userData) {
    QJsonObject user = userData.object();
    QJsonArray roles = user.value("roles").toArray();
    if (!roles.contains(QString("admin")) && !roles.contains(QString("jury"))) {
        this->processUserLoadError("You need at least the jury or admin role. Did you forget to enter a username / password?");
    } else {
        Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
        apiManager->loadGroupsData(this->contest->getId());
    }
}

void MainController::processUserLoadError(QString error){
    QMessageBox::warning(this->mainDialog, "DOMjura failed loading data", "Failed loading user data: " + error);

    this->mainDialog->hideContest();
}

void MainController::processContestsData(QJsonDocument contestsData) {
    foreach (auto contestValue, contestsData.array()) {
        auto contestObject = contestValue.toObject();
        Model::Contest *contest = new Model::Contest(contestObject);
        this->contests[contest->getId()] = contest;
    }

    this->mainDialog->setContestsComboboxData(this->contests);
}

void MainController::processContestsLoadError(QString error) {
    QMessageBox::warning(this->mainDialog, "DOMjura failed loading data", "Failed loading contests data: " + error);

    this->mainDialog->hideContest();
}

void MainController::processGroupData(QJsonDocument groupsData) {
    foreach (auto groupValue, groupsData.array()) {
        auto groupObject = groupValue.toObject();
        Model::Group *group = new Model::Group(groupObject);
        this->groups[group->getId()] = group;
    }

    Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
    apiManager->loadTeamData(this->contest->getId());
}

void MainController::processGroupsLoadError(QString error){
    QMessageBox::warning(this->mainDialog, "DOMjura failed loading data", "Failed loading groups data: " + error);

    this->mainDialog->hideContest();
}

void MainController::processTeamData(QJsonDocument teamData) {
    foreach (auto teamValue, teamData.array()) {
        auto teamObject = teamValue.toObject();
        Model::Team *team = new Model::Team(teamObject, this->groups);
        if (team->getGroup()) {
            // Only teams in a category are useful
            this->teams[team->getId()] = team;
        } else {
            delete team;
        }
    }

    Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
    apiManager->loadProblemData(this->contest->getId());
}

void MainController::processTeamsLoadError(QString error){
    QMessageBox::warning(this->mainDialog, "DOMjura failed loading data", "Failed loading teams data: " + error);

    this->mainDialog->hideContest();
}

void MainController::processProblemData(QJsonDocument problemData) {
    foreach (auto problemValue, problemData.array()) {
        auto problemObject = problemValue.toObject();
        Model::Problem *problem = new Model::Problem(problemObject);
        this->problems[problem->getId()] = problem;
    }

    this->mainDialog->displayContest(this->contest, this->groups);
}

void MainController::processProblemsLoadError(QString error){
    QMessageBox::warning(this->mainDialog, "DOMjura failed loading data", "Failed loading problems data: " + error);

    this->mainDialog->hideContest();
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
    apiManager->loadJudgings(this->contest->getId());
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
        this->standingsController = nullptr;
    }

    QHash<QString, Model::Team *> selectedTeams;
    QList<QString> selectedGroups = this->mainDialog->selectedGroups().keys();
    foreach (Model::Team *team, this->teams) {
        if (selectedGroups.contains(team->getGroup()->getId())) {
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
            problem.state = rankedProblem->problemState;
            problem.problemId = rankedProblem->shortname;
            if (rankedProblem->problemState == SOLVED){
                problem.time = rankedProblem->timeFirstCorrectTry;
                problem.numTries = rankedProblem->tries;
            } else {
                problem.time = rankedProblem->timeLastTry;
                problem.numTries = rankedProblem->total_tries;
            }
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
//    QString stats;

//    StatsController *statsController = new StatsController(this->readDataController->getScoreboard(), this->readDataController->getEvents());

//    stats += "Total number of submissions: " + QString::number(statsController->getTotalSubmissions()) + "\n";
//    stats += "Total number of correct submissions: " + QString::number(statsController->getTotalCorrect()) + "\n";
//    for (int i = 0; i < this->readDataController->getScoreboard()->getNumProblems(); i++) {
//        QString problemid = this->readDataController->getScoreboard()->getProblem(i)->getId();
//        stats += "\n== Problem " + problemid + " ==\n";
//        stats += "Number of submissions: " + QString::number(statsController->getNumSubmissionsOfProblem(problemid)) + "\n";
//        if (statsController->getNumSubmissionsOfProblem(problemid) > 0) {
//            stats += "Last submission time: " + statsController->getLastSubmission(problemid).toString("dd-MM-yyyy hh:mm:ss") + "\n";
//        }
//        stats += "Is solved? " + QString(statsController->problemIsSolved(problemid) ? "Yes" : "No") + "\n";
//        if (statsController->problemIsSolved(problemid)) {
//            stats += "First solve time: " + statsController->getFirstSolved(problemid).toString("dd-MM-yyyy hh:mm:ss") + "\n";
//            stats += "Number of correct submissions: " + QString::number(statsController->getNumCorrectofProblem(problemid)) + "\n";
//        }
//    }

//    delete statsController;

//    this->statsDialog->setStats(stats);
//    this->statsDialog->exec();
//}

void MainController::loadContest(){
    if (!this->contests.empty()){
        qDeleteAll(this->contests);
        this->contests.clear();
    }

    Shared::DomjudgeApiManager *apiManager = Shared::DomjudgeApiManager::sharedApiManager();
    apiManager->setConnectionInfo(this->mainDialog->getProtocol(), this->mainDialog->getURL(), this->mainDialog->getUsername(), this->mainDialog->getPassword());
    apiManager->loadContestsData();
}

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
    apiManager->loadSubmissions(this->contest->getId());
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
                problem.state = rankedProblem->problemState;
                problem.problemId = rankedProblem->shortname;
                if (rankedProblem->problemState == SOLVED){
                    problem.time = rankedProblem->timeFirstCorrectTry;
                    problem.numTries = rankedProblem->tries;
                } else {
                    problem.time = rankedProblem->timeLastTry;
                    problem.numTries = rankedProblem->total_tries;
                }
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
                problem.state = rankedProblem->problemState;
                problem.problemId = rankedProblem->shortname;
                if (rankedProblem->problemState == SOLVED){
                    problem.time = rankedProblem->timeFirstCorrectTry;
                    problem.numTries = rankedProblem->tries;
                } else {
                    problem.time = rankedProblem->timeLastTry;
                    problem.numTries = rankedProblem->total_tries;
                }
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
