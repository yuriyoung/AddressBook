#include "preferences.h"
#include "global.h"

#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QList>

Preferences::Preferences()
{
    reset();
    load();
}


Preferences::~Preferences()
{
    save();
    qDebug() << "saving";
}

void Preferences::reset()
{
    m_lastOpenFilePath = QDir::homePath();
    QString p = Global::rootPath();
    m_backgrounds << p +"/images/bg0.jpg" << p + "/images/bg1.jpg" \
                  << p + "/images/bg2.jpg" << p + "/images/bg3.jpg";
    m_backmusics << p + "/misc/music0.mp3" << p + "/misc/music1.mp3" \
                 << p + "/misc/music2.mp3" << p + "/misc/music3.mp3";
    m_backgroundId = 0;
    m_backmusicId = 0;
    m_rgbFontColor << 0 << 0 << 0;
}

void Preferences::load()
{
    QSettings *set = Global::g_settings;

    set->beginGroup("Settings");
    m_lastOpenFilePath = set->value("LastOpenFilePath", m_lastOpenFilePath).toString();
    set->endGroup();
    set->beginGroup("Themes");
    m_backgroundId = set->value("BackgoundId", m_backgroundId).toInt();
    m_backgrounds = set->value("Backgound", m_backgrounds).toStringList();
    m_backmusicId = set->value("BackmusicId", m_backmusicId).toInt();
    m_backmusics = set->value("Backmusic", m_backmusics).toStringList();
    m_rgbFontColor = set->value("FontColor", m_rgbFontColor).toList();
    set->endGroup();
}

void Preferences::save()
{
    QSettings *set = Global::g_settings;

    set->beginGroup("Settings");
    set->setValue("LastOpenFilePath", m_lastOpenFilePath);
    set->endGroup();
    set->beginGroup("Themes");
    set->setValue("BackgoundId", m_backgroundId);
    set->setValue("Backgound", m_backgrounds);
    set->setValue("BackmusicId", m_backmusicId);
    set->setValue("Backmusic", m_backmusics);
    set->setValue("FontColor", m_rgbFontColor);
    set->endGroup();
}
