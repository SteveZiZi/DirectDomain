#ifndef HTTX_JILIAN
#define HTTX_JILIAN

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef _cfg_h_
#include "../config/confuse/confuse.h"
#endif

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

#ifndef INCLUDED_REG_ITEM
#include "RegItem.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

//#ifndef SEMAPHORE_H
//#include "SemObject.h"
//#endif

typedef vector <unsigned int> int32KLIDs;
struct Meter;

struct SEND_BUFFER
{
public:
	void SetFinishFlag(int task_type, int id);
	void CleanFlag(int task_type);
	bool IsFinished(int task_type);
private:
	char task_finished[9];
//当增加采集终端时，通知路由进程
public:
	int newRelays;

//============以下用于级联============================================
	public:
	SEND_BUFFER();
	~SEND_BUFFER();
	bool lock;

		bool IsMasterChannelOnline;//主通道是否在线，用于级联判断是否应该主动上报
		//苏煦烽2008-3-14修改
		bool IsMasterChannelIdle;//主通道是否空闲，级联进程判断此值为"true"时，才会对从集中器进行轮询

	public:
		//int DbcjToWork;//电表采集进程保存完日电量数据之后应发消息通知工作管理进程进行线损计算
	//=========以上用于级联=================================================

	//===========以下专用于拉合闸===========================================
	//bstring sendto_dbcj;  //和电表通讯进程通讯的地方
private:
    unsigned char realReadCommand[SHARE_MEM_LENGTH];	
	int oc_length;
public:
	int HttxWrite_toDbcj_openclose(bstring req);
	int Read_openclose(bstring& rep,int &TN,int instanceID=1000);
	unsigned short openClosebackdata;//返回数据
	int aim;//1 to dbcj/ 2 to httx	3为空闲 4为DBCJ控制超时 每次读写后都要改为空闲状态。
	int timeout;//任务控制超时时间.
	time_t whentodo;//这是我发给DBCJ进程任务时的时间。
	//======================================================================

	//===========以下用于实时招测===========================================
private:
	  unsigned char sendto_dbcj[SHARE_MEM_LENGTH];
	  INT32U real_length;
public:
	  int HttxWrite_toDbcj_realRead(bstring req);
		/*电表采集进程调用该函数读取主站下发的实时招测命令*/
	  int Read_realRead(bstring& rep,int &TN);

	  int Read_RealReadcomand(bstring & rep);
	  void HttxReadRealTimeDataTimeOut();
	  void HttxReadRealTimeDataOk();	   	  
	  DataBitSet idbs;//下发命令实时召测的数据标识对应的掩码
	  unsigned char ReadBackchar[10];
	  
	  RegItem ReadBackRegItems[CONS_REG_ID_MAX_COUNT];
	  int Reg_num;
	  
