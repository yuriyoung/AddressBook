#ifndef FAVORITE_TABLEVIEW_H
#define FAVORITE_TABLEVIEW_H

#include <QTableView>
#include <QEvent>

class QSqlTableModel;
class QSqlRecord;

class FavoriteTableView : public QTableView
{
    Q_OBJECT
public:
    explicit FavoriteTableView(QWidget *parent = 0);
    
    void addFavourite(const QSqlRecord &record);
    QList<QVariant> contactDetail();

signals:
    void updateButton(bool);
    void findContactAct();
    void deleteContactAct();
    void sendMailAct();
    void viewDetailAct();

public slots:
    void findContacts(const QString &findWhat, const QString &keyword);
    void deleteContact();
    void groupFilter(const QString &group);
    int  groupContactCounts(const QString &group);

protected:
    void focusOutEvent( QFocusEvent* );
    void mousePressEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QSqlTableModel*     m_tableModel;
};

#endif // FAVORITE_TABLEVIEW_H
