#include "main_window.h"
#include "ui_main_window.h"
#include "global.h"
#include "about_dialog.h"
#include "addcontact_dialog.h"
#include "editcontact_dialog.h"
#include "viewdetail_dialog.h"
#include "setting_dialog.h"
#include "setting_music.h"
#include "setting_themes.h"
#include "preferences.h"

#include <QListWidgetItem>
#include <QTimer>
#include <QDateTime>
#include <QFileInfo>
#include <QPushButton>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bEdit(false)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 15);
    ui->splitter->setStretchFactor(1, 65);
    ui->splitter->setStretchFactor(2, 20);
    ui->grouplistWidget->setCurrentRow(1);
    ui->findWidget->hide();

    initStatusbar();
    /*connect all signals and slots*/
    setActions();

    updateMenuStatus();
    int r = Global::g_preferences->m_rgbFontColor.at(0).toInt();
    int g = Global::g_preferences->m_rgbFontColor.at(1).toInt();
    int b = Global::g_preferences->m_rgbFontColor.at(2).toInt();
    updateFontColor(r, g, b);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateButton(bool enabled)
{
    ui->action_Delete->setEnabled(enabled);

    ui->action_Edit->setEnabled(enabled);
    if( Global::g_editContactDlg->isHidden())
        ui->action_Edit->setChecked(false);
    else
        ui->action_Edit->setChecked(true);

    if(ui->tabWidget->currentIndex() == 1)
    {
        ui->action_Add->setEnabled(false);
        ui->action_Edit->setEnabled(false);
        ui->action_Favorites->setEnabled(false);
    }
    else
    {
        ui->action_Edit->setEnabled(enabled);
        ui->action_Favorites->setEnabled(enabled);
    }

    ui->action_Email->setEnabled(enabled);
    ui->action_Details->setEnabled(enabled);
}

/**
 * @author Macai
 * @connect signal and slot
 * @brief MainWindow::setActions
 */
void MainWindow::setActions()
{
    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()) );
    connect(ui->action_Add, SIGNAL(triggered()), this, SLOT(showAddContactDlg()) );
    connect(ui->action_Delete, SIGNAL(triggered()),
            this, SLOT(deleteContact()) );
    connect(ui->action_Edit, SIGNAL(triggered()), this, SLOT(showEditContactDlg()) );
    connect(ui->action_Query, SIGNAL(triggered()), this, SLOT(showFindWidget()) );
    connect(ui->action_Favorites, SIGNAL(triggered()), this, SLOT(addFavourite()) );
    connect(ui->action_Details, SIGNAL(triggered()), this, SLOT(showDetailDlg()) );
    connect(ui->action_Settings, SIGNAL(triggered()), this, SLOT(showSettingDlg()) );
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(showAboutDlg()) );

    connect(ui->menu_Background, SIGNAL(triggered(QAction*)), this, SLOT(backgroundChanged(QAction*)) );
    connect(ui->menu_Backmusic, SIGNAL(triggered(QAction*)), this, SLOT(backmusicChanged(QAction*)) );

    connect(Global::g_settingThemes, SIGNAL(setMainWinBackground(QString)),
            this, SLOT(setBackground(QString)) );
    connect(Global::g_settingThemes, SIGNAL(setMainWinFontColor(int,int,int)),
            this, SLOT(updateFontColor(int,int,int)) );


    connect(ui->btn_findBtn, SIGNAL(clicked(bool)), this, SLOT(findContact(bool)) );

    connect(ui->grouplistWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(filterGroup(QListWidgetItem*, QListWidgetItem*)) );
    connect(ui->grouplistWidget, SIGNAL(updateGroupContacts(const QString&)),
            this, SLOT(updateGroupContacts(const QString&)) );

    connect(ui->contactTableView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(showDetail(QModelIndex)) );
    connect(ui->contactTableView, SIGNAL(updateButton(bool)),
            this, SLOT(updateButton(bool)) );

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(tabWidgetChanged(int)) );

    connect(ui->favoritTableView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(showDetail(QModelIndex)) );
    connect(ui->favoritTableView, SIGNAL(updateButton(bool)),
            this, SLOT(updateButton(bool)) );
    connect(ui->favoritTableView, SIGNAL(findContactAct()),
            this, SLOT(showFindWidget()) );
    connect(ui->favoritTableView, SIGNAL(deleteContactAct()),
            this, SLOT(deleteContact()) );
    connect(ui->favoritTableView, SIGNAL(sendMailAct()),
            this, SLOT(showDetailDlg()) );
    connect(ui->favoritTableView, SIGNAL(viewDetailAct()),
            this, SLOT(showDetailDlg()) );

    connect(ui->contactTableView, SIGNAL(addContactAct()),
            this, SLOT(showAddContactDlg()) );
    connect(ui->contactTableView, SIGNAL(editContactAct()),
            this, SLOT(showEditContactDlg()) );
    connect(ui->contactTableView, SIGNAL(findContactAct()),
            this, SLOT(showFindWidget()) );
    connect(ui->contactTableView, SIGNAL(deleteContactAct()),
            this, SLOT(deleteContact()) );
    connect(ui->contactTableView, SIGNAL(addFavouriteAct()),
            this, SLOT(addFavourite()) );
    connect(ui->contactTableView, SIGNAL(sendMailAct()),
            this, SLOT(showDetailDlg()) );
    connect(ui->contactTableView, SIGNAL(viewDetailAct()),
            this, SLOT(showDetailDlg()) );
}

