#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>

namespace DJ {
namespace View {

namespace Ui {
	class StatsDialog;
}

class StatsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit StatsDialog(QWidget *parent = 0);
	~StatsDialog();
	void setStats(QString stats);

private:
	Ui::StatsDialog *ui;
};


} // namespace View
} // namespace DJ
#endif // STATSDIALOG_H
