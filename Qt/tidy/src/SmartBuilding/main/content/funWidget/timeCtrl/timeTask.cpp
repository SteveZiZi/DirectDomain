#include "timeTask.h"

CTimeOrder::CTimeOrder()
: m_cHour(0)
, m_cMinute(0)
, m_cOrder(0)
{

}

CTimeOrder::~CTimeOrder()
{

}


CTimeTask::CTimeTask()
: m_cPriority(0)
, m_cCircuitSelection(0)
, m_iOrderCount(0)
{

}

// CTimeTask::CTimeTask(const CTimeTask& timeTask)
// {
//     setEnable(timeTask.enable());
//     setId(timeTask.id());
//     setStartTime(timeTask.startTime());
//     setEndTime(timeTask.endTime());
//     setWeekSelection(timeTask.weekSelection());
//     m_cPriority = timeTask.priority();
//     m_cCircuitSelection = timeTask.circuitSelection();
//     m_iOrderCount = timeTask.orderCount();
//     m_iCircuitCount = timeTask.circuitCount();
//     for (int i = 0;i < 12;i++)
//     {
//         m_sTimeOrder[i].setHour(timeTask.timeOrder()[i].hour());
//         m_sTimeOrder[i].setMinute(timeTask.timeOrder()[i].minute());
//         m_sTimeOrder[i].setOrder(timeTask.timeOrder()[i].order());
//     }
//     m_lCircuitList = timeTask.getCircuitList();
// }

CTimeTask::~CTimeTask()
{

}