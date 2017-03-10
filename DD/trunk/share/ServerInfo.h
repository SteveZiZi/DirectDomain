///*ServerInfo.h服务进程信息*/

#ifndef INCLUDED_SERVER_INFO
#define INCLUDED_SERVER_INFO

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_KL_TRACE
#include "../base/KLTrace.h"
#endif

#ifndef INCLUDED_PPP_SETTING
#include "PPPSetting.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

#ifndef INCLUDED_CHANNLE
#include "Channel.h"
#endif

#ifndef INCLUDED_TIMER
#include "../base/Timer.h"
#endif

#define CONS_DEFAULT_HEART_BEAT_INTERS 60
#define CONS_MAX_DEAD_COUNT 2

class DirScan;

struct ExeInfo
{
	char PathFile[CONS_PATH_FILE_MAX_LEN+1];//程序文件路径文件，例如/mnt/eac/program/eac或./eac
	char Path[CONS_PATH_FILE_MAX_LEN+1];//程序文件所在绝对路径，例如/mnt/eac/program
	char File[CONS_FILE_NAME_MAX_LEN+1];//程序名，例如eac

	void Init(DirScan& dirScan);

	KLDump& Dump(KLDump& dump) const;
};

struct ServerInfo
{
	INT8U Enabled;//是否有效,非0表示有效
	char Key[CONS_SERVER_NAME_MAX_LEN+1];//服务进程关键字,例如task,httx
	char Name[CONS_SERVER_NAME_MAX_LEN+5];//服务进程名称,例如task,httx1,dbcj2
	INT8U	Type;//服务进程类型，参见SERVER_TYPE
	INT8U InstanceID;//实例ID，一般为0，对于后台通讯进程有可能为非0
	INT8U Mode;//模式,参见SERVER_MODE
	INT8U DeadCount;//心跳停止的次数，当心跳连续停止次数超过CONS_MAX_DEADCOUNT时，将视为此进程已死
	PID_T PID;//进程ID
	//PID_T PID_LAST_KILLED;//上一次杀死的进程ID。这个ID如果和PID一致，则说明该进程已经处于僵死或者僵尸态，无法由守护进程回收它的资源，必须复位系统才能保证程序继续运行。
	time_t LHBTime;//最后心跳时间
	time_t StartSoftTime;//启动时间，软时钟
	time_t StartTrueTime;//启动时间，真时钟

	INT32S ExecTimes;//被执行的次数,每被fork一次就加1
	INT16U HBInterS;//心跳间隔
	INT8U Status;//参见SERVER_STATUS
	struct TraceSetting TraceSetting;//跟踪设置
	INT32U AlrmInterUS;//时钟间隔,为0表示禁止
	char TraceFile[CONS_PATH_FILE_MAX_LEN];//跟踪文件名称，当为空时，输出到标准输出，可配置
	char MainInfo[CONS_SERVER_MAIN_INFO_MAX_LEN];
public:
	inline bool IsStartable()
	{
		return (Enabled==1 && Mode==SERVER_MODE_ACTIVED && Status==SERVER_STATUS_STOPPED);
	}
	inline INT16U GetServerID() const
	{
		return MakeServerID(Type,InstanceID);
	}
	static INT16U MakeServerID(INT8U serverType,INT8U instanceID)
	{
		return (serverType<<8)+instanceID;
	}
	inline bool IsKillable() const
	{
		return (Enabled==0 || Mode!=SERVER_MODE_ACTIVED);
	}
	inline void SetActive()
	{
		Mode=SERVER_MODE_ACTIVED;
	}
	inline bool IsRunning() const
	{
		return (Status==SERVER_STATUS_RUNNING && PID>0);
	}
	inline void SetInactive()
	{
		Mode=SERVER_MODE_INACTIVED;
	}
	inline void SetSuspend()
	{
		if (Mode==SERVER_MODE_ACTIVED)
		{
			Mode=SERVER_MODE_SUSPENDED;
		}
	}
	inline void SetResume()
	{
		if (Mode==SERVER_MODE_SUSPENDED)
		{
			Mode=SERVER_MODE_ACTIVED;
		}		
	}
	void Init(bool enabled,const char* serverKey,SERVER_TYPE serverType,INT8U instanceID,const char* name=NULL,INT16U hbInterS=CONS_DEFAULT_HEART_BEAT_INTERS);
	void OnStarted(PID_T pid,time_t softTime);
	void OnStopped();
	int HBTimoutCheck(time_t& tNow);
	static void MakeName(char* name,const char* serverKey,INT8U instanceID);
	KLDump& Dump(KLDump& dump) const;
private:
	void BackupAbnormalLog(void);

};


