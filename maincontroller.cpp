#include "maincontroller.h"

#include <QDebug>

namespace DJ {
namespace Controller {

MainController::MainController(QObject *parent) : QObject(parent) {
	this->readDataController = new ReadDataController(this);

	this->mainDialog = new View::MainDialog;
	this->aboutDialog = new View::AboutDialog(this->mainDialog);

	connect(this->mainDialog, SIGNAL(URLChanged(QString)), this, SLOT(updateURL(QString)));
	connect(this->mainDialog, SIGNAL(usernameChanged(QString)), this, SLOT(updateUsername(QString)));
	connect(this->mainDialog, SIGNAL(passwordChanged(QString)), this, SLOT(updatePassword(QString)));
	connect(this->mainDialog, SIGNAL(switchedToDir()), this, SLOT(switchToDir()));
	connect(this->mainDialog, SIGNAL(switchedToURL()), this, SLOT(switchToURL()));
	connect(this->mainDialog, SIGNAL(loadDataClicked()), this, SLOT(loadData()));
	connect(this->mainDialog, SIGNAL(saveXMLclicked(QString)), this, SLOT(saveXML(QString)));
	connect(this->mainDialog, SIGNAL(dirChanged(QDir)), this, SLOT(updateDir(QDir)));

	connect(this->mainDialog, SIGNAL(aboutClicked()), this->aboutDialog, SLOT(exec()));

	connect(this->readDataController, SIGNAL(dataRead()), this, SLOT(enableSave()));
}

MainController::~MainController() {
	delete this->mainDialog;
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
}

void MainController::switchToURL() {
	this->readDataController->setOfDir(false);
	this->mainDialog->setSaveXMLEnabled(false);
}

void MainController::loadData() {
	this->readDataController->refresh();
}

void MainController::enableSave() {
	this->mainDialog->setSaveXMLEnabled(true);
}

void MainController::saveXML(QString dir) {
	this->readDataController->saveXML(dir);
}

void MainController::updateDir(QDir dir) {
	this->readDataController->setDir(dir);
}

} // namespace Controller
} // namespace DJ
