///*装置规约*/

#ifndef INCLUDED_DEVICE_RULE
#define INCLUDED_DEVICE_RULE

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../share/SysBase.h"
#endif

#ifndef INCLUDED_DEVICE_RULE_INFO
#include "../share/DeviceRuleInfo.h"
#endif

#include "Context.h"


class Server;
class Connection;
struct HttxPlan;
struct DeviceRuleConfig;


struct loged_on
{
	time_t log_time;
	int msecond;
	bool loged;//是否已经登陆
	//bool b_nedlog;
	INT32U m_LogOnTimes;

public:
	bool timeout()
	{
		if(log_time==0)
		    return false;
		    
		time_t timeNow = Server::GetCurServer()->GetTickSecond();
		//time(&timeNow);
		return (datediff(1,log_time,timeNow)>msecond ? true : false); // 超时与否
	}
	void init()
	{
		log_time=0;
		//#ifdef WIN32
		//loged=true;
		//#else
		loged=false;
		//#endif
		DECLARE_CONTEXT(context);
		if (context.gprsInfo.EnableFlowCtrl)
		{
			msecond = context.gprsInfo.LogOnInterS;
		}
		else
		{
			msecond=LOG_ON_TIME_OUT; 
		}
		m_LogOnTimes = 0;
	}
};

struct DeviceRuleDataBuff
{
	bstrings SendDatas;//需要发送的包，响应主站命令时，发送这些包
	bstrings ReportDatas;//需要主动上报的包，先进先出原则发送。主集中器将一次性将其全部发完，而从集中器将在每次收到传输控制命令之后再去检查，需要确认的帧将存在DataToReSend里面
	bstring ConfirmData;
	bstring NeedConfirmPackage;
	int MaxSendTimes;
	int SendTimes;
	time_t LastSendTime;
	bool ConfirmValid;
	bool JilianControlReceived;
	time_t m_FirstSendTime;

	inline DeviceRuleDataBuff()
	{
		Init();
	}
	bool IsTimeOut()
	{
		if(m_FirstSendTime == 0)
		    return false;
		    
		return (datediff(2,m_FirstSendTime,time(NULL)) > 30)?true:false;//缓冲区中的数据在30分钟没有动静之后就清空
	}
	inline void ResetTimeout()
	{
		m_FirstSendTime = time(NULL);
	}
	inline void DisableTimeout()
	{
		m_FirstSendTime = 0;
	}
	void Init()
	{
		SendDatas.clear();
		ReportDatas.clear();
		ConfirmData.clear();
		NeedConfirmPackage.clear();
		MaxSendTimes=3;
		SendTimes=0;
		m_FirstSendTime = 0;//初始化不判断超时
		ConfirmValid=false;
		JilianControlReceived = true;
	}
	void ClearBuffer()
	{
		DisableTimeout();
		Init();
	}
};

class DeviceRule
{
public:
	DeviceRule();
	virtual ~DeviceRule()
	{
		m_pRuleInfo=NULL;
		m_pServer=NULL;
	};
public:
	//整个系统初始化时将调用此方法
	virtual int OnSysInit();

	inline void SetInstanceServerID(int iiInstanceID)
	{
		InstanceServerID = iiInstanceID;
	}

	//获取装置规约基础信息
	virtual int GetRuleBaseInfo(DeviceRuleInfo& oRuleInfo)=0;

	////suxufeng:[2010-3-25]收到消息后的处理过程。
	//virtual void OnReceivedMessage(IpcInfo &iiMessage);

	virtual void Init(Server& irServer,Connection& irConnection,const HttxPlan& plan,const DeviceRuleConfig& config);

	virtual bool PreConnect(Connection& irConnection);
	virtual void PostConnect(Connection& irConnection);
	virtual bool PreDisconnect(Connection& irConnection);
	virtual void PostDisconnect(Connection& irConnection);

	virtual int OnNotify(INT32U iiNotifyID,INT32S iiObjID,void* ipOther);
	virtual void Make_Package(bstring& reply,bstring& reqst,INT8U ctrlCode);
	virtual bool OnSendData(bstrings& orReplyDatas);
	virtual bool IsJilianHttx();

    //接收超时
	void OnRecvTimeoutMs(Connection& irConnection);

	//接收数据时处理
	int OnReceiveData(Connection& irConnection,bstring& irRecvData);

	//上送数据时处理
	void OnReportData(Connection& irConnection,bool ibForceCheck = false);

	inline void SetDeviceRuleInfo(DeviceRuleInfo* pRuleInfo)
	{
		m_pRuleInfo=pRuleInfo;
	}
	inline DeviceRuleInfo* GetDeviceRuleInfo() const
	{
		ASSERT(m_pRuleInfo);
		return m_pRuleInfo;
	}
	inline INT32U GetPackageMaxLen() const
	{
		ASSERT(m_pRuleInfo);
		return m_pRuleInfo->PackageMaxLen;
	}
	const DeviceRuleConfig& GetConfig() const;
	virtual size_t SendData(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstrings &irSendDatas);
protected:
	//包被发送后处理
	virtual void OnPackageSended(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irSendedData);

	//包被接收后处理
	virtual void OnPackageReceived(Connection& irConnection,bstring& irReceivedData);

	//上送包被确定后处理
	virtual void OnPackageConfirmed(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irConfirmedData);

	//上送包未被确定后处理
	virtual void OnPackageNotConfirmed(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irNotConfirmedData);

	//确认上送包，返回0表示确认成功
	virtual int OnConfirmPackage(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irDataForConfirm);

	//获取上送包
	virtual int GetReportPackage(Connection& irConnection,bstrings& orSendDatas,bstring& orConfirmData,int& irMaxSendTime,INT32U iiPackageMaxLen,bool bForceCheck);

	//获取应答包
	virtual int GetReplyPackage(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen)=0;

	//解析接收包
	virtual PARSE_RESULT ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd)=0;
public:	
	//登陆主站   2006-6-20刘志强+
	virtual int logon(Connection& m_pconnection);
	//上报报警信息   2006-6-20刘志强+
	virtual int ReportAlarm(Connection& m_pconnection);

	//用于检查超时  2006-6-28刘志强+
	virtual void OnTickRule(Connection& m_pconnection);
	//用于用于处理级联进程上报的数据  2006-6-28刘志强+
	virtual int JilianReports(Connection& m_pconnection);
	//发送给级联进程
	virtual int SendToJilian(Connection& irConnection,bstring& irRequestData);
	virtual int SendToJilian(bstring &irRequestData,bstrings &orRecvPackage);

	//主动上报函数
	virtual int AutoReport_MonthData(Connection& irConnection);
	virtual int AutoReport_DayData(Connection& irConnection);
	virtual int AutoReport_VipData(Connection& irConnection);
	
	virtual bool HeartBeating(Connection& m_pconnection)=0;
//#ifdef WIN32
//	virtual int test();
//#endif
	static INT8U GetFrameCount();
public:
	bstrings m_bssJilianBuff;
	DeviceRuleDataBuff m_SendInfo;
	struct loged_on log_status;
	int InstanceServerID;

protected:
	DeviceRuleInfo* m_pRuleInfo;
	Server* m_pServer;
	INT32U m_iPackageMaxLen;
	const DeviceRuleConfig* m_pConfig;
private:
	bstring m_sParsedBuff;
	bool bInReport;
};

#endif


