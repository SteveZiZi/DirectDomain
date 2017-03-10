/****************************************************************************
**
** file: timeCtrlWidget.h
**
** description:
**     时控列表控件
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef TIMECTRLWIDGET_H
#define TIMECTRLWIDGET_H

#include <QtGui/QWidget>

class CMissionWidget;
class CTimeCtrlW : public QWidget
{
    Q_OBJECT

public:
    CTimeCtrlW(QWidget *parent = 0);
    ~CTimeCtrlW();

private:
    void init_();
    void localConnect();

private:
    CMissionWidget*                 m_pMissionWidget;
};

#endif // TIMECTRLWIDGET_H
