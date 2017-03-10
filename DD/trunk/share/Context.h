///*Context.h系统上下文，将存放在共享内存之中*/

#ifndef INCLUDED_CONTEXT
#define INCLUDED_CONTEXT

#ifndef INCLUDED_SYS_CONFIG
#include "SysConfig.h"
#endif

#ifndef INCLUDED_SERVER_INFO
#include "ServerInfo.h"
#endif

#ifndef INCLUDED_DATA_ID
#include "DataID.h"
#endif

#ifndef INCLUDED_ACTION
#include "Action.h"
#endif

#ifndef INCLUDED_METER
#include "Meter.h"
#endif

#ifndef INCLUDED_RELAY
#include "Relay.h"
#endif

#include "clrelay.h"
#include "RtfCmdItem.h"

#ifndef INCLUDED_TASK
#include "Task.h"
#endif

#ifndef INCLUDED_METER_RULE_INFO
#include "MeterRuleInfo.h"
#endif

#ifndef INCLUDED_DEVICE_RULE_INFO
#include "DeviceRuleInfo.h"
#endif


#ifndef INCLUDED_SHARE_MEM
#include "../base/ShareMem.h"
#endif

#ifndef INCLUDED_SERVER
#include "../share/Server.h"
#endif

#ifndef HTTX_JILIAN
#include "Httx_Dbcj3.h"
#endif

#ifndef INCLUDED_FTS_DATA
#include "FtsData.h"
#endif

#ifndef INCLUDED_GPRS_INFO
#include "GprsInfo.h"
#endif
#include "../share/ZBSnifferTask.h"
#include "Status.h"
#include "BusChannelsRule.h"
#include "../include/FileDir.h"
#include "../base/DirScan.h"
#include "../base/FileBase.h"
#include "../config/confuse/confuse.h"
#include "../share/dataItem.h"
#include "../share/nortask.h"
#include "../share/luxtask.h"
#include "../share/consluxtask.h"

#ifndef INCLUDED_ZBSNIFFERTASK
#include "ZBSnifferTask.h"
#endif

#ifndef INCLUDED_STATUS
#include "Status.h"
#endif

#ifndef PBZD_CLASS
#include "JilianSlaves.h"
#endif

#ifndef FIXDATA_CLASS
#include "FixData.h"
#endif

class DirScan;

struct DeviceStatus
{
	INT8U PowerSource;//电源，参见DATA_SOURCE
	bool m_AutoReport_Day,m_AutoReport_Month,m_AutoReport_Vip;//主动上报状态
	INT32U JilianPackageFromWhichHttxServer;
	bool OnSystemUpdate;
	DeviceStatus()
	{
		PowerSource = 0x00;
		m_AutoReport_Day = false;
		m_AutoReport_Month = false;
		m_AutoReport_Vip = false;
		JilianPackageFromWhichHttxServer = 1;
		OnSystemUpdate = false;
	}
};

struct GatherMeters
{
	MeterBitSet GatherMBS;//被采集的电表，由参数更改进程刷新
};
struct GatherTasks
{
	TaskBitSet GatherTBS;//需要采集的任务，由参数更改进程刷新
	TaskBitSet WaitGatherTBS;//正在等待采集的任务，由各个电表通讯进程刷新
	TaskBitSet FinishGatherTBS;//已采集完成的任务，由各个电表通讯进程刷新
};


//复位信息，注意：为了避免竟争，多种标志不能组合在同一个成员中
struct ResetInfo
{
	INT16U SoftwareResetDelayS;//软件复位延时秒数，当此计数达到一定值之后才进行软件复位
	INT16U HardwareResetDelayS;//硬件复位延时秒数,根据此计延时来进程程序挂起以及硬件复位
	
	INT8U HardwareResetFlag;//硬件复位标志，当此值非0时，表示需要硬件复位
	INT8U SoftwareResetFlag;//软件复位标志，当此值非0时，表示需要软件复位，升级后一般需要软件复位（延时执行）
	INT8U DogResetFlag;//看门狗复位标志，当此值非0时，表示需要进行看门狗复位
	INT8U ReserveFlag;//保留
	INT8U ReserveFlag2;//保留

