///**/
#ifndef INCLUDED_DYJC_RULE
#define INCLUDED_DYJC_RULE

#ifndef INCLUDED_TASK_DATA_FILE
#include "../../share/TaskDataFile.h"
#endif

#ifndef INCLUDED_REG_ITEM
#include "../../share/RegItem.h"
#endif

#ifndef INCLUDED_REG_FILE
#include "../../share/RegFile.h"
#endif

#ifndef INCLUDED_TASK
#include "../../share/Task.h"
#endif

#ifndef INCLUDED_STATUS
#include "../../share/Status.h"
#endif

#ifndef	INCLUDED_GDDYJC_DATA
#include "GDDYJCData.h"
#endif

#ifndef INCLUDED_FILE_DIR
#include "../../include/FileDir.h"
#endif

#ifndef INCLUDED_FILE_BASE
#include "../../base/FileBase.h"
#endif

#ifndef INCLUDED_STRING
#include "../../base/String.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../../share/SysBase.h"
#endif

#ifndef INCLUDED_DEVICE_RULE
#include "../../share/DeviceRule.h"
#endif

#ifndef INCLUDED_DEVICE_RULE_INFO
#include "../../share/DeviceRuleInfo.h"
#endif

#ifndef INCLUDED_CONNECTION
#include "../../share/Connection.h"
#endif

#define GDDYJC_RULE_NAME "低压集抄"

#ifndef INCLUDED_CONTEXT
#include "../../share/Context.h"
#endif

#ifndef INCLUDED_GDDYJC_DATA
#include "GDDYJCData.h" 
#endif

#ifndef INCLUDED_GDDYJC_RULE_FUNCTION
#include "GDDYJCRuleFunction.h"
#endif

#ifndef INCLUDED_METER_CFG
#include "../../config/MeterCfg.h"
#endif

#ifndef INCLUDED_ALARM_FILE
#include "../../share/AlarmFile.h"
#endif

#ifndef INCLUDED_SYS_IPC
#include "../../share/SysIpc.h"
#endif

#ifndef INCLUDED_IPC_INFO
#include "../../base/IpcInfo.h"
#endif

#ifndef	INCLUDED_CONTEXT_CFG
#include "../../config/ContextCfg.h"
#endif

#define CONS_LOGON_HEARTBEAT_MSTA 255

#define CONST_HTTX_TO_JILIAN_PACKAGES_BUFFER_SIZE 20

#ifndef INCLUDED_GDDYJC_STRUCT_DATA
#include "GDDYJCRule_struct.h"
#endif

#ifndef INCLUDED_GDDYJC_SET_SETTINGS
#include "GDDYJCRule_set_settings.h"
#endif

#ifndef INCLUDED_GDDYJC_STATUS
#include "GddyjcStatus.h"
#endif



#define CONS_FIX_DATE_LABEL "FixDateLabel"

typedef enum _GDDYJC_RULE_MESSAGE_TYPE
{
	GDDYJC_RULE_MESSAGE_TYPE_REAL_GATHER = 1,//suxufeng:[2010-3-25]实时召测
	GDDYJC_RULE_MESSAGE_TYPE_CANCEL_REAL_GATHER = 2,//suxufeng:[2010-3-25]取消实时召测
	GDDYJC_RULE_MESSAGE_TYPE_METER_OPEN_CLOSE = 3,//suxufeng:[2010-3-25]实时召测
	GDDYJC_RULE_MESSAGE_TYPE_DEBUG_COMMAND = 4,//suxufeng:[2010-3-25]实时召测
}GDDYJC_RULE_MESSAGE_TYPE;

