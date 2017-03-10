#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QtGui/QToolTip>
#include <QObject>
#include <QString>
#include <QPoint>

/*
 *  @Func:          popSwitchCtrlDlg
 *  @Desc:          弹出确认框进行确认
 *  
 *  @Param[in]:     
 *          title:     标题
 *          msg:       内容
 */
inline void popToolTip(const QPoint& point, const QString& msg)
{
    QToolTip::showText(point,msg);
}

#endif // TOOLTIP_H
