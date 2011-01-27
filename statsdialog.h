/** \file statsdialog.h
  * \brief Contains the stats dialog.
  */

#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>

namespace DJ {
namespace View {

namespace Ui {
	class StatsDialog;
}

/** The stats dialog (uses a Qt Designer form).
  */
class StatsDialog : public QDialog
{
	Q_OBJECT

public:
	/** Constructs a new stats dialog.
	  * \param parent The parent of this dialog.
	  */
	explicit StatsDialog(QWidget *parent = 0);
	/** Destructs the stats dialog.
	  */
	~StatsDialog();
	/** Sets the stats to display.
	  * \param stats The stats to display.
	  */
	void setStats(QString stats);

private:
	Ui::StatsDialog *ui;
};


} // namespace View
} // namespace DJ
#endif // STATSDIALOG_H
