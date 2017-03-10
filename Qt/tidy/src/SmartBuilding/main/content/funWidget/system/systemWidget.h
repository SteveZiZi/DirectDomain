/****************************************************************************
**
** file: systemWidget.h
**
** description:
**     设置切换功能界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef SYSTEMWIDGET_H
#define SYSTEMWIDGET_H

#include <QtGui/QTabWidget>

class CBaseSetW;
class CDefendWidget;
class CPwdWidget;
class CSystemW : public QTabWidget
{
    Q_OBJECT

public:
    CSystemW(QWidget *parent = 0);
    ~CSystemW();

private:
    void init_();

private:
    CBaseSetW*              m_pBaseSetW;
    CDefendWidget*          m_PDefendWidget;
    CPwdWidget*             m_pPwdWidget;
};

#endif // SYSTEMWIDGET_H
