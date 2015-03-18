#ifndef CONTACTSQLMODEL_H
#define CONTACTSQLMODEL_H

#include <QSqlQueryModel>

class QSqlDatabase;

class ContactSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ContactSqlModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;

signals:
    
public slots:
    
};

#endif // CONTACTSQLMODEL_H
