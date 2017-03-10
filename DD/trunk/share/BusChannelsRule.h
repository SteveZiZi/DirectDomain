#ifndef INCLUDED_BUSCHANNELSRULE
#define INCLUDED_BUSCHANNELSRULE


#include "../include/StdInclude.h"
#include "../include/Config.h"
#include "../base/Timer.h"
#include "SysBase.h"
#include "../base/PortSet.h"
#include "meter.h"


#define CONS_BUS_RULE_MAX_COUNT 32

class Connection;
struct Meter;
struct Relay;
class GDBusRuleFactory;


/*ͨ����Ĭ��������Ϣ*/
struct BusRuleInfo
{
	INT8U BusNo;//ͨ��No
	INT8U CommID;
	char Name[CONS_RULE_NAME_MAX_LEN+1];//���Լ��
	struct PortSet DefPortSet;//Ĭ�϶˿�����


	INT32U CharReadTimeoutMs;//�ַ���ʱ
	INT32U WriteTimeoutMS;//Ĭ�ϵ�д��ʱ
	INT32U ReadTimeoutMS;//Ĭ�ϵĶ���ʱ
	INT32U SubReadTimeoutMS;//���Ե���������ʱ

	INT16U DefMinCommInterS;//Ĭ�ϵ�����ͨѶ֮�����С���
	INT16U DefCommRetryTimes;//Ĭ�ϵ�ͨѶ���Դ���

};

struct BusRuleInfos
{
	INT32U Count;
	BusRuleInfo bruleInfos[CONS_BUS_RULE_MAX_COUNT];

public:

	inline BusRuleInfos()
	{
		Init();
	}
	inline void Init()
	{
		memset(this,0,sizeof(*this));
	}

};


class BusRuleIntercept
{
public:
	virtual bool OnGatherTick()
	{
		return true;
	}
};


/*BusChannelsRule::GetMeterData������������������͡����ա�����Ⱥ���*/
class BusChannelsRule
{
	friend class GDBusRuleFactory;
	friend class CommRule;
public:
	BusChannelsRule() {}
	BusChannelsRule(Connection* conn)
	{
		b_conn=conn;
	}

	inline virtual  ~BusChannelsRule() {}



	static void SetIntercept(BusRuleIntercept* pIntercept)
	{
		b_pIntercept=pIntercept;
	}   

	void GetTimeOutMS(Meter& meter, const int &iiTimeoutMS,int &ooTimeoutMS);
	void GetBusRuleInfo(BusRuleInfo &busruleinfo)
	{
		busruleinfo = b_RuleInfo;
	}

	int GetData(Meter &meter,bstring &oretData);
	int ChangeConnConfig(const PortSet &iporset);

	virtual int UpdateRelay(const INT8U &opcode,Relay &oldRelay,Relay &newRelay){return 0;}
	virtual int UpdateRouter(const INT8U &opcode,const INT16U &routerNo){return 0;}

	virtual void OnSysInit(){}
	virtual int OnExecuteRouter() {return -1;}
	virtual int OnAcitveRouter(){return -1;}
	virtual bool IsRouterActived(){return false;}
	virtual int TestRelay(Relay &relay, int maxtimes,int CanFailTimes,char XWFlag=2){return 0;}//����������ָ���ն�
	virtual int CallTerminalNode(Relay &relay, INT8U times, INT8U &sucessTimes){return 0;} //�ز��ӽڵ����

	virtual int GetDebugData(INT16U DataID,const bstring& data,bstring& sendData) {return -1;}//����·�ɵ�������
	virtual int GetMeterData(Meter& imeter,const unsigned char* idata,int idataLen,bstring& opReturnData,bstring* iaddr=NULL,const int &iiTimesOuts=0) = 0;
	
