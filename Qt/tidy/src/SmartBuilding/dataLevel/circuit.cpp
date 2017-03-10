#include "circuit.h"

#include "src/SmartBuilding/dataLevel/rectifier.h"

CCircuit::CCircuit()
{

}

CCircuit::~CCircuit()
{
    Q_FOREACH(CRectifier* rectifier,m_lRectifierList){
        if (rectifier)
            delete rectifier;
    }
}

bool CCircuit::appendRectifier(CRectifier* rectifier)
{
    if (!rectifier)
        return false;

    m_lRectifierList.push_back(rectifier);
    rectifier->setCircuit(this);
    return true;
}

void CCircuit::clearRectifier()
{
    for (int index = m_lRectifierList.size() - 1;index >= 0;index--){
        delete m_lRectifierList.takeAt(index);
    }
}

CRectifier* CCircuit::findRectifier(int index)
{
    Q_ASSERT(index >= 0 && index < m_lRectifierList.size());

    return m_lRectifierList[index];
}

void CCircuit::updateParam()
{
    for (int index = 0;index < m_lRectifierList.size();index++)
    {
        if (m_dCurrent <= m_lRectifierList[index]->current())
            m_dCurrent = m_lRectifierList[index]->current();
        if (m_dVolgeta <= m_lRectifierList[index]->volgeta())
            m_dVolgeta = m_lRectifierList[index]->volgeta();
        if (m_dTemp <= m_lRectifierList[index]->temp())
            m_dTemp = m_lRectifierList[index]->temp();
        if (m_lRectifierList[index]->state())
            m_sState = 1;
    }
}
