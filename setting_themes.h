#ifndef SETTING_THEMES_H
#define SETTING_THEMES_H

#include <QWidget>

namespace Ui {
class SettingThemes;
}

class SettingThemes : public QWidget
{
    Q_OBJECT
    
public:
    explicit SettingThemes(QWidget *parent = 0);
    ~SettingThemes();
    void setCurrentBkground(int id);

signals:
    void setMainWinBackground(const QString &filename);
    void setMainWinFontColor(int r, int g, int b);

private slots:
    void backgroundChanged(int);
    void selectBackground();
    void selectFontColor();

private:
    bool setBackground(const QString &filename);
    void initialize();

    Ui::SettingThemes *ui;
};

#endif // SETTING_THEMES_H
