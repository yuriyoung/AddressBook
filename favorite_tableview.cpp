#include "favorite_tableview.h"
#include "database_manager.h"
#include "constances.h"

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QSqlField>

FavoriteTableView::FavoriteTableView(QWidget *parent) :
    QTableView(parent)
{
    DatabaseManager *dbMrg = new DatabaseManager;
    dbMrg->createConnect();
    dbMrg->createFavouriteTable();
    dbMrg->setStatus(DatabaseManager::Read);
    QSqlDatabase db = dbMrg->database();

    m_tableModel = new QSqlTableModel(0, db);

    m_tableModel->setTable(FavoTable);
    m_tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
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
}

void FavoriteTableView::addFavourite(const QSqlRecord &record)
{
    int rowCnt = m_tableModel->rowCount();
    m_tableModel->insertRecord( rowCnt, record );
    m_tableModel->submitAll();
}

QList<QVariant> FavoriteTableView::contactDetail()
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

void FavoriteTableView::findContacts(const QString &findWhat, const QString &keyword)
{
    QString filter = QString::fromLocal8Bit("%1 like '%%2%'").arg(findWhat).arg(keyword);
    m_tableModel->setFilter( filter );
    m_tableModel->select();
}

void FavoriteTableView::deleteContact()
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

void FavoriteTableView::groupFilter(const QString &group)
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

int FavoriteTableView::groupContactCounts(const QString &group)
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

void FavoriteTableView::focusOutEvent(QFocusEvent *event)
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

void FavoriteTableView::mousePressEvent(QMouseEvent *event)
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

void FavoriteTableView::contextMenuEvent(QContextMenuEvent *event)
{

    QMenu* popMenu = new QMenu(this);
    QAction *findAction = popMenu->addAction( tr("&Find Contact") );
        findAction->setIcon( QIcon(":/icons/green/find.png") );
    QAction *deleteAction = popMenu->addAction( tr("&Delete Contact") );
        deleteAction->setIcon( QIcon(":/icons/green/delete.png") );
    popMenu->addSeparator();
    QAction *sendEmailAction = popMenu->addAction( tr("&Send Email") );
        sendEmailAction->setIcon( QIcon(":/icons/green/mail.png") );
    QAction *viewDetailAction = popMenu->addAction( tr("&View Detail") );
        viewDetailAction->setIcon( QIcon(":/icons/green/detail.png") );

    QModelIndex idx = this->indexAt( mapFromGlobal( QPoint( event->globalPos().x(), event->globalPos().y()-this->horizontalHeader()->height())) );

    if( idx.isValid() )
    {
        connect( findAction, SIGNAL(triggered()), this, SIGNAL(findContactAct()) );
        connect( deleteAction, SIGNAL(triggered()), this, SIGNAL(deleteContactAct()) );
        connect( sendEmailAction, SIGNAL(triggered()), this, SIGNAL(sendMailAct()) );
        connect( viewDetailAction, SIGNAL(triggered()), this, SIGNAL(viewDetailAct()) );
    }
    else
    {
        deleteAction->setEnabled(false);
        sendEmailAction->setEnabled(false);
        viewDetailAction->setEnabled(false);
        this->clearSelection();
    }
    popMenu->exec(event->globalPos());
    event->accept();
}
