/****************************************************************************
**
** file: lineEditDelegate.h
**
** description:
**     ÊäÈë¿òÎ¯ÍÐ
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QLineEdit>
#include "src/SmartBuilding/main/content/common_ctl/nofocusItemdelegate.h"

class CLineEditDelegate : public NoFocusItemDelegate
{
public:
    CLineEditDelegate(QWidget *parent = 0);

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *, const QStyleOptionViewItem &option, const QModelIndex &) const;
};

#endif // LINEEDITDELEGATE_H
