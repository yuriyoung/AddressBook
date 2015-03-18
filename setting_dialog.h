#ifndef SETTING_DIALOG_H
#define SETTING_DIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();
    
private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void createIcons();

    Ui::SettingDialog *ui;
};

#endif // SETTING_DIALOG_H
