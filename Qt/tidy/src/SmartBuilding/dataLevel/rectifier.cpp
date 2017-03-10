#include "rectifier.h"
#include "src/SmartBuilding/dataLevel/circuit.h"

CRectifier::CRectifier()
: m_iDeviceNo(0)
, m_pCircuit(NULL)
{

}

CRectifier::~CRectifier()
{

}

INT8U CRectifier::circuitNo()
{
    return m_pCircuit->circuitNo();
}
