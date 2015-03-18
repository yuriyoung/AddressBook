#include "database_manager.h"
#include "constances.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QByteArray>

#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent),
    m_state(Read)
{
//    if( createConnect() )
//    {
//        createContactTable();
//        createGroupTable();
//        createFavouriteTable();
//    }
}

bool DatabaseManager::createConnect()
{
    if( m_state == Read )
    {
        if(QSqlDatabase::contains("READDATA"))
            m_db = QSqlDatabase::database("READDATA");
        else
        {
            m_db = QSqlDatabase::addDatabase("QSQLITE", "READDATA");
            m_db.setDatabaseName(DbFile);
        }
    }
    else if( m_state == Write )
    {
        if(QSqlDatabase::contains("WRITEDATA"))
            m_db = QSqlDatabase::database("WRITEDATA");
        else
        {
            m_db = QSqlDatabase::addDatabase("QSQLITE", "WRITEDATA");
            m_db.setDatabaseName(DbFile);
        }
    }

    if( !m_db.isOpen() )
    {
        if ( !m_db.open() )
        {
            qDebug()<< QString(" can't open database >>>>>> data.sqlite");
            qDebug() << "error code: " << m_db.lastError();
            return false;
        }
    }
    return true;
}

bool DatabaseManager::createContactTable()
{
    QStringList tableList = m_db.tables();
    QSqlQuery query(m_db);
    if( !tableList.contains(BaseTable) )
    {
        QString createTable = QString("CREATE TABLE %1 (ID integer PRIMARY KEY AUTOINCREMENT,"
                                      "Groups text DEFAULT UnGrouped,LastName text,FirstName text,"
                                      "NickName text,Sex text,Age text,BirthDay text,MobilePhone text,"
                                      "Email text,QQ text,Photo blob,Weibo text,WebSite text,"
                                      "Job text,OfficeTel text, OfficeAddress text,"
                                      "OfficeAddrCode text,HomeTel text,HomeAddress text,HomeAddrCode text,"
                                      "Note text)"
                                      ).arg(BaseTable);
        if(!query.exec(createTable))
        {
            QMessageBox::warning(0, QObject::tr("Create table error"), QObject::tr("Error: ")+ query.lastError().text(), QMessageBox::Ok);
            qDebug()<< "Create table error: " << query.lastError();
            return false;
        }
    }

    return true;
}

bool DatabaseManager::createGroupTable()
{
    QStringList tableList = m_db.tables();
    QSqlQuery query(m_db);
    if( !tableList.contains(GroupTable) )
    {
        QString createTable = QString("CREATE TABLE %1 (ID integer PRIMARY KEY AUTOINCREMENT,"
                                      "ContactGroup text)"
                                      ).arg(GroupTable);
        if(!query.exec(createTable))
        {
            QMessageBox::warning(0, QObject::tr("Create table error"),
                                 QObject::tr("Error: ")+ query.lastError().text(), QMessageBox::Ok);
            qDebug()<< "Create table error: " << query.lastError();
            return false;
        }

        QString str1 = QString("INSERT INTO %1 VALUES(0, '%2')")
                .arg(GroupTable).arg(GROUP_ALL);
        QString str2 = QString("INSERT INTO %1 VALUES(1, '%2')")
                .arg(GroupTable).arg(GROUP_DEFAULT);
        query.exec(str1);
        query.exec(str2);

        return 1;
    }

    return true;
}

bool DatabaseManager::createFavouriteTable()
{
    QStringList tableList = m_db.tables();
    QSqlQuery query(m_db);
    if( !tableList.contains(FavoTable) )
    {
        QString createTable = QString("CREATE TABLE %1 (ID integer PRIMARY KEY AUTOINCREMENT,"
                                      "Groups text DEFAULT UnGrouped,LastName text,FirstName text,"
                                      "NickName text,Sex text,Age text,BirthDay text,MobilePhone text,"
                                      "Email text,QQ text,Photo blob,Weibo text,WebSite text,"
                                      "Job text,OfficeTel text, OfficeAddress text,"
                                      "OfficeAddrCode text,HomeTel text,HomeAddress text,HomeAddrCode text,"
                                      "Note text)"
                                      ).arg(FavoTable);
        if(!query.exec(createTable))
        {
            qDebug()<< "Create table error: " << query.lastError();
            return false;
        }
    }

    return true;
}

QStringList DatabaseManager::groups()
{
    QStringList list;
    list.clear();
    QSqlQuery query(m_db);
    QString select = QString("SELECT ContactGroup FROM %1").arg(GroupTable);
    query.prepare( select );
    query.exec();

    while( query.next() )
    {
        QString group = query.value(0).toString();
        if( ! list.contains(group) )
            list.append( group );
    }

    return list;
}

int DatabaseManager::renameGroup(const QString &oldName, const QString &newName)
{
    qDebug() << oldName << newName;
    QSqlQuery query(m_db);
    QString grp = QString("UPDATE %1 SET ContactGroup=? WHERE ContactGroup=?")
                        .arg(GroupTable);
    query.prepare(grp);
    query.addBindValue(newName, QSql::In);
    query.addBindValue(oldName, QSql::Out);
    if( query.exec())
    {
        QString str = QString("UPDATE %1 SET Groups=? WHERE Groups=?")
                        .arg(BaseTable);
        query.prepare(str);
        query.addBindValue(newName, QSql::In);
        query.addBindValue(oldName, QSql::Out);
        if( ! query.exec())
            return 0;

        return 1;
    }

    return 0;
}

int DatabaseManager::removeGroup(const QString &groupname)
{
    QString str = QString("DELETE FROM %1 WHERE Groups=?").arg(BaseTable);
    QSqlQuery query(m_db);
    query.prepare(str);
    query.addBindValue(groupname, QSql::Out);
    if( query.exec() )
    {
        QString grp = QString("DELETE FROM %1 WHERE ContactGroup=?").arg(GroupTable);
        query.prepare(grp);
        query.addBindValue(groupname, QSql::Out);
        if( ! query.exec())
            return 0;

        return 1;
    }

    return 0;
}

int DatabaseManager::addGroup(const QString &groupname)
{
    qDebug() << groupname;
    QString str = QString("INSERT INTO %1 VALUES(null,'%2')")
            .arg(GroupTable).arg(groupname);
    QSqlQuery query(m_db);
    if( ! query.exec(str) )
    {
        qDebug() << "error code: " << m_db.lastError();
        return 0;
    }
    return 1;
}

