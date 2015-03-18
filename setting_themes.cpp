#include "setting_themes.h"
#include "ui_settingthemes.h"
#include "global.h"
#include "preferences.h"
#include "main_window.h"

#include <QPalette>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QColorDialog>

SettingThemes::SettingThemes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingThemes)
{
    ui->setupUi(this);

    initialize();

    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(backgroundChanged(int)) );
    connect(ui->btn_bg0, SIGNAL(clicked()), this, SLOT(selectBackground()) );
    connect(ui->btn_bg1, SIGNAL(clicked()), this, SLOT(selectBackground()) );
    connect(ui->btn_bg2, SIGNAL(clicked()), this, SLOT(selectBackground()) );
    connect(ui->btn_bg3, SIGNAL(clicked()), this, SLOT(selectBackground()) );
    connect(ui->btn_selectColor, SIGNAL(clicked()), this, SLOT(selectFontColor()) );
}

SettingThemes::~SettingThemes()
{
    delete ui;
}

void SettingThemes::initialize()
{
    // page background init
    ui->buttonGroup->setId(ui->rbtn_bg0, 0);
    ui->buttonGroup->setId(ui->rbtn_bg1, 1);
    ui->buttonGroup->setId(ui->rbtn_bg2, 2);
    ui->buttonGroup->setId(ui->rbtn_bg3, 3);

    ui->edt_bg0->setText(Global::g_preferences->m_backgrounds.at(0));
    ui->edt_bg1->setText(Global::g_preferences->m_backgrounds.at(1));
    ui->edt_bg2->setText(Global::g_preferences->m_backgrounds.at(2));
    ui->edt_bg3->setText(Global::g_preferences->m_backgrounds.at(3));

    int id = Global::g_preferences->m_backgroundId;
    if( id == 0)
        ui->rbtn_bg0->setChecked(true);
    else if( id == 1 )
        ui->rbtn_bg1->setChecked(true);
    else if( id == 2 )
        ui->rbtn_bg2->setChecked(true);
    else if( id == 3 )
        ui->rbtn_bg3->setChecked(true);
    setCurrentBkground(id);

    int r = Global::g_preferences->m_rgbFontColor.at(0).toInt();
    int g = Global::g_preferences->m_rgbFontColor.at(1).toInt();
    int b = Global::g_preferences->m_rgbFontColor.at(2).toInt();
    ui->lb_fontColor->setStyleSheet(QString("QLabel { background-color : rgb(%1,%2,%3); }").arg(r).arg(g).arg(b));

}

void SettingThemes::setCurrentBkground(int id)
{
    backgroundChanged(id);
}

bool SettingThemes::setBackground(const QString &filename)
{
    if( filename.isEmpty() ) return false;
    QFileInfo file(filename);
    if( !file.exists() )
    {
        QMessageBox::warning(this, tr("Set baackground failed."),
                             tr("Background image file is missing."));
        return false;
    }

    emit setMainWinBackground(filename);
    return true;
}

/**
 * @ set backbround
 * @brief SettingThemes::backgroundChanged
 * @param id
 */
void SettingThemes::backgroundChanged(int id)
{
    Global::g_preferences->m_backgroundId = id;
    switch( id )
    {
    case 0:
    {
        ui->edt_bg0->setEnabled(true);
        ui->btn_bg0->setEnabled(true);
        ui->edt_bg1->setEnabled(false);
        ui->btn_bg1->setEnabled(false);
        ui->edt_bg2->setEnabled(false);
        ui->btn_bg2->setEnabled(false);
        ui->edt_bg3->setEnabled(false);
        ui->btn_bg3->setEnabled(false);

        ui->rbtn_bg0->setChecked(true);
        QString bkground = ui->edt_bg0->text();
        setBackground(bkground);
    };
        break;
    case 1:
    {
        ui->edt_bg0->setEnabled(false);
        ui->btn_bg0->setEnabled(false);
        ui->edt_bg1->setEnabled(true);
        ui->btn_bg1->setEnabled(true);
        ui->edt_bg2->setEnabled(false);
        ui->btn_bg2->setEnabled(false);
        ui->edt_bg3->setEnabled(false);
        ui->btn_bg3->setEnabled(false);

        ui->rbtn_bg1->setChecked(true);
        QString bkground = ui->edt_bg1->text();
        setBackground(bkground);
    };
        break;
    case 2:
    {
        ui->edt_bg0->setEnabled(false);
        ui->btn_bg0->setEnabled(false);
        ui->edt_bg1->setEnabled(false);
        ui->btn_bg1->setEnabled(false);
        ui->edt_bg2->setEnabled(true);
        ui->btn_bg2->setEnabled(true);
        ui->edt_bg3->setEnabled(false);
        ui->btn_bg3->setEnabled(false);

        ui->rbtn_bg2->setChecked(true);
        QString bkground = ui->edt_bg2->text();
        setBackground(bkground);
    };
        break;
    case 3:
    {
        ui->edt_bg0->setEnabled(false);
        ui->btn_bg0->setEnabled(false);
        ui->edt_bg1->setEnabled(false);
        ui->btn_bg1->setEnabled(false);
        ui->edt_bg2->setEnabled(false);
        ui->btn_bg2->setEnabled(false);
        ui->edt_bg3->setEnabled(true);
        ui->btn_bg3->setEnabled(true);

        ui->rbtn_bg3->setChecked(true);
        QString bkground = ui->edt_bg3->text();
        setBackground(bkground);
    };
        break;
    default:break;// never reached.
    }
}

void SettingThemes::selectBackground()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if( !btn ) return;

    QString filename = QFileDialog::getOpenFileName(
           this,
           tr("Open 3dsmax maxscript"),
           Global::g_preferences->m_lastOpenFilePath,
           tr("maxscript files (*.JPG *.PNG *.BMP);;All files(*.*)") );
    if (filename.isEmpty())
    {
        return;
    }

    Global::g_preferences->m_lastOpenFilePath = filename;
    QStringList& bkgrounds = Global::g_preferences->m_backgrounds;
    if( btn->objectName() == "btn_bg0" )
    {
        ui->edt_bg0->setText(filename);
        bkgrounds.replace(0, filename);
    }
    else if( btn->objectName() == "btn_bg1" )
    {
        ui->edt_bg1->setText(filename);
        bkgrounds.replace(1, filename);
    }
    else if( btn->objectName() == "btn_bg2" )
    {
        ui->edt_bg2->setText(filename);
        bkgrounds.replace(2, filename);
    }
    else if( btn->objectName() == "btn_bg3" )
    {
        ui->edt_bg3->setText(filename);
        bkgrounds.replace(3, filename);
    }
    setBackground(filename);
}

void SettingThemes::selectFontColor()
{
    QColorDialog* clrDlg = new QColorDialog;
    QColor color = clrDlg->getColor();
    int r,g,b;
    color.getRgb(&r, &g, &b);
    ui->lb_fontColor->setStyleSheet(QString("QLabel { background-color : rgb(%1,%2,%3); }").arg(r).arg(g).arg(b));

    Global::g_preferences->m_rgbFontColor.clear();
    Global::g_preferences->m_rgbFontColor << r << g << b;
    emit setMainWinFontColor(r, g, b);
}