/**
 * @author Macai
 * @slot function: show dialog for add a contact
 * @brief MainWindow::showAddDlg
 */
void MainWindow::showAddContactDlg()
{
    AddContactDialog* addDlg = new AddContactDialog(this);

    connect(addDlg, SIGNAL(updateGroup(QString)),
            ui->grouplistWidget, SLOT(updateGroup(QString)) );
    connect(addDlg, SIGNAL(submitContact(QList<QVariant>)),
            ui->contactTableView, SLOT(addContact(QList<QVariant>)) );

    QStringList list;
    int cnt = ui->grouplistWidget->count();
    for( int i = 0; i < cnt; ++i)
    {
        QString text = ui->grouplistWidget->item(i)->text();
        if( text == GROUP_ALL) continue;
        list.append(text);
    }
    int idx = ui->grouplistWidget->currentRow();
    addDlg->initGroupCombobox(idx, list);

    int ret = addDlg->exec();
    addDlg->deleteLater();
}

void MainWindow::showDetail(const QModelIndex index)
{
    if( ! index.isValid() )
    {
        return;
    }

    Global::g_editContactDlg->readContactInfo(index);
    Global::g_editContactDlg->setCurrentModelIndex(index);

    QString txt;
    int r = index.row();
    QString group = tr("<b>Group:</b>") + index.model()->index(r, GROUP).data().toString();
    QString lname = tr("<b>Last Name:</b>") + index.model()->index(r, LASTNAME).data().toString();
    QString fname = tr("<b>First Name:</b>") + index.model()->index(r, FirstName).data().toString();
    QString nname = tr("<b>Nick Name:</b>") + index.model()->index(r, NICKNAME).data().toString();
    QString sex = tr("<b>Sex:</b>") + index.model()->index(r, SEX).data().toString();
    QString age = tr("<b>Age:</b>") + index.model()->index(r, AGE).data().toString();
    QString birth = tr("<b>Birth Day:</b>") + index.model()->index(r, BIRTHDAY).data().toString();
    QString mobile = tr("<b>Mobile:</b>") + index.model()->index(r, MOBILE).data().toString();
    QString email = tr("<b>E-mail:</b>") + index.model()->index(r, EMAIL).data().toString();
    QString qq = tr("<b>QQ:</b>") + index.model()->index(r, QQ).data().toString();

    QByteArray photo = index.model()->index(r, PHOTO).data().toByteArray();

    QString office = tr("<b>Office:</b>") + index.model()->index(r, OFFICE).data().toString();
//    QString officeTel = tr("<b>Office Tel:</b>") + index.model()->index(r, OFFICETEL).data().toString();
    QString officeAddr = tr("<b>Office Address:</b>") + index.model()->index(r, OFFICEADDR).data().toString();
//    QString officeCode = tr("<b>Office Code:</b>") + index.model()->index(r, OFFICECODE).data().toString();
//    QString homeTel = tr("<b>Home Tel:</b>") + index.model()->index(r, HOMETEL).data().toString();
    QString homeAddr = tr("<b>Home Address:</b>") + index.model()->index(r, HOMEADDR).data().toString();
//    QString homeCode = tr("<b>Home Code:</b>") + index.model()->index(r, HOMECODE).data().toString();
    QString weibo = tr("<b>Weibo:</b>") + index.model()->index(r, WEIBO).data().toString();
    QString site = tr("<b>WebSite:</b>") + index.model()->index(r, WEBSITE).data().toString();

    txt = /*id + "<br>" +*/ group + "<br>" + lname + "<br>" + fname + "<br>" + nname + "<br>" + sex + "<br>" + age + "<br>" + \
          birth + "<br>" + mobile + "<br>" + email + "<br>" + qq + "<br>" + office + "<br>" + /*officeTel +"\n" +*/ \
          officeAddr + "<br>" /*+ officeCode + "<br>"*/ + /*homeTel + "\n" +*/ homeAddr + "<br>" + /*homeCode + "\n" +*/ \
          weibo + "<br>" + site;
    ui->detailsLabel->setText( txt );
    setPhoto(photo);
}

