#include "maindialog.h"
#include "ui_maindialog.h"

#include <QFileDialog>

namespace DJ {
namespace View {

MainDialog::MainDialog(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MainDialog) {
	ui->setupUi(this);
}

MainDialog::~MainDialog() {
	delete ui;
}

void MainDialog::on_buttonAbout_clicked() {
	emit aboutClicked();
}

void MainDialog::on_buttonSettings_clicked() {
	emit settingsClicked();
}

void MainDialog::on_buttonTestConnection_clicked() {
	emit testConnectionClicked();
}

QString MainDialog::getProtocol() {
	return this->ui->comboProtocol->currentText();
}

QString MainDialog::getURL() {
	return this->ui->lineEditURL->text();
}

QString MainDialog::getUsername() {
	return this->ui->lineEditUsername->text();
}

QString MainDialog::getPassword() {
	return this->ui->lineEditPassword->text();
}

} // namespace View
} // namespace DJ
