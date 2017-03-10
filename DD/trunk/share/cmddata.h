#ifndef _CMDDATA_H_
#define _CMDDATA_H_

#include "dataItem.h"
#include "meter.h"
#include "clrelay.h"
class CCmdBase
{
public:
	CCmdBase()
	{
	}
	
	virtual ~CCmdBase()
	{
	}
	virtual int OnExc(Device &device,bstring& orData)
	{
		return 0;
	}

	virtual int OnExc(void)
	{
		return 0;
	}
	void SetHandler(CCmdBase *pHandler)
	{
		m_pHandler = pHandler;
	}
CCmdBase*m_pHandler;	
};
class CPollCmd:CCmdBase
{
public:
	CPollCmd();
	~CPollCmd();
	 int OnExc(Device &device,bstring& orData);
	 int OnExcRcu(Device &device,bstring& orData);
	 int OnExcRtf(Devices& devices);
	 int OnExcClArg(Devices& devices);
private:
	bool PollDoorCard(Device &device,bstring& orData);
	bool PollDoorBell(Device &device,bstring& orData);
	bool PollUrgentButton();
	bool UrgentKeyControl(MapItem *pmapItem);
	bool IrdaDetect(Device &device,bstring& orData);
	void IrdaSceneCompose(INT8U state);
	int GetAirconditionStatus(Device &device,bstring& orData);
private://整流器相关	
	bool DetectClRtfComm(S_CLRelay& clRelay);
	bool ForceClVolArg(S_CLRelay& clRelay,INT16U Vol, INT16U Cur);
private:
	DevicesBitSet m_PollRtfBS;

	
};

class CLoadMapCmd:CCmdBase
{
public:
	CLoadMapCmd()
	{
	}
	~CLoadMapCmd()
	{
	}
	 int OnExc(Device &device,bstring& orData);
};


class CDataItemCmd:CCmdBase
{
public:
	CDataItemCmd()
	{
	}
	~CDataItemCmd()
	{
	}
	 int OnExc(Device &device,bstring& orData);
	 void OpenDoorNoDisplay();
public:
	void SceneCompose(INT8U planNo);
	
private:
	INT16U Filter(INT16U dataid);
	bool KeyBacklightCtrl(Device device,int state);
	bool OpenBedLight();
	bool IsBedLightOn();
	bool PowerSwitchControl(Device device);
	bool CleanRoomKeyControl(MapItem mapItem);
	bool DonotDisturbKeyControl(MapItem mapItem);
	bool BellKeyControl(MapItem mapItem);
	int SoftKeyProcess(Device device,INT8U subno,CDataItem *pDataItem,bstring idata);
	int SoftLongKeyProcess(Device device,INT8U subno,CDataItem *pDataItem,bstring idata);
	void IORelayShortKey(Device device,INT16U bitStat,bstring idata);
	void IORelayLongKey(Device device,INT16U bitStat,bstring idata);
	void TotalPowerCompose(INT8U state);
	void CloseOtherSceneCompose(INT8U planNo,INT8U refScenePlanNo);
	bool IsBelongToOtherSceneComposeMember(SSceneComposeMember member,INT8U refScenePlanNo);
private:
	INT8U m_WindSpeed[CONS_DEVICE_MAX_COUNT];
};

class CPollAirConditionCmd:CCmdBase
{
public:
	CPollAirConditionCmd()
	{
	}
	~CPollAirConditionCmd()
	{
	}
	int OnExc(void);
	void SetAirConditonTemperature(Device device,INT8U tmp);
private:
	int PollAirConditionHandle(Device device);
	int AirConditonHandle(Device device);
	
	bool GetAirConditonSettingTemperature(Device device,INT8U &value);
private:
	INT8U temperature;
	Device m_ACDevice;
};

class CPollDoorCmd:CCmdBase
{
public:
	CPollDoorCmd()
	{
	}
	~CPollDoorCmd()
	{
	}
	int OnExc(void);
private:
	bool GetDoorCheckState();
	void OpenGalleryLight(Device device ,int min);

};


class CCheckTimeCmd:CCmdBase
{
public:
	CCheckTimeCmd()
	{
	}
	~CCheckTimeCmd()
	{
	}
	 int OnExc(Device &device,bstring& orData);
};
#endif

