/****************************************************************************
**
** file: timeTask.h
**
** description:
**     时控任务基本属性
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef TIMETASK_H
#define TIMETASK_H

#include <QList>
#include "src/SmartBuilding/main/content/common_ctl/baseTask.h"

class CTimeOrder
{
public:
    CTimeOrder();
    ~CTimeOrder();

    INT8U hour() {return m_cHour;}
    void setHour(INT8U hour_) {m_cHour = hour_;}
    INT8U minute() {return m_cMinute;}
    void setMinute(INT8U minute_) {m_cMinute = minute_;}
    INT8U order() {return m_cOrder;}
    void setOrder(INT8U order_) {m_cOrder = order_;}

private:
    INT8U                   m_cHour;
    INT8U                   m_cMinute;
    INT8U                   m_cOrder;
};

class CTimeTask : public CBaseTask
{
public:
    CTimeTask();
    //CTimeTask(const CTimeTask& timeTask);
    ~CTimeTask();
    INT8U priority() {return m_cPriority;}
    void setPriority(INT8U priority_) {m_cPriority = priority_;}
    S_WORD circuitSelection() {return m_cCircuitSelection;}
    void setCircuitSelection(S_WORD circuitSelection_) {m_cCircuitSelection = circuitSelection_;}
    INT8U circuitCount() {return m_iCircuitCount;}
    void setCircuitCount(INT8U circuitCount_) {m_iCircuitCount = circuitCount_;}
    S_DWORD orderCount() {return m_iOrderCount;}
    void setOrderCount(S_DWORD orderCount_) {m_iOrderCount = orderCount_;}
    CTimeOrder* timeOrder() {return m_sTimeOrder;}
    QList<int>& getCircuitList() {return m_lCircuitList;}

private:
    INT8U                   m_cPriority;
    S_WORD                  m_cCircuitSelection;
    S_DWORD                 m_iOrderCount;
    INT8U                   m_iCircuitCount;
    CTimeOrder              m_sTimeOrder[12];
    QList<int>              m_lCircuitList;
};

#endif // TIMETASK_H
