#ifndef GLOBAL_H
#define GLOBAL_H
#include "constances.h"

class QSettings;
class MainWindow;
class EditContactDialog;
class Preferences;
class SettingRegister;
class SettingThemes;
class SettingMusic;

namespace Global
{
    extern QSettings            *g_settings;
    extern MainWindow           *g_mainWindow;
    extern EditContactDialog    *g_editContactDlg;
    extern Preferences          *g_preferences;
    extern SettingRegister      *g_settingRegister;
    extern SettingThemes        *g_settingThemes;
    extern SettingMusic         *g_settingMusic;


    void Start();
    void End();
    QString rootPath();
}

#endif // GLOBAL_H
