#ifndef POPSUREWIDGET_H
#define POPSUREWIDGET_H

#include <QtGui/QMessageBox>
#include <QObject>
#include <QString>

/*
 *  @Func:          popSwitchCtrlDlg
 *  @Desc:          弹出确认框进行确认
 *  
 *  @Param[in]:     
 *          title:     标题
 *          msg:       内容
 */
inline int popSwitchCtrlDlg(const QString& title, const QString& msg)
{
    QMessageBox messagebox(QMessageBox::Warning,title,msg,QMessageBox::Ok|QMessageBox::Cancel);
    messagebox.setButtonText(QMessageBox::Ok,QObject::tr("是"));
    messagebox.setButtonText(QMessageBox::Cancel,QObject::tr("否"));
    messagebox.setWindowModality(Qt::WindowModal);
    return messagebox.exec();
}

/*
 *  @Func:          popSwitchCtrlDlg
 *  @Desc:          弹出确定对话框
 *  
 *  @Param[in]:     
 *          title:     标题
 *          msg:       内容
 */
inline void popEnsureDlg(const QString& title, const QString& msg)
{
    QMessageBox messagebox(QMessageBox::Warning,QObject::tr("提示"),QObject::tr("密码错误！"));
    messagebox.exec();
}

#endif // POPSUREWIDGET_H
