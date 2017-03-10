#ifndef INCLUDED_CONNECTION
#define INCLUDED_CONNECTION

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

#ifndef INCLUDED_CHANNEL
#include "Channel.h"
#endif

#ifndef INCLUDED_CONTEXT
#include "Context.h"
#endif

//#ifndef INCLUDED_TRAFFIC_STAT
//#include "TrafficStat.h"
//#endif

#ifndef INCLUDED_SYS_CONFIG_CFG
#include "../config/SysConfigCfg.h"
#endif

#ifndef INCLUDED_SERVER_CFG
#include "../config/ServerCfg.h"
#endif

class ConnectionIntercept
{
public:
	virtual bool PreConnect()=0;//raised before connect
	virtual void PostConnect()=0;//raised after connect
	virtual bool PreDisconnect()=0;//raised before disconnect
	virtual void PostDisconnect()=0;//raised after disconnect
};


//TODO:关于连接的接收，需要进一步细化，考虑字符超时，响应超时，以及总超时
//TODO:关于大数据量的发送，需要进一步优化，考虑采用阻塞方式，逐条发送

class Connection
{
public:
	inline Connection(Channel& channel):m_pIntercept(NULL),m_Channel(channel)
	{
		m_OnlineChannelType=CHANNEL_TYPE_UNKNOWN;
		memset(&m_OnlineSetting,0,sizeof(m_OnlineSetting));
		m_Channel.Status=CHANNEL_STATUS_FREE;
		//m_iRegCount=0;

		//LastConnectInterS = m_Channel.ConnectInterS;
	}
	virtual ~Connection(void);
public:	
	bool EnsureOnLine(bool bOffLineFirst,bool bLastForEver);
	void EnsureOffLine(bool bIsConnectionBreaked);
	bool Connect(int iiTimeoutSeconds,int iiConnectIntervalS);
	inline bool Connect()
	{
		return Connect(m_Channel.ConnectSubTimeoutS,m_Channel.ConnectInterS);
	}

	virtual bool SendData(bstring& irSendData)
	{
		return Send(STRING_DATA(irSendData),(int)irSendData.size(),m_Channel.WriteTimeoutMS);
	}
	inline bool Send(const void* ipData,int iiDataLen)
	{
		return Send(ipData,iiDataLen,m_Channel.WriteTimeoutMS);
	}
	bool Send(const void* ipData,int iiDataLen,int iiWriteTimeoutMS);
	bool SendData(bstrings& irSendDatas);
	virtual bool RecvData(bstring& orRecvData)
	{
		return RecvData(orRecvData,m_Channel.ReadTimeoutMS);			
	}
	bool RecvData(bstring& orRecvData,int iiReadTimeoutMS);
	int Recv(unsigned char* ipRecvData,int iiCount,int iiTimeoutMS);
	bool WaitByte(unsigned char ipByteWaited,int iiWaitTimeoutMS);
	bool RecvByte(unsigned char* ipByte);

	inline CHANNEL_TYPE GetConnectionType()
	{
		return (CHANNEL_TYPE)m_Channel.Type;
	}
	inline INT32U GetPackageMaxLen() const
	{
		return m_Channel.PackageMaxLen;
	}
	inline bool IsConnected()
	{
		return IsOnLine();
	}
	inline bool IsChannelChanged()
	{
		return (memcmp(&m_Channel.Setting, &m_OnlineSetting,sizeof(m_OnlineSetting))!=0)||(m_Channel.Enabled==0);
	}
	inline INT32U GetSendBytes()
	{
		return m_Channel.SendBytes;
	}
	inline INT32U GetRecvBytes()
	{
		return m_Channel.RecvBytes;
	}
	inline void SetIntercept(ConnectionIntercept* pIntercept)
	{
		m_pIntercept=pIntercept;
	}
	inline Channel& GetChannel()
	{
		return m_Channel;
	}
	int CheckStatus(bool bForce);
	virtual int Config(const void* pSetting,int iiLen);

	void PrintData(char* ipKey,unsigned char* ipData,unsigned int iLen,unsigned int iMaxLen=0);
	void PrintStream(char* ipKey,unsigned char* ipData,unsigned int iLen,bool bHexMode=true,unsigned int iMaxLen=0);

	//void RegisterConnection();
	//void UnRegisterConnection(bool bIsConnectionBreaked);
	virtual int GetPortFd();
protected:
	virtual bool OnConnect();
	virtual bool IsOnLine();
	virtual void OnDisconnect(bool ibIsBreaked);
	virtual int OnWriteData(const unsigned char* ipSendData,int iiCount)=0;
	virtual int OnReadData(unsigned char* ipRecvBuff,int iiCount,int iiTimeoutMS)=0;
	virtual int OnLineCheck();//在线检测,为0表示在线,其它值表示连接已不可用
	virtual void OnConnectFailed();
	bool CanConnect(Context &context);			//连接前调用，判断是否进行连接
	void OnConnectedFail(Context &context);		//连接失败后调用，对连接次数处理
	void OnConnectedSuceed(Context &context);		//连接失败后调用，对连接次数处理
	int Write(const void* ipSendData,int iiCount,int iiTimeoutMS);
	int Read(unsigned char* ipRecvData,int iiCount, int iiTimeoutMS);
	int ReadData(bstring& orRecvData,int iiTimeoutMS);	
	//int HeartBeat();

	//void PrintData(char* ipKey,bstring::const_iterator begin,bstring::const_iterator end);
protected:
	ConnectionIntercept* m_pIntercept;
	INT8U m_OnlineChannelType;//当连接成功时,将保存连接成功时的通道类型,以便用于通道更改的检测
	union ChannelSetting m_OnlineSetting;//当连接成功时,将保存连接成功时的通道设置,以便用于通道更改的检测
	//int m_iRegCount;
	//INT16U LastConnectInterS;
public:
	Channel& m_Channel;
};


#endif


