#ifndef CONNECTION_H
#define CONNECTION_H

#endif // CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static bool createConnect()
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

static bool createTable()
{
    QStringList tableList = m_db.tables();
    QSqlQuery query(m_db);
    if( !tableList.contains(BaseTable) )
    {
        QString createTable = QString("CREATE TABLE %1 (ID integer PRIMARY KEY AUTOINCREMENT,"
                                      "Groups text DEFAULT UnGrouped,LastName text,FirstName text,"
                                      "NickName text,Sex text,BirthDay text,MobilePhone text,"
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

