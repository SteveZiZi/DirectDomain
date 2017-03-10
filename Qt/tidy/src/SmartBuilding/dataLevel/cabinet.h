/****************************************************************************
**
** file: cabinet.h
**
** description:
**     机柜数据
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef CABINET_H
#define CABINET_H

#include "src/SmartBuilding/dataLevel/baseDevice.h"

#include <QList>

class CCircuit;
class CCabinet : public CBaseDevice
{
public:
    CCabinet();
    ~CCabinet();

    INT8U cabinetNo() {return m_iCabinetNo;}
    void setCabinetNo(int cabinetNo_) {m_iCabinetNo = cabinetNo_;}
    int circuitCount() {return m_lCircuitList.size();}
    bool appendCircuit(CCircuit* circuit);
    void clearCircuit();
    CCircuit* itemOfCircuit(int cicruitNo);                             // 根据回路号查找回路信息
    CCircuit* findCircuit(int index);                                   // 根据索引查找回路信息
    virtual void updateParam();

private:
    INT8U                   m_iCabinetNo;
    QList<CCircuit*>        m_lCircuitList;
};

#endif // CABINET_H
