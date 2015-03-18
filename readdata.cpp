#include "readdata.h"
#include "constances.h"

#include <QSqlDatabase>
#include <QSqlError>

#include <QDebug>

ReadData::ReadData(QSqlDatabase &db):
    m_db(db)
{
    createConnect();
}


bool ReadData::createConnect()
{
    if(QSqlDatabase::contains("READDATA"))
        m_db = QSqlDatabase::database("READDATA");
    else
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "READDATA");
        m_db.setDatabaseName("F:/MacaiSVN/trunk/QT/MacaiCore/AddressBook/data/data.sqlite");
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

bool ReadData::createTable()
{
//    QStringList tableList = m_db.tables();
//    QSqlQuery query(m_db);
//    if( !tableList.contains( SQL_TABLE_FILES ) )
//    {
//        QString createTable = QString("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT,"
//                                      "%2 VARCHAR(128), %3 VARCHAR(128) UNIQUE, %4 BLOB,"
//                                      "%5 VARCHAR(64), %6 VARCHAR(24), %7 VARCHAR(256) )"
//                                      )\
//                .arg(SQL_TABLE_FILES)   // 表名
//                .arg(SQL_FIELD_CATEGORY)// 类别名
//                .arg(SQL_FIELD_NAME)    // 文件名
//                .arg(SQL_FIELD_CONTENT) // 文件内容
//                .arg(SQL_FIELD_SIZE)    // 文件大小
//                .arg(SQL_FIELD_TYPE)    // 文件类型
//                .arg(SQL_FIELD_USAGE);  // 文件用法
//        if(!query.exec(createTable))
//        {
//            QMessageBox::warning(0, QObject::tr("Create table error"), QObject::tr("Error: ")+ query.lastError().text(), QMessageBox::Ok);
//            qDebug()<< "Create table error: " << query.lastError();
//            return false;
//        }
//    }

    return true;
}