void MainWindow::showAboutDlg()
{
    AboutDialog* aboutDlg = new AboutDialog(this);
    aboutDlg->exec();
}

void MainWindow::showDetailDlg()
{
    ViewDetailDialog* detailDlg = new ViewDetailDialog(this);


    int tab = ui->tabWidget->currentIndex();
    switch( tab )
    {
        case 0:
        {
            QModelIndex idx = ui->contactTableView->currentIndex();
            if( !idx.isValid() ) return;
            QList<QVariant> list = ui->contactTableView->contactDetail();
            detailDlg->setDetail(list);
        }
        break;
        case 1:
        {
            QModelIndex idx = ui->favoritTableView->currentIndex();
            if( !idx.isValid() ) return;
            QList<QVariant> list = ui->favoritTableView->contactDetail();
            detailDlg->setDetail(list);
        }
        break;
    }

    detailDlg->exec();
}

void MainWindow::showSettingDlg()
{
    SettingDialog* settingDlg = new SettingDialog(this);

    settingDlg->exec();
}

void MainWindow::addFavourite()
{
    QModelIndex index = ui->contactTableView->currentIndex();
    if( ! index.isValid() )
        return;
    QSqlRecord record = ui->contactTableView->favouriteRecord(index);
    ui->favoritTableView->addFavourite(record);
}

void MainWindow::deleteContact()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        ui->contactTableView->deleteContact();
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        ui->favoritTableView->deleteContact();
    }
}

void MainWindow::tabWidgetChanged(int index)
{
    if( index == 1)
    {
        ui->action_Add->setEnabled(false);
        ui->action_Edit->setEnabled(false);
        ui->action_Favorites->setEnabled(false);
        ui->rbtn_findAll->setChecked(true);
        ui->rbtn_findCurGroup->setEnabled(false);
    }
    else
    {
        ui->action_Add->setEnabled(true);
        ui->action_Edit->setEnabled(true);
        ui->action_Favorites->setEnabled(true);
        ui->rbtn_findAll->setChecked(true);
        ui->rbtn_findCurGroup->setEnabled(true);
    }
}

void MainWindow::setBackground(const QString &filename)
{
    this->setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap(filename);
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    this->setPalette(pal);
}

void MainWindow::updateMenuStatus()
{
    int id0 = Global::g_preferences->m_backgroundId;
    QList<QAction*> acts0 = ui->menu_Background->actions();
    acts0.at(id0)->setChecked(true);

    int id1 = Global::g_preferences->m_backmusicId;
    QList<QAction*> acts1 = ui->menu_Backmusic->actions();
    acts1.at(id1)->setChecked(true);

    Global::g_settingThemes->setCurrentBkground(id0);
    Global::g_settingMusic->setCurrentBkmusic(id1);
}

