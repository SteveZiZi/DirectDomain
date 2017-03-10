
#ifndef INCLUDED_MODEM
#define INCLUDED_MODEM

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_SMS
#include "Sms.h"
#endif

#ifndef INCLUDED_KL_DUMP
#include "KLDump.h"
#endif

//PPP的Modem型号,0-63为普通Modem,64-127为GPRS Modem,128-191为CDMA Modem,其它值保留
typedef enum _MODEM_TYPE
{
	MODEM_TYPE_COMMON=0,
	MODEM_TYPE_GPRS_TELIT_GC864=64,
	MODEM_TYPE_GPRS_YOUFANG_M590=65,
	MODEM_TYPE_GPRS_SIEMENS_MC35=66,
	MODEM_TYPE_GPRS_SIEMENS_MC39I=67,
	MODEM_TYPE_GPRS_SONY_ERICSSON_GM47=68,
	MODEM_TYPE_GPRS_SONY_ERICSSON_GR47=69,
	MODEM_TYPE_GPRS_SONY_ERICSSON_GR64=70,
	MODEM_TYPE_GPRS_WAVECOM_Q39=71,
	MODEM_TYPE_GPRS_TELIT_GL868=72,
	MODEM_TYPE_GPRS_DINGXIN_M72=73,
	//MODEM_TYPE_CDMA_FIDELIX_FD5105_A=128,
	MODEM_TYPE_CDMA_ANYDATA=129,

}MODEM_TYPE;

KLDump& operator<< (KLDump& dump, MODEM_TYPE v);
int MAKE_MODEM_TYPE_KEY(INT8U val,char* oKey);
INT8U MAKE_MODEM_TYPE_VAL(const char* key);

//class SM_PARAM;

class Modem
{
public:
	Modem(MODEM_TYPE type,const char* name);

	virtual ~Modem(){}

	virtual int PowerOn();
	virtual int PowerOff();

	//检查CD状态，返回-1表示未知，返回0表示有CD，返回1表示无CD
	virtual int CheckCD();

	virtual int GetIprCommand(int baud,string& buffer);//用于设置Modem波特率
	//初始化命令可能需要分步进行，step表示步骤，从0开始，返回0表示没有下一步骤，返回1表示还有下一步骤，返回-1表示不支持
	virtual int GetInitCommand(string& buffer,int step);
	virtual int GetTestCommand(string& buffer);
	virtual int GetSignalCommand(string& command);
	virtual int GetAutoConnectCommand(string& command);

	virtual int ParseSignal(string& recvStr,int* opSignalVal);

	virtual int GetSmsReadAllCommand(string& command);
	virtual int ParseSmsReadAllCommand(string& recvStr,SM_PARAM* pMsg,int* count);
	virtual int GetSmsReadOneCommand(int index,string& command);

	//解析单条短消息，返回-1表示失败，返回0表示成功（可进行下一条读取），返回1表示无短消息（不需进行下一条读取）
	virtual int ParseSmsReadOneCommand(int index,string& recvStr,SM_PARAM* pMsg);

	//发送命令可能需要分步进行，step表示步骤，从0开始，返回0表示没有下一步骤，返回1表示还有下一步骤，返回-1表示不支持
	virtual int GetSmsSendCommand(const SM_PARAM* pSrc,string& command,int step);
	virtual int ParseSmsSendCommand(const SM_PARAM* pSrc,string& recvStr,int step);

	virtual int GetSmsDeleteCommand(int index,string& command);
	virtual int ParseSmsDeleteCommand(int index,string& recvStr);

	virtual bool HasRing(const char* answer);
	virtual int ParseConnectBaud(const char* answer);
	int Reset();
public:
	inline MODEM_TYPE GetType() const
	{
		return m_Type;
	}
	inline const char* GetName() const
	{
		return m_Name.c_str();
	}
public:
	int CharTimeoutMS;
	int LongCharTimeoutMS;
	int ResponseTimeoutMS;
	int LongResponseTimeoutMS;
	int MaxTimeoutMS;
	int LongMaxTimeoutMS;
	int PowerOffDelayMS;
	int PowerOnDelayMS;
protected:
	MODEM_TYPE m_Type;
	string m_Name;
};

class GprsModem:public Modem
{
public:
	inline GprsModem(MODEM_TYPE type,const char* name):Modem(type,name){}
	int PowerOn();

	int GetIprCommand(int baud,string& buffer);
	int GetInitCommand(string& buffer,int step);
	int GetSignalCommand(string& command);
	int ParseSignal(string& recvStr,int* opSignalVal);

	int GetSmsReadAllCommand(string& command);
	int ParseSmsReadAllCommand(string& recvStr,SM_PARAM* pMsg,int* count);
	int GetSmsSendCommand(const SM_PARAM* pSrc,string& command,int step);
	int ParseSmsSendCommand(const SM_PARAM* pSrc,string& recvStr,int step);

