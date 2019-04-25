/** \file maindialog.h
  * \brief Contains the main dialog widget.
  */
#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QWidget>
#include <QCheckBox>

namespace DJ {
namespace Model {
class Contest;
class Group;
}

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
    enum class DisplayMode {
        LIVE,
        LIVE_AND_FREEZE,
        RESULTS
    };

    /** Constructs a new main dialog and initializes its widgets.
      * \param parent The parent of this widget.
      */
    explicit MainDialog(QWidget *parent = nullptr);
    /** Destructs the main dialog.
      */
    ~MainDialog();

    QString getProtocol();
    QString getURL();
    QString getUsername();
    QString getPassword();
    Model::Contest* getContest();
    DisplayMode getDisplayMode();

    void hideContest();
    void displayContest(Model::Contest *contest, QHash<QString, Model::Group *> groups);
    void setContestsComboboxData(QHash<QString, Model::Contest *> contests);
    QHash<QString, Model::Group *> selectedGroups();

signals:
    void aboutClicked();
    void settingsClicked();
    void connectClicked();
    void startClicked();
    void loadContestsClicked();

private slots:
    void on_buttonAbout_clicked();
    void on_buttonSettings_clicked();
    void on_buttonConnect_clicked();
    void on_buttonStart_clicked();
    void on_loadContestsButton_clicked();
    void groupCheckboxClicked(bool checked);

private:
    Ui::MainDialog *ui;
    QList<QCheckBox *> groupCheckboxes;
    QHash<QString, Model::Group *> selectedGroupsHash;
};


} // namespace View
} // namespace DJ

#endif // MAINDIALOG_H