void MainWindow::initStatusbar()
{
    //    QProgressBar *progressBar = new QProgressBar();
        QLabel *mode = new QLabel( tr(" Ready ") );
        int counts = ui->contactTableView->groupContactCounts(GROUP_DEFAULT);
        m_countLb = new QLabel( tr(" Contact(s): %1 ").arg(counts) );
        m_musciPlayBtn = new QPushButton(tr(""));
        m_statustimeLb = new QLabel( tr("  Time: 0000/00/00 00:00:00") );

        mode->setMinimumSize( mode->sizeHint() );
        mode->setAlignment( Qt::AlignCenter );
        mode->setText( tr("Ready") );
        mode->setToolTip( tr("The current working mode.") );

        m_countLb->setMinimumSize( m_countLb->sizeHint() );
        m_countLb->setAlignment( Qt::AlignCenter );
        m_countLb->setToolTip( tr("Current contacts count(s): %1 ").arg(counts) );

        m_musciPlayBtn->setCheckable(true);
        m_musciPlayBtn->setChecked(true);
        m_musciPlayBtn->setMinimumSize( QSize(20, 20) );
        m_musciPlayBtn->setIcon(QIcon(":/icons/green/sound.png"));
        int id = Global::g_preferences->m_backmusicId;
        QString msic = Global::g_preferences->m_backmusics.at(id);
        QFileInfo fi(msic);
        m_musciPlayBtn->setToolTip( tr("Background music is playing: %1").arg(fi.fileName()));
        connect( m_musciPlayBtn, SIGNAL(toggled(bool)),
                 this, SLOT(updateStatusbarPlay(bool)) );
        connect( m_musciPlayBtn, SIGNAL(toggled(bool)),
                 Global::g_settingMusic, SLOT(musicPlay(bool)) );
        connect( Global::g_settingMusic, SIGNAL(updatePlayBtn(bool)),
                 this, SLOT(updateStatusbarPlay(bool)) );

        QTimer * timer = new QTimer();
        int sec = 1;
        QObject::connect(timer,SIGNAL(timeout()), this, SLOT(updateStatusbarTime()));
        timer->start(sec * 1000);
        m_statustimeLb->setMinimumSize( m_statustimeLb->sizeHint() );
        m_statustimeLb->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
        m_statustimeLb->setToolTip( tr("Current system time.") );

        ui->statusBar->addWidget( mode );
        ui->statusBar->addWidget( m_countLb );
        ui->statusBar->addWidget(m_musciPlayBtn);
        ui->statusBar->addWidget(m_statustimeLb);
        ui->statusBar->addPermanentWidget( m_musciPlayBtn );//现实永久信息
        ui->statusBar->addPermanentWidget( m_statustimeLb );//现实永久信息
}

void MainWindow::updateStatusbarTime()
{
    QDateTime dt;
    QTime curTime;
    QDate curData;
    dt.setDate(curData.currentDate());
    dt.setTime(curTime.currentTime());
    QString currentDate = dt.toString("yyyy/MM/dd HH:mm:ss");

    m_statustimeLb->setText(" Time: "+ currentDate );
}

void MainWindow::updateStatusbarPlay(bool checked)
{
//    QPushButton* btn = qobject_cast<QPushButton*>(sender());
//    if( !btn ) return;
//    btn->setChecked(checked);
    if( checked )
    {
        int id = Global::g_preferences->m_backmusicId;
        QString msic = Global::g_preferences->m_backmusics.at(id);
        QFileInfo fi(msic);
        m_musciPlayBtn->setToolTip( tr("Background music now playing: %1").arg(fi.fileName()));
        m_musciPlayBtn->setIcon( QIcon(":/icons/green/sound.png") );
    }
    else
    {
        m_musciPlayBtn->setToolTip( tr("Background music now puased.") );
        m_musciPlayBtn->setIcon( QIcon(":/icons/green/no_sound.png") );
    }
}

void MainWindow::updateStatusbarCount(int count)
{
    m_countLb->setText( tr("Contact(s): %1 ").arg(count) );
    m_countLb->setToolTip(tr("Current contact count(s): %1 ").arg(count));
}

void MainWindow::updateFontColor(int r, int g, int b)
{
    this->setStyleSheet( QString("QLabel,QPushButton,QRadioButton, QGroupBox {color : rgb(%1,%2,%3);}").arg(r).arg(g).arg(b) );
}

void MainWindow::backgroundChanged(QAction *act)
{
    QList<QAction*> acts = ui->menu_Background->actions();
    foreach (QAction* a, acts)
    {
        a->setChecked(false);
    }
    int id = 0;
    act->setChecked(true);
    if( act->objectName() == "actionBackground_1" )
        id = 0;
    else if( act->objectName() == "actionBackground_2" )
        id = 1;
    else if( act->objectName() == "actionBackground_3" )
        id = 2;
    else if( act->objectName() == "actionBackground_4" )
        id = 3;
    Global::g_settingThemes->setCurrentBkground(id);
}

