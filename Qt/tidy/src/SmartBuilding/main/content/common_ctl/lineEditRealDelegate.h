/****************************************************************************
**
** file: lineEditRealDelegate.h
**
** description:
**     lineEdit输入委托(输入值为实数)
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef LINEEDITREALDELEGATE_H
#define LINEEDITREALDELEGATE_H

#include <QLineEdit>
#include "src/SmartBuilding/main/content/common_ctl/nofocusItemdelegate.h"

class CLineEditRealDelegate : public NoFocusItemDelegate
{
public:
    CLineEditRealDelegate(QWidget *parent = 0);

protected:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *, const QStyleOptionViewItem &option, const QModelIndex &) const;
};

#endif // LINEEDITREALDELEGATE_H
