/****************************************************************************
**
** file: constanceAsTask.h
**
** description:
**     恒照功能任务属性
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef CONSTANCEASTASK_H
#define CONSTANCEASTASK_H

#include "src/SmartBuilding/main/content/common_ctl/baseTask.h"

class CConstanceAsW;
class CConstanceAsTask : public CBaseTask
{
public:
    CConstanceAsTask();
    ~CConstanceAsTask();

    INT8U priority() {return m_cPriority;}
    void setPrority(INT8U priority_) {m_cPriority = priority_;}
    INT8U circuitSelection() {return m_cCircuitSelection;}
    void setCircuitSelection(INT8U circuitSelection_) {m_cCircuitSelection = circuitSelection_;}
    S_DWORD constanceLux() {return m_iConstanceLux;}
    void setConstanceLux(S_DWORD constanceLux_) {m_iConstanceLux = constanceLux_;}
    S_DWORD sensorSelection() {return m_iSensorSelection;}
    void setSensorSelection(S_DWORD sensorSelection_) {m_iSensorSelection = sensorSelection_;}
    INT8U beginHour() {return m_cBeginHour;}
    void setBeginHour(INT8U beginHour_) {m_cBeginHour = beginHour_;}
    INT8U beginMins() {return m_cBeginMins;}
    void setBeginMins(INT8U beginMins_) {m_cBeginMins = beginMins_;}
    INT8U endHour() {return m_cEndHour;}
    void setEndHour(INT8U endHour_) {m_cEndHour = endHour_;}
    INT8U endMins() {return m_cEndMins;}
    void setEndMins(INT8U endMins_) {m_cEndMins = endMins_;}
    bool getWeight(INT8U* weight_,INT8U size);
    bool setWeight(INT8U* weight_,INT8U size);

private:
    INT8U                   m_cPriority;
    INT8U                   m_cCircuitSelection;
    S_DWORD                 m_iSensorSelection;                         // 方案相关传感器有效位，bit1对应sensor1~bit31对应sensor31，目前只用了bit1~bit16对应1~16号sensor
    S_DWORD                 m_iConstanceLux;                            // 恒照度
    INT8U                   m_cBeginHour;                               // 执行时段
    INT8U                   m_cBeginMins;                               // 执行时段
    INT8U                   m_cEndHour;                                 // 执行时段
    INT8U                   m_cEndMins;                                 // 执行时段
    INT8U                   m_cWeight[16];                              // 权重

private:
    friend class CConstanceAsW;
};

#endif // CONSTANCEASTASK_H
