#ifndef INCLUDED_MODEM_CONNECTION
#define INCLUDED_MODEM_CONNECTION

//#include <linux/timer.h>
#include "SerialConnection.h"
#include "../base/Modem.h"

#define RING_INTERVAL 6		//振铃间隔6秒

class ModemIntercept
{
public:
	virtual int OnModemAnswer(const unsigned char* pAnswer,int len)
	{
		return -1;
	};
	virtual int OnRing(const char* pTeleNo)
	{
		return -1;
	}
	virtual int OnSmsData(SM_PARAM* pMsg)
	{
		return -1;
	}
};

class SmsModemConnection;
class ModemConnection:public SerialConnection
{
	friend class SmsModemConnection;
public:
	ModemConnection(INT8U modemType,Channel& channel);
	~ModemConnection(void);

	//检测Modem，必要时进行复位、同步波特率、测试、初始化；返回1表示已经执行了自我恢复操作，返回0表示正常（未做自我恢复操作）
	int CheckModem(const char* initCmd,bool forceReset,bool forceSyncBaud);
	//Modem型号自适应函数，苏煦烽2008年1月15日添加
	MODEM_TYPE FixModemType();

	virtual int ResetModem();
	virtual int PowerOffModem();
	virtual int PowerOnModem();
	virtual int InitModem(const char* initCmd=NULL);
	virtual int TestModem(int times=3);
	
	int GetSignal(int* opSignalVal,int times=3,int expandtimes=6);

	int OnIdle(string* pAnswer=NULL);
	int ModemStdComm(const char* command,string* pAnswer=NULL);
	int ModemSendRecv(const char* command,string* pAnswer,int charTimeoutMS,int responseWaitMS,int maxTimeoutMS);
	int ModemSendRecv(const char* command,string* pAnswer,bool bLongCharTimeout=false,bool bLongResponse=false,bool bLongTimeout=false);

	int SyncModemBaud();
	int ClearModemBuffer(int charTimeoutMS);

	int ReadSms(bstring& orResult);
	int SendSms(const unsigned char* pData,int dataLen,const char* pCenterTeleNo,const char* pDestTeleNo,int times=1);
	int DeleteSms(int index,int times=3);
	int EnsureReady();
	
protected:
	//标准的MODEM通讯：接收到的回应中有OK返回0，有ERROR返回-1，其它结果返回1
	int ModemStdComm(const char* command,string* pAnswer,int charTimeoutMS,int responseWaitMS,int maxTimeoutMS);
	int AutoConnect(int times=3);
	//command：发送给MODEM的命令
	//answer：从MODEM收到的回应
	//charTimeoutMS：回应的字符间隔时间(毫秒,1/1000秒)，如果在此时间内没有收到后续字符，则认为接收已结束
	//responseWaitMS：等待回应的最大时间(毫秒,1/1000秒)，如果发出命令后在此时间内没有收到第一个字符，则认为MODEM不能回应
	//maxTimeoutMS：接收数据的最长时间(毫秒,1/1000秒)，总之接收数据的时间不能超过此数值
	int ModemSendRecv(const char* command,bstring* pAnswer,int charTimeoutMS,int responseWaitMS,int maxTimeoutMS);

	int SendCommand(const char*  command);
	int ReadAnswer(bstring* pAnswer,int charTimeoutMS,int responseWaitMS,int maxTimeoutMS);


	int AssertModem();
	int SetModemBaud(int baud);

	virtual void OnNotExpectedAnswer(const char* cmd,const char* answer);

	virtual int OnModemAnswer(const unsigned char* pAnswer,int len);
	virtual int OnRing(const char* pTeleNo);
	virtual int OnSmsData(SM_PARAM* pMsg);

public:
	ModemIntercept* Intercept;
protected:
	Modem* m_pModem;
	INT8U m_ModemType;
	bool m_bCommandMode;
};

