#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    enum Status
    {
        Read,
        Write
    };

    explicit DatabaseManager(QObject *parent = 0);
    inline QSqlDatabase database() const { return m_db; }
    QStringList groups();
    Status status() const { return m_state; }
    void setStatus(DatabaseManager::Status state = Read) { m_state = state; }
    int renameGroup(const QString &oldName, const QString &newName);
    int removeGroup(const QString &groupname);
    int addGroup(const QString &groupname);

    bool createConnect();
    bool createContactTable();
    bool createGroupTable();
    bool createFavouriteTable();

signals:
    
public slots:

private:
    QSqlDatabase            m_db;
    DatabaseManager::Status m_state;
};

#endif // DATABASE_MANAGER_H
