/****************************************************************************
**
** file: logWidget.h
**
** description:
**     日志选择界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QtGui/QTabWidget>

class CRecordWidget;
class CBlackBoxW;
class CLogWidget : public QTabWidget
{
    Q_OBJECT

public:
    CLogWidget(QWidget *parent = 0);
    ~CLogWidget();

private:
    void init_();
    void localConnect();

private:
    CRecordWidget*              m_pRecordWidget;
    CBlackBoxW*                 m_pBlackBoxW;
};

#endif // LOGWIDGET_H
