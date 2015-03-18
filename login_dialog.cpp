#include "login_dialog.h"
#include "ui_login_dialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowFlags( this->windowFlags() &~Qt::WindowContextHelpButtonHint);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
