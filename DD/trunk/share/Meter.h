///*Meter.h�а�����������ϵĵ���������㣩�Ķ���*/
//������ţ���0��CONS_METER_MAX_COUNT-1

#ifndef INCLUDED_METER
#define INCLUDED_METER

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_STATUS
#include "Status.h"
#endif

#ifndef INCLUDED_PORT_SET
#include "../base/PortSet.h"
#endif

#ifndef INCLUDED_DATA_ID
#include "DataID.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

#ifndef INCLUDED_METER_GATHER_INFO
#include "MeterGatherInfo.h"
#endif

class KLDump;
//�յ����ɼ�ʱ��(3)���յ�������(4)���µ����ɼ�ʱ��(3)���µ�������(4)+���ɼ�ʧ������(1)+Уʱʧ�ܴ���(1)��
#define METER_GATHER_DATALEN 20
#define VIP_GATHER_DATALEN 8

struct Meter
{
	INT16U MeterNo;//���ţ��������,MeterNo��1��ʾ�๦���ܱ�	
	INT8U operatecode;//��������
	INT8U Enable;//��Ч�ԣ�1��ʾ��Ч��0��Ч
	INT8U BusNo;//���ߺţ�0��ʾδ֪,��1��CONS_DBCJ_MAX_COUNT
	INT8U RuleID;//��ԼID��0��ʾ��Ч
	INT8U MeterType;//���ԣ��μ�METER_TYPE	������ͣ�1��ʾ��ͨ�û���2��ʾ���ܱ�3�ܱ�
	INT8U Prop;//���ԣ��μ�METER_PROPERTY	������ԣ�1��ʾ��ͨ485��6��ʾ�ز�ͨѶ��7��ʾ���׶๦�ܱ�
	//INT8U PowerOffFlag;//�Ƿ�߱�ͣ���¼�
	//INT8U VolOverLoadFlag;//�Ƿ�߱���ѹ����
	INT8U  PortNo;//�˿ں�
	INT8U ConnectType;//���ԣ��μ�CONNECT_TYPE	���ͨѶ����

	char  Addr[CONS_METER_ADDR_MAX_LEN+1];//485���ӱ�ͨѶ��ַ
	struct PortSet PortSet;//�˿�����

	/*�ֱ��˵����
	�ֱ�ŵ�1~5�ֽ�ΪASC�룬��6~9�ֽ�Ϊѹ��BCD�루ÿ2λ����0~9ռ��һ�ֽڣ���
	��10ʮ�ֽ��Զ���д��ʮ��������ġ�00��������Ϊѹ����ŵı�־
	�Ϸ���ţ� 	XXXXX 12345678
     ��XXXXX����ʾASC�룬��Ӣ����ĸ�����ֵȾ��ɣ���12345678����ʾ0~9����	
	*/
	char  BureauNo[CONS_METER_ADDR_MAX_LEN+3];//�ֱ��1��485�ܱ�,2��485�ֱ�,3���ز�
	INT8U CompressFlag;//�ֱ�ŵ�ѹ����־

	char  User[CONS_METER_USER_MAX_LEN+1];//�û���
	char  Pwd[CONS_METER_PWD_MAX_LEN+1];//����

	INT8U SetRegGrade;//���������Ĵ�������������բ��Ȩ�޵ȼ�
	char  SetRegPwd[CONS_METER_PWD_MAX_LEN+1];//���������Ĵ�������������բ������
	INT8U BrakeFlag;//��բ���ܣ�0��ʾ������բ��1��ʾ����բ
	INT32U CT;//��������ϵ����0��ʾδ֪
	INT32U PT;//��ѹ����ϵ����0��ʾδ֪	
	
	INT16S CommRetryTimes;//ͨѶ���Դ�����-1��ʾ���ù�ԼĬ�ϵ�ֵ
	
	INT8U PrefixChar;//ǰ���ַ�
	INT8U PrefixCount;//ǰ���ַ�����	

