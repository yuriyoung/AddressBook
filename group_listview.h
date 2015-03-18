#ifndef GROUP_LISTVIEW_H
#define GROUP_LISTVIEW_H

#include <QListView>
#include <QThread>

class DatabaseManager;

class GroupListView : public QListView
{
    Q_OBJECT
public:
    explicit GroupListView(QWidget *parent = 0);
    ~GroupListView();

signals:
    
public slots:
    void updateGroups();

private:
    DatabaseManager *m_dbMrg;
    QThread          m_thread;
};

#endif // GROUP_LISTVIEW_H
