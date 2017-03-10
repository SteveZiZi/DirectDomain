#include "comboBoxDelegate.h"

#include <QComboBox>

CComboBoxDelegate::CComboBoxDelegate(QWidget *parent)
: NoFocusItemDelegate(parent)
{

}

QWidget* CComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->addItem(tr("¹Ø±Õ"));
    editor->addItem(tr("´ò¿ª"));
    return editor;
}

void CComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    int tindex = comboBox->findText(text);
    comboBox->setCurrentIndex(tindex);
}

void CComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = dynamic_cast<QComboBox *>(editor);
    QString text = comboBox->currentText();
    model->setData(index,text,Qt::EditRole);
}

void CComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}