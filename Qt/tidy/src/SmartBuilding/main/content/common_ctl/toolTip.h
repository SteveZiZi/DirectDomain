#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QtGui/QToolTip>
#include <QObject>
#include <QString>
#include <QPoint>

/*
 *  @Func:          popSwitchCtrlDlg
 *  @Desc:          ����ȷ�Ͽ����ȷ��
 *  
 *  @Param[in]:     
 *          title:     ����
 *          msg:       ����
 */
inline void popToolTip(const QPoint& point, const QString& msg)
{
    QToolTip::showText(point,msg);
}

#endif // TOOLTIP_H
