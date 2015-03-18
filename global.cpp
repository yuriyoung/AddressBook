#include "global.h"
#include "main_window.h"
#include "editcontact_dialog.h"
#include "preferences.h"
#include "setting_register.h"
#include "setting_themes.h"
#include "setting_music.h"

#include <QApplication>
#include <QSettings>
#include <QDir>
#include <QDebug>

using namespace Global;

QSettings               *Global::g_settings = 0;

MainWindow              *Global::g_mainWindow = 0;
EditContactDialog       *Global::g_editContactDlg = 0;
Preferences             *Global::g_preferences = 0;
SettingRegister         *Global::g_settingRegister = 0;
SettingThemes           *Global::g_settingThemes = 0;
SettingMusic            *Global::g_settingMusic = 0;

void Global::Start()
{
    QString homePath =  QApplication::applicationDirPath();
    if (homePath.isEmpty())
    {
        g_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                 QString(COMPANY), QString(PROGRAM));
    }
    else
    {
        QString filename = rootPath() + QDir::separator() \
                + QApplication::applicationName() + ".ini";
        g_settings = new QSettings(filename, QSettings::IniFormat);
    }
    g_preferences = new Preferences;
    g_settingMusic = new SettingMusic;
    g_settingThemes = new SettingThemes;
    g_mainWindow = new MainWindow;
    g_editContactDlg = new EditContactDialog(g_mainWindow);

//    g_settingRegister = new SettingRegister;

    g_mainWindow->show();
}

void Global::End()
{
    qDebug() << " end programm...";
    delete g_preferences;
    delete g_settings;
    delete g_settingRegister;
    delete g_settingThemes;
    delete g_settingMusic;
    delete g_editContactDlg;
    delete g_mainWindow;
}


QString Global::rootPath()
{
    return QDir::currentPath();
}
