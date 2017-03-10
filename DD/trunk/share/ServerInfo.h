///*ServerInfo.h���������Ϣ*/

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
	char PathFile[CONS_PATH_FILE_MAX_LEN+1];//�����ļ�·���ļ�������/mnt/eac/program/eac��./eac
	char Path[CONS_PATH_FILE_MAX_LEN+1];//�����ļ����ھ���·��������/mnt/eac/program
	char File[CONS_FILE_NAME_MAX_LEN+1];//������������eac

	void Init(DirScan& dirScan);

	KLDump& Dump(KLDump& dump) const;
};

struct ServerInfo
{
	INT8U Enabled;//�Ƿ���Ч,��0��ʾ��Ч
	char Key[CONS_SERVER_NAME_MAX_LEN+1];//������̹ؼ���,����task,httx
	char Name[CONS_SERVER_NAME_MAX_LEN+5];//�����������,����task,httx1,dbcj2
	INT8U	Type;//����������ͣ��μ�SERVER_TYPE
	INT8U InstanceID;//ʵ��ID��һ��Ϊ0�����ں�̨ͨѶ�����п���Ϊ��0
	INT8U Mode;//ģʽ,�μ�SERVER_MODE
	INT8U DeadCount;//����ֹͣ�Ĵ���������������ֹͣ��������CONS_MAX_DEADCOUNTʱ������Ϊ�˽�������
	PID_T PID;//����ID
	//PID_T PID_LAST_KILLED;//��һ��ɱ���Ľ���ID�����ID�����PIDһ�£���˵���ý����Ѿ����ڽ������߽�ʬ̬���޷����ػ����̻���������Դ�����븴λϵͳ���ܱ�֤����������С�
	time_t LHBTime;//�������ʱ��
	time_t StartSoftTime;//����ʱ�䣬��ʱ��
	time_t StartTrueTime;//����ʱ�䣬��ʱ��

	INT32S ExecTimes;//��ִ�еĴ���,ÿ��forkһ�ξͼ�1
	INT16U HBInterS;//�������
	INT8U Status;//�μ�SERVER_STATUS
	struct TraceSetting TraceSetting;//��������
	INT32U AlrmInterUS;//ʱ�Ӽ��,Ϊ0��ʾ��ֹ
	char TraceFile[CONS_PATH_FILE_MAX_LEN];//�����ļ����ƣ���Ϊ��ʱ���������׼�����������
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

//suxufeng:[2010-3-31]ȥ��work����
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
	INT32U PackageMaxLen;//Ϊ0ʱ���ù�ԼĬ�ϵİ�����
	char Option[CONS_DEVICE_RULE_OPTION_MAX_LEN];	//ֻ�������洢��Լѡ�����ĺ����ɹ�Լ���н���
	
	INT8U Configed;//Ϊ0��ʾδ����

	KLDump& Dump(KLDump& dump) const;

	inline DeviceRuleConfig()
	{
		memset(this,0,sizeof(*this));
	}
};

//��̨ͨѶ���÷���
struct HttxPlan
{
	INT8U Enabled;
	INT8U Index;
	char Key[CONS_CONFIG_PLAN_KEY_MAX_LEN+1];//�ؼ���
	char Name[CONS_CONFIG_PLAN_NAME_MAX_LEN+1];//���ƣ�һ��Ϊ����
	INT8U RuleCount;
	DeviceRuleConfig DeviceRule[CONS_DEVICE_RULE_MAX_COUNT];//֧�ֵ�װ�ù�Լ�����ȵĹ�Լ����ǰ��

	INT8U Configed;//Ϊ0��ʾδ����
	KLDump& Dump(KLDump& dump) const;
	bool IsUsable() const
	{
		return Enabled!=0 && RuleCount>0;
	}

};

//��̨ͨѶ���÷�������
struct HttxPlans
{
	INT16U Count;
	INT16U DefaultIndex;//Ĭ�Ϸ���������
	HttxPlan Item[CONS_HTTX_CONFIG_PLAN_COUNT];

	const HttxPlan* GetDefaultPlan() const;

	KLDump& Dump(KLDump& dump) const;

	void Init();
	const HttxPlan* GetHttxPlan(INT16U planIndex,bool enableFix=true) const;
	const HttxPlan* GetHttxPlan(const char* planKey,bool enableFix=true) const;
};

struct HttxServerInfo
{
	INT8U Master;//�Ƿ���������1Ϊ��0Ϊ��
	INT8U RuleChannelEnabled;//�Ƿ����ʹ��װ�ù�Լ�Դ���ͨ��
	INT8U ChannelCount;
	INT8U Configed;//�Ƿ����ã����Ϊ0��ʾû���������ļ��д���

	INT16U IdleExitTimeoutM;//�˺�̨ͨѶ��������ʱ�䣨ֻҪ���յ����ݾ͵����ǿ��У���Ϊ0ʱ��ʾ�����п����жϣ��������ػ����̼��˺�̨ͨѶ���е�����������ʱ�䣬���������ֵ����δ�յ����ݣ������ػ�����ɱ���˽��̡�	
	INT16U IdleRebootTimeoutM;//�˺�̨ͨѶ��������ʱ�䣨ֻҪ���յ����ݾ͵����ǿ��У���Ϊ0ʱ��ʾ�����п����жϣ��������ػ����̼��˺�̨ͨѶ���е�����������ʱ�䣬���������ֵ����δ�յ����ݣ�ϵͳ������

