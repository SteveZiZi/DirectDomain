
#ifndef INCLUDED_PPP_SETTING
#define INCLUDED_PPP_SETTING

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_CHANNEL
#include "Channel.h"
#endif

class KLDump;

struct PPPDialSetting
{
	char Apn[CONS_PPP_APN_MAX_LEN+1];//APN
	char TeleNo[CONS_PPP_TELENO_MAX_LEN+1];//电话号码，为CONS_DEFAULT_KEY时表示采用默认值
	char User[CONS_PPP_USER_MAX_LEN+1];//拨号用户，为CONS_DEFAULT_KEY时表示采用默认值
	char Pwd[CONS_PPP_PWD_MAX_LEN+1];//拨号密码，为CONS_DEFAULT_KEY时表示采用默认值
	char ModemInitCmd[CONS_MODEM_INIT_CMD_MAX_LEN+1];//MODEM初始化命令

	void Init();
	KLDump& Dump(KLDump& dump) const;
};

struct PPPSetting
{
	INT8U Enabled;//是否可用
	INT16U CheckInterM;//检测间隔（分钟）
	INT16U CheckFailTimes;//检测失败阀值，如果连续此数值次检测失败，将断线
	PPPDialSetting Gprs;
	PPPDialSetting Cdma;
	Channel ModemChannel;

	inline PPPSetting()
	{
		Init();
	}

	void Init();
	KLDump& Dump(KLDump& dump) const;
	void InitChannel(Channel& channel);

	PPPDialSetting& GetDialSetting(INT8U wirelessModemType);
};
inline bool operator== (const PPPSetting& lhs,const PPPSetting& rhs)
{
	return (memcmp(&lhs,&rhs,sizeof(lhs))==0);
}


#endif

