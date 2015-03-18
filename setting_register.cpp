#include "setting_register.h"
#include "ui_settingregister.h"

SettingRegister::SettingRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingRegister)
{
    ui->setupUi(this);
}

SettingRegister::~SettingRegister()
{
    delete ui;
}