	inline void Init()
	{
		memset(this,0,sizeof(*this));
	}
	KLDump& Dump(KLDump& dump) const;
};
//终端集中抄表状态信息
struct GatherStateInfo
{
	INT8U PortNo;//终端通信端口号
	INT16U MetersTotalCnt;//要抄电表总数

	INT8U GathertFlag;//当前抄表工作状态标志
	INT16U SucceedMetersCnt;//抄表成功块数
	MeterBitSet GatherFailedMBS;
	INT8U VIPMeterCnt;//重点表块数
	char BegTime[6];//抄表开始时间
	char EndTime[6];//抄表结束时间

	INT16U firstno;
	INT16U lastno;

	KLDump& Dump(KLDump& dump) const
	{
		dump<<"PortNo="<<PortNo<<"\n";
		dump<<"MetersTotalCnt=";
		dump.DumpAsHex(MetersTotalCnt)<<"\n";
		dump<<"GatherFlag=";
		dump.DumpAsHex(GathertFlag)<<"\n";
		dump<<"SucceedMetersCnt=";
		dump.DumpAsHex(SucceedMetersCnt)<<"\n";
		dump<<"VIPMeterCnt=";
		dump.DumpAsHex(VIPMeterCnt)<<"\n";
		dump<<"BegTime=";
		for (int i = 0; i < 6; i++)
		{
			dump<<" ";
			dump.DumpAsHex((INT8U)(BegTime[i]>>4));
			dump.DumpAsHex((INT8U)(BegTime[i]&0xF));
		}
		dump<<"\n";
		dump<<"EndTime=";
		for (int i = 0; i < 6; i++)
		{
			dump<<" ";
			dump.DumpAsHex((INT8U)(EndTime[i]>>4));
			dump.DumpAsHex((INT8U)(EndTime[i]&0xF));
		}
		dump<<"\n";
		return dump;
	}
};
//以日数据抄收为一个时间段，也即一天为一个时间段
struct GatherStateInfos
{
public:
	GatherStateInfo statusinfo[3];
	//MeterBitSet GatherFailedMBS;

public:
	void Init();
	void ResetMeterNum(INT8U PortType=0);
	void ResetVipNum(INT8U PortType=0);
	void GetFailedMBS(MeterBitSet &mbs);
	void SetFailedMBS(INT16U meterNo);
	void SetSucceedMBS(INT16U meterNo);
	void IncMeterNum(Meter& meter);
	void IncSucceedGatherNum(INT8U PortNo);
	void ResetAllSucceedGatherMeter(INT8U PortNo);
	void ResetAllSucceedMeters();
	void IncVIPNum(INT8U PortNo);
	void UpdateBeginTime(Meter& meter);
	void UpdateEndTime(Meter& meter);
	void UpdateGatherFlag(INT8U PortNo,INT8U Gathering);
	KLDump& Dump(KLDump& dump) const
	{
		dump<<"********list GatherStateInfos begin:***********\n";
		dump<<"\n";
		for (int i = 0; i < 3; i++)
		{
			dump<<"GatherStateInfos "<<i+1<<" information:\n";
			statusinfo[i].Dump(dump);
			dump<<"\n";
		}
		dump<<"********list GatherStateInfos end **************\n";
		return dump;
	}
private:
	bool InitTimeFlag;
};
/*********************************************************************************************/
/*                                               Device       define                                                                                       */
/*																							  */
/*********************************************************************************************/

/* devices context define*/

struct SRealControlCommand
{
	INT8U m_DeviceNo;
	INT8U m_SubNo;
	INT8U m_Function;
	INT8U m_FuncData[4];
};
struct SRealControlCommandQueue
{
	bool m_bBusy;
	int m_Num;//待处理命令数
	int m_FinishedNum;//已处理完的命令数
	SRealControlCommand m_RealCtlCmd[CONS_REAL_CONTROL_COMMAND_MAX_COUNT];
	T_emResult m_Result[CONS_REAL_CONTROL_COMMAND_MAX_COUNT];
};

struct SDeviceRegData
{
	INT8U  mReg_0501[9];
	INT8U  mReg_c343[4];
};

