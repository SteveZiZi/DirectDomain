///*电表规约*/

#ifndef INCLUDED_METER_RULE
#define INCLUDED_METER_RULE

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_TIMER
#include "../base/Timer.h"
#endif

#ifndef INCLUDED_METER
#include "../share/Meter.h"
#endif




struct DataBitSet;
struct PortSet;
struct Meter;
struct RegID;
class RegItem;
class RegItems;

struct MeterRuleInfo;

struct RegIDS;

//class SetRegBacks;

;
class KLDump;
struct MeterDataItem;
class CommRule;

class MeterRule
{
	/*friend class GatherRule;*/
public:
	//inline MeterRule():
	inline MeterRule():m_pRuleInfo(NULL),CanLogonFlag(false)
	{
	};
	virtual ~MeterRule(){};

	int ParseMeterPackage(Meter& meter,int iiKey,bstring& irSendData,bstring* opReturnData,bstring &ipMeterBuff);
	int ParseMeterPackage(Meter& meter,int iiKey,bstring& irSendData,int* opReturnKey,bstring* opReturnData,bstring &ipMeterBuff);
	//整个系统初始化时将调用此方法
	virtual int OnSysInit();

	//获取电表规约基础信息
	virtual int GetRuleBaseInfo(MeterRuleInfo& oRuleInfo)=0;

	virtual int GetSetTimeMethodInfo(Meter& meter,MeterSetTimeMethodInfo& oMethodInfo);


	//获取规约支持信息
	virtual int GetRuleSupportInfo(RegIDS& iRegIDS, MeterRuleInfo& oRuleInfo)=0;	

	virtual bool IsBatchReadEnabled() {return false;}
	virtual bool IsSetBrakeEnabled() {return false;}
	virtual bool IsGetStateEnabled() {return false;}
	virtual bool IsGetEventEnabled() {return false;}


	virtual MeterDataItem * FindItem(INT16U GBID) {return NULL;}
	virtual int GetModeKey(const INT8U &ackType ) {return -1;}



	//virtual int GetLibCfgPathFile(Meter& meter,INT8U dataType,char* opPathFile);
	virtual int MeterChngAddrPackage(Meter &meter,const bstring &oldPackage, bstring & oPackage) { oPackage.assign(oldPackage);return -1;}

	virtual MeterDataItem *FindItem(INT32U regID) {return NULL;}

	//获取登陆数据包
	virtual int MakeLogonPackage(Meter& meter,bstring& oPackage);

	virtual void GetMeterPwd(Meter& meter,const char *ipSrc,bstring& oData){}

	//获取退出登陆数据包
	virtual int MakeLogoutPackage(Meter& meter,bstring& oPackage);


	// 0 getDayData Item, 根据采集的命令类型获取采集的ID号和返回数据长度等信息
	virtual int MakeRuleItem(Meter &meter,const int & regID,MeterDataItem *&DataItem);

	virtual bool GetMeter_Status(Meter& meter,unsigned short *o_state){return false;}
	
	virtual void SetRegItems(Meter &meter, MeterDataItem &DataItem,const int &ItemLen,bstring &irecvData,RegItems &oOkReuslts)
	{
		KL_VERBOSE( "调用虚函数\n");
	}
	virtual int SetRegItems(Meter &meter,DataBitSet& dbs,bstring& Data,RegItems& orResultDatas,DataBitSet& oFailedDBS){return -1;}

	virtual int TransRegItem(Meter &meter, int klId, bstring &Data,bstring& retData) {return 0;}

	virtual 	int MakeAckPackage(Meter& meter,bstring& oPackage, const  INT8U &ackType);

	bool MakePwdCheckPackage(Meter &Meter,bstring &sendData) {return true;}
	virtual int ConvertReadPackage(Meter &meter,bstring &receivedPackage, bstring &oparsedData,unsigned short mapID=0);

	//采集单个数据项获取采表数据包
	virtual int MakePackage(Meter& meter,INT16U &RegID,bstring& oPackage,bool broadcast = false);

