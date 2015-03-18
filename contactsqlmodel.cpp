#include "contactsqlmodel.h"

#include <QSqlDatabase>
#include <QVariant>
#include <QColor>

ContactSqlModel::ContactSqlModel()
{
}

Qt::ItemFlags ContactSqlModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 1) //第二个字段可更改
        flags |= Qt::ItemIsEditable;
    return flags;
}

bool ContactSqlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    return false;
}

QVariant ContactSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if (value.isValid() && role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return value.toString().prepend("#");
        else if (index.column() == 2)
            return value.toString().toUpper();
    }
    if (role == Qt::TextColorRole && index.column() == 1)
        return QVariant::fromValue(QColor(Qt::blue));
    return value;
}