struct DaemonServerInfo
{
	struct ServerInfo BaseInfo;
public:
	inline void Init(const char* name)
	{
		BaseInfo.Init(true,CONS_DAEMON_KEY,SERVER_TYPE_DAEMON,0,name,0);
		BaseInfo.AlrmInterUS=CONS_DAEMON_ALRM_INTER_US;
	}
	inline KLDump& Dump(KLDump& dump) const
	{
		dump<<BaseInfo;
		return dump;
	}
};

//suxufeng:[2010-3-31]去除work进程
//struct WorkServerInfo
//{
//	struct ServerInfo BaseInfo;
//public:
//	inline void Init()
//	{
//		BaseInfo.Init(true,CONS_WORK_KEY,SERVER_TYPE_WORK,0,NULL,CONS_WORK_HEART_BEAT_INTER_S);
//		BaseInfo.AlrmInterUS=CONS_WORK_ALRM_INTER_US;
//	}
//	inline KLDump& Dump(KLDump& dump) const
//	{
//		dump<<BaseInfo;
//		return dump;
//	}
//};
//
//struct UpdateServerInfo
//{
//	struct ServerInfo BaseInfo;
//public:
//	inline void Init()
//	{
//		BaseInfo.Init(true,CONS_UPDATE_KEY,SERVER_TYPE_UPDATE,0,NULL,CONS_UPDATE_HEART_BEAT_INTER_S);
//		BaseInfo.AlrmInterUS=CONS_UPDATE_ALRM_INTER_US;
//	}
//	inline KLDump& Dump(KLDump& dump) const
//	{
//		dump<<BaseInfo;
//		return dump;
//	}
//};
struct DeviceRuleConfig
{
	INT8U Enabled;
	INT8U RuleID;
	INT16U RuleSubID;
	INT32U PackageMaxLen;//为0时采用规约默认的包长度
	char Option[CONS_DEVICE_RULE_OPTION_MAX_LEN];	//只是用来存储规约选项，具体的含义由规约自行解析
	
	INT8U Configed;//为0表示未配置

	KLDump& Dump(KLDump& dump) const;

	inline DeviceRuleConfig()
	{
		memset(this,0,sizeof(*this));
	}
};

//后台通讯配置方案
struct HttxPlan
{
	INT8U Enabled;
	INT8U Index;
	char Key[CONS_CONFIG_PLAN_KEY_MAX_LEN+1];//关键字
	char Name[CONS_CONFIG_PLAN_NAME_MAX_LEN+1];//名称，一般为中文
	INT8U RuleCount;
	DeviceRuleConfig DeviceRule[CONS_DEVICE_RULE_MAX_COUNT];//支持的装置规约，优先的规约在最前面

	INT8U Configed;//为0表示未配置
	KLDump& Dump(KLDump& dump) const;
	bool IsUsable() const
	{
		return Enabled!=0 && RuleCount>0;
	}

};

//后台通讯配置方案集合
struct HttxPlans
{
	INT16U Count;
	INT16U DefaultIndex;//默认方案的索引
	HttxPlan Item[CONS_HTTX_CONFIG_PLAN_COUNT];

	const HttxPlan* GetDefaultPlan() const;

	KLDump& Dump(KLDump& dump) const;

	void Init();
	const HttxPlan* GetHttxPlan(INT16U planIndex,bool enableFix=true) const;
	const HttxPlan* GetHttxPlan(const char* planKey,bool enableFix=true) const;
};

struct HttxServerInfo
{
	INT8U Master;//是否是主进程1为主0为从
	INT8U RuleChannelEnabled;//是否可以使用装置规约自带的通道
	INT8U ChannelCount;
	INT8U Configed;//是否被配置，如果为0表示没有在配置文件中存在

