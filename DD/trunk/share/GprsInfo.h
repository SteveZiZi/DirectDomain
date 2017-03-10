#ifndef INCLUDED_GPRS_INFO
#define INCLUDED_GPRS_INFO

#ifndef INCLUDED_FILE_BASE
#include "../base/FileBase.h"
#endif

struct GprsInfo
{
public:
	bool EnableFlowCtrl;				//�Ƿ������������ƹ���
	INT16U LogOnInterS;					//��½��ʱʱ��
	INT8U LogOnRetryTimes;				//��½���Դ���
	INT16U SocketConnectInterS;			//SOCKET����������ǳ�����
	INT16U DialUpInterS;				//�������Լ��
	bool EnableMaxFlowLimit;			//�Ƿ����������������
	INT32U GenericTrafficStatOfMonth;	//��������ͳ��ֵ
	INT8U DialFailTimes;				//�ɹ������㣬���ɹ����ۼӣ������ֵ��SysConfig��ȡ��
	INT8U ReConnectTimes;				//ͬ��
	time_t LastConnectFailedTime;		//�ϴ�����ʧ��ʱ��
	time_t LastConnectTime;				//�ϴ�����ʱ��

	unsigned char UplinkStatus;			// ����ͨ����״̬
	unsigned char SimCardIP[4];			// SIM��IP
	unsigned char SignalValue;			// GPRS/CDMA�ź�ֵ

	void Init();

	inline void SetMainServeMode() // ����ģʽΪ��ģʽ
	{
		UplinkStatus |= 0x01;
	}
	inline void SetSlaveServeMode() // ����ģʽΪ��ģʽ
	{
		UplinkStatus &= 0xFE;
	}
	inline bool IsMainMachine()
	{
		return ((UplinkStatus & 0x01) == 0x01);
	}
	inline void SetLogonStatus() // �Ѿ���¼�Ϻ�̨
	{
		UplinkStatus |= 0x02;
		if (!IsMainMachine())
		{
			SetJilianOnlineStatus();
		}
	}
	inline void SetLogoutStatus() // û��¼�Ϻ�̨
	{
		UplinkStatus &= 0xFD;
		if (!IsMainMachine())
		{
			SetJilianOnlineStatus();
		}
	}
	inline bool HasLogon()
	{
		return ((UplinkStatus & 0x02) == 0x02);
	}
	inline void SetJilianOnlineStatus() // ���ü�����״̬Ϊ����
	{
		UplinkStatus |= 0x04;
	}
	inline void SetJilianOfflineStatus() // ���ü�����״̬Ϊ������
	{
		UplinkStatus &= 0xFB;
	}
	inline bool IsJilianOnline()
	{
		return ((UplinkStatus & 0x04) == 0x04);
	}
	inline void SetModemOnlineStatus() // ����MODEM����״̬
	{
		UplinkStatus |= 0x08;
	}
	inline void SetModemOfflineStatus() // ����MODEM����״̬
	{
		UplinkStatus &= 0xF7;
	}
	inline bool IsModemOnline()
	{
		return ((UplinkStatus & 0x08) == 0x08);
	}
};

#endif
