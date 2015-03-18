#include "contacts_tableview.h"
#include "database_manager.h"
#include "constances.h"

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QHeaderView>
#include <QFocusEvent>
#include <QMenu>
#include <QAction>
#include <QSqlField>

#include <QDebug>

ContactsTableView::ContactsTableView(QWidget *parent)
    : QTableView(parent)
{
    DatabaseManager *dbMrg = new DatabaseManager;
    dbMrg->createConnect();
    dbMrg->createContactTable();
    dbMrg->setStatus(DatabaseManager::Read);
    QSqlDatabase db = dbMrg->database();

    // initialize contacts view
    m_tableModel = new QSqlTableModel(0, db);
    m_tableModel->moveToThread(&m_thread);
    m_thread.start();

    m_tableModel->setTable(BaseTable);
//    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_tableModel->select();
//    m_tableModel->setHeaderData(ID, Qt::Horizontal, tr("#"));
    m_tableModel->setHeaderData(GROUP, Qt::Horizontal, tr("Groups"));
    m_tableModel->setHeaderData(LASTNAME, Qt::Horizontal, tr("Last Name"));
    m_tableModel->setHeaderData(FirstName, Qt::Horizontal, tr("First Name"));
    m_tableModel->setHeaderData(NICKNAME, Qt::Horizontal, tr("Nick Name"));
    m_tableModel->setHeaderData(SEX, Qt::Horizontal, tr("Sex"));
    m_tableModel->setHeaderData(AGE, Qt::Horizontal, tr("Age"));
    m_tableModel->setHeaderData(BIRTHDAY, Qt::Horizontal, tr("Birth Day"));
    m_tableModel->setHeaderData(MOBILE, Qt::Horizontal, tr("Mobile Phone"));
    m_tableModel->setHeaderData(EMAIL, Qt::Horizontal, tr("Email"));
    m_tableModel->setHeaderData(QQ, Qt::Horizontal, tr("QQ"));
    this->setModel(m_tableModel);

    this->hideColumn(0);
    this->hideColumn(1);
    int cnt = m_tableModel->columnCount();
    if( cnt > 10 )
    {
        for( int i = 11; i < cnt; ++i)
        this->hideColumn(i);
    }
    this->setColumnWidth(0, 20);
    groupFilter(GROUP_DEFAULT);
}

ContactsTableView::~ContactsTableView()
{
    if(m_thread.isRunning())
    {
        m_thread.quit();
        m_thread.wait();
    }
    m_tableModel->clear();
    m_tableModel->deleteLater();
}

void ContactsTableView::groupFilter(const QString &group)
{
    if( group == GROUP_ALL )
    {
        m_tableModel->setFilter( "" );
        m_tableModel->select();
        return;
    }
    QString filter = QString("Groups='%1'").arg( group );
    m_tableModel->setFilter( filter );
    m_tableModel->select();
}

int ContactsTableView::groupContactCounts(const QString &group)
{
    if( group == GROUP_ALL )
    {
        m_tableModel->setFilter( "" );
        m_tableModel->select();
        int cnts = m_tableModel->rowCount();
        return cnts;
    }
    QString filter = QString("Groups='%1'").arg( group );
    m_tableModel->setFilter( filter );
    m_tableModel->select();
    return m_tableModel->rowCount();
}

QSqlRecord ContactsTableView::favouriteRecord(const QModelIndex &index)
{
    int row = index.row();
    return m_tableModel->record(row);
}

QList<QVariant> ContactsTableView::contactDetail()
{
    QList<QVariant> list;
    QModelIndex idx = this->currentIndex();
    if( !idx.isValid() ) return list;
    int r = idx.row();
    QSqlRecord record = m_tableModel->record(r);
    for( int i = 0; i < record.count(); ++i)
    {
        if( i == PHOTO)
        {
            QSqlField field = record.field(i);
            QByteArray data = field.value().toByteArray();
            list.append(data);
        }
        else
        {
            QSqlField field = record.field(i);
            QString data = field.value().toString();
            list.append(data);
        }
    }

    return list;
}

void ContactsTableView::updateContact(int row, const QList<QVariant> &valueList)
{
    int col = 1;
    for( int i = 0; i < valueList.count(); ++i)
    {
        if( i == PHOTO-1)
        {
            QByteArray photo = valueList.at(i).toByteArray();
            m_tableModel->setData(m_tableModel->index(row, col), photo);
        }
        else
        {
            QString data = valueList.at(i).toString();
            m_tableModel->setData(m_tableModel->index(row, col), data);
        }
        col++;
    }
    m_tableModel->submitAll();
}

void ContactsTableView::updateGroupContacts(const QString &group)
{
    QString filter = QString("Groups='%1'").arg( group );
    m_tableModel->setFilter( filter );
    m_tableModel->select();
}

