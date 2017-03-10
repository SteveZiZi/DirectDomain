///*SysConfig.hͨ�õ�����*/

#ifndef INCLUDED_SYS_CONFIG
#define INCLUDED_SYS_CONFIG

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

//SysConfig�е����ý��ڳ���ʱ���ã���ʹװ�ò�����λҲ�����ʧ
struct SysConfig
{
	INT32U DeviceNo;			//װ�ñ��
	INT16U HardwareVerion;		//Ӳ���汾 //880A
	char m_softVerion[19+1];		//����汾�� 8809
	char ModuleSoftversion[19+1];

	//2006-6-8��־ǿ+ 
	//������Ͳο�<<�㶫������˾��ѹ�����û����г���ϵͳ���������й�Լ(����)2006-5-1ʵʩ�汾>> �������������ò����趨
	char RTUA[4+1];				//��������8016 
								//��������ַ ���8017
	INT32U m_RTUA;				//RTUA �������߼���ַ   A0 A1 B1 B2	  //2006-6-8��־ǿ+
	INT16U m_MSTA;				//RTUA ��վ��ַ��ַ   ����һ������	  //2006-6-8��־ǿ+
	//INT8U  m_BusType;               //����ͨѶ����
	char m_ChipAddr[6];
	INT8U ChipChannel;
	INT8U m_RouterMode;			//·��оƬ����ģʽ
	//char IP_ADDR[8];			//IP��ַ   8010
	//INT8U m_ChannelType;		//ͨ������	 8010 

	//char IP_ADDR_bk1[8];		//����IP��ַ1		
	//INT8U m_ChannelType_bk1;	//����ͨ������1	

	//char IP_ADDR_bk2[8];		//����IP��ַ2	
	//INT8U m_ChannelType_bk2;	//����ͨ������2
	int LCDPassword;

	char m_PhoneNum[16+1];			//�������ĺ���8013
	char m_Default_GateWay[16+6+1];	//Ĭ�����ص�ַ���ߴ����������ַ�Ͷ˿�8014


	char m_psw_Read[6+1];		//��ͨ����,ֻ��Ȩ��8020  
	char m_psw_lowSet[6+1];		//�ͼ�����Ȩ�� 8021 "111111" 
	char m_Admin_psw[6+1];		//����Ա���� ȱʡΪ��������ַ��BCD���ʾ 8022	"CMNET"
	

	INT16U m_Customs;			//��ͨ�û������� 8812
	INT16U m_MutilFunMeters;	//���׶๦�ܱ����� 8813
	INT8U  m_VIP;				//�ص㻧�� 8814
	INT8U m_TimeLock;			//��ʱ���� 8815	ʱ
	INT8U m_Space;				//ִ�м�� 8816 00H:30���� 01H:60���� 02H:120���� 03H:240���� 04H:360���� 05H:720���� 
	INT8U m_Relay;				//�м̷�ʽ 8817 00:�Զ��м� ����:�̶��м�
	char m_Alarm_gate[2+1];		//���𱨾���ֵ 8818 %

	INT8U m_ReadTime;			//�ն������ݳ���ʱ�� 8819 ʱ
	INT8U m_Reportable;			//VIP�����ϱ�ʱ�� FFΪ��ס���ϱ� Ĭ��ΪFF �ϴ����Ϊ24Сʱ 0-23Ϊ�����ϱ�ʱ�� 881A
	INT8U m_Alarm_Shield[4];		//����������881B��D7=1�������𳬷�ֵ������D6=1����ʧѹ������D5=1����ʧ��������D4=1����������򱨾���D3=1�������̱�����D2=1������ʧ�ܱ�����D1=1����Уʱʧ�ܱ�����D0=1����̵�����λ�¼�����
	char m_Lose_ToDay[3+1];		//���������� 881C
	char m_SideDJS[16+1];		//�Ӽ�������ַ 881D
	char m_SlaveDistribut[4+1];	//������ն˵�ַ8822
	//��־ǿע �˴�,Ϊʹ�ڴ��б������ʽΪHEX��ʽ,�����ļ���ֻ���õȼ۵�ascii��ʽ��.��HEX��21���������33.
	INT8U AlarmCheckStartTime;	//2006-8-7���������չ881F ����ֵ����������ʧ�ܱ�����Уʱʧ�ܱ���������ʱ�䣬Ĭ����ÿ��ҹ��23�㿪ʼ���
	INT16U m_Month_trail_read;	//��ĩ���ݳ��տ�ʼʱ�� 8820 DD:1-28 HH:1-23
	INT16U m_ReportTime;		//��ĩ�����ϴ�ʱ�� 8821 DD:1-28 HH:0-23
	//INT16U m_BroadcastSetTime;	//
	INT8U m_BroadcastSetTime[3];	//�㲥Уʱ��ʼʱ�䣬3���ֽڷֱ�洢��ʱ��
	INT8U m_BroadcastInterval;	//�㲥Уʱ��У������Է���Ϊ��λ
	INT16U MaxMeterCount;		//֧�ֵ��������8811
	INT8U WirelessModemType;	//MODEM�ͺ�,�μ�MODEM_TYPE
	//2006-9-10��־ǿ+ 	
	INT32U BeatReply;//������� ����
	INT32U BEATIMES;//��������  ��

	char RelaysVersion[4];		// laosu[2010-11-30] �������������ҵĲɼ����İ汾��

