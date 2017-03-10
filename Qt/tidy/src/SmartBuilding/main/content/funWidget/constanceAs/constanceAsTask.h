/****************************************************************************
**
** file: constanceAsTask.h
**
** description:
**     ���չ�����������
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
    S_DWORD                 m_iSensorSelection;                         // ������ش�������Чλ��bit1��Ӧsensor1~bit31��Ӧsensor31��Ŀǰֻ����bit1~bit16��Ӧ1~16��sensor
    S_DWORD                 m_iConstanceLux;                            // ���ն�
    INT8U                   m_cBeginHour;                               // ִ��ʱ��
    INT8U                   m_cBeginMins;                               // ִ��ʱ��
    INT8U                   m_cEndHour;                                 // ִ��ʱ��
    INT8U                   m_cEndMins;                                 // ִ��ʱ��
    INT8U                   m_cWeight[16];                              // Ȩ��

private:
    friend class CConstanceAsW;
};

#endif // CONSTANCEASTASK_H