void ContactsTableView::addContact(const QList<QVariant> &valueList)
{
    // column 0 is ID, 1 is Group Name.
    //qDebug() << valueList.count(); // 20
    //int colNum = m_tableModel->columnCount();
    int rowNum = m_tableModel->rowCount();
    m_tableModel->insertRow(rowNum);
    int col = 1;
    for( int i = 0; i < valueList.count(); ++i)
    {
        if( i == PHOTO-1)
        {
            QByteArray photo = valueList.at(i).toByteArray();
            m_tableModel->setData(m_tableModel->index(rowNum, col), photo);
        }
        else
        {
            QString data = valueList.at(i).toString();
            m_tableModel->setData(m_tableModel->index(rowNum, col), data);
        }
        col++;
    }
    m_tableModel->submitAll();
}

void ContactsTableView::deleteContact()
{
    QModelIndexList idxList = this->selectionModel()->selectedIndexes();

    QMap<int, int> rowMap;
    foreach (QModelIndex index, idxList)
        rowMap.insert(index.row(), 0);

    int rowDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    while (rowMapIterator.hasPrevious())
    {
        rowMapIterator.previous();
        rowDel = rowMapIterator.key();
        this->model()->removeRow(rowDel);
    }
    m_tableModel->submitAll();
}

void ContactsTableView::findContacts(const QString &filter, const QString &findWhat, const QString &keyword)
{
    QString str = QString::fromLocal8Bit("Groups='%1' AND %2 like '%%3%'")
                                .arg(filter).arg(findWhat).arg(keyword);
    m_tableModel->setFilter( str );
    m_tableModel->select();
}

void ContactsTableView::findContacts(const QString &findWhat, const QString &keyword)
{
    QString filter = QString::fromLocal8Bit("%1 like '%%2%'").arg(findWhat).arg(keyword);
    m_tableModel->setFilter( filter );
    m_tableModel->select();
}

void ContactsTableView::focusOutEvent(QFocusEvent *event)
{
    switch( event->reason() )
    {
    case Qt::MouseFocusReason:
    case Qt::TabFocusReason:
    case Qt::BacktabFocusReason:
    case Qt::ActiveWindowFocusReason:
    case Qt::ShortcutFocusReason:
        emit updateButton(false);
        break;
    case Qt::MenuBarFocusReason:
    case Qt::PopupFocusReason:
        emit updateButton(true);
        break;
    case Qt::OtherFocusReason:
        emit updateButton(false);
        break;
    }

    QTableView::focusOutEvent(event);
}

void ContactsTableView::mousePressEvent(QMouseEvent *event)
{
    int r = this->rowAt( mapFromGlobal(event->globalPos()).y() - this->horizontalHeader()->height() );
    if( r == -1)
    {
        this->clearFocus();
        this->clearSelection();
        this->selectionModel()->clearSelection();
        emit updateButton(false);

        return;
    }
    this->selectRow(r);
    emit updateButton(true);
    QTableView::mousePressEvent(event);
}

void ContactsTableView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu* popMenu = new QMenu(this);
    QAction *addAction = popMenu->addAction( tr("&Add Contact") );
        addAction->setIcon( QIcon(":/icons/green/add.png") );
    QAction *editAction = popMenu->addAction( tr("&Edit Contact") );
        editAction->setIcon( QIcon(":/icons/green/edit.png") );
    QAction *findAction = popMenu->addAction( tr("&Find Contact") );
        findAction->setIcon( QIcon(":/icons/green/find.png") );
    QAction *deleteAction = popMenu->addAction( tr("&Delete Contact") );
        deleteAction->setIcon( QIcon(":/icons/green/delete.png") );
    popMenu->addSeparator();
    QAction *favoriteAction = popMenu->addAction( tr("&Favorite") );
        favoriteAction->setIcon( QIcon(":/icons/green/favourite.png") );
    QAction *sendEmailAction = popMenu->addAction( tr("&Send Email") );
        sendEmailAction->setIcon( QIcon(":/icons/green/mail.png") );
    QAction *viewDetailAction = popMenu->addAction( tr("&View Detail") );
        viewDetailAction->setIcon( QIcon(":/icons/green/detail.png") );

    QModelIndex idx = this->indexAt( mapFromGlobal( QPoint( event->globalPos().x(), \
            event->globalPos().y()-this->horizontalHeader()->height())) );
    if( idx.isValid() )
    {
        connect( addAction, SIGNAL(triggered()), this, SIGNAL(addContactAct()) );
        connect( editAction, SIGNAL(triggered()), this, SIGNAL(editContactAct()) );
        connect( findAction, SIGNAL(triggered()), this, SIGNAL(findContactAct()) );
        connect( deleteAction, SIGNAL(triggered()), this, SIGNAL(deleteContactAct()) );
        connect( favoriteAction, SIGNAL(triggered()), this, SIGNAL(addFavouriteAct()) );
        connect( sendEmailAction, SIGNAL(triggered()), this, SIGNAL(sendMailAct()) );
        connect( viewDetailAction, SIGNAL(triggered()), this, SIGNAL(viewDetailAct()) );
    }
    else
    {
        editAction->setEnabled(false);
        deleteAction->setEnabled(false);
        favoriteAction->setEnabled(false);
        sendEmailAction->setEnabled(false);
        viewDetailAction->setEnabled(false);
        this->clearSelection();
    }
    popMenu->exec(event->globalPos());
    event->accept();
}


