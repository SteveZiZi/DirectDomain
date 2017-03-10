#ifndef INCLUDED_SCANF_SERVER
#define INCLUDED_SCANF_SERVER

#include "../../share/Server.h"
#include "../../share/Meter.h"
#include "../../share/Context.h"
#include "../../share/AlarmFile.h"
#include "../../share/TaskDataFile.h"
#include "../../share/dataItem.h"
#include "../../share/cmddata.h"
#include "../../share/ipcSharemem.h"

class ScanfServer:public Server
{
public:
	ScanfServer();
	~ScanfServer();

	static ScanfServer *GetCurServer()
	{
		return (ScanfServer *)Server::GetCurServer();
	}
protected:
	int OnTickServer();

private:
	void RealCommandProcess();
	void QuiRealCommandProcess();
	int RealCmdRelayAction(SRealControlCommand &command, INT8U onOffValue);
	int RealCmdLightLevel(SRealControlCommand &command);
	int RealCmdRtfVolCurSet(SRealControlCommand &command, INT16U vol,INT16U cur);
	int RealCmdClVolCurSet(SRealControlCommand &command, INT16U vol,INT16U cur);
	int RealCmdRtfSwitchSet(SRealControlCommand &command, INT8U state, INT8U delayTime);
	int RealCmdClOn(SRealControlCommand &command);
	int RealCmdClOff(SRealControlCommand &command);

	int ManualRealCmdClVolCurSet(SRealControlCommand &command, INT16U vol,INT16U cur);
	int ManualRealCmdClOn(SRealControlCommand &command);
	int ManualRealCmdClOff(SRealControlCommand &command);
	
private:
	void TaskProcess(void);
	void LuxTaskProcess(void);
	void ConstantLuxTaskProcess(void);
	void ManualCtlOutOfDateProcess();
	void ClLeakProcess(void);
private:
	Devices &m_Devices;
	CDataItems m_DataItems;
	CPollCmd *m_pPollCmd;
	CDataItemCmd *m_pDataItemCmd;
	SIpcLuxTasks   m_IpcLuxTasksBak;//IpcLuxTask数据备份，用来判断光控任务是否在QTUI界面修改，如果有修改则清除任务的运行状态
};


#endif
