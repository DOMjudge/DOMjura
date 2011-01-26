#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "defines.h"
#include "gradientcache.h"

#include <QFileDialog>

namespace DJ {
namespace View {

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog) {
	ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() {
	delete ui;
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton* button) {
	switch (ui->buttonBox->buttonRole(button)) {
	case QDialogButtonBox::ApplyRole:
		applyChanges();
		break;
	case QDialogButtonBox::ResetRole:
		settings.clear();
		reloadSettings();
		break;
	default:
		break;
	}
}

void SettingsDialog::reloadSettings() {
	this->ui->lineEditWinner->setText(WINNER_TEXT);
	this->ui->lineEditBranding->setText(BRANDING_IMAGE);

	if (!BRANDING_IMAGE.isEmpty()) {
		QPixmap pixmap(BRANDING_IMAGE);
		if (!pixmap.isNull()) {
			QSize pmSize = pixmap.size();
			if (pmSize.width() > 64 || pmSize.height() > 64) {
				this->ui->labelBranding->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			} else {
				this->ui->labelBranding->setPixmap(pixmap);
			}
		} else {
			this->ui->labelBranding->setPixmap(QPixmap());
		}
	} else {
		this->ui->labelBranding->setPixmap(QPixmap());
	}

	this->ui->spinBoxGold->setValue(GOLD);
	this->ui->spinBoxSilver->setValue(SILVER);
	this->ui->spinBoxBronze->setValue(BRONZE);

	this->ui->spinBoxPenalty->setValue(PENALTY_TIME);
	this->ui->spinBoxHeaderHeight->setValue(HEADER_HEIGHT);
	this->ui->spinBoxTeamHeight->setValue(TEAMITEM_HEIGHT);
	this->ui->spinBoxScrollBelowOffset->setValue(SCROLL_BELOW_OFFSET);
	this->ui->spinBoxResovOffset->setValue(RESOLV_BELOW_OFFSET);

	this->ui->spinBoxNeedToClick->setValue(NEED_TO_CLICK);

	this->ui->spinBoxRankWidth->setValue(RANK_WIDTH);
	this->ui->spinBoxSolvedWidth->setValue(SOLVED_WIDTH);
	this->ui->spinBoxTimeWidth->setValue(TIME_WIDTH);
	this->ui->spinBoxLeftMargin->setValue(LEFT_MARGIN);
	this->ui->spinBoxRightMargin->setValue(RIGHT_MARGIN);

	this->ui->spinBoxLegendaRight->setValue(LEGENDA_RIGHT_OFFSET);
	this->ui->spinBoxLegendaBottom->setValue(LEGENDA_BOTTOM_OFFSET);

	this->ui->spinBoxProbMargin->setValue(PROB_MARGIN);
	this->ui->spinBoxNameProbsMargin->setValue(NAME_PROBS_MARGIN);
	this->ui->spinBoxProbsBelowMargin->setValue(PROBS_BELOW_MARGIN);
	GradientCache::getInstance()->clearCache();
}

int SettingsDialog::exec() {
	reloadSettings();
	return QDialog::exec();
}

void SettingsDialog::accept() {
	applyChanges();
	QDialog::accept();
}

void SettingsDialog::applyChanges() {
	settings.setValue("brandingImage", this->ui->lineEditBranding->text());
	settings.setValue("winnerText", this->ui->lineEditWinner->text());

	settings.setValue("gold", this->ui->spinBoxGold->value());
	settings.setValue("silver", this->ui->spinBoxSilver->value());
	settings.setValue("bronze", this->ui->spinBoxBronze->value());

	settings.setValue("penaltyTime", this->ui->spinBoxPenalty->value());
	settings.setValue("headerHeight", this->ui->spinBoxHeaderHeight->value());
	settings.setValue("teamItemHeight", this->ui->spinBoxTeamHeight->value());
	settings.setValue("scrollBelowOffset", this->ui->spinBoxScrollBelowOffset->value());
	settings.setValue("resolvBelowOffset", this->ui->spinBoxResovOffset->value());

	settings.setValue("needToClick", this->ui->spinBoxNeedToClick->value());

	settings.setValue("rankWidth", this->ui->spinBoxRankWidth->value());
	settings.setValue("timeWidth", this->ui->spinBoxTimeWidth->value());
	settings.setValue("solvedWidth", this->ui->spinBoxSolvedWidth->value());
	settings.setValue("leftMargin", this->ui->spinBoxLeftMargin->value());
	settings.setValue("rightMargin", this->ui->spinBoxRightMargin->value());

	settings.setValue("legendaRightOffset", this->ui->spinBoxLegendaRight->value());
	settings.setValue("legendaBottomOffset", this->ui->spinBoxLegendaBottom->value());

	settings.setValue("probMargin", this->ui->spinBoxProbMargin->value());
	settings.setValue("nameProbsMargin", this->ui->spinBoxNameProbsMargin->value());
	settings.setValue("probsBelowMargin", this->ui->spinBoxProbsBelowMargin->value());
	GradientCache::getInstance()->clearCache();
}

void SettingsDialog::on_buttonBrandingBrowse_clicked() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select branding image"),
													QDir().homePath(),
													tr("Images (*.png *.xpm *.jpg)"));
	setBrandingImage(fileName);
}

void SettingsDialog::on_buttonBrandingClear_clicked() {
	setBrandingImage("");
}

void SettingsDialog::setBrandingImage(QString filename) {
	this->ui->lineEditBranding->setText(filename);
	if (!filename.isEmpty()) {
		QPixmap pixmap(filename);
		if (!pixmap.isNull()) {
			QSize pmSize = pixmap.size();
			if (pmSize.width() > 64 || pmSize.height() > 64) {
				this->ui->labelBranding->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			} else {
				this->ui->labelBranding->setPixmap(pixmap);
			}
		} else {
			this->ui->labelBranding->setPixmap(QPixmap());
		}
	} else {
		this->ui->labelBranding->setPixmap(QPixmap());
	}
}

} // namespace View
} // namespace DJ

