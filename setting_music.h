#ifndef SETTING_MUSIC_H
#define SETTING_MUSIC_H

#include <QWidget>
#include <phonon>
#include <phonon/MediaSource>
#include <phonon/MediaObject>

namespace Ui {
class SettingMusic;
}

class SettingMusic : public QWidget
{
    Q_OBJECT
    
public:
    explicit SettingMusic(QWidget *parent = 0);
    ~SettingMusic();
    
    bool setBackmusic(const QString &filename);
    void setCurrentBkmusic(int id);

signals:
    void updatePlayBtn(bool);

public slots:
    void musicPlay(bool);
    void musicStop();
    void musicPause();
    void playAgain();

private slots:
    void backmusicChanged(int);
    void selectBackmusic();

private:
    void initialize();
    QString m_curMusic;
    Phonon::MediaObject     *m_mediaObj;
    Ui::SettingMusic *ui;
};

#endif // SETTING_MUSIC_H
