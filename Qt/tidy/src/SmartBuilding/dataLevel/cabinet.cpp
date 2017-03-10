#include "cabinet.h"

#include "src/SmartBuilding/dataLevel/circuit.h"

CCabinet::CCabinet()
{

}

CCabinet::~CCabinet()
{
    Q_FOREACH(CCircuit* circuit,m_lCircuitList){
        if (circuit)
            delete circuit;
    }
}

bool CCabinet::appendCircuit(CCircuit* circuit)
{
    if (!circuit)
        return false;

    m_lCircuitList.push_back(circuit);
    return true;
}

void CCabinet::clearCircuit()
{
    for (int index = 0;index < m_lCircuitList.size();index++){
        delete m_lCircuitList.takeAt(index);
    }
}

CCircuit* CCabinet::itemOfCircuit(int cicruitNo)
{
    for (int circuitIndex = 0;circuitIndex < m_lCircuitList.size();circuitIndex++)
    {
        if (cicruitNo == m_lCircuitList[circuitIndex]->circuitNo())
            return m_lCircuitList[circuitIndex];
    }

    return NULL;
}

CCircuit* CCabinet::findCircuit(int index)
{
    if (index >= 0 && index < m_lCircuitList.size())
        return m_lCircuitList[index];
    else
        return NULL;
}

void CCabinet::updateParam()
{
    for (int index = 0;index < m_lCircuitList.size();index++)
    {
        if (m_dCurrent <= m_lCircuitList[index]->current())
            m_dCurrent = m_lCircuitList[index]->current();
        if (m_dVolgeta <= m_lCircuitList[index]->volgeta())
            m_dVolgeta = m_lCircuitList[index]->volgeta();
        if (m_dTemp <= m_lCircuitList[index]->temp())
            m_dTemp = m_lCircuitList[index]->temp();
        if (m_lCircuitList[index]->state())
            m_sState = 1;
    }
}
