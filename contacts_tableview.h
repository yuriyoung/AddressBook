#ifndef CONTACTS_TABLEVIEW_H
#define CONTACTS_TABLEVIEW_H

#include <QTableView>
#include <QEvent>
#include <QThread>
#include <QSqlRecord>

class QSqlDatabase;
class QSqlTableModel;

class ContactsTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ContactsTableView(QWidget *parent = 0);
    ~ContactsTableView();

    void groupFilter(const QString &group);
    int groupContactCounts(const QString &group);

    QSqlRecord favouriteRecord(const QModelIndex& index);
    QList<QVariant> contactDetail();

signals:
    void updateButton(bool);
    void finishedEdit();

    void addContactAct();
    void editContactAct();
    void findContactAct();
    void deleteContactAct();
    void addFavouriteAct();
    void sendMailAct();
    void viewDetailAct();

public slots:
    void updateContact(int row, const QList<QVariant> &valueList);
    void updateGroupContacts(const QString &group);
    void addContact(const QList<QVariant> &valueList);
    void deleteContact();
    void findContacts(const QString &filter, const QString &findWhat, const QString &keyword);
    void findContacts(const QString &findWhat, const QString &keyword);

protected:
    void focusOutEvent( QFocusEvent* );
    void mousePressEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QSqlTableModel*     m_tableModel;
    QThread             m_thread;
};

#endif // CONTACTS_TABLEVIEW_H
