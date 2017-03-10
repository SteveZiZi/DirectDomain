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


/*通道的默认设置信息*/
struct BusRuleInfo
{
	INT8U BusNo;//通道No
	INT8U CommID;
	char Name[CONS_RULE_NAME_MAX_LEN+1];//表规约名
	struct PortSet DefPortSet;//默认端口设置


	INT32U CharReadTimeoutMs;//字符超时
	INT32U WriteTimeoutMS;//默认的写超时
	INT32U ReadTimeoutMS;//默认的读超时
	INT32U SubReadTimeoutMS;//尝试的连续读超时

	INT16U DefMinCommInterS;//默认的两次通讯之间的最小间隔
	INT16U DefCommRetryTimes;//默认的通讯重试次数

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


/*BusChannelsRule::GetMeterData（）包含了组包、发送、接收、解包等函数*/
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
	virtual int TestRelay(Relay &relay, int maxtimes,int CanFailTimes,char XWFlag=2){return 0;}//适用于无需指定终端
	virtual int CallTerminalNode(Relay &relay, INT8U times, INT8U &sucessTimes){return 0;} //载波从节点点名

	virtual int GetDebugData(INT16U DataID,const bstring& data,bstring& sendData) {return -1;}//处理路由调试任务
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
	*@brief 对表规约的数据进行组包，该组包方式由通讯介质决定，不同的通讯介质有不同的组包方式
	*@param [in] iCtrFlag 数据包中的控制码（控制码不同 数据包的功能不同）
	*@param [in] iiKey 透传若控制码为0X00 则需要根据控制码来确定组网方式,否则无效
	*/
	virtual int MakePackage(Meter& imeter,const unsigned char* idata,int idataLen,INT8U iCtrlFlag,bstring& oSendPackage,bstring* iaddr=NULL){return 0;};

	/** 
	*@brief 要考虑多帧的情况，当通道层数据全部接收完毕后，才到电表规约去解析
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

