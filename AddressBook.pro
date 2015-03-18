#-------------------------------------------------
#
# Project created by QtCreator 2013-08-16T08:24:21
#
#-------------------------------------------------

QT       += core gui sql phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AddressBook
TEMPLATE = app


SOURCES += main.cpp \
    main_window.cpp \
    login_dialog.cpp \
    addcontact_dialog.cpp \
    contacts_tableview.cpp \
    favorite_tableview.cpp \
    contactsqlmodel.cpp \
    database_manager.cpp \
    global.cpp \
    group_listwidget.cpp \
    editcontact_dialog.cpp \
    viewdetail_dialog.cpp \
    setting_dialog.cpp \
    preferences.cpp \
    setting_themes.cpp \
    setting_music.cpp \
    about_dialog.cpp

HEADERS  += \
    main_window.h \
    login_dialog.h \
    addcontact_dialog.h \
    contacts_tableview.h \
    favorite_tableview.h \
    constances.h \
    contactsqlmodel.h \
    database_manager.h \
    global.h \
    group_listwidget.h \
    editcontact_dialog.h \
    viewdetail_dialog.h \
    setting_dialog.h \
    preferences.h \
    setting_themes.h \
    setting_music.h \
    about_dialog.h

FORMS    += \
    main_window.ui \
    login_dialog.ui \
    addcontact_dialog.ui \
    editcontact_dialog.ui \
    viewdetaildialog.ui \
    settingdialog.ui \
    settingregister.ui \
    settingthemes.ui \
    settingmusic.ui \
    aboutdialog.ui

RESOURCES += \
    res.qrc

OTHER_FILES += \
    theme_green.theme
