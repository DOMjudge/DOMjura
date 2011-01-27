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

	/** Enables or disables the "Save XML" button.
	  * \param enabled Whether to enable or disable the button.
	  */
	void setSaveXMLEnabled(bool enabled);
	/** Enables or disables the "Actions" group.
	  * \param enabled Whether to enable or disable the group.
	  */
	void setActionsEnabled(bool enabled);
	/** Sets the categories for the drop down.
	  * \param categories The categories to display.
	  */
	void setCategories(QList<QString> categories);
	/** Selects the "Participants" category, if it exists.
	  */
	void selectParticipantsCategory();
	/** Returns the selected category.
	  * \return The selected category.
	  */
	QString getSelectedCategory();

signals:
	/** The user clicked on the "About" button.
	  */
	void aboutClicked();
	/** The user switched to using a directory.
	  */
	void switchedToDir();
	/** The user switched to using a URL.
	  */
	void switchedToURL();
	/** The user changed the directory.
	  * \param dir The new directory.
	  */
	void dirChanged(QDir dir);
	/** The user changed the URL.
	  * \param url The new URL.
	  */
	void URLChanged(QString url);
	/** The user changed the username.
	  * \param username The new username.
	  */
	void usernameChanged(QString username);
	/** The user changed the password.
	  * \param password The new password.
	  */
	void passwordChanged(QString password);
	/** The user clicked the "Load data" button.
	  */
	void loadDataClicked();
	/** The user clicked the "Save XML" button.
	  * \param dir The directory the user selected.
	  */
	void saveXMLclicked(QString dir);
	/** The user clicked the "Stats" button.
	  */
	void statsClicked();
	/** The user clicked the "Results" button.
	  */
	void resultsClicked();
	/** The user clicked the "Settings" button.
	  */
	void settingsClicked();

private slots:
	void on_buttonStartResultsPresentation_clicked();
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
	void on_buttonViewStats_clicked();

	void on_buttonSettings_clicked();

private:
	Ui::MainDialog *ui;

};


} // namespace View
} // namespace DJ
#endif // MAINDIALOG_H
