#include "maindialog.h"
#include "ui_maindialog.h"

#include <QVariant>
#include <QDebug>

#include "contest.h"
#include "category.h"

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
	this->ui->groupBoxCategories->setEnabled(false);
	this->ui->groupBoxMode->setEnabled(false);

	this->ui->labelContestName->setText("");
	this->ui->labelContestStart->setText("");
	this->ui->labelContestFreeze->setText("");
	this->ui->labelContestEnd->setText("");
	this->ui->labelContestUnfreeze->setText("");
	this->ui->labelContestPenalty->setText("");

	qDeleteAll(this->categoryCheckboxes);
	this->categoryCheckboxes.clear();
	this->selectedCategoriesHash.clear();
}

void MainDialog::displayContest(Model::Contest *contest, QHash<int, Model::Category *> categories) {
	qDeleteAll(this->categoryCheckboxes);
	this->categoryCheckboxes.clear();

	this->ui->groupBoxContest->setEnabled(true);
	this->ui->groupBoxCategories->setEnabled(true);

	this->ui->labelContestName->setText(contest->getName());
	this->ui->labelContestStart->setText(contest->getStart().toString());
	this->ui->labelContestFreeze->setText(contest->getFreeze().toString());
	this->ui->labelContestEnd->setText(contest->getEnd().toString());
	this->ui->labelContestUnfreeze->setText(contest->getUnfreeze().toString());
	this->ui->labelContestPenalty->setText(QString::number(contest->getPenaltyMinutes()) + " minutes");

	int row = 0;
	foreach (auto category, categories) {
		QString categoryText = category->getName();
		categoryText += " (";
		int numTeams = category->numTeams();
		categoryText += QString::number(numTeams);
		categoryText += " ";
		categoryText += (numTeams == 1) ? "team)" : "teams)";
		QCheckBox *checkbox = new QCheckBox(categoryText);
		checkbox->setProperty("category", QVariant::fromValue(category));
		this->ui->gridLayoutCategories->addWidget(checkbox, row, 0);
		this->categoryCheckboxes.append(checkbox);
		++row;

		connect(checkbox, &QCheckBox::clicked, this, &MainDialog::categoryCheckboxClicked);
	}
}

void MainDialog::categoryCheckboxClicked(bool checked) {
	QCheckBox *sender = (QCheckBox *)this->sender();
	Model::Category *category = sender->property("category").value<Model::Category *>();
	if (checked) {
		this->selectedCategoriesHash[category->getId()] = category;
		this->ui->groupBoxMode->setEnabled(true);
	} else {
		this->selectedCategoriesHash.remove(category->getId());
		if (this->selectedCategoriesHash.isEmpty()) {
			this->ui->groupBoxMode->setEnabled(false);
		}
	}
}

QHash<int, Model::Category *> MainDialog::selectedCategories() {
	return this->selectedCategoriesHash;
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
