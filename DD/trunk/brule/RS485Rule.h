#ifndef INCLUDED_RS485RULE
#define INCLUDED_RS485RULE




#ifndef INCLUDED_CONTEXT
#include "../share/Context.h"
#endif

#ifndef INCLUDED_SYS_CONFIG
#include "../share/SysConfig.h"
#endif

#include <algorithm>

#include <stdio.h>

#ifndef WIN32
#include <sys/file.h>
#endif

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../share/SysBase.h"
#endif


#ifndef INCLUDED_CONNECTION
#include "../share/Connection.h"
#endif

#ifndef INCLUDED_METER
#include "../share/Meter.h"
#endif


#ifndef INCLUDED_METER_RULE
#include "../share/MeterRule.h"
#endif

#include "../include/Config.h"

#ifndef INCLUDED_BUSCHANNELSRULE
#include "../share/BusChannelsRule.h"
#endif


#ifndef INCLUDED_PORT_SET
#include "../base/PortSet.h"
#endif



#define RS485_RULE_NAME  "RS485总线规约"
#define RS485_CHAR_READ_TIMEOUT_MS 500
#define RS485_SUB_READ_TIMEOUT_MS 500
#define RS485_READ_TIMEOUT_MS 1000 //CGH 2010-04-23
#define RS485_WRITE_TIMEOUT_MS 1000
//TODO list: added Class definition here
const int RS485_WRITE_MAX_TIME = 500;

class RS485Rule: public BusChannelsRule
{
public:
	inline RS485Rule(){InitRuleBaseInfo();}
	inline RS485Rule(Connection *conn);
	inline ~RS485Rule()
	{


	}
	int GetMeterData(Meter& imeter,const unsigned char* idata,int idataLen,bstring& opReturnData,bstring* iaddr=NULL,const int &iiTimesOuts=0);
    bool SendData(Meter& meter,const unsigned char* data,int dataLen,bstring &retPackage,bstring* iaddr = NULL);
	bool SendPackage(const unsigned char* idata,int idataLen);
	int  GetDeviceData(Device& device,const unsigned char* data,int dataLen,bstring& opReturnData,bstring* iaddr=NULL,const int &iiTimesOuts=0); 
    int BroadCastData(Meter &meter, INT8U *data, const INT8U &dataLen);
public:
	/* 整流器底层通讯接口*/
	int  GetRtfData(Device& device,const unsigned char* data,int dataLen,bstring& opReturnData,bstring* iaddr=NULL,const int &iiTimesOuts=0); 	
	PARSE_RESULT ParseRtfDataPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& opReturnData);
protected:

	void InitRuleBaseInfo();
	bool SendPackage(Meter& imeter,const unsigned char* idata,int idataLen,INT8U iCtrlFlag,bstring& oSendPackage,bstring* iaddr=NULL);

	int MakePackage(Meter& imeter,const unsigned char* idata,int idataLen,INT8U iCtrlFlag,bstring& oSendPackage,bstring* iaddr=NULL);
	
	//PARSE_RESULT ParsePackage(Meter& meter,bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,int iiKey,bstring& irSendData,int* opReturnKey,bstring& opReturnData);
	PARSE_RESULT ParsePackage(Meter& meter,bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& irSendData,bstring& opReturnData);
	PARSE_RESULT ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& opReturnData);
	PARSE_RESULT ParseModbusPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,INT8U key,bstring& opReturnData);
       
	int GetTimeOutMS(Meter& meter);
};


#endif

