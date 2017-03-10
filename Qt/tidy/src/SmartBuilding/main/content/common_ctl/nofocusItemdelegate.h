/****************************************************************************
**
** file: noficusItemdelegate.h
**
** description:
**     ȥ�����ؼ���ѡ��ʱ������
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef NOFOCUSITEMDELEGATE_H
#define NOFOCUSITEMDELEGATE_H

#include <QItemDelegate>

class NoFocusItemDelegate : public QItemDelegate
{
public:
    NoFocusItemDelegate(QObject * parent = 0);

protected:
    virtual void drawFocus(QPainter *painter, const QStyleOptionViewItem &option,
                           const QRect &rect) const;

    virtual void drawCheck(QPainter *painter, const QStyleOptionViewItem &option,
                           const QRect &rect, Qt::CheckState state) const;
};

#endif // NOFOCUSITEMDELEGATE_H
