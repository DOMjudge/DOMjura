/** \file aboutdialog.h
  * \brief Contains the class for the about dialog.
  */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace DJ {
namespace View {

namespace Ui {
    class AboutDialog;
}

/** The about dialog. This uses a Qt Designer form.
  */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    /** Construts an about dialog.
      * \param parent The parent of this dialog.
      */
    explicit AboutDialog(QWidget *parent = nullptr);

    /** Destructs an about dialog.
      */
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
};


} // namespace View
} // namespace DJ
#endif // ABOUTDIALOG_H
