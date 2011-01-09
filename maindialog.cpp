#include "maindialog.h"
#include "ui_maindialog.h"

namespace DJ {
namespace View {

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
}

MainDialog::~MainDialog()
{
    delete ui;
}

} // namespace View
} // namespace DJ
