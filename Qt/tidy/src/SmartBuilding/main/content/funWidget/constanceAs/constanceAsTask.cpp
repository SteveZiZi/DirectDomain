#include "constanceAsTask.h"

CConstanceAsTask::CConstanceAsTask()
: m_cPriority(0)
, m_cCircuitSelection(0)
, m_iSensorSelection(0)
, m_iConstanceLux(0)
, m_cBeginHour(0)
, m_cBeginMins(0)
, m_cEndHour(0)
, m_cEndMins(0)
{
    memset(m_cWeight,0,16);
}

CConstanceAsTask::~CConstanceAsTask()
{

}

bool CConstanceAsTask::getWeight(INT8U* weight_,INT8U size)
{
    if (size != 16)
        return false;

    memcpy(weight_,m_cWeight,16);
    return true;
}

bool CConstanceAsTask::setWeight(INT8U* weight_,INT8U size)
{
    if (size != 16)
        return false;

    memcpy(m_cWeight,weight_,16);
    return true;
}