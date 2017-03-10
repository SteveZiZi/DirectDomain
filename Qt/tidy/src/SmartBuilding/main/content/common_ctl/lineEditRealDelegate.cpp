#include "lineEditRealDelegate.h"

#include <QRegExpValidator>

CLineEditRealDelegate::CLineEditRealDelegate(QWidget *parent)
: NoFocusItemDelegate(parent)
{

}

QWidget * CLineEditRealDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    QRegExp regExp("^[0-9]\\.[0-9]{0,3}$");
    editor->setValidator(new QRegExpValidator(regExp, parent));
    return editor;
}

void CLineEditRealDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double number = index.model()->data(index, Qt::EditRole).toDouble();
    QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
    QString text = QString::number(number,'f',3);
    lineEdit->setText(text);
}

void CLineEditRealDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
    double number = lineEdit->text().toDouble();
    model->setData(index,number,Qt::EditRole);
}

void CLineEditRealDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}