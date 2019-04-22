/** \file settingsdialog.h
  * \brief Contains the settings dialog.
  */
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

/** The settings dialog (this uses a Qt Designer form).
  */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /** Constructs a new settings dialog.
      * \param parent The parent of this widget.
      */
    explicit SettingsDialog(QWidget *parent = 0);
    /** Destructs this settings dialog.
      */
    ~SettingsDialog();

public slots:
    /** Opens the settings dialog (and reloads the settings).
      */
    int exec();
    /** Closes the dialog and saves the settings.
      */
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
