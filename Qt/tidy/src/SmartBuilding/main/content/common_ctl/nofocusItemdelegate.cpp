#include "nofocusItemdelegate.h"

NoFocusItemDelegate::NoFocusItemDelegate(QObject * parent)
    :QItemDelegate(parent)
{

}

void NoFocusItemDelegate::drawFocus(QPainter *,
                                    const QStyleOptionViewItem &,
                                    const QRect &) const
{

}

void NoFocusItemDelegate::drawCheck(QPainter *,
                                    const QStyleOptionViewItem &,
                                    const QRect &,
                                    Qt::CheckState) const
{

}

