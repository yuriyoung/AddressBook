#ifndef GROUP_LISTVIEW_H
#define GROUP_LISTVIEW_H

#include <QListWidget>
#include <QThread>

class DatabaseManager;
class QStringListModel;

class GroupListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit GroupListWidget(QWidget *parent = 0);
    ~GroupListWidget();
    QStringList groups();

signals:
    void updateGroupContacts(const QString &group);

public slots:
    void updateGroup(const QString &group);

private slots:
    void iniGroups();
    void addGroupAct();
    void renameGroupAct();
    void delGroupAct();
    void groupTextChanged(QListWidgetItem*);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    DatabaseManager  *m_dbMrg;
    QThread           m_thread;

    QString m_oldname;
};

#endif // GROUP_LISTVIEW_H
