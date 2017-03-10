/****************************************************************************
**
** file: circuit.h
**
** description:
**     回路数据类
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "src/SmartBuilding/dataLevel/baseDevice.h"
#include <QList>

class CRectifier;
class CCircuit : public CBaseDevice
{
public:
    CCircuit();
    ~CCircuit();

    INT8U circuitNo() {return m_iCircuitNo;}
    void setCircuitNo(int circuitNo_) {m_iCircuitNo = circuitNo_;}
    int rectifierCount() {return m_lRectifierList.size();}
    bool appendRectifier(CRectifier* rectifier);
    void clearRectifier();
    CRectifier* findRectifier(int index);
    virtual void updateParam();

private:
    INT8U                       m_iCircuitNo;
    QList<CRectifier*>          m_lRectifierList;
};

#endif // CIRCUIT_H
