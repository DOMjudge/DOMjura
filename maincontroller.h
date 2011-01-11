#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>

#include "maindialog.h"
#include "aboutdialog.h"

#include "readdatacontroller.h"

namespace DJ {
namespace Controller {

class MainController : public QObject {
	Q_OBJECT
public:
	explicit MainController(QObject *parent = 0);
	~MainController();
	void showMainWindow();

signals:

private:
	View::MainDialog *mainDialog;
	View::AboutDialog *aboutDialog;
	ReadDataController *readDataController;

private slots:
	void updateURL(QString url);
	void updateUsername(QString username);
	void updatePassword(QString password);
	void updateDir(QDir dir);
	void switchToDir();
	void switchToURL();
	void loadData();
	void enableSave();
	void saveXML(QString dir);
};
} // namespace Controller
} // namespace DJ

#endif // MAINCONTROLLER_H