struct SDevicesRegData
{
	SDeviceRegData m_Items[CONS_DEVICE_MAX_COUNT];
	void Init(void){memset(this,0x00,sizeof(*this));}
};

/* end */



struct Context
{
	struct HeaderChecker Checker;
	//struct Setting BaseSetting;
	struct SysConfig SysConfig;
	struct HttxPlans HttxCfgPlans;
	struct ServerInfos ServerInfos;
	struct RegIDS RegIDS;
	struct Actions Actions;
	struct Meters Meters;
	/* start */
	struct Devices Devices;
	struct S_CLRelays ClRelays;
	struct DeviceFlags DeviceFlags ;
	class CDataItems	m_DataItems;
	class CRtfCmdItems m_RtfCmdItems;
	struct SDevicesRegData m_DevicesRegData;
	struct SNorTasks NorTasks;//程控任务
	struct SLuxTasks LuxTasks;//光控任务
	struct SConsLuxTasks ConsLuxTasks;//恒照任务
	/* end */	
	struct Relays Relays;
	struct Tasks Tasks;
	struct MeterRuleInfos MeterRuleInfos;
	struct BusRuleInfos BusInfos;
	struct DeviceRuleInfos DeviceRuleInfos;
	struct GatherMeters GatherMeters;
	struct GatherTasks GatherTasks;         
	struct GatherStateInfos GatherStateInfos;
	struct ZBSnifferTask ZBSnifferTask;
	
	struct SEND_BUFFER httx_sendBuffer;
	struct DEBUG_BUFFER debug_buffer;
	struct FtsData FtsDatas;
	struct GprsInfo gprsInfo;
	struct DeviceStatus m_DeviceStatus;	
	struct ResetInfo ResetInfo;	
	struct JilianSlaves JilianSlaves;
	struct FixData FixData;	
	time_t GatherInfoChangeTime;//更改时间
	time_t ContextInitTime;//上下文初始化时间，实际上大约就是系统启动时间
	time_t TickSecond;
	char LocalIp[15];
	struct S_IpConfig m_IpConfig;
	INT8U RecMeterPhase;//接受电表主动上报数据帧相位

	SRealControlCommandQueue m_RealControlCommandQueue; 
	
	INT8U m_DnProtocolType;
	Context();
	static Context& Instance();

	void RefreshGatherInfo();

	bool IsDaemonRunning()
	{
		return IsProcessRunning(ServerInfos.Daemon.BaseInfo.PID,ServerInfos.Daemon.BaseInfo.Name);
	}

	int OnCommand(KLDump& dump,const char* cmdKey,const char* objName,INT8U id,const char *propName,const char* propVal);

	void SendSigNotifyToDaemon(Server::SigTag tag);

	void SetEventBitSet(const char* valList,DataBitSet& oDBS);
	int Save(bool ibForce);
	inline PPPDialSetting& GetDialSetting()
	{
		return ServerInfos.PPP.Setting.GetDialSetting(SysConfig.WirelessModemType);
	}
	inline PPPSetting& GetPPPSetting()
	{
		return ServerInfos.PPP.Setting;
	}
	INT16U FindRelayNo(INT8U addr[3]);
	INT16U FindRelayNo(INT8U addr[6],bool reverse);
	int ModemSignalValue;
	bool MasterHttxOnline;

	void CopyRelayMemory(struct Relays& relays,struct Routers& routers)
	{
		memcpy(&Relays,&relays,sizeof(struct Relays));
	}

private:
	static bool InitFile(FILE* fp, void* ipOther);
	static bool CheckFile(FILE* fp, void* ipOther);

	//检查fileName是哪种配置文件,-1表示不是系统配置文件，0表示是需要刷新上下文的配置文件，1表示是需要软件重启的文件
	//int GetCfgFileType(const char* fileName);
	static ShareMem m_ShareMem;


};

#define CONTEXT Context::Instance()
#define DECLARE_CONTEXT(context) Context& context=CONTEXT

INT8U Context_GetDbcjRS485I(void);
INT8U Context_GetDbcjRS485II(void);
INT8U Context_GetDbcjZBWX(void);

#endif

