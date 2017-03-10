/****************************************************************************
**
** file: lightCtrlWidget.h
**
** description:
**     光控控制功能
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef LIGHTCTRLWIDGET_H
#define LIGHTCTRLWIDGET_H

#include <QtGui/QWidget>

class CMissionWidget;
class CLightCtrlW : public QWidget
{
    Q_OBJECT

public:
    CLightCtrlW(QWidget *parent = 0);
    ~CLightCtrlW();

private slots:

private:
    void init_();
    void localConnect();

private:
    CMissionWidget*                 m_pMissionWidget;
};

#endif // LIGHTCTRLWIDGET_H
