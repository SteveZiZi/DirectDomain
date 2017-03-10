/****************************************************************************
**
** file: rectifier.h
**
** description:
**     整流器数据
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef RECTIFIER_H
#define RECTIFIER_H

#include "src/SmartBuilding/dataLevel/baseDevice.h"

class CCircuit;
class CRectifier : public CBaseDevice
{
public:
    CRectifier();
    ~CRectifier();

    INT8U deviceNo() {return m_iDeviceNo;}
    void setDeviceNo(int deviceNo_) {m_iDeviceNo = deviceNo_;}
    void setCurrent(double current_) {m_dCurrent = current_;}
    void setVolgate(double volgate_) {m_dVolgeta = volgate_;}
    void setTemp(double temp_) {m_dTemp = temp_;}
    void setState(INT16U& state_) {m_sState = state_;}
    void setCircuit(CCircuit* circuit_) {m_pCircuit = circuit_;}
    INT8U circuitNo();
    virtual void updateParam() {return;}

private:
    INT8U           m_iDeviceNo;
    CCircuit*       m_pCircuit;
};

#endif // RECTIFIER_H
