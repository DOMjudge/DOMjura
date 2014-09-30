/** \file maincontroller.h
  * \brief Contains the main controller class.
  */
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>

#include "maindialog.h"
#include "aboutdialog.h"
#include "statsdialog.h"
#include "resultswindow.h"
#include "settingsdialog.h"

#include "statscontroller.h"
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
	View::StatsDialog *statsDialog;
	View::ResultsWindow *resultsWindow;
	View::SettingsDialog *settingsDialog;

	StandingsController *standingsController;

private slots:
	void testConnection();
	void processContestData(QJsonObject contestData);
	void processContestLoadError();
//	void updateURL(QString url);
//	void updateUsername(QString username);
//	void updatePassword(QString password);
//	void updateDir(QDir dir);
//	void switchToDir();
//	void switchToURL();
//	void loadData();
//	void enableSave();
//	void enableActions();
//	void saveXML(QString dir);
//	void showStats();
//	void showResults();
//	void updateStanding();

};
} // namespace Controller
} // namespace DJ

#endif // MAINCONTROLLER_H
