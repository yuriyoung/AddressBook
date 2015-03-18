#ifndef READDATA_H
#define READDATA_H

#include <QSqlDatabase>

class QSqlDatabase;

class ReadData
{
public:
    ReadData(QSqlDatabase &db);
private:
    bool createConnect();
    bool createTable();

    QSqlDatabase        m_db;
};

#endif // READDATA_H