	  int HttxRead_RealRead(RegItems &regitems);
	  time_t realTimeNow;//传递该命令的时刻;
	  //real_Aim：每次到共享内存读写都需要修，各个符号代表：
	  //1 to Dbcj;2 to httx；3:空闲状态,或主站取消实时招测命令；
	  //4:HTTX不能读写，DBCJ只写不读，数据已经被某个DBCJ进程取走；
	  //5：DBCj进程判断超时；
	  //6：Dbcj进程实时召测失败
	  int real_Aim;
	  int instanceID;
	  MilliSecondTimeout recvTs;
	//======================================================================
public:	
	void Init();	
	
//======================================================================
/*用于广播校时*/
public:
    INT8U CheckTimeFlag;//1 to dbcj 3 no command
//======================================================================
public:
	/*0表示未校，3表示已校*/
	INT8U SetTimeFlag;
	//用于存放实时组网状态,格式为(序号表示字节在RouterState中位置)
	/*
	0：组网是否完成。0完成，1未完成
	1：正在组网的相位。1表示A相，2表示B相，3表示C相，4表示专线
	2：终端个数
	3-5：中继1地址
	6-8：中继2地址
	9-11：中继3地址
	12-14：中继4地址
	15-17：中继5地址
	*/
	INT8U RouterState[32];
	//用于线损告警
	bool LostAlarmFlag;
//=====================================================================================


/*用于报警主动上报2006-11-6*/
//0没有报警或刚启动，N表示有多少告警
unsigned char Alarm_report_flag;

//用于PPP进程告诉后台通信进程可以进行连接了
bool PPPHasOnline;
unsigned char Lock_GPRSConnectFailedTimes;
unsigned char Lock_GPRSConnectFailedMaxTimes;
unsigned char GPRSConnectFailedTimes;
unsigned char GPRSConnectFailedMaxTimes;
unsigned char Get_GPRSConnectFailedTimes();
unsigned char Get_GPRSConnectFailedMaxTimes();
unsigned char Set_GPRSConnectFailedTimes();
unsigned char Set_GPRSConnectFailedMaxTimes();
unsigned char Reset_GPRSConnectFailedTimes();
unsigned char Reset_GPRSConnectFailedMaxTimes();
};
/***************************************************************************************/
#define MAX_DEBUG_TASK 10
#define DEFAULT_DELAY_TIME 30
//struct DEBUG_COMMAND
//{
//	INT16U 
//};
struct DEBUG_BUFFER
{
public:
	DEBUG_BUFFER();
	void Init();
	/*bool OnDebug(INT8U selc) const;
	bool OffDebug(INT8U selc) const;*/
	int SetCommand(INT16U DataID, const bstring& data,INT16U _instanceID,const INT8U hData[6]=NULL);
	int GetCommand(INT16U &DataID, bstring& data,INT8U selc);
	int SetRplData(INT16U DataID, const bstring& data,INT8U ansType);
	int GetRplData(INT16U &DataID, bstring& data,INT8U &ansType,INT16U _instanceID);
	bool IsDebugState();
	bool IsTimeOut();
	bool IsTaskCanceled(INT16U DataID);
	INT8U GetState(INT16U taskNo);
	bool GetHead(INT8U hData[6]);
	bool IsMe(int _instanceID)
	{
		return _instanceID==instanceID;
	}
	INT8U GetTaskStatus(INT16U DataID);
	void SetTerminalResetFlag(const bstring &data);
	void GetTerminalResetFlag(bstring &data);
	void SystemCommand(const bstring &data);
	void ExecCommand(const bstring &data);
	void ClearAll();
private:
	bool  InDebugFlag;//是否需要进入调试态，
	INT8U head[6];
	INT8U HasTask();
	INT8U HasRunningTask();
	INT8U HasReply();
	INT8U FindTaskNo(INT16U DataID);
	bool Set4152Data(const bstring& data);
	INT8U TerminalResetFlag[7];
public:
	//相应的任务号的cmd的值含义：0无任务；1有任务但没执行；2任务被取消；3任务正常回应；4任务异常回应；5任务正在执行；其它为无效值。
	//对于进入退出调试态的命令单独处理，3表示收到上位机的进入调试态的命令，4表示收到上位机的退出调试态的命令，1，表示已经进入调试态，0表示已经不在调试态。
	INT8U cmd[16];
	INT16U delayTime;//如果延时时候过后还没有调试命令，自动结束调试态
	time_t dbgTime;//调试开始时间
	INT16U instanceID;
	union
	{
		INT8U dbg_InOutDebug[2];
		INT8U dbg_StaticRouter[20];
		INT8U dbg_LinkState[25];
		INT8U dbg_SendPackage[25];
		INT8U dbg_StartRouter[3];
		INT8U dbg_StopRouter[1];
		INT8U dbg_AdjustRouter[771];
		INT8U dbg_SetZBSniffer[149];
		INT8U dbg_BatchRealTimeRead[511];
		INT8U dbg_CallTest[25];
		INT8U dbg_GetNetStatus[32];
		INT8U dbg_GetNetFailedNodes[25];
		
	}FromHttx;
	union
	{
		INT8U rpl_StaticRoutre[1];
		INT8U rpl_LinkState[1];
		INT8U rpl_SendPackage[1];
		INT8U rpl_StartRouter[766];
		INT8U rpl_StopRouter[1];
		INT8U rpl_AdjustRouter[766];
		INT8U rpl_SetZBSniffer[1];
		INT8U rpl_BatchRealTimeRead[151];
		INT8U rpl_CallTest[1];
		INT8U rpl_GetNetStatus[9];
		INT8U rpl_GetNetFailedNodes[542];
		
	}ToHttx;


};

#endif