void MainWindow::backmusicChanged(QAction *act)
{
    QList<QAction*> acts = ui->menu_Backmusic->actions();
    foreach (QAction* a, acts)
    {
        a->setChecked(false);
    }
    int id = 0;
    act->setChecked(true);
    if( act->objectName() == "actionBackmusic_1" )
        id = 0;
    else if( act->objectName() == "actionBackmusic_2" )
        id = 1;
    else if( act->objectName() == "actionBackmusic_3" )
        id = 2;
    else if( act->objectName() == "actionBackmusic_4" )
        id = 3;
    Global::g_settingMusic->setCurrentBkmusic(id);
}

void MainWindow::setPhoto(const QByteArray &data)
{
    QPixmap pixmap;
    if( data.size() == 0 )
    {
        QPixmap pixmap(":/images/photo.png");
        ui->photoLabel->setPixmap(pixmap);
    }
    else
    {
        pixmap.loadFromData(data);
        ui->photoLabel->setPixmap(pixmap);
    }
}

/**
 * @author Macai
 * @slot function: select group item then hide other group contacts.
 * @brief MainWindow::filterGroup
 * @param index
 */
void MainWindow::filterGroup(QListWidgetItem *current, QListWidgetItem *previous)
{
    if( !current || !previous ) return;
    if( current == previous) return;
    QString groupname = current->text();
    int counts  = 0;
    if(ui->tabWidget->currentIndex() == 0)
    {
        ui->contactTableView->groupFilter(groupname);
        counts = ui->contactTableView->groupContactCounts(groupname);
    }
    else
    {
        ui->favoritTableView->groupFilter(groupname);
        counts = ui->favoritTableView->groupContactCounts(groupname);
    }
    this->updateStatusbarCount(counts);
}

void MainWindow::showEditContactDlg()
{
    if( !Global::g_editContactDlg->isHidden() )
        return;
    Global::g_editContactDlg->setShown(true);

    connect(Global::g_editContactDlg, SIGNAL(updateContact(int, QList<QVariant>)),
            ui->contactTableView, SLOT(updateContact(int, QList<QVariant>)) );
    connect(Global::g_editContactDlg, SIGNAL(updateButton(bool)),
            this, SLOT(updateButton(bool)) );

    QModelIndex curIndex = ui->contactTableView->currentIndex();
    if( curIndex.isValid() )
    {
        QStringList groups = ui->grouplistWidget->groups();
        Global::g_editContactDlg->initGroupCombobox(groups);
        Global::g_editContactDlg->readContactInfo(curIndex);
        Global::g_editContactDlg->setCurrentModelIndex(curIndex);
    }
}

void MainWindow::showFindWidget()
{
    if( ui->findWidget->isHidden())
    {
        ui->action_Query->setChecked(true);
        ui->findWidget->setShown(true);
        ui->edt_findKey->setFocus();
    }
    else
    {
        ui->action_Query->setChecked(false);
        ui->findWidget->setHidden(true);
    }
}

void MainWindow::findContact(bool /*checked*/)
{
    QString findwhat = ui->cbox_findWhat->currentText();
    findwhat.replace(QString(" "), QString(""));
    QString keyword = ui->edt_findKey->text();
    QString filter = ui->grouplistWidget->currentItem()->text();

    if( ui->tabWidget->currentIndex() == 1 )
    {
        ui->favoritTableView->findContacts(findwhat, keyword);
        return;
    }

    if( filter == GROUP_ALL )
    {
        ui->contactTableView->findContacts(findwhat, keyword);
        return;
    }
    if ( ui->rbtn_findCurGroup->isChecked() )
    {
        ui->contactTableView->findContacts(filter, findwhat, keyword);
    }
    else if(ui->rbtn_findAll->isChecked())
    {
        ui->grouplistWidget->setCurrentRow(0);
        ui->contactTableView->findContacts(findwhat, keyword);
    }
}

void MainWindow::updateGroupContacts(const QString &group)
{
    ui->contactTableView->updateGroupContacts(group);
}

