#include "lightTask.h"

CLightOrder::CLightOrder()
: m_iMinutes(0)
, m_cOrder(0)
{

}

CLightOrder::~CLightOrder()
{

}

CLightTask::CLightTask()
: m_cPriority(0)
, m_cCircuitSelection(0)
, m_iSensorSelection(0)
, m_iStartLux(0)
, m_iEndLux(0)
, m_iOrderCount(0)
{

}

CLightTask::~CLightTask()
{

}
