/****************************************************************************
**
** file: sqlTableModel.h
**
** description:
**     重载QSqlTableModel，设置位置居中.
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>

class CSqlTableModel : public QSqlTableModel
{
public:
    CSqlTableModel(QObject *parent = 0,QSqlDatabase db = QSqlDatabase());

public:
    QVariant data(const QModelIndex & item, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // SQLTABLEMODEL_H