class SmsModemConnection:public Connection
{
public:
	SmsModemConnection(ModemConnection& modemConnection,Channel& channel):Connection(channel),m_ModemConnection(modemConnection)
	{
		m_pCenterTeleNo=NULL;
		m_pDestTeleNo=NULL;
		channel.PackageMaxLen=SMS_DATA_LENGTH;
	}
protected:
	bool OnConnect();
	void OnDisconnect(bool ibIsBreaked);
	int OnWriteData(const unsigned char* ipSendData,int iiCount);
	int OnReadData(unsigned char* ipRecvBuff,int iiCount,int iiTimeoutMS);
public:
	ModemConnection& m_ModemConnection;
	bstring m_RecvBuff;
	char* m_pCenterTeleNo;
	char* m_pDestTeleNo;
};

//PSTN（公共开关电话网络）MODEM连接，用于普通电话线拨号连接
class PstnModemConnection:public ModemConnection
{
public:
	PstnModemConnection(Channel& channel);
	~PstnModemConnection();
	inline bool IsReadyChannelChanged()
	{
		return (m_Channel.Type!=m_ReadyChannelType || memcmp(&m_Channel.Setting, &m_ReadySetting,sizeof(m_ReadySetting))!=0);
	}
	/*改写Connection类的两个虚函数，当用PSTN方式进行通信时，数据的收发将调用这两个函数*/
	/*virtual bool SendData(bstring& irSendData);
	virtual bool RecvData(bstring& orRecvData);*/
protected:
	bool OnConnect();//建立连接
	void OnDisconnect(bool ibIsBreaked);//断开连接
	virtual bool IsOnLine()
	{
		if (CheckCD()==0)
		{
			m_bIsConnected=true;
			return true;
		}
		else
		{
			return false;
		}
	}
	void OnConnectFailed();//连接失败时相应
	int PowerOffModem();//PSTN模块内核掉电。
		/*这里有问题,DJS-II-5100装置有两个继电器来控制MODEM的电源。第一个继电器控制整个MODEM的电源，第二个继电器在PSTN模块上
		，用于控制PSTN模块内部的核心电源。*/
	/*问题已经解决，PSTN中的PowerOnModem和PowerOffModem函数用于控制PSTN模块内核电源，Modem类中的PowerOn和PowerOff控制在整个
	PSTN模块的电源*/
	int PowerOnModem();//PSTN模块内核上电
	//int PowerOnPSTN();//PSTN模块上电
	//int PowerOffPSTN();//PSTN模块掉电
	int ResetModem();
	//int ResetPSTN();
	int CheckModem(bool bForceReset);
	bool HandupModem();
	int CheckHardwareRing();
	int CheckCD();
	int ClearCD();
	int PrepareConnect();
	int OnRing(const char* pTeleNo);
	int OnModemAnswer(const unsigned char* pAnswer,int len);
private:
	int ResetStatus();
	//void ResetLink();
	//void ScanSendData();
	//void SendConfirmFrame();
	//void GetPstnFrame(bstring& irSendData, unsigned char tSendNo);
	//bool IsConfirmFrame(bstring& orRecvData);
	//int ParseFrame(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);//帧解析
private:
	int m_nModemFailedCount;
	time_t m_LastCheckTime;//最后一次检测时间（软时钟）
	time_t m_ConnectBeginTime;//连接开始时间（一般指收到振铃后的时间，软时钟）
	int m_nHardwareRingCount;//硬件振铃次数

	INT8U m_ReadyChannelType;//保存准备连接时的通道类型,以便用于通道更改的检测
	union ChannelSetting m_ReadySetting;//保存准备连接时的通道设置,以便用于通道更改的检测
	int m_ConnectedBaud;
	bool m_bIsModemPowerOn;
	bool m_bIsConnected;//已经连接成功标志
	//unsigned char recvNo;//接收序号
	//unsigned char sendNo;//发送序号
	//unsigned int sendNumber;//发送次数
	//unsigned int maxSendN;//最大发送次数
	//time_t sendTime;//发送数据的时间
	//unsigned char ilTime;//重发时间间隔
	//bstring m_bsRecvData;//接收数据缓冲区
	//bstrings m_vFrame;//发送帧缓冲向量
};

#endif