	INT32U Option;//ѡ�����չ��


	//�����Ҫ�ɼ���������
	DataBitSet RegDBS;//��������Ĵ���

	//����������������ɼ���Ϣ
	MeterGatherInfo GatherInfo[DATA_TYPE_METER_MAX_VALUE+1];//�����Ĵ���������¼�������ʱ
   	INT32U CommOkCount;//ͨѶ�ɹ�����
	INT8U BrakeChangeFlag;//�̵�����λ��־,0x00�ޱ�λ,0x01,��բ,0x02��բ	
	INT16U RelayNo;//�ɼ��ն˺�
	INT8U SetTimeFailTimes;
	INT8U Phase;

public:
	static bool IsValidNo(INT32U MeterNo)
	{
		return MeterNo<CONS_METER_MAX_COUNT;
	}
	inline bool IsValid() const 
	{
		return (Enable>0 &&
			ConnectType!=METER_CONNECTTYPE_UNKNOWN &&
			Prop!=METER_PROPERTY_UNKNOWN &&
			Prop!=METER_PROPERTY_JXB &&
			RuleID!=METER_RULE_UNKNOWN);
	}
	inline bool IsForBus(INT8U cBusNo) const
	{
		return BusNo==cBusNo;
	}
	inline INT8U GetCommMaxTimes() const
	{
		return CommRetryTimes<0?3:(CommRetryTimes+1);
	}
	inline bool CanRealRead()
	{
		return true;
	}

 INT8U GetMeterRuleID() const ;
#if 0
	inline INT8U GetMeterRuleID() const 
	{		
		
	
		   if (RuleID==METER_RULE_GB645)
			{
				return METER_RULE_GB645;
			}
			else if(RuleID==METER_RULE_GD645)
			{		
				return METER_RULE_GD645;			
			}
			else
		      return RuleID;
	}
#endif

	bool IsNeedTouChuan()
	{
		if (BusNo==DBCJ_3_BUSRULE_GENERAL)
		{
			if (RuleID!=METER_RULE_GB645 && RuleID!=METER_RULE_GB2007 &&RuleID!=METER_RULE_GD645)
			{
				return true;
			}
		}
		return false;
	}

	void Init(INT16U meterNo);	
	void InitGatherInfo();
	void OnChanged();
	KLDump& BaseDump(KLDump& dump) const;
	KLDump& Dump(KLDump& dump) const;
	bool GetAddr_Bcd(unsigned char *addr,int which_0_to_4=0);
	int OnCommand(KLDump& dump,const char* cmdKey,const char* objName,INT8U id,const char *propName,const char* propVal);
	bool SetPassWord(char temp[CONS_METER_ADDR_MAX_LEN+1]);
	char* GetPassword(char temp[CONS_METER_ADDR_MAX_LEN+1]);
	bool lsValidBureauNo();
	void GetAddr_6(unsigned char *ipDest);

	//�������ж�������������TN�ź�ͨѶ��ַ�����Ƿ����
	bool operator==(Meter& meter);
	//Meter* operator=(Meter& meter);
//private:
//	bool IsGBMeter();
};

//struct MeterCount
//{
//public:
//	/*�����б����벻Ҫ����Ķ�*/
//	int vipmc,usemc,zb;//�����ص㻧�������û��������ܱ���
//	int jydgn;//���м��׶๦�ܱ���
//	void init();
//};
//

struct Meters
{
	MeterBitSet MBS;
	MeterBitSet ValidMBS;//��Ч�ĵ��
	MeterBitSet UserMBS;//�û���
	MeterBitSet VIPMBS;//�ص��û�
	MeterBitSet JYDGNMBS;//���׶๦��

	Meter Item[CONS_METER_MAX_COUNT];
	time_t LastFsDataTime[CONS_METER_MAX_COUNT];//���һ�η�ʱ����ʱ��

