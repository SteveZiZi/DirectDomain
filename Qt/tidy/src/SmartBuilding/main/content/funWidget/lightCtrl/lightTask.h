/****************************************************************************
**
** file: lightTask.h
**
** description:
**     光控功能任务属性
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef LIGHTTASK_H
#define LIGHTTASK_H

#include "src/SmartBuilding/main/content/common_ctl/baseTask.h"

class CLightOrder
{
public:
    CLightOrder();
    ~CLightOrder();

    S_DWORD minutes() {return m_iMinutes;}
    void setMinutes(S_DWORD minutes_) {m_iMinutes = minutes_;}
    INT8U order() {return m_cOrder;}
    void setOrder(INT8U order_) {m_cOrder = order_;}
private:
    S_DWORD             m_iMinutes;
    INT8U               m_cOrder;
};

class CLightTask : public CBaseTask
{
public:
    CLightTask();
    ~CLightTask();

    INT8U priority() {return m_cPriority;}
    void setPrority(INT8U priority_) {m_cPriority = priority_;}
    INT8U circuitSelection() {return m_cCircuitSelection;}
    void setCircuitSelection(INT8U circuitSelection_) {m_cCircuitSelection = circuitSelection_;}
    S_DWORD sensorSelection() {return m_iSensorSelection;}
    void setSensorSelection(S_DWORD sensorSelection_) {m_iSensorSelection = sensorSelection_;}
    S_DWORD startLux() {return m_iStartLux;}
    void setStartLux(S_DWORD startLux_) {m_iStartLux = startLux_;}
    S_DWORD endLux() {return m_iEndLux;}
    void setEndLux(S_DWORD endLux_) {m_iEndLux = endLux_;}
    S_DWORD orderCount() {return m_iOrderCount;}
    void setOrderCount(S_DWORD orderCount_) {m_iOrderCount = orderCount_;}
    CLightOrder* lightOrder() {return m_sLightOrder;}

private:
    INT8U                   m_cPriority;
    INT8U                   m_cCircuitSelection;
    S_DWORD                 m_iSensorSelection;
    S_DWORD                 m_iStartLux;
    S_DWORD                 m_iEndLux;
    S_DWORD                 m_iOrderCount;
    CLightOrder             m_sLightOrder[12];
};

#endif // LIGHTTASK_H
