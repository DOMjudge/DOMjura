#include "maindialog.h"
#include "ui_maindialog.h"

#include <QVariant>
#include <QDebug>

#include "contest.h"
#include "group.h"

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

void MainDialog::on_loadContestsButton_clicked()
{
    this->ui->buttonConnect->setDisabled(true);
    this->ui->contestsComboBox->setDisabled(true);
    this->ui->contestsComboBox->clear();
    emit loadContestsClicked();
}

void MainDialog::setContestsComboboxData(QHash<QString, Model::Contest *> contests) {
    this->ui->contestsComboBox->setEnabled(true);
    this->ui->buttonConnect->setEnabled(true);
    foreach (auto contest, contests){
        this->ui->contestsComboBox->addItem(contest->getName(), QVariant::fromValue(contest));
    }
}

void MainDialog::hideContest() {
    this->ui->groupBoxGroups->setEnabled(false);
    this->ui->groupBoxContest->setEnabled(false);
    this->ui->groupBoxMode->setEnabled(false);

    this->ui->labelContestName->setText("");
    this->ui->labelContestStart->setText("");
    this->ui->labelContestFreeze->setText("");
    this->ui->labelContestEnd->setText("");
    this->ui->labelContestPenalty->setText("");

    qDeleteAll(this->groupCheckboxes);
    this->groupCheckboxes.clear();
    this->selectedGroupsHash.clear();
}

void MainDialog::displayContest(Model::Contest *contest, QHash<QString, Model::Group *> groups) {
    qDeleteAll(this->groupCheckboxes);
    this->groupCheckboxes.clear();

    this->ui->groupBoxGroups->setEnabled(true);
    this->ui->groupBoxContest->setEnabled(true);

    this->ui->labelContestName->setText(contest->getName());
    this->ui->labelContestStart->setText(contest->getStart().toString("yyyy-MM-dd hh:mm:ss"));
    this->ui->labelContestFreeze->setText(contest->getFreeze().toString("yyyy-MM-dd hh:mm:ss"));
    this->ui->labelContestEnd->setText(contest->getEnd().toString("yyyy-MM-dd hh:mm:ss"));
    this->ui->labelContestPenalty->setText(QString::number(contest->getPenaltyMinutes()) + " minutes");

    int row = 0;
    foreach (auto group, groups) {
        QString groupText = group->getName();
        groupText += " (";
        int numTeams = group->numTeams();
        groupText += QString::number(numTeams);
        groupText += " ";
        groupText += (numTeams == 1) ? "team)" : "teams)";
        QCheckBox *checkbox = new QCheckBox(groupText);
        checkbox->setProperty("group", QVariant::fromValue(group));
        this->ui->gridLayoutGroups->addWidget(checkbox, row, 0);
        this->groupCheckboxes.append(checkbox);
        ++row;

        connect(checkbox, &QCheckBox::clicked, this, &MainDialog::groupCheckboxClicked);
    }
}

void MainDialog::groupCheckboxClicked(bool checked) {
    QCheckBox *sender = (QCheckBox *)this->sender();
    Model::Group *group = sender->property("group").value<Model::Group *>();
    if (checked) {
        this->selectedGroupsHash[group->getId()] = group;
        this->ui->groupBoxMode->setEnabled(true);
    } else {
        this->selectedGroupsHash.remove(group->getId());
        if (this->selectedGroupsHash.isEmpty()) {
            this->ui->groupBoxMode->setEnabled(false);
        }
    }
}

QHash<QString, Model::Group *> MainDialog::selectedGroups() {
    return this->selectedGroupsHash;
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

Model::Contest* MainDialog::getContest() {
    return this->ui->contestsComboBox->currentData().value<Model::Contest *>();
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
