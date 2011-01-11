#include "statsdialog.h"
#include "ui_statsdialog.h"

namespace DJ {
namespace View {

StatsDialog::StatsDialog(QWidget *parent) : QDialog(parent),
	ui(new Ui::StatsDialog) {
	ui->setupUi(this);
}

StatsDialog::~StatsDialog() {
	delete ui;
}

void StatsDialog::setStats(QString stats) {
	this->ui->textEditStats->setText(stats);
}

} // namespace View
} // namespace DJ