	INT16U IdleExitTimeoutM;//此后台通讯的最大空闲时间（只要接收到数据就当做非空闲），为0时表示不进行空闲判断，否则将由守护进程检查此后台通讯进行的最后接收数据时间，如果超过此值分钟未收到数据，将由守护进程杀死此进程。	
	INT16U IdleRebootTimeoutM;//此后台通讯的最大空闲时间（只要接收到数据就当做非空闲），为0时表示不进行空闲判断，否则将由守护进程检查此后台通讯进行的最后接收数据时间，如果超过此值分钟未收到数据，系统将重启

	INT16U OffLineRequest;//断链请求
	INT16U ConnectedCount;//已经连接上的次数（只要建立Socket成功，就应该将此值加1）
	time_t LastRecvTime;//最近一接收到数据的时间

	char PlanKey[CONS_CONFIG_PLAN_KEY_MAX_LEN+1];//配置方案关键字
	struct Channel Channels[CONS_HTTX_CHANNEL_MAX_COUNT];
	INT8U CompressMethod;//压缩方式，参见COMPRESS_METHOD
	INT8U EncryptMethod;//加密方式，参见 ENCRYPT_METHOD

	INT8U CurPPPNeedLevel;//当前的PPP拨号需求等级
	INT8U EventCheckFlag;//非0时表示需要检查是否有需要上报的事件

	struct ServerInfo BaseInfo;
public:
	void Init(INT8U instanceID);
	inline void SetCurChannel(Channel* ipChannel)
	{
		CurPPPNeedLevel=(ipChannel==NULL?PPP_NEED_LEVEL_NONE:ipChannel->GetPPPNeedLevel());
	}
	inline INT8U GetCurPPPNeedLevel() const
	{
		return CurPPPNeedLevel;
	}
	inline bool HasPPPNeedChannel() const
	{
		for (INT8U i=0;i<ChannelCount;i++)
		{
			if (Channels[i].GetPPPNeedLevel()==PPP_NEED_LEVEL_NONE)
			{
				return true;
			}
		}
		return false;
	}
	inline void OnChannelChanged()
	{
		OffLineRequest=0;
		ConnectedCount=0;
	}
	inline void OnConnected()
	{
		OffLineRequest=0;
		if (ConnectedCount<0xffff)
		{
			ConnectedCount++;
		}
	}
	inline void OnReceived(time_t curSoftTime)
	{
		LastRecvTime=curSoftTime;
	}
	inline bool ShouldExit(time_t curSoftTime)
	{
		return (ReceiveCheck(curSoftTime) & 0x01) >0;
	}
	inline bool ShouldReboot(time_t curSoftTime)
	{
		return (ReceiveCheck(curSoftTime) & 0x02) >0;
	}
	int ReceiveCheck(time_t curSoftTime);
	KLDump& Dump(KLDump& dump) const;
};

struct DbcjServerInfo
{
	INT8U GatherStatus;//采集状态，参见GATHER_STATUS
	INT8U Configed;//是否被配置，如果为0表示没有在配置文件中存在
	struct ServerInfo BaseInfo;

	//2009-03-11 陈桂华 增加通道4=无线
	//struct Channel Channel[3];
	//100302 cgh 统一端口号
	inline int Get485Port1()
	{
		return /*GATHER_PORT_RS485I;*/Channel[0].Setting.Com.PortNo;//hzm,100303
	}

	//100302 cgh 统一端口号
	inline int Get485Port2()
	{
		return /*GATHER_VPORT_RS485II;*/Channel[1].Setting.Com.PortNo;//hzm,100303
	}
    
	//100302 cgh 统一端口号
	inline int GetZBMeterPort()
	{
		return /*GATHER_VPORT_ZB;*/Channel[2].Setting.Com.PortNo;//hzm,100303
	}
      struct Channel Channel[BUS_COUNT];
public:
	void Init();
	KLDump& Dump(KLDump& dump) const;
};

struct ScanfServerInfo
{
	struct ServerInfo BaseInfo;

	//2009-03-11 陈桂华 增加通道4=无线
	//struct Channel Channel[3];
	//100302 cgh 统一端口号
	inline int Get485Port1()
	{
		return /*GATHER_PORT_RS485I;*/Channel[0].Setting.Com.PortNo;//hzm,100303
	}

