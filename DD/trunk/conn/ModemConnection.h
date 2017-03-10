#ifndef INCLUDED_MODEM_CONNECTION
#define INCLUDED_MODEM_CONNECTION

//#include <linux/timer.h>
#include "SerialConnection.h"
#include "../base/Modem.h"

#define RING_INTERVAL 6		//������6��

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

	//���Modem����Ҫʱ���и�λ��ͬ�������ʡ����ԡ���ʼ��������1��ʾ�Ѿ�ִ�������һָ�����������0��ʾ������δ�����һָ�������
	int CheckModem(const char* initCmd,bool forceReset,bool forceSyncBaud);
	//Modem�ͺ�����Ӧ�����������2008��1��15�����
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
	//��׼��MODEMͨѶ�����յ��Ļ�Ӧ����OK����0����ERROR����-1�������������1
	int ModemStdComm(const char* command,string* pAnswer,int charTimeoutMS,int responseWaitMS,int maxTimeoutMS);
	int AutoConnect(int times=3);
	//command�����͸�MODEM������
	//answer����MODEM�յ��Ļ�Ӧ
	//charTimeoutMS����Ӧ���ַ����ʱ��(����,1/1000��)������ڴ�ʱ����û���յ������ַ�������Ϊ�����ѽ���
	//responseWaitMS���ȴ���Ӧ�����ʱ��(����,1/1000��)���������������ڴ�ʱ����û���յ���һ���ַ�������ΪMODEM���ܻ�Ӧ
	//maxTimeoutMS���������ݵ��ʱ��(����,1/1000��)����֮�������ݵ�ʱ�䲻�ܳ�������ֵ
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

//PSTN���������ص绰���磩MODEM���ӣ�������ͨ�绰�߲�������
class PstnModemConnection:public ModemConnection
{
public:
	PstnModemConnection(Channel& channel);
	~PstnModemConnection();
	inline bool IsReadyChannelChanged()
	{
		return (m_Channel.Type!=m_ReadyChannelType || memcmp(&m_Channel.Setting, &m_ReadySetting,sizeof(m_ReadySetting))!=0);
	}
	/*��дConnection��������麯��������PSTN��ʽ����ͨ��ʱ�����ݵ��շ�����������������*/
	/*virtual bool SendData(bstring& irSendData);
	virtual bool RecvData(bstring& orRecvData);*/
protected:
	bool OnConnect();//��������
	void OnDisconnect(bool ibIsBreaked);//�Ͽ�����
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
	void OnConnectFailed();//����ʧ��ʱ��Ӧ
	int PowerOffModem();//PSTNģ���ں˵��硣
		/*����������,DJS-II-5100װ���������̵���������MODEM�ĵ�Դ����һ���̵�����������MODEM�ĵ�Դ���ڶ����̵�����PSTNģ����
		�����ڿ���PSTNģ���ڲ��ĺ��ĵ�Դ��*/
	/*�����Ѿ������PSTN�е�PowerOnModem��PowerOffModem�������ڿ���PSTNģ���ں˵�Դ��Modem���е�PowerOn��PowerOff����������
	PSTNģ��ĵ�Դ*/
	int PowerOnModem();//PSTNģ���ں��ϵ�
	//int PowerOnPSTN();//PSTNģ���ϵ�
	//int PowerOffPSTN();//PSTNģ�����
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
	//int ParseFrame(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);//֡����
private:
	int m_nModemFailedCount;
	time_t m_LastCheckTime;//���һ�μ��ʱ�䣨��ʱ�ӣ�
	time_t m_ConnectBeginTime;//���ӿ�ʼʱ�䣨һ��ָ�յ�������ʱ�䣬��ʱ�ӣ�
	int m_nHardwareRingCount;//Ӳ���������

	INT8U m_ReadyChannelType;//����׼������ʱ��ͨ������,�Ա�����ͨ�����ĵļ��
	union ChannelSetting m_ReadySetting;//����׼������ʱ��ͨ������,�Ա�����ͨ�����ĵļ��
	int m_ConnectedBaud;
	bool m_bIsModemPowerOn;
	bool m_bIsConnected;//�Ѿ����ӳɹ���־
	//unsigned char recvNo;//�������
	//unsigned char sendNo;//�������
	//unsigned int sendNumber;//���ʹ���
	//unsigned int maxSendN;//����ʹ���
	//time_t sendTime;//�������ݵ�ʱ��
	//unsigned char ilTime;//�ط�ʱ����
	//bstring m_bsRecvData;//�������ݻ�����
	//bstrings m_vFrame;//����֡��������
};

#endif


