/****************************************************************************
**
** file: cabinet.h
**
** description:
**     ��������
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
    CCircuit* itemOfCircuit(int cicruitNo);                             // ���ݻ�·�Ų��һ�·��Ϣ
    CCircuit* findCircuit(int index);                                   // �����������һ�·��Ϣ
    virtual void updateParam();

private:
    INT8U                   m_iCabinetNo;
    QList<CCircuit*>        m_lCircuitList;
};

#endif // CABINET_H