	//采集多个数据项获取采表数据包	
	virtual int MakePackage(Meter& meter,DataBitSet& iDBS, bstring& oPackage);

	//适用与写电表的能量寄存器，iData和iDataLen表示写入电表的数据  
	virtual int MakePackage(Meter& imeter,INT16U &iMapID, INT8U* iData,INT8U iDataLen,bstring& oPackage,bool broadcast=false);


	//适应于电表数据标示为字符串形式(LGR电表)
	virtual int MakePackage(Meter &meter, bstring &DataItem,const INT8U *datas, const INT8U &dataLen, bstring &oSendData);

	//特殊表的单个能量寄存器的读取
	virtual int MakePackage(Meter& meter,bstring &RegID,bstring& oPackage,bool broadcast = false);
	virtual int MakePackage(Meter& meter,INT8U control, const unsigned char* data,int dataLen,bstring& oPackage,bool broadcast=false)
	{
		return -1;
	}

	//获取广播校时数据包
	virtual int MakeBroadcastSetTimePackage(bstring& oPackage);


	virtual PARSE_RESULT ParsePackage(Meter& meter,bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,int iiKey,bstring& irSendData,bstring* opReturnData){return PARSE_RESULT_ERROR_PACKAGE;}

	virtual PARSE_RESULT ParsePackage(Meter& meter,bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,int iiKey,bstring& irSendData,int* opReturnKey,bstring* opReturnData){return PARSE_RESULT_ERROR_PACKAGE;}		   

	virtual GATHER_RESULT GetReg(Meter& meter,INT32U mapID,bstring& orData,const int& maxTimes=2){return GATHER_RESULT_NOT_GATHERED;}

	virtual GATHER_RESULT GetRegs(Meter& meter,DataBitSet& dbs,RegItems& oOkResults,DataBitSet& oFailedDBS,DataBitSet& oNotSupportDBS){return GATHER_RESULT_NOT_GATHERED;}

	virtual GATHER_RESULT GetRegs(Meter &meter, DataBitSet &dbs,  bstring& orData, const INT8U & MaxTimes){return GATHER_RESULT_NOT_GATHERED;}

	virtual GATHER_RESULT GetTime(Meter& meter,time_t& oTime){return GATHER_RESULT_NOT_GATHERED;}

	virtual GATHER_RESULT GetDate(Meter& meter,char * date_4){return GATHER_RESULT_NOT_GATHERED;}

	virtual int SetBrake(Meter& imeter,bstring& ibstr_Command){return -1;}

	inline void SetMeterRuleInfo(MeterRuleInfo* pRuleInfo)
	{
		m_pRuleInfo=pRuleInfo;
	}
	inline MeterRuleInfo* GetMeterRuleInfo() const
	{
		ASSERT(m_pRuleInfo);
		return m_pRuleInfo;
	}
	inline bool OnGatherTick()
	{
		/*if (m_pIntercept) 
		{
		return m_pIntercept->OnGatherTick();
		}*/
		return true;
	}
	virtual GATHER_RESULT BaseGatherData(Meter& meter,bstring& parseData,bstring& recvData,int iiTimeoutMS,int iiKey,int& opReturnKey){return GATHER_RESULT_NOT_GATHERED;}
	virtual int OnReceiveData(Meter& meter,int iiTimeoutMS,int iiKey,bstring& irSendData,int* opReturnKey,bstring* opReturnData){return 0;}
protected:
	int GetCfgPathFile(Meter& meter,INT8U dataType,const char* ruleKey,char* opPathFile);
	time_t m_tLastBroadcastSetTimeTime;//最近一次广播对时时间，避免在短时间内多次发送广播对时命令
	//!为了便于调试改变属性protected 为public
public:
	MeterRuleInfo* m_pRuleInfo;
	bool CanLogonFlag;
	PortSet m_PortSetChanged;
	enum ProtolcolEnum ProtolcolType;
	CommRule *comm;
};

#endif