	int GetSmsDeleteCommand(int index,string& command);
	int ParseSmsDeleteCommand(int index,string& recvStr);
};
class CdmaModem:public Modem
{
public:
	inline CdmaModem(MODEM_TYPE type,const char* name):Modem(type,name){}

	
	int PowerOn();
	
	int GetInitCommand(string& buffer,int step);

	int GetSignalCommand(string& command);
	int ParseSignal(string& recvStr,int* opSignalVal);

	int GetSmsReadOneCommand(int index,string& command);
	int ParseSmsReadOneCommand(int index,string& recvStr,SM_PARAM* pMsg);

	int GetSmsSendCommand(const SM_PARAM* pSrc,string& command,int step);
	int ParseSmsSendCommand(const SM_PARAM* pSrc,string& recvStr,int step);

	int GetSmsDeleteCommand(int index,string& command);
	int ParseSmsDeleteCommand(int index,string& recvStr);
};


class GprsMc35Modem:public GprsModem
{
public:
	inline GprsMc35Modem():GprsModem(MODEM_TYPE_GPRS_SIEMENS_MC35,"MC35")
	{
		CharTimeoutMS=500;
	}
	int GetInitCommand(string& buffer,int step);
};

class GprsMC39iModem:public GprsModem
{
public:
	inline GprsMC39iModem():GprsModem(MODEM_TYPE_GPRS_SIEMENS_MC39I,"MC39i")
	{
		CharTimeoutMS=500;
	}
	int GetInitCommand(string& buffer,int step);
};

class GprsGR47Modem:public GprsModem
{
public:
	inline GprsGR47Modem():GprsModem(MODEM_TYPE_GPRS_SONY_ERICSSON_GR47,"GR47")
	{
		CharTimeoutMS=500;
	}
};

class GprsGR64Modem:public GprsModem
{
public:
	inline GprsGR64Modem():GprsModem(MODEM_TYPE_GPRS_SONY_ERICSSON_GR64,"GR64")
	{
		CharTimeoutMS=500;
	}
};

class GprsQ39Modem:public GprsModem
{
public:
	inline GprsQ39Modem():GprsModem(MODEM_TYPE_GPRS_WAVECOM_Q39,"Q39")
	{
		CharTimeoutMS=500;
	}
};

class GprsGM47Modem:public GprsModem
{
public:
	inline GprsGM47Modem():GprsModem(MODEM_TYPE_GPRS_SONY_ERICSSON_GM47,"GM47")
	{
		CharTimeoutMS=500;
	}
};

class GprsGC864Modem:public GprsModem
{
public:
	inline GprsGC864Modem():GprsModem(MODEM_TYPE_GPRS_TELIT_GC864,"GC864")
	{
		CharTimeoutMS=500;
	}
	int GetInitCommand(string& buffer,int step);
};
class GprsGL868Modem:public GprsModem
{
public:
	inline GprsGL868Modem():GprsModem(MODEM_TYPE_GPRS_TELIT_GL868,"GL868")
	{
		CharTimeoutMS=500;
	}
	int GetInitCommand(string& buffer,int step);
};

class GprsM590Modem:public GprsModem
{
public:
	inline GprsM590Modem():GprsModem(MODEM_TYPE_GPRS_YOUFANG_M590,"M590")
	{
		CharTimeoutMS=500;
	}
	int GetInitCommand(string& buffer,int step);
};
class GprsM72Modem:public GprsModem
{
public:
	inline GprsM72Modem():GprsModem(MODEM_TYPE_GPRS_DINGXIN_M72,"M72")
	{
		CharTimeoutMS=500;
	}
	int GetInitCommand(string& buffer,int step);
};

//class CdmaFD5105AModem:public CdmaModem
//{
//public:
//	inline CdmaFD5105AModem():CdmaModem(MODEM_TYPE_CDMA_FIDELIX_FD5105_A,"FD5105_A")
//	{
//		PowerOnDelayMS=3000;
//	}
//};
//
class AnyDataModem:public CdmaModem
{
public:
	inline AnyDataModem():CdmaModem(MODEM_TYPE_CDMA_ANYDATA,"AnyData")
	{
		PowerOnDelayMS=1000;
	}
	int GetInitCommand(string& buffer,int step);
	int GetSignalCommand(string& command);
	int ParseSignal(string& recvStr,int* opSignalVal);

	int GetSmsReadOneCommand(int index,string& command);
	int ParseSmsReadOneCommand(int index,string& recvStr,SM_PARAM* pMsg);

	int GetSmsSendCommand(const SM_PARAM* pSrc,string& command,int step);
	int ParseSmsSendCommand(const SM_PARAM* pSrc,string& recvStr,int step);

	int GetSmsDeleteCommand(int index,string& command);
	int ParseSmsDeleteCommand(int index,string& recvStr);
};

class ModemFactory
{
public:
	static Modem* CreateModem(INT8U modemType);
};

#endif

