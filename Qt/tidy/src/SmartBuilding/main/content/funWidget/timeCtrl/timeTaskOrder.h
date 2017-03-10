/****************************************************************************
**
** file: timeTaskOrder.h
**
** description:
**     时控指令动作
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef TIMETASKORDER_H
#define TIMETASKORDER_H

#include "src/SmartBuilding/main/content/common_ctl/baseOrderWidget.h"

class QLineEdit;
class CTimeTaskOrder : public CBaseOrderWidget
{
    Q_OBJECT

public:
    CTimeTaskOrder(QWidget *parent = 0);
    ~CTimeTaskOrder();

    S_BYTE hour();
    void setHour(S_BYTE hour_);
    S_BYTE minute();
    void setMinute(S_BYTE minute_);

private:
    virtual void init_();
    void layout_();

private:
    QLineEdit*                  m_pHourEdit;
    QLineEdit*                  m_pMinEdit;
};

#endif // TIMETASKORDER_H
