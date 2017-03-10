
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
	char TeleNo[CONS_PPP_TELENO_MAX_LEN+1];//�绰���룬ΪCONS_DEFAULT_KEYʱ��ʾ����Ĭ��ֵ
	char User[CONS_PPP_USER_MAX_LEN+1];//�����û���ΪCONS_DEFAULT_KEYʱ��ʾ����Ĭ��ֵ
	char Pwd[CONS_PPP_PWD_MAX_LEN+1];//�������룬ΪCONS_DEFAULT_KEYʱ��ʾ����Ĭ��ֵ
	char ModemInitCmd[CONS_MODEM_INIT_CMD_MAX_LEN+1];//MODEM��ʼ������

	void Init();
	KLDump& Dump(KLDump& dump) const;
};

struct PPPSetting
{
	INT8U Enabled;//�Ƿ����
	INT16U CheckInterM;//����������ӣ�
	INT16U CheckFailTimes;//���ʧ�ܷ�ֵ�������������ֵ�μ��ʧ�ܣ�������
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

