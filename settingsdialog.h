#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QAbstractButton>

namespace DJ {
namespace View {

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

public slots:
	int exec();
	void accept();

private slots:
	void on_buttonBox_clicked(QAbstractButton* button);
	void on_buttonBrandingBrowse_clicked();
	void on_buttonBrandingClear_clicked();

private:
	void reloadSettings();
	void applyChanges();
	void setBrandingImage(QString filename);

	Ui::SettingsDialog *ui;
	QSettings settings;
};


} // namespace View
} // namespace DJ
#endif // SETTINGSDIALOG_H
