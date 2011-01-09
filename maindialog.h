#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

namespace DJ {
namespace View {

namespace Ui {
    class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private:
    Ui::MainDialog *ui;
};


} // namespace View
} // namespace DJ
#endif // MAINDIALOG_H
