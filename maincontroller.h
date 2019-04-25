/** \file maincontroller.h
  * \brief Contains the main controller class.
  */
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QJsonDocument>

#include "maindialog.h"
#include "aboutdialog.h"
#include "resultswindow.h"
#include "settingsdialog.h"

#include "standingscontroller.h"

namespace DJ {
namespace Controller {

/** Controls all the other controllers, the views and the model.
  */
class MainController : public QObject {
    Q_OBJECT
public:
    /** Constructs a new main controller.
      * \param parent The parent of this object.
      */
    explicit MainController(QObject *parent = 0);
    /** Destructs the main controller.
      */
    ~MainController();
    /** Shows the main windows.
      */
    void showMainWindow();

signals:

private:
    View::MainDialog *mainDialog;
    View::AboutDialog *aboutDialog;
    View::ResultsWindow *resultsWindow;
    View::SettingsDialog *settingsDialog;

    StandingsController *standingsController;
    Model::Contest *contest;
    QHash<QString, Model::Contest *> contests;
    QHash<QString, Model::Group *> groups;
    QHash<QString, Model::Team *> teams;
    QHash<QString, Model::Problem *> problems;
    QHash<QString, Model::Submission *> submissions;
    QList<Model::Judging *> judgings;

private slots:
    void connectToServer();
    void processUser(QJsonDocument userData);
    void processContestsData(QJsonDocument contestsData);
    void processGroupData(QJsonDocument groupData);
    void processTeamData(QJsonDocument teamData);
    void processProblemData(QJsonDocument problemData);
    void processSubmissionData(QJsonDocument submissionData);
    void processJudgingData(QJsonDocument judgingData);
    void processUserLoadError(QString error);
    void processContestsLoadError(QString error);
    void processGroupsLoadError(QString error);
    void processTeamsLoadError(QString error);
    void processProblemsLoadError(QString error);
    void processEventLoadError(QString error);
//    void updateURL(QString url);
//    void updateUsername(QString username);
//    void updatePassword(QString password);
//    void updateDir(QDir dir);
//    void switchToDir();
//    void switchToURL();
//    void loadData();
//    void enableSave();
//    void enableActions();
//    void saveXML(QString dir);
//    void showStats();
    void showResults();
    void updateStanding();
    void loadContest();

};
} // namespace Controller
} // namespace DJ

#endif // MAINCONTROLLER_H
