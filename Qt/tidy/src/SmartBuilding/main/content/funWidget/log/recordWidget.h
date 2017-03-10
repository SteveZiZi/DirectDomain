/****************************************************************************
**
** file: recordWidget.h
**
** description:
**     日志功能界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include <QtGui/QWidget>

class CLogReportW;
class CRecordWidget : public QWidget
{
    Q_OBJECT

public:
    CRecordWidget(QWidget *parent = 0);
    ~CRecordWidget();

private:
    void init_();
    void layout_();
    void localConnect();

private:
    CLogReportW*                m_pLogReportW;
};

#endif // RECORDWIDGET_H
