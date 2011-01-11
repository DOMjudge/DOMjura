#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QWidget>
#include <QDir>

namespace DJ {
namespace View {

namespace Ui {
	class MainDialog;
}

class MainDialog : public QWidget
{
	Q_OBJECT

public:
	explicit MainDialog(QWidget *parent = 0);
	~MainDialog();

	void setSaveXMLEnabled(bool enabled);

signals:
	void aboutClicked();
	void switchedToDir();
	void switchedToURL();
	void dirChanged(QDir dir);
	void URLChanged(QString url);
	void usernameChanged(QString username);
	void passwordChanged(QString password);
	void loadDataClicked();
	void saveXMLclicked(QString dir);

private slots:
	void on_buttonBrowseLocal_clicked();
	void on_buttonAbout_clicked();
	void on_comboProtocol_currentIndexChanged(QString text);
	void on_lineEditURL_textChanged(QString text);
	void on_lineEditPassword_textChanged(QString text);
	void on_lineEditUsername_textChanged(QString text);
	void on_buttonLoadData_clicked();
	void on_radioDOMjudge_clicked();
	void on_radioLocalXML_clicked();
	void on_buttonSaveXMLData_clicked();

private:
	Ui::MainDialog *ui;

};


} // namespace View
} // namespace DJ
#endif // MAINDIALOG_H
