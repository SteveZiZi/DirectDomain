/****************************************************************************
**
** file: baseDevice.h
**
** description:
**     设备数据基类
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef BASEDEVICE_H
#define BASEDEVICE_H

#include "src/SmartBuilding/inc/xtDef.h"

class CBaseDevice
{
public:
    CBaseDevice();
    virtual~CBaseDevice();

    double power() {return m_dCurrent * m_dVolgeta;}
    double current() {return m_dCurrent;}
    double volgeta() {return m_dVolgeta;}
    double temp() {return m_dTemp;}
    INT16U& state() {return m_sState;}
    virtual void updateParam() = 0;

protected:
    double          m_dCurrent;
    double          m_dVolgeta;
    double          m_dTemp;
    INT16U          m_sState;
};

#endif // BASEDEVICE_H