	time_t ChangeTime;//����ʱ��
	bool  IsCheckTime;//�Ƿ��Ѿ�Уʱ,�����Уʱ,Ϊtrue

public:

	void Init();
	int HasZBMeter() const;
	void OnChanged();
	KLDump& BaseDump(KLDump& dump) const;
	KLDump& Dump(KLDump& dump) const;

	int OnCommand(KLDump& dump,const char* cmdKey,const char* objName,INT8U id,const char *propName,const char* propVal);
	bool GetTnChar(bstring temp,unsigned char *tn);
	bool GetTnInt(bstring temp,unsigned int *tn);
	INT16U GetUserMeterTN(char * VIPAddr);
	void altmeter(INT32U code,int how);
	void DelMeter(INT16U tn);
	//void countmeters(struct MeterCount &mc);
private:
	int Load(const char* cfgFile);
};

/***************************************     DEVICE    ****************************************/
#define NAME_SET_device "device.ini"	



typedef enum
{
	MD_RCU=0,
	MD_AIRCONDITION=1,
	MD_RELAY_8=2,
	MD_LIGHT_4=3,
	MD_RELAY_2=4,
	MD_LEDV12_3=5,
	MD_DOORDISPLAY=6,
	MD_RFID_CARD=7,
	MD_KEYBOARD_20=8,
	MD_RTF =9,
	MD_LUX_SENSOR=10,
	MD_TYPE_MAX,
}T_emDeviceType;



struct NoBitSet:public FastBitSet<CONS_MAP_DEVICE_SUBNO_MAX_COUNT>
{
};

struct DevicesBitSet:public FastBitSet<CONS_DEVICE_MAX_COUNT>
{
};


struct MapItem
{
	INT8U m_bValid;
	INT8U m_subno;
	INT8U m_funcType;//�����⹦�ܶ����//bit0~5:������,bit7:1,IORELAYһһ��Ӧ�������ӳ���,0:������Ҫ��
	INT8U m_mapTn;
	NoBitSet m_mapNbs;
};

struct DeviceFlags
{
	INT8U m_bDeviceParamRefresh;
};

struct Device
{
	INT8U m_Enable;//��Ч�ԣ�1��ʾ��Ч��0��Ч
	INT8U m_Tn;//���ţ��������,MeterNo��1��ʾ�๦���ܱ�	
	INT8U m_Kind;//��������
	INT8U m_Type;//�豸������
	char  m_Addr[CONS_METER_ADDR_MAX_LEN+1];//485���ӱ�ͨѶ��ַ
	struct PortSet m_PortSet;//�˿�����
	INT8U m_MapItemCount;
	MapItem m_MapItems[CONS_MAP_DEVICE_SUBNO_MAX_COUNT];
	INT8U m_StatusData[8];
	INT8U m_ClRelayNo;//��·��
	INT8U m_CommBreakTimes;//ͨѶ�жϴ���
	INT32U m_LuxValue;//��ж�
	INT32U m_LuxRate;//��б���
	time_t m_ResumeOnlineRefreshTime;//�������ָ����߻ָ��������õ�ѹʱ��
	
public:
	static bool IsValidNo(INT8U MeasureNo)
	{
		return MeasureNo<CONS_DEVICE_MAX_COUNT;
	}
	inline bool IsValid() const 
	{
		if (m_Enable>0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Init(INT8U MeasureNo)
	{
		memset(this,0x00,sizeof(*this));
	}
//	void OnChanged();
	bool GetAddr_Bcd(unsigned char *addr,int which_0_to_4=0);
	void GetAddr_6(unsigned char *ipDest);
};

struct Devices
{
	DevicesBitSet m_DBS;
	DevicesBitSet m_RtfBS;
	DevicesBitSet m_LuxSensorBS;
	DevicesBitSet m_RelayBS;//�̵���
	Device m_Item[CONS_DEVICE_MAX_COUNT];
public:
	void Init();
};



#endif