	INT8U GPRSDialMaxTimes;				//����ز�����������GPRS
	INT8U GPRSReConnectMaxTimes;		//�����������������GPRS
	INT8U GPRSAutoConnectStartDate;		//GPRS�Զ�������ʼ����
	INT8U GPRSAutoConnectStartHour;		//GPRS�Զ�������ʼʱ��
	INT8U GPRSAutoConnectEndDate;		//GPRS�Զ�������������
	INT8U GPRSAutoConnectEndHour;		//GPRS�Զ���������ʱ��
	INT8U GPRSAutoDialInterMinute;		//GPRS�Զ��ز��������λ������
	INT8U GPRSAutoConnectInterMinute;	//GPRS�Զ�������Ƶ�ʣ���λ������
	//INT32U GPRSMonthGeneralTraffics;	//GPRS����������
	// 1:�㶫�ն�������ͻ�䴦����:���춳�������outvalue���ȼۣ���������Ϊ���ݹ���outvalue = (float)(��һ��*(1.0+TOLERANCE)+2*���������ݵ�������ƽ��ֵ��    //   ֵ);          
    // 2���������ն�������ͻ�䴦��ʽ: ���������ݡ��������ݣ�>5���������ݡ�ǰ�����ݣ����ҵ����õ�������1000��ʱ��ѡ�����,����һ�춳�������滻
	INT8U m_Check_Daydata_Type;	
	INT8U m_XC_Protocol_Type;	// 1:N6+N12, 2:N12
	//2006-8-22�����+
	INT8U mBusType;

	/** �˿�ӳ���
	 */
	INT8U m_PortMap[SYSTEM_SERIAL_PORT_MAX]; ///< �˿�ӳ���

	inline const char* GetUserCode() const
	{
		return UserCode;
	}
	inline const char* GetPlanKey() const
	{
		return PlanKey;
	}
	const char* SetUserCode(const char* userCode);
	const char* SetPlanKey(const char* planKey);
	//2006-6-8��־ǿ+ 
	const char* Setm_PhoneNum(const char* m_phonenum);
	const char* bcd_to_asc_PhoneNum(const char* bcd8PhoneNum,int r=1);//1��ʾ������������0����
	const char* asc_to_bcd_PhoneNum(char *tep,int r=1);//1��ʾ������������0����
	const char* Setm_Default_GateWay(const char* m_default_gateway);
	const char* Setm_psw_Read(const char* m_psw_read);
	const char* Setm_psw_lowSet(const char* m_psw_lowset);
	const char* Setm_Admin_psw(const char* m_admin_psw);
	const char* Setm_softVerion(const char* m_softVerion);
	const char* Setm_Alarm_gate(const char* m_alarm_gate);
	const char* Setm_Lose_ToDay(const char* m_lose_today);
	const char* Setm_ReadTime(const char *m_readtime);
	//const char* Setm_Alarm_Door(const char* m_alarm_door);
	const char* Setm_Reportable(const char* m_reportable);
	const char* Setm_Alarm_Shield(const char* m_alarm_shield);
	const char* Setm_ReportTime(const char *m_readtime);
	const char* Setm_BroadcastSetTime(const char *m_readtime);
	const char* SetalarmCheckStartTime(const char *alarmCheckStartTime);

    const char* SetChipAddr(const char* rouble_addr);


	    const char*  Setm_ModuleSoftversion(const char* m_softverion);
		void    Getm_ModuleSoftversion(char* m_softverion);
	//const char* Set
	//2006-6-8��־ǿ+ 
private:
	char UserCode[CONS_USER_CODE_MAX_LEN+1];		//ϵͳ�û���ÿ��������һ�����
	char PlanKey[CONS_CONFIG_PLAN_KEY_MAX_LEN+1];	//ϵͳ���÷���
public:
	//int daylock,viplock,monthlock;
	//2006-6-8��־ǿ+
	const char* SetRTUA(const char* rtua);
	INT32U GetRTUA(void);
	void SetMSTA(INT16U RT);
	INT16U GetMSTA(void);
    //2006-6-8��־ǿ+ 	
	void Get_psw_Read(unsigned char *psw);
	void set_psw_read3(unsigned char *psw);
	//void set_psw_read7(char *psw);
	void Get_psw_lowSet(unsigned char *psw);
	void set_psw_lowSet3(unsigned char *psw);
	//void set_psw_read7(char *psw);
	void Get_Admin_psw(unsigned char *psw);
	void set_Admin_psw3(unsigned char *psw);

    void GetDeviceAddr(char addr[6]);

	void SetConnChannel(INT8U channelNO)
	{
		   ChipChannel = channelNO;
	}
	void GetConnChannel(INT8U &ChannelNo)
	{
		 ChannelNo = ChipChannel;
	}
	void SetRouterWorkMode(INT8U routerWorkMode)
	{
		m_RouterMode = routerWorkMode;
	}
	void GetRouterWorkMode(INT8U &routerWorkMode)
	{
		routerWorkMode = m_RouterMode;
	}
	void SaveBusAddr(INT8U *addr)
	{
		memcpy(m_ChipAddr,(char *)addr,sizeof(m_ChipAddr));
	}

	//void set_Admin_psw7(char *psw);
	inline SysConfig()
	{
		Init();
	}
	inline void Init()
	{
		memset(this,0,sizeof(*this));
	}

	inline bool HasChipAddr() const
	{
		char validaddr[6];
		memset(validaddr,0x00,6);
		return strncmp(m_ChipAddr,validaddr,6)==0?false:true;
	}
	KLDump& Dump(KLDump& dump) const;
};

#endif

