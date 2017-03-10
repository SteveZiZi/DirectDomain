/****************************************************************************
**
** file: contentMgr.h
**
** description:
**     主要显示窗口管理
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef CONTENTMGR_H
#define CONTENTMGR_H

#include <QtGui/QWidget>

class CMenuBar;
class QStackedWidget;
class CMonitorW;
class CTimeCtrlW;
class CLightCtrlW;
class CConstanceAsW;
class CLogWidget;
class CSystemW;
class CConfigeW;
class CContentMgr : public QWidget
{
    Q_OBJECT

public:
    CContentMgr(QWidget *parent = 0);
    ~CContentMgr();

private slots:
    void updateFunWidget(int index);

private:
    void init_();
    void layout_();
    void localConnect();

private:
    CMenuBar*               m_pMenuBar;
    QStackedWidget*         m_pFunWidget;
    CMonitorW*              m_pMonitorW;
    CTimeCtrlW*             m_pTimeCtrlW;
    CLightCtrlW*            m_pLightCtrlW;
    CConstanceAsW*          m_pConstanceAsW;
    CLogWidget*             m_pLogWidget;
    CSystemW*               m_pSystemW;
    CConfigeW*              m_pConfigW;
};

#endif // CONTENTMGR_H
