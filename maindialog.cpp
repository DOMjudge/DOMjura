#include "maindialog.h"
#include "ui_maindialog.h"

#include <QVariant>
#include <QDebug>

#include "contest.h"

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

void MainDialog::on_buttonConnect_clicked() {
    emit connectClicked();
}

void MainDialog::on_buttonStart_clicked() {
    emit startClicked();
}

void MainDialog::hideContest() {
    this->ui->groupBoxContest->setEnabled(false);
    this->ui->groupBoxMode->setEnabled(false);

    this->ui->labelContestName->setText("");
    this->ui->labelContestStart->setText("");
    this->ui->labelContestFreeze->setText("");
    this->ui->labelContestEnd->setText("");
    this->ui->labelContestPenalty->setText("");

}

void MainDialog::displayContest(Model::Contest *contest) {
    this->ui->groupBoxContest->setEnabled(true);
    this->ui->groupBoxMode->setEnabled(true);

    this->ui->labelContestName->setText(contest->getName());
    this->ui->labelContestStart->setText(contest->getStart().toString("yyyy-MM-dd hh:mm:ss"));
    this->ui->labelContestFreeze->setText(contest->getFreeze().toString("yyyy-MM-dd hh:mm:ss"));
    this->ui->labelContestEnd->setText(contest->getEnd().toString("yyyy-MM-dd hh:mm:ss"));
    this->ui->labelContestPenalty->setText(QString::number(contest->getPenaltyMinutes()) + " minutes");
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

QString MainDialog::getContestId() {
    return this->ui->lineEditContestId->text();
}

MainDialog::DisplayMode MainDialog::getDisplayMode() {
    if (this->ui->radioModeLive->isChecked()) {
        if (this->ui->checkDuringFreeze->isChecked()) {
            return DisplayMode::LIVE_AND_FREEZE;
        } else {
            return DisplayMode::LIVE;
        }
    } else {
        return DisplayMode::RESULTS;
    }
}

} // namespace View
} // namespace DJ
