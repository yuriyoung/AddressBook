#include "group_listwidget.h"
#include "database_manager.h"
#include "main_window.h"
#include "global.h"

#include <QStringList>
#include <QStringListModel>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QContextMenuEvent>
#include <QStandardItem>
#include <QMessageBox>

#include <QDebug>

GroupListWidget::GroupListWidget(QWidget *parent) :
    QListWidget(parent)
{
    m_dbMrg = new DatabaseManager;
    m_dbMrg->createConnect();
    m_dbMrg->createGroupTable();
    m_dbMrg->moveToThread(&m_thread);
    m_thread.start();
    iniGroups();

    connect(this, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(groupTextChanged(QListWidgetItem*)));
}

GroupListWidget::~GroupListWidget()
{
    if(m_thread.isRunning())
    {
        m_thread.quit();
        m_thread.wait();
    }
}

QStringList GroupListWidget::groups()
{
    QStringList list;
    int cnt = this->count();
    for( int i = 0; i < cnt; ++i)
    {
        QString text = this->item(i)->text();
        list.append(text);
    }
    return list;
}

void GroupListWidget::updateGroup(const QString &group)
{
    QList<QListWidgetItem*> list = this->findItems(group, Qt::MatchExactly);
    if( list.size() == 0)
    {
        QListWidgetItem* item = new QListWidgetItem(group);
        this->addItem(item);
        this->setCurrentItem(item);
        m_dbMrg->addGroup(group);
    }
    else
        this->setCurrentItem(list.at(0));

    emit updateGroupContacts(group);
}

void GroupListWidget::iniGroups()
{
    QStringList groups = m_dbMrg->groups();
    this->addItems(groups);
    this->setCurrentRow(1);
}

void GroupListWidget::addGroupAct()
{
    QString newGroupname = tr("Rename Group");
    QList<QListWidgetItem*> list = this->findItems( newGroupname, Qt::MatchStartsWith );
    QListWidgetItem* item = new QListWidgetItem;
    item->setText(newGroupname);
    if( list.count() != 0)
    {
        newGroupname = tr("Rename Group (%1)").arg(list.count());
        item->setText( newGroupname );
    }
    m_dbMrg->addGroup(newGroupname);
    this->addItem( item );
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    this->setCurrentItem( item );
    this->editItem(item);
}

void GroupListWidget::renameGroupAct()
{
    QListWidgetItem* item = this->currentItem();
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    m_oldname = item->text();
    this->editItem( item );
}

void GroupListWidget::delGroupAct()
{
    QListWidgetItem* item = this->currentItem();
    if( ! item )
        return;
    QMessageBox msgbox;
    msgbox.setWindowTitle( WindowTitle );
    msgbox.setText( tr("Are you suer delete the [%1] Group?").arg( item->text() ));
    msgbox.setInformativeText( tr("Click 'Yes' will delete all contacts from [%1] Group.").arg(item->text()));
    msgbox.setStandardButtons( QMessageBox::Yes | QMessageBox::No);
    msgbox.setDefaultButton( QMessageBox::No );
    int ret = msgbox.exec();
    switch( ret )
    {
    case QMessageBox::Yes:
        // remove all contacts from database
        m_dbMrg->removeGroup( item->text() );
        delete item;
        break;
    case QMessageBox::No:
        break;
    default:
        // should never be reached.
        break;
    }
}

void GroupListWidget::groupTextChanged(QListWidgetItem *item)
{
    if( m_oldname.isEmpty() )
        return;
    QString newname = item->text();
    m_dbMrg->renameGroup(m_oldname, newname);

    emit updateGroupContacts(newname);
}

void GroupListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidgetItem* item = this->itemAt( mapFromGlobal(event->globalPos()) );
    if( !item )
    {
        this->clearSelection();
    }

    QMenu *popMenu = new QMenu(this);
    QAction *addGroupAct = popMenu->addAction( tr("&Add Group") );
    popMenu->addSeparator();
    QAction *renameGroupAct = popMenu->addAction( tr("&Rename Group") );
    popMenu->addSeparator();
    QAction *delGroupAct = popMenu->addAction( tr("&Delete Group") );
    connect( addGroupAct, SIGNAL(triggered()), this, SLOT(addGroupAct()) );

    if( item )
    {
        if( item->text() == GROUP_DEFAULT ||
                item->text() == GROUP_ALL)
        {
            renameGroupAct->setEnabled(false);
            delGroupAct->setEnabled(false);
        }
        connect( renameGroupAct, SIGNAL(triggered()), this, SLOT(renameGroupAct()) );
        connect( delGroupAct, SIGNAL(triggered()), this, SLOT(delGroupAct()) );
    }
    else
    {
        renameGroupAct->setEnabled(false);
        delGroupAct->setEnabled(false);
    }

    popMenu->exec(event->globalPos());
    event->accept();
}
