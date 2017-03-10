/****************************************************************************
**
** file: lightTaskOrder.h
**
** description:
**     光控控制任务指令
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef LIGHTTASKORDER_H
#define LIGHTTASKORDER_H

#include "src/SmartBuilding/main/content/common_ctl/baseOrderWidget.h"

class QLineEdit;
class CLightTaskOrder : public CBaseOrderWidget
{
    Q_OBJECT

public:
    CLightTaskOrder(QWidget *parent = 0);
    ~CLightTaskOrder();

    S_DWORD durationTime();
    void setDurationTime(S_DWORD durationTime_);

private:
    virtual void init_();
    void layout_();

private:
    QLineEdit*                  m_pDurationEdit;
};

#endif // LIGHTTASKORDER_H
