///*Context.hϵͳ�����ģ�������ڹ����ڴ�֮��*/

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
	INT8U PowerSource;//��Դ���μ�DATA_SOURCE
	bool m_AutoReport_Day,m_AutoReport_Month,m_AutoReport_Vip;//�����ϱ�״̬
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
	MeterBitSet GatherMBS;//���ɼ��ĵ���ɲ������Ľ���ˢ��
};
struct GatherTasks
{
	TaskBitSet GatherTBS;//��Ҫ�ɼ��������ɲ������Ľ���ˢ��
	TaskBitSet WaitGatherTBS;//���ڵȴ��ɼ��������ɸ������ͨѶ����ˢ��
	TaskBitSet FinishGatherTBS;//�Ѳɼ���ɵ������ɸ������ͨѶ����ˢ��
};


//��λ��Ϣ��ע�⣺Ϊ�˱��⾹�������ֱ�־���������ͬһ����Ա��
struct ResetInfo
{
	INT16U SoftwareResetDelayS;//�����λ��ʱ���������˼����ﵽһ��ֵ֮��Ž��������λ
	INT16U HardwareResetDelayS;//Ӳ����λ��ʱ����,���ݴ˼���ʱ�����̳�������Լ�Ӳ����λ
	
	INT8U HardwareResetFlag;//Ӳ����λ��־������ֵ��0ʱ����ʾ��ҪӲ����λ
	INT8U SoftwareResetFlag;//�����λ��־������ֵ��0ʱ����ʾ��Ҫ�����λ��������һ����Ҫ�����λ����ʱִ�У�
	INT8U DogResetFlag;//���Ź���λ��־������ֵ��0ʱ����ʾ��Ҫ���п��Ź���λ
	INT8U ReserveFlag;//����
	INT8U ReserveFlag2;//����

	inline void Init()
	{
		memset(this,0,sizeof(*this));
	}
	KLDump& Dump(KLDump& dump) const;
};
//�ն˼��г���״̬��Ϣ
struct GatherStateInfo
{
	INT8U PortNo;//�ն�ͨ�Ŷ˿ں�
	INT16U MetersTotalCnt;//Ҫ���������

	INT8U GathertFlag;//��ǰ������״̬��־
	INT16U SucceedMetersCnt;//����ɹ�����
	MeterBitSet GatherFailedMBS;
	INT8U VIPMeterCnt;//�ص�����
	char BegTime[6];//����ʼʱ��
	char EndTime[6];//�������ʱ��

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
//�������ݳ���Ϊһ��ʱ��Σ�Ҳ��һ��Ϊһ��ʱ���
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
	int m_Num;//������������
	int m_FinishedNum;//�Ѵ������������
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
	struct SNorTasks NorTasks;//�̿�����
	struct SLuxTasks LuxTasks;//�������
	struct SConsLuxTasks ConsLuxTasks;//��������
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
	time_t GatherInfoChangeTime;//����ʱ��
	time_t ContextInitTime;//�����ĳ�ʼ��ʱ�䣬ʵ���ϴ�Լ����ϵͳ����ʱ��
	time_t TickSecond;
	char LocalIp[15];
	struct S_IpConfig m_IpConfig;
	INT8U RecMeterPhase;//���ܵ�������ϱ�����֡��λ

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

	//���fileName�����������ļ�,-1��ʾ����ϵͳ�����ļ���0��ʾ����Ҫˢ�������ĵ������ļ���1��ʾ����Ҫ����������ļ�
	//int GetCfgFileType(const char* fileName);
	static ShareMem m_ShareMem;


};

#define CONTEXT Context::Instance()
#define DECLARE_CONTEXT(context) Context& context=CONTEXT

INT8U Context_GetDbcjRS485I(void);
INT8U Context_GetDbcjRS485II(void);
INT8U Context_GetDbcjZBWX(void);

#endif

