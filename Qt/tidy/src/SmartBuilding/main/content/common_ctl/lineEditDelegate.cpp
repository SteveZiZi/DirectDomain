#include "lineEditDelegate.h"


CLineEditDelegate::CLineEditDelegate(QWidget *parent)
: NoFocusItemDelegate(parent)
{

}

QWidget* CLineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    QRegExp regExp("[0-9]{0,4}");
    editor->setValidator(new QRegExpValidator(regExp, parent));
    return editor;
}

void CLineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int number = index.model()->data(index, Qt::EditRole).toInt();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString text = QString::number(number);
    lineEdit->setText(text);
}

void CLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    int number = lineEdit->text().toInt();
    model->setData(index,number,Qt::EditRole);
}

void CLineEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