class GDDYJCRule:public DeviceRule
{

private:
	//带帧序号的方式 from=1表示从主站下的帧中取MSE&SEQ,0表示从本地取MSE&SEQ
	unsigned char AddRepHead(bstring& rep,bstring& irRequestData,INT8U ctl,unsigned char& frame_count,int _1_from_request_0_from_config=1);
	bool AddRepHead(bstring& rep,bstring& irRequestData,INT8U ctl,int _1_from_request_0_from_config=1);
	bool AddRepFeed(bstring& rep,INT16U len=0);
	unsigned char AddRepFeedWithMCount(bstring& rep,bool need_count=false,INT8U m_count=0,INT16U len=0);
	unsigned char MakeRepLastFrame(bstrings & bs);
	bool IsMainDJS();
	bool IsJilianHttx();
public:
	GDDYJCRule();
	~GDDYJCRule(void);
	int GetRuleBaseInfo(DeviceRuleInfo& oRuleInfo);
	//void OnReceivedMessage(IpcInfo &iiMessage);
	//int StatPackages(bstring &irRecvDatas,bstrings &orRecvPackages);
	PARSE_RESULT ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int ParsePackageBase(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
    int ParseCompressOrEncryptPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int GetReplyPackage(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int AdjustLinkAddr(bstring& irRequestData,int addr=1);
	int GetReportPackage(Connection& irConnection,bstrings& orSendDatas,bstring& orConfirmData,int& irMaxSendTime,INT32U iiPackageMaxLen,bool bForceCheck);
	void OnPackageConfirmed(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irConfirmedData);//受到确认包后
	void Make_Package(bstring& reply,bstring& reqst,INT8U ctrlCode);
	int CheckRtuaAddr(const unsigned char *addr);

//协议处理函数
public:
	//写对象参数
	int SetDJSsetings(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen,INT8U m_iCtrl=0x88);
	//suxufeng:[2010-3-24]设置参数命令的公用部分，由SetDJSsettings和SetRealTimeSetting共用。
	int SetSettings(Connection &irConnection, int &i, INT16U &DataLen, bstring &irRequestData, bstring::iterator &operatorNow, bstring::iterator &operatorEnd, bstring &orAns);
	//短信唤醒
	int WakeUp(bstring& irRequestData);
	//读当前数据
	int RealRead(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//集中器抄收日常综合数据
	int ReadDailyData(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//重发日常数据
	int reReplyDalyData(Connection& irConnection,bstring& irRequestData);
	 //集中器抄收重点户负荷数据
	int ReadVIPData(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//重发重点户
	int reReplyVIP(Connection& irConnection,bstring& irRequestData);
	//拉合闸命令
	int MeterOpenClose(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);   
	//处理没有及时转交给DBCJ进程的分合闸命令
	int DillMeterOpenClose(Connection& irConnection);
	//集中器异常报警,报警确认.
	int DJSAlarmConfirm(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//心跳检测
	bool HeartBeating(Connection& m_pconnection);
	//发送给级联进程
	int SendToJilian(Connection& irConnection,bstring& irRequestData);
	int SendToJilian(bstring &irRequestData,bstrings &orRecvPackage);
	void BroadCastToJilian(bstring &irRequestData);
	bstring ChangeBroadCastPkg2DstPkg(bstring &irRequest, const unsigned char *addr);
	//void OnPackageSended(Connection &irConnection,SendInfo &irSendInfo,bstring &irSendedData);
	//void OnPackageReceived(Connection &irConnection,SendInfo &irSendInfo,bstring &irReceivedData);
	//确认是否为确认返回帧
	bool IsConfirmCtrl(Connection& irConnection,bstring req);
	inline unsigned char GetMSTA(unsigned char ibByte1,unsigned char ibByte2)
	{
		return (ibByte1 & 0x3f);
	}
	inline unsigned char GetFSEQ(unsigned char ibByte1,unsigned char ibByte2)
	{
		return ((ibByte2<<2) &0x7c)+((ibByte1>>6) & 0x03);
	}
	//当连接的时候,主动上报一个登陆帧.在允许主动上报的时候才有用
	int logon(Connection& m_pconnection);
	//主动上报报警信息   2006-6-20刘志强+
	int ReportAlarm(Connection& irConnection,bstrings& orSendDatas,bstring& orConfirmData,int& irMaxSendTime,INT32U iiPackageMaxLen,unsigned char &frame_count);
    //检查上报超时
	void OnTickRule(Connection& m_pconnection);
	//处理级联进程上报
	int JilianReports(Connection& m_pconnection);

	// 压缩函数
	bool DecompressData(bstring& irCompressedData,bstring& irDecompressedData);
	// 发送数据前的处理函数。
	bool OnSendData(bstrings& orReplyDatas);

	//处理主动上报日综合数据和重点户数据
	int AutoReport_MonthData(Connection& irConnection);
	int AutoReport_DayData(Connection& irConnection);
	int AutoReport_VipData(Connection& irConnection);
	bool Check_Time(INT8U ts);

	//处理级联命令
public:
	int Master_DJS_request(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//级联传输控制
	int ReplyJilianControl(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//接收配变终端数据
	int ReplyFromTerminal(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//only for test
	//处理招收电表设置
	int Read_Meter_set(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);

	void OnSystemUpdated();

//#ifdef WIN32
//	int test();
//#endif
private:
	int ChangeMeterAndTerminal(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int ChangeMeter(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int ChangeTerminal(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
//处理升级和文件传输的命令帧，及其应答
	int GetUpdateReply(Connection &irConnection,bstring &irRequestData,bstrings &orReplyDatas,INT32U iiPackageMaxLen);
	int FileTransffer(Connection &irConnection,bstring &irRequestData,bstrings &orReplyDatas,INT32U iiPackageMaxLen);

private:
	//实时招测
	int ReadrealtimeData(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int CancelReadRealTimeData(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int DillReadRealTimeData(Connection& irConnection);
	//实时写对象参数
	int SetRealTimeSetting(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//取当前时间的BCD吗表示方法
	void Get_CurBCDTime5(char* o_cpBcdTime);
	// 集中器收到一个完整报文，并且地址匹配成功之后的处理方式。
	inline void SetMasterHttxOnlineStatus(const bool status = true)
	{
		CONTEXT.MasterHttxOnline = status;
	}
	bool bcddatedifff(char when[7],char now[7],long howlong);
	INT8U GetReplyCtrlCode(INT8U ctrl,int _1_right_0_wrong=1);
	void Abnormity_Frame(Connection& irConnection,bstring& irRequestData,INT8U Ctr_Code,INT8U Abnormity_Code,int _1_from_request_0_from_config=1);
	void Abnormity_Frame(Connection& irConnection,bstring& irRequestData,INT8U Ctr_Code,const bstring& Abnormity_Code,int _1_from_request_0_from_config=1);
	void Abnormity_Fram(Connection& irConnection,bstring& irRequestData,INT8U Ctr_Code,INT16U Abnormity_Code,int _1_from_request_0_from_config=1);
	bool IsNeedGather();//判断是否需要进行采表
	int GetFtsAnswer(bstring &StrDown,bstring &StrFts,bstring &StrUp,bool result);
	int GetFtsData(bstring &strIn,bstring &strOut);//获取文件传输规约内容，即剥离广东省规约的包头包尾，然后直接丢给文件传输规约去完成
	int OnTerminalReset();
	unsigned char GetLastDayOfMonth(const char *iBcdtime);

	INT8U OnFixDataCommand(const bstring &irRequestData);
protected:
	unsigned char oldVersion[8];
	unsigned char newVersion[8];
private:
	struct sRTUA_Struct m_RTUA;
	int iDataLen1,iDataLen2;
	bstring m_bHeartBeat;
	byte bMSTA;
	byte bFSEQ;
	byte bISEQ;
	INT8U m_iCtrl;
	INT16U Msta;
	byte seq;
	int curMonth;//抄月末数据时的当前位置
	int curDay;//抄日末数据时的当前位置
	INT8U curFrame;//当前帧是第几帧
	int m_Tn;
private:
	unsigned char m_ctrlfiled,m_cLastCtrlField,m_cTypeID,m_cVSQ,m_cCOT;
	//m_Meter m_meter[4];
	unsigned char My_Ip[4];
	GDDYJCData m_WriteSettings;
	bool NeedGatherSlaveTerminal;

	
	//REPORTDATA Alarm_repData;//报警上报帧专用非主动上报
	REPORTDATA VIPData;//重点户上报
	REPORTDATA DalyData;//日常综合数据
	INT8U m_Frame_FSEQ;//主动上报时的帧序号
	bstring m_sParsedBuffer;

	bstrings bssMeterOpenClose;//分合闸命令缓冲存储区
	bool m_bZipEnable;

	bool m_bFixDateLabel;//suxufeng:[2010-6-25]是否修正电表时标
	long m_lOutLineStartTick; //掉线时间累计
public:
	HEARTBEATING m_HeartBeat;
};

#endif
