/****************************************************************************
**
** file: execTimeWidget.h
**
** description:
**     Ö´ÐÐÊ±¶Î
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef EXECTIMEWIDGET_H
#define EXECTIMEWIDGET_H

#include <QtGui/QWidget>
#include "src/SmartBuilding/inc/xtDef.h"

class QLineEdit;
class CExecTimeWidget : public QWidget
{
    Q_OBJECT

public:
    CExecTimeWidget(QWidget *parent = 0);
    ~CExecTimeWidget();

    S_BYTE startHour();
    void setStartHour(S_BYTE startHour_);
    S_BYTE startMin();
    void setStartMin(S_BYTE startMin_);
    S_BYTE endHour();
    void setEndHour(S_BYTE endHour_);
    S_BYTE endMin();
    void setEndMin(S_BYTE endMin_);

private:
    void init_();
    void layout_();

private:
    QLineEdit*                  m_pStartHourEdit;
    QLineEdit*                  m_pStartMinEdit;
    QLineEdit*                  m_pEndHourEdit;
    QLineEdit*                  m_pEndMinEdit;
};

#endif // EXECTIMEWIDGET_H
