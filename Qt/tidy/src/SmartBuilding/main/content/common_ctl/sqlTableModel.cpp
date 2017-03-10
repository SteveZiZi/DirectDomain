#include "sqlTableModel.h"

CSqlTableModel::CSqlTableModel(QObject *parent,QSqlDatabase db)
: QSqlTableModel(parent,db)
{

}

QVariant CSqlTableModel::data(const QModelIndex & item, int role) const
{
    QVariant value = QSqlTableModel::data(item, role);
    if (role == Qt::TextAlignmentRole)
    {
        value = (Qt::AlignCenter);
        return value;
    }
    return value;
}

Qt::ItemFlags CSqlTableModel::flags(const QModelIndex &index) const
{
    if (index.column() < 1)
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