	INT16U OffLineRequest;//��������
	INT16U ConnectedCount;//�Ѿ������ϵĴ�����ֻҪ����Socket�ɹ�����Ӧ�ý���ֵ��1��
	time_t LastRecvTime;//���һ���յ����ݵ�ʱ��

	char PlanKey[CONS_CONFIG_PLAN_KEY_MAX_LEN+1];//���÷����ؼ���
	struct Channel Channels[CONS_HTTX_CHANNEL_MAX_COUNT];
	INT8U CompressMethod;//ѹ����ʽ���μ�COMPRESS_METHOD
	INT8U EncryptMethod;//���ܷ�ʽ���μ� ENCRYPT_METHOD

	INT8U CurPPPNeedLevel;//��ǰ��PPP��������ȼ�
	INT8U EventCheckFlag;//��0ʱ��ʾ��Ҫ����Ƿ�����Ҫ�ϱ����¼�

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
	INT8U GatherStatus;//�ɼ�״̬���μ�GATHER_STATUS
	INT8U Configed;//�Ƿ����ã����Ϊ0��ʾû���������ļ��д���
	struct ServerInfo BaseInfo;

	//2009-03-11 �¹� ����ͨ��4=����
	//struct Channel Channel[3];
	//100302 cgh ͳһ�˿ں�
	inline int Get485Port1()
	{
		return /*GATHER_PORT_RS485I;*/Channel[0].Setting.Com.PortNo;//hzm,100303
	}

	//100302 cgh ͳһ�˿ں�
	inline int Get485Port2()
	{
		return /*GATHER_VPORT_RS485II;*/Channel[1].Setting.Com.PortNo;//hzm,100303
	}
    
	//100302 cgh ͳһ�˿ں�
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

	//2009-03-11 �¹� ����ͨ��4=����
	//struct Channel Channel[3];
	//100302 cgh ͳһ�˿ں�
	inline int Get485Port1()
	{
		return /*GATHER_PORT_RS485I;*/Channel[0].Setting.Com.PortNo;//hzm,100303
	}

	//100302 cgh ͳһ�˿ں�
	inline int Get485Port2()
	{
		return /*GATHER_VPORT_RS485II;*/Channel[1].Setting.Com.PortNo;//hzm,100303
	}
    
	//100302 cgh ͳһ�˿ں�
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

	INT8U Inactive;//��0��ʾPPP��ʱ�����в��ţ����ڹ���״̬������PPP�����Կ�����Ч���������ģʽʱ�п��ܽ���PPP���ţ�
	INT8U OnLine;//��0ʱ��ʾ����

	INT32U RecvPackages;//���հ���Ŀ
	INT32U SendPackages;//���Ͱ���Ŀ
	INT32U TotalRecvPackages;//�ܵĽ��հ���Ŀ
	INT32U TotalSendPackages;//�ܵķ��Ͱ���Ŀ
	INT32U OffLineTimes;//�ܵ��ߴ���������̨ͨѶ�����ã��жϴ�ֵ�Ƿ�仯������б仯����̨ͨѶ�����п�����Ҫ���м�����������
	INT32U DialTimes;//�ܲ��Ŵ���
	time_t LastDialTime;//���һ�β���ʱ��
	time_t LastRecvTime;//���һ���յ����ݰ���ʱ��
	time_t LastRecvSoftTime;//���һ�ν��յ����ݰ�ʱ��ʱ�䣨��ʱ�ӣ�
	time_t PackageStatInitTime;//��ͳ�ƵĿ�ʼʱ�䣨TotalRecvPackages�Լ�TotalSendPackages�ǴӴ�ʱ��ʼ������)
	INT16S Signal;
	struct ServerInfo BaseInfo;
public:

	//Lcdͨ�����ô˷�������ͳ����ʼʱ�临λ
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

//2006-6-10������������
struct JilianServerInfo
{
	//bool JilianMasterMod;//�������̹���ģʽ��
	INT8U Configed;//�Ƿ����ã����Ϊ0��ʾû���������ļ��д���
	struct ServerInfo BaseInfo;
	struct Channel Channel;
public:
	void Init(INT8U instanceID);
	KLDump& Dump(KLDump& dump) const;
};
struct ServerInfos
{
	ExeInfo Exe;
	DaemonServerInfo Daemon;//�ػ�����
	//suxufeng:[2010-3-31]ȥ��work����
	//WorkServerInfo Work;//�������̣���ԭ�е��������(Ϊ�˲���ϵͳ���������ͻ,�ʸ���Ϊ��������)
	HttxServerInfo Httx[CONS_HTTX_MAX_COUNT];//��̨ͨѶ����
	DbcjServerInfo Dbcj;//���ͨѶ����
	JilianServerInfo Jilian[CONS_JILIAN_MAX_COUNT];//���Ӽ������̣�2006-6-11������������
	PPPServerInfo PPP;
	LcdServerInfo  Lcd;//Һ������
	ScanfServerInfo Scanf;//���ͨѶ����
	//suxufeng:[2010-3-31]ȥ��wrok����
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

