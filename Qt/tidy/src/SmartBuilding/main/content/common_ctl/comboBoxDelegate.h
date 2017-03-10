/****************************************************************************
**
** file: comboBoxDelegate.h
**
** description:
**     下拉框输入委托
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include "src/SmartBuilding/main/content/common_ctl/nofocusItemdelegate.h"

class CComboBoxDelegate : public NoFocusItemDelegate
{
public:
    CComboBoxDelegate(QWidget *parent = 0);

public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *, const QStyleOptionViewItem &option, const QModelIndex &) const;
};

#endif // COMBOBOXDELEGATE_H
