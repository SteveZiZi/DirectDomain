///*װ�ù�Լ*/

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
	bool loged;//�Ƿ��Ѿ���½
	//bool b_nedlog;
	INT32U m_LogOnTimes;

public:
	bool timeout()
	{
		if(log_time==0)
		    return false;
		    
		time_t timeNow = Server::GetCurServer()->GetTickSecond();
		//time(&timeNow);
		return (datediff(1,log_time,timeNow)>msecond ? true : false); // ��ʱ���
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
	bstrings SendDatas;//��Ҫ���͵İ�����Ӧ��վ����ʱ��������Щ��
	bstrings ReportDatas;//��Ҫ�����ϱ��İ����Ƚ��ȳ�ԭ���͡�����������һ���Խ���ȫ�����꣬���Ӽ���������ÿ���յ������������֮����ȥ��飬��Ҫȷ�ϵ�֡������DataToReSend����
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
		    
		return (datediff(2,m_FirstSendTime,time(NULL)) > 30)?true:false;//�������е�������30����û�ж���֮������
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
		m_FirstSendTime = 0;//��ʼ�����жϳ�ʱ
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
	//����ϵͳ��ʼ��ʱ�����ô˷���
	virtual int OnSysInit();

	inline void SetInstanceServerID(int iiInstanceID)
	{
		InstanceServerID = iiInstanceID;
	}

	//��ȡװ�ù�Լ������Ϣ
	virtual int GetRuleBaseInfo(DeviceRuleInfo& oRuleInfo)=0;

	////suxufeng:[2010-3-25]�յ���Ϣ��Ĵ�����̡�
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

    //���ճ�ʱ
	void OnRecvTimeoutMs(Connection& irConnection);

	//��������ʱ����
	int OnReceiveData(Connection& irConnection,bstring& irRecvData);

	//��������ʱ����
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
	//�������ͺ���
	virtual void OnPackageSended(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irSendedData);

	//�������պ���
	virtual void OnPackageReceived(Connection& irConnection,bstring& irReceivedData);

	//���Ͱ���ȷ������
	virtual void OnPackageConfirmed(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irConfirmedData);

	//���Ͱ�δ��ȷ������
	virtual void OnPackageNotConfirmed(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irNotConfirmedData);

	//ȷ�����Ͱ�������0��ʾȷ�ϳɹ�
	virtual int OnConfirmPackage(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irDataForConfirm);

	//��ȡ���Ͱ�
	virtual int GetReportPackage(Connection& irConnection,bstrings& orSendDatas,bstring& orConfirmData,int& irMaxSendTime,INT32U iiPackageMaxLen,bool bForceCheck);

	//��ȡӦ���
	virtual int GetReplyPackage(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen)=0;

	//�������հ�
	virtual PARSE_RESULT ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd)=0;
public:	
	//��½��վ   2006-6-20��־ǿ+
	virtual int logon(Connection& m_pconnection);
	//�ϱ�������Ϣ   2006-6-20��־ǿ+
	virtual int ReportAlarm(Connection& m_pconnection);

	//���ڼ�鳬ʱ  2006-6-28��־ǿ+
	virtual void OnTickRule(Connection& m_pconnection);
	//�������ڴ����������ϱ�������  2006-6-28��־ǿ+
	virtual int JilianReports(Connection& m_pconnection);
	//���͸���������
	virtual int SendToJilian(Connection& irConnection,bstring& irRequestData);
	virtual int SendToJilian(bstring &irRequestData,bstrings &orRecvPackage);

	//�����ϱ�����
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


