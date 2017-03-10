#ifndef INCLUDED_PPP_SERVER
#define INCLUDED_PPP_SERVER

#ifndef INCLUDED_SERVER
#include "../../share/Server.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../../base/SysBase.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../../include/Config.h"
#endif

#ifndef INCLUDED_MODEM_CONNECTION
#include "../../conn/ModemConnection.h"
#endif

#define CONS_FILE_PPP_TRAFFIC_STAT_LOG "/mnt/data/log/ppp"		//文本文件，统计每日装置的流量
#define CONS_FILE_PPP_TRAFFIC_STAT_DAT "/mnt/data/log/ppp.dat"		//二进制文件，保存当前装置流量
#define CONS_PPP_RECV_INTER_SECOND 30								//PPP接收包的最大延时，超过此延时则认为PPP已经掉线

//TODO:考虑短信唤醒模式下的空闲时间判断，需要在SetLastRecvTime()函数的基础上添加一个判断空闲时间的函数，用以切换到短信唤醒方式。

struct SysConfig;
struct Channels;
class ModemConnection;

class PPPServer:public Server
{
public:
	PPPServer();
	~PPPServer();
	int OnTickServer();
	static int ResetModem();
	static int PowerOffModem();
	static int PowerOnModem();
	static long GetSimIP(void){return GetNetDevIP(PPP_DEV_NAME);}
	static long GetNetDevIP(char *pDev);
	inline static int SendOnLineRequest()
	{
		return SendLineRequest(IPC_INFO_TYPE_PPP_ONLINE_REQUEST);
	}
	inline static int SendOffLineRequest()
	{
		return SendLineRequest(IPC_INFO_TYPE_PPP_OFFLINE_REQUEST);
	}
	inline static int SendReOnLineRequest()
	{
		return SendLineRequest(IPC_INFO_TYPE_PPP_REONLINE_REQUEST);
	}
protected:
	int CheckModem();
	int CheckNotify();
	int DetectSignal(ModemConnection& conn,int &signal);
	int OnLine();
	int OffLine(bool bForce=false);
	int WaitPPP();

	bool NeedOnLine();
	bool NeedOffLine();
	
	void RefreshStatus();

	int GetDevInfo(INT32U* ipRecvPackages,INT32U* ipSendPackages);
	void SetLastRecvTime();

	int CheckWirelessType();
	void ClearRecvSendStat();

	bool IsOnLine();
	static int SendLineRequest(IPC_INFO_TYP infoType);
	void SetModemConnection();
	bool HasTraffics();
	/*
	是否有通信，假如能够接收到GPRS网络上的数据，则认为此时GPRS在线。
	在决定掉线前调用这个函数检查是否有必要掉线，没有必要则不必急于掉线。
	适用于可以使用PPP连接建立SOCKET，但是无法登陆成功的情况；
	以及无法建立SOCKET连接，但是正在使用PPP进行FTP文件传输的情况下，避免FTP传输终端。
	检查间隔：30秒。也就是说，在30秒钟之内，如果PPP接收了数据，就认为PPP在线。
	*/
	int OnPPPOffLine(Context &context);		//拨号失败后调用，统计拨号失败次数
	int OnPPPOnLine(Context &context);		//拨号成功后调用，复位拨号失败次数计数器
	bool CanOnLine(Context &context);		//拨号前调用，判断是否进行连接
	void OnTraffics();						//流量发生后调用，对ppp0链接进行流量统计
	void LoadTraffics();					//加载流量统计数据
	void ResetTraffics();					//流量统计数据清零，每月1日0点清零，或有用户命令时清零
	void SaveTraffics();					//保存流量统计数据
	void FprintTraffics();					//将流量数据保存为文本文件
	//int RemoveFiles(char* FName);			//将以前的文件删除
	bool CheckIdleness();					//判断空闲时间，用于短信唤醒模式下的空闲检查，超过空闲时间PPP自动下线
private:
	PPPServerInfo& m_serverInfo;
	SysConfig& m_sysConfig;

	PPPSetting m_curSetting;
	PID_T m_curPPP;
	int m_curModemType;
	int m_nPPPFailedCount;//PPP拨号连续失败的次数
	int m_nModemFailedCount;//Modem测试失败的次数
	INT8U m_curNetType;
	INT32U m_lastRecvPackages;		//上次接收的包总数，仅此次拨号
	INT32U m_lastSendPackages;		//上次发送的包总数，仅此次拨号
	INT32U m_RecvBytes;				//接收的字节总数
	INT32U m_SendBytes;				//发送的字节总数
	INT32U m_RecvBytesOnce;			//接收的字节总数
	INT32U m_SendBytesOnce;			//发送的字节总数
	INT32U m_LastRecvBytes;			//上次接收的包总数，仅此次拨号
	INT32U m_LastSendBytes;			//上次发送的包总数，仅此次拨号
	INT32U m_RecvStat;				//接收的字节总数
	INT32U m_SendStat;				//接收的字节总数
	INT32U m_TrafficStat;			//发送和接收的字节总数
	ModemConnection* m_pModemConnection;
	INT8U   m_PppdKillTimes; //pppd 被ppp_gprs_off.srt 杀掉的次数
};

#endif