	//100302 cgh 统一端口号
	inline int Get485Port2()
	{
		return /*GATHER_VPORT_RS485II;*/Channel[1].Setting.Com.PortNo;//hzm,100303
	}
    
	//100302 cgh 统一端口号
	inline int GetZBMeterPort()
	{
		return /*GATHER_VPORT_ZB;*/Channel[2].Setting.Com.PortNo;//hzm,100303
	}
      struct Channel Channel[BUS_COUNT];
public:
	void Init();
	KLDump& Dump(KLDump& dump) const;
};

struct LcdServerInfo
{
	struct ServerInfo BaseInfo;
public:
	void Init();
	KLDump& Dump(KLDump& dump) const;

};

struct PPPServerInfo
{
	struct PPPSetting Setting;

	INT8U Inactive;//非0表示PPP暂时不进行拨号，处于挂起状态（但是PPP进程仍可能有效，例如短信模式时有可能禁用PPP拨号）
	INT8U OnLine;//非0时表示在线

	INT32U RecvPackages;//接收包数目
	INT32U SendPackages;//发送包数目
	INT32U TotalRecvPackages;//总的接收包数目
	INT32U TotalSendPackages;//总的发送包数目
	INT32U OffLineTimes;//总掉线次数（供后台通讯进程用，判断此值是否变化，如果有变化，后台通讯进程有可能需要进行检测在线情况）
	INT32U DialTimes;//总拨号次数
	time_t LastDialTime;//最后一次拨号时间
	time_t LastRecvTime;//最后一次收到数据包的时间
	time_t LastRecvSoftTime;//最后一次接收到数据包时的时间（软时钟）
	time_t PackageStatInitTime;//包统计的开始时间（TotalRecvPackages以及TotalSendPackages是从此时开始计数的)
	INT16S Signal;
	struct ServerInfo BaseInfo;
public:

	//Lcd通过调用此方法将包统计起始时间复位
	inline void ResetPackageStatTime()
	{
		PackageStatInitTime=0;
	}
	inline time_t UnRecvTime()
	{
		return 0;
	}

	void Init();
	KLDump& Dump(KLDump& dump) const;
};

//2006-6-10，由苏煦烽添加
struct JilianServerInfo
{
	//bool JilianMasterMod;//级联进程工作模式，
	INT8U Configed;//是否被配置，如果为0表示没有在配置文件中存在
	struct ServerInfo BaseInfo;
	struct Channel Channel;
public:
	void Init(INT8U instanceID);
	KLDump& Dump(KLDump& dump) const;
};
struct ServerInfos
{
	ExeInfo Exe;
	DaemonServerInfo Daemon;//守护进程
	//suxufeng:[2010-3-31]去除work进程
	//WorkServerInfo Work;//工作进程，即原有的任务进程(为了不与系统的任务相冲突,故改名为工作进程)
	HttxServerInfo Httx[CONS_HTTX_MAX_COUNT];//后台通讯进程
	DbcjServerInfo Dbcj;//电表通讯进程
	JilianServerInfo Jilian[CONS_JILIAN_MAX_COUNT];//主从级联进程，2006-6-11，由苏煦烽添加
	PPPServerInfo PPP;
	LcdServerInfo  Lcd;//液晶进程
	ScanfServerInfo Scanf;//电表通讯进程
	//suxufeng:[2010-3-31]去除wrok进程
	enum {InfoCount=CONS_HTTX_MAX_COUNT+CONS_JILIAN_MAX_COUNT+5};//daemon+work+httx+dbcj+jilian+ppp

public:
	inline void BaseInit(DirScan& dirScan)
	{
		Exe.Init(dirScan);
	}
	void Init();
	//ServerInfo* FindBaseInfo(const char* serverKey,INT8U instanceID);
	ServerInfo** GetBaseInfos();
	ServerInfo* FindBaseInfo(const char* serverKey,INT8U instanceID);
	ServerInfo* FindBaseInfo(INT16U serverID);

	INT8U GetCurPPPNeedLevel() const;
	int GetDeviceRuleBitSet(const HttxPlans& plans,DeviceRuleBitSet& oDRBS);
private:
	void InitBaseInfos(ServerInfo** baseInfos);
};

#endif

