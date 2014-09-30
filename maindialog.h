/** \file maindialog.h
  * \brief Contains the main dialog widget.
  */
#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QWidget>
#include <QDir>

namespace DJ {
namespace View {

namespace Ui {
	class MainDialog;
}

/** The main dialog (uses a Qt Designer form).
  */
class MainDialog : public QWidget
{
	Q_OBJECT

public:
	/** Constructs a new main dialog and initializes its widgets.
	  * \param parent The parent of this widget.
	  */
	explicit MainDialog(QWidget *parent = 0);
	/** Destructs the main dialog.
	  */
	~MainDialog();

	QString getProtocol();
	QString getURL();
	QString getUsername();
	QString getPassword();

signals:
	void aboutClicked();
	void settingsClicked();
	void testConnectionClicked();

private slots:
	void on_buttonAbout_clicked();
	void on_buttonSettings_clicked();
	void on_buttonTestConnection_clicked();

private:
	Ui::MainDialog *ui;

};


} // namespace View
} // namespace DJ
#endif // MAINDIALOG_H
