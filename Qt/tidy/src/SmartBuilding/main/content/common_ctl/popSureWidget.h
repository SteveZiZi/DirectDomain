#ifndef POPSUREWIDGET_H
#define POPSUREWIDGET_H

#include <QtGui/QMessageBox>
#include <QObject>
#include <QString>

/*
 *  @Func:          popSwitchCtrlDlg
 *  @Desc:          ����ȷ�Ͽ����ȷ��
 *  
 *  @Param[in]:     
 *          title:     ����
 *          msg:       ����
 */
inline int popSwitchCtrlDlg(const QString& title, const QString& msg)
{
    QMessageBox messagebox(QMessageBox::Warning,title,msg,QMessageBox::Ok|QMessageBox::Cancel);
    messagebox.setButtonText(QMessageBox::Ok,QObject::tr("��"));
    messagebox.setButtonText(QMessageBox::Cancel,QObject::tr("��"));
    messagebox.setWindowModality(Qt::WindowModal);
    return messagebox.exec();
}

/*
 *  @Func:          popSwitchCtrlDlg
 *  @Desc:          ����ȷ���Ի���
 *  
 *  @Param[in]:     
 *          title:     ����
 *          msg:       ����
 */
inline void popEnsureDlg(const QString& title, const QString& msg)
{
    QMessageBox messagebox(QMessageBox::Warning,QObject::tr("��ʾ"),QObject::tr("�������"));
    messagebox.exec();
}

#endif // POPSUREWIDGET_H
