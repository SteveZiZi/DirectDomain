#ifndef INCLUDED_STATUS_CFG
#define INCLUDED_STATUS_CFG

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../share/SysBase.h"
#endif

#ifndef INCLUDED_STATUS
#include "../share/Status.h"
#endif

#ifndef INCLUDED_REG_FILE
#include "../share/RegFile.h"
#endif

class RegItem;

struct StatusCookie
{
	INT8U MeterProgramCount;//用于电表编程事件检测
	INT8U MeterTimeRegCount;//用于检查电表时钟异常

	inline StatusCookie()
	{
		Reset();
	}
	inline void Reset()
	{
		memset(this,0,sizeof(*this));
	}
};

class StatusCfg
{
public:
	static int OnSetPowerSource(INT8U newVal);
	inline static int SetMeterCommTrouble(INT16U meterNo,INT8U newVal)
	{
		DECLARE_STATUS(status);
		if (meterNo>=sizeof(status.MeterStatus)/sizeof(status.MeterStatus[0]))
		{
			ASSERT(false);
			return -1;
		}
		return SetMeterFlag(meterNo,status.MeterStatus[meterNo].CommTrouble,newVal,NOTIFY_TYPE_METER_COMMTROUBLE_HAPPEN,NOTIFY_TYPE_METER_COMMTROUBLE_RESUME);
	}
	inline static void OnHardwareReset()
	{
		//DECLARE_STATUS(status);
	/*	status.DeviceStatus.HardwareResetCount++;
		time(&status.DeviceStatus.LastHadrwareResetTime);*/
		OnStatusChanged();
		StatusCfg::OnNotify(NOTIFY_TYPE_DEVICE_HARDWARE_RESET,0,NULL);
	}
	inline static void OnSoftwareReset()
	{
		//DECLARE_STATUS(status);
		//status.DeviceStatus.SoftwareResetCount++;
		//time(&status.DeviceStatus.LastSoftwareResetTime);
		OnStatusChanged();
		StatusCfg::OnNotify(NOTIFY_TYPE_DEVICE_SOFTWARE_RESET,0,NULL);
	}
	inline static void OnDataReset()
	{
		//DECLARE_STATUS(status);
		//status.DeviceStatus.DataResetCount++;
		//time(&status.DeviceStatus.LastDataResetTime);
		OnStatusChanged();
		OnNotify(NOTIFY_TYPE_DEVICE_DATA_RESET,0,NULL);
	}
	inline static void OnMemReset()
	{
		//DECLARE_STATUS(status);
		//status.DeviceStatus.MemResetCount++;
		//time(&status.DeviceStatus.LastMemResetTime);
		OnStatusChanged();
		OnNotify(NOTIFY_TYPE_DEVICE_MEM_RESET,0,NULL);
	}
	inline static void OnParamReset()
	{
		//DECLARE_STATUS(status);
		//status.DeviceStatus.ParamResetCount++;
		//time(&status.DeviceStatus.LastParamResetTime);
		OnStatusChanged();
		OnNotify(NOTIFY_TYPE_DEVICE_PARAM_RESET,0,NULL);
	}
	inline static void OnParamChanged()
	{
		OnNotify(NOTIFY_TYPE_DEVICE_PARAM_CHANGED,0,NULL);
	}
	inline static void OnTimeChanged()
	{
		OnNotify(NOTIFY_TYPE_DEVICE_TIME_CHANGED,0,NULL);		
	}
	inline static void OnDataFileRebuilded()
	{
		OnNotify(NOTIFY_TYPE_DEVICE_DATA_LOST,0,NULL);		
	}
	inline static void OnSysShutdown()
	{
		OnNotify(NOTIFY_TYPE_DEVICE_SHUTDOWN,0,NULL);		
	}
	inline static void OnSysStart()
	{
		OnNotify(NOTIFY_TYPE_DEVICE_START,0,NULL);
	}
	inline static void OnBatteryCutted()
	{
		OnNotify(NOTIFY_TYPE_DEVICE_START,0,NULL);
	}

	inline static void OnStatusChanged()
	{
		DECLARE_STATUS(status);
		status.OnChanged();
	}
	inline static int SaveStatus(bool ibForce)
	{
		DECLARE_STATUS(status);
		return status.Save(ibForce);
	}
	static void BeforeUpdateReg(unsigned short isObjID);
	static void AfterUpdateReg(unsigned short isObjID);
	static void AfterWriteReg(const RegItem& irRegItem,const RegItem& irOldRegItem);
	static void SetRegHook();
protected:
	static int OnNotify(INT32U iiNotifyID,INT32S iiObjID,void* ipOther);
private:
	static int SetMeterFlag(INT16U meterNo,INT8U& flag,INT8U newVal,INT32S happenNotify,INT32S resumeNotify);
	static void MeterTimeCheck(unsigned short isObjID);
private:
	static bool m_bNeedSaveStatus;
	static StatusCookie m_Cookie;
};

#endif
