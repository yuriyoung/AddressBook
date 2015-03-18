#ifndef SETTING_REGISTER_H
#define SETTING_REGISTER_H

#include <QWidget>

namespace Ui {
class SettingRegister;
}

class SettingRegister : public QWidget
{
    Q_OBJECT
    
public:
    explicit SettingRegister(QWidget *parent = 0);
    ~SettingRegister();
    
private:
    Ui::SettingRegister *ui;
};

#endif // SETTING_REGISTER_H
