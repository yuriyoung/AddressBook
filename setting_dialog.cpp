#include "setting_dialog.h"
#include "ui_settingdialog.h"
#include "setting_register.h"
#include "setting_themes.h"
#include "setting_music.h"

#include "global.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    this->setWindowFlags( this->windowFlags() &~Qt::WindowContextHelpButtonHint);
    ui->settingContents->setViewMode(QListView::IconMode);
    ui->settingContents->setIconSize(QSize(32, 32));
    ui->settingContents->setMovement(QListView::Static);
    ui->settingContents->setMinimumWidth(96);
    ui->settingContents->setMaximumWidth(96);
    ui->settingContents->setSpacing(12);

//    ui->settingPages->addWidget(Global::g_settingRegister);
    ui->settingPages->addWidget(Global::g_settingThemes);
    ui->settingPages->addWidget(Global::g_settingMusic);
    ui->settingPages->addWidget(ui->pageConfig);
    ui->settingPages->addWidget(ui->pageHelp);

    createIcons();
    ui->settingContents->setCurrentRow(0);

    setWindowTitle(tr("System Settings"));

    connect(ui->settingContents,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui->settingPages->setCurrentIndex(ui->settingContents->row(current));
}

void SettingDialog::createIcons()
{
//    QListWidgetItem *regButton = new QListWidgetItem(ui->settingContents);
//    regButton->setIcon(QIcon(":/icons/green/monitor.png"));
//    regButton->setText(tr("Regisger"));
//    regButton->setTextAlignment(Qt::AlignHCenter);
//    regButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *themeButton = new QListWidgetItem(ui->settingContents);
    themeButton->setIcon(QIcon(":/icons/green/flag.png"));
    themeButton->setText(tr("Theme"));
    themeButton->setTextAlignment(Qt::AlignHCenter);
    themeButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *musicButton = new QListWidgetItem(ui->settingContents);
    musicButton->setIcon(QIcon(":/icons/green/music.png"));
    musicButton->setText(tr("Music"));
    musicButton->setTextAlignment(Qt::AlignHCenter);
    musicButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *configButton = new QListWidgetItem(ui->settingContents);
    configButton->setIcon(QIcon(":/icons/green/gear_wheel.png"));
    configButton->setText(tr("System"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *helpButton = new QListWidgetItem(ui->settingContents);
    helpButton->setIcon(QIcon(":/icons/green/help.png"));
    helpButton->setText(tr("Help"));
    helpButton->setTextAlignment(Qt::AlignHCenter);
    helpButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}
