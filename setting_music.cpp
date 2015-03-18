#include "setting_music.h"
#include "ui_settingmusic.h"
#include "global.h"
#include "preferences.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QStringList>
#include <QDebug>

SettingMusic::SettingMusic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingMusic)
{
    ui->setupUi(this);

    initialize();

    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(backmusicChanged(int)) );
    connect(ui->btn_music0, SIGNAL(clicked()), this, SLOT(selectBackmusic()) );
    connect(ui->btn_music1, SIGNAL(clicked()), this, SLOT(selectBackmusic()) );
    connect(ui->btn_music2, SIGNAL(clicked()), this, SLOT(selectBackmusic()) );
    connect(ui->btn_music3, SIGNAL(clicked()), this, SLOT(selectBackmusic()) );
}

SettingMusic::~SettingMusic()
{
    delete ui;
}

void SettingMusic::initialize()
{
    m_mediaObj = NULL;
    // page background init
    ui->buttonGroup->setId(ui->rbtn_music0, 0);
    ui->buttonGroup->setId(ui->rbtn_music1, 1);
    ui->buttonGroup->setId(ui->rbtn_music2, 2);
    ui->buttonGroup->setId(ui->rbtn_music3, 3);

    ui->edt_music0->setText(Global::g_preferences->m_backmusics.at(0));
    ui->edt_music1->setText(Global::g_preferences->m_backmusics.at(1));
    ui->edt_music2->setText(Global::g_preferences->m_backmusics.at(2));
    ui->edt_music3->setText(Global::g_preferences->m_backmusics.at(3));

    int id = Global::g_preferences->m_backmusicId;

    if( id == 0)
    {
        ui->btn_music0->setChecked(true);
        m_curMusic = ui->edt_music0->text();
    }
    else if( id == 1 )
    {
        ui->btn_music1->setChecked(true);
        m_curMusic = ui->edt_music1->text();
    }
    else if( id == 2 )
    {
        ui->btn_music2->setChecked(true);
        m_curMusic = ui->edt_music2->text();
    }
    else if( id == 3 )
    {
        ui->btn_music3->setChecked(true);
        m_curMusic = ui->edt_music3->text();
    }
    m_mediaObj = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(m_curMusic));
    // play again
    connect( m_mediaObj, SIGNAL(aboutToFinish()), this, SLOT(playAgain()) );
    setCurrentBkmusic(id);
}

bool SettingMusic::setBackmusic(const QString &filename)
{
    if( filename.isEmpty() ) return false;
    QFileInfo file(filename);
    if( !file.exists() )
    {
        QMessageBox::warning(this, tr("Set baackmusic failed."),
                             tr("Backmusic media file is missing:%1").arg(filename));
        return false;
    }

    m_curMusic = filename;
    if( m_mediaObj )
    {
        musicStop();
        m_mediaObj->clear();
    }
    m_mediaObj->setCurrentSource(Phonon::MediaSource(m_curMusic));
    // 选择后立即播放?
    musicPlay(true);

    return true;
}

void SettingMusic::setCurrentBkmusic(int id)
{
    backmusicChanged(id);
}

void SettingMusic::musicPlay(bool play)
{
    if( play )
        m_mediaObj->play();
    else
        m_mediaObj->pause();
}

void SettingMusic::musicStop()
{
    m_mediaObj->stop();
}

void SettingMusic::musicPause()
{
    m_mediaObj->pause();
}

void SettingMusic::playAgain()
{
    m_mediaObj->setCurrentSource(Phonon::MediaSource(m_curMusic));
    m_mediaObj->play();
}

void SettingMusic::backmusicChanged(int id)
{
    switch( id )
    {
    case 0:
    {
        ui->edt_music0->setEnabled(true);
        ui->btn_music0->setEnabled(true);
        ui->edt_music1->setEnabled(false);
        ui->btn_music1->setEnabled(false);
        ui->edt_music2->setEnabled(false);
        ui->btn_music2->setEnabled(false);
        ui->edt_music3->setEnabled(false);
        ui->btn_music3->setEnabled(false);

        ui->rbtn_music0->setChecked(true);
        QString bkmusic = ui->edt_music0->text();
        setBackmusic(bkmusic);
    };
        break;
    case 1:
    {
        ui->edt_music0->setEnabled(false);
        ui->btn_music0->setEnabled(false);
        ui->edt_music1->setEnabled(true);
        ui->btn_music1->setEnabled(true);
        ui->edt_music2->setEnabled(false);
        ui->btn_music2->setEnabled(false);
        ui->edt_music3->setEnabled(false);
        ui->btn_music3->setEnabled(false);

        ui->rbtn_music1->setChecked(true);
        QString bkmusic = ui->edt_music1->text();
        setBackmusic(bkmusic);
    };
        break;
    case 2:
    {
        ui->edt_music0->setEnabled(false);
        ui->btn_music0->setEnabled(false);
        ui->edt_music1->setEnabled(false);
        ui->btn_music1->setEnabled(false);
        ui->edt_music2->setEnabled(true);
        ui->btn_music2->setEnabled(true);
        ui->edt_music3->setEnabled(false);
        ui->btn_music3->setEnabled(false);

        ui->rbtn_music2->setChecked(true);
        QString bkmusic = ui->edt_music2->text();
        setBackmusic(bkmusic);
    };
        break;
    case 3:
    {
        ui->edt_music0->setEnabled(false);
        ui->btn_music0->setEnabled(false);
        ui->edt_music1->setEnabled(false);
        ui->btn_music1->setEnabled(false);
        ui->edt_music2->setEnabled(false);
        ui->btn_music2->setEnabled(false);
        ui->edt_music3->setEnabled(true);
        ui->btn_music3->setEnabled(true);

        ui->rbtn_music3->setChecked(true);
        QString bkmusic = ui->edt_music3->text();
        setBackmusic(bkmusic);
    };
        break;
    default:break;// never reached.
    }

    Global::g_preferences->m_backmusicId = id;
    emit updatePlayBtn(true);
}

void SettingMusic::selectBackmusic()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if( !btn ) return;

    QString filename = QFileDialog::getOpenFileName(
           this,
           tr("Open 3dsmax maxscript"),
           Global::g_preferences->m_lastOpenFilePath,
           tr("maxscript files (*.wav *.mp3 *.ogg);;All files(*.*)") );
    if (filename.isEmpty())
    {
        return;
    }
    Global::g_preferences->m_lastOpenFilePath = filename;
    QStringList& bgmusic = Global::g_preferences->m_backmusics;
    if( btn->objectName() == "btn_music0" )
    {
        ui->edt_music0->setText(filename);
        bgmusic.replace(0, filename);
    }
    else if( btn->objectName() == "btn_music1" )
    {
        ui->edt_music1->setText(filename);
        bgmusic.replace(1, filename);
    }
    else if( btn->objectName() == "btn_music2" )
    {
        ui->edt_music2->setText(filename);
        bgmusic.replace(2, filename);
    }
    else if( btn->objectName() == "btn_music3" )
    {
        ui->edt_music3->setText(filename);
        bgmusic.replace(3, filename);
    }
    setBackmusic(filename);
    emit updatePlayBtn(true);
}