	virtual int  GetDeviceData(Device& device,const unsigned char* data,int dataLen,bstring& opReturnData,bstring* iaddr=NULL,const int &iiTimesOuts=0)=0; 
	virtual int  GetRtfData(Device& device,const unsigned char* data,int dataLen,bstring& opReturnData,bstring* iaddr=NULL,const int &iiTimesOuts=0)=0; 
	
	virtual bool SendData(Meter& meter,const unsigned char* data,int dataLen,bstring &retPackage,bstring* iaddr =NULL) {return 0;}
	virtual int BroadCastData(Meter &meter, INT8U *data, const INT8U &dataLen) {return 0;}
	virtual int GetMasterChipInfo(bstring &oChipInfo){return 0;}
	virtual void UpdateRecord(INT16U router_no,bool is_succ)
	{
		return;
	}

protected:
	virtual void InitRuleBaseInfo()=0;
	virtual int  SetRouterStrategy();
	/*virtual int ReceiveData(unsigned char *recvData, int icount,int timesout);*/
	virtual int GetNextFrameData(Meter &meter, INT8U &FraemCnt,INT8U &FrameNo);
	virtual bool SendPackage(Meter& imeter,const unsigned char* idata,int idataLen,INT8U iCtrlFlag,bstring& oSendPackage,bstring* iaddr=NULL)=0;
	/**
	*@file BusChannelsRule.h
	*@function virtual int MakePackage(Meter& imeter,const unsigned char* idata,int idataLen,INT8U iCtrlFlag,const INT8U &iiKey,bstring& oSendPackage,bstring* iaddr=NULL)=0;
	*@brief �Ա��Լ�����ݽ���������������ʽ��ͨѶ���ʾ�������ͬ��ͨѶ�����в�ͬ�������ʽ
	*@param [in] iCtrFlag ���ݰ��еĿ����루�����벻ͬ ���ݰ��Ĺ��ܲ�ͬ��
	*@param [in] iiKey ͸����������Ϊ0X00 ����Ҫ���ݿ�������ȷ��������ʽ,������Ч
	*/
	virtual int MakePackage(Meter& imeter,const unsigned char* idata,int idataLen,INT8U iCtrlFlag,bstring& oSendPackage,bstring* iaddr=NULL){return 0;};

	/** 
	*@brief Ҫ���Ƕ�֡���������ͨ��������ȫ��������Ϻ󣬲ŵ�����Լȥ����
	*/
	virtual  PARSE_RESULT ParsePackage(Meter& meter,bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& irSendData,bstring& opReturnData){return PARSE_RESULT_ERROR_PACKAGE;};
	virtual PARSE_RESULT ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& irSendData,bstring& opReturnData){return PARSE_RESULT_ERROR_PACKAGE;};
	virtual PARSE_RESULT ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& opReturnData){return PARSE_RESULT_ERROR_PACKAGE;}
	virtual PARSE_RESULT ParseRtfDataPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& opReturnData){return PARSE_RESULT_ERROR_PACKAGE;}
	virtual PARSE_RESULT ParseModbusPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,INT8U key,bstring& opReturnData){return PARSE_RESULT_ERROR_PACKAGE;}
	virtual int GetTimeOutMS(Meter& meter) = 0;

	int ConnectionConfig();
	bool BeforeConnection();
	int ChangeConenction(Connection *newconn);
	int OnReceiveData(Meter& meter,bstring& irSendData,bstring& opReturnData,const int &iiTimesOuts=0,bstring *psecndPackge=NULL);
	int OnReceiveMeterData(Meter& meter,bstring& irSendData,bstring& opReturnData,INT8U &FrameCnt,INT8U &FrameNo,const int &iiTimeoutMS,bstring *psecndPackge=NULL);
	int OnReceiveDeviceData(Device& device,bstring& opReturnData);
	int OnReceiveRtfData(Device& device,bstring& opReturnData);
	static bool OnGatherTick();

public:
	BusRuleInfo b_RuleInfo;

protected:
	Connection* b_conn;
	static BusRuleIntercept* b_pIntercept;	
};

#endif

