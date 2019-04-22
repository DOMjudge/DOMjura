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
    explicit MainDialog(QWidget *parent = 0);
    /** Destructs the main dialog.
      */
    ~MainDialog();

    QString getProtocol();
    QString getURL();
    QString getUsername();
    QString getPassword();
    QString getContestId();
    DisplayMode getDisplayMode();

    void hideContest();
    void displayContest(Model::Contest *contest);

signals:
    void aboutClicked();
    void settingsClicked();
    void connectClicked();
    void startClicked();

private slots:
    void on_buttonAbout_clicked();
    void on_buttonSettings_clicked();
    void on_buttonConnect_clicked();
    void on_buttonStart_clicked();

private:
    Ui::MainDialog *ui;
};


} // namespace View
} // namespace DJ

#endif // MAINDIALOG_H
