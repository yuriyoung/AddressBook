#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QStringList>
#include <QString>

class Preferences
{
public:
    Preferences();

    ~Preferences();

    QString     m_lastOpenFilePath;
    QStringList m_backgrounds;
    QStringList m_backmusics;
    int         m_backgroundId;
    int         m_backmusicId;
    QList<QVariant>  m_rgbFontColor;

private:
    void reset();
    void load();
    void save();
};

#endif // PREFERENCES_H
