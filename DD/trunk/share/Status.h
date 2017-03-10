///*Status.hϵͳ״̬��ʹ�ù����ڴ�*/

#ifndef INCLUDED_STATUS
#define INCLUDED_STATUS

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_SHARE_MEM
#include "../base/ShareMem.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

#ifndef __KLB_BASE_H
#include "../base/klb_base.h"
#endif

//#include "nortask.h"

#include "Channel.h"
#define LAST_RECORD_NUM 5
#define RECORD_NUM 2 //��¼��������
#define MAX_METER_DATA 0xF4240	//���ĵ�����ݣ�1000000��(XXXXXX.XX��BCD������ʾ��ô��)
#define OVERLOAD 2	//������������
#define GATHER_RETRY_TIMES 0x03
#define TOLERANCE 0.02
#define LW_TIME "lwtime"
#define VIP_TIME "viptime"
#define DAY_TIME "daytime"
#define MONTH_TIME "monthtime"

#define CONFIG_PORT_NUM 1
class KLDump;

struct ReportStatus
{
	void Init()
	{
		memset(LastReportTime,0x00,3);
	}
	char LastReportTime[3];

	inline void Set(char *itLastReportTime)
	{
		if (itLastReportTime != NULL)
		{
			memcpy(LastReportTime,itLastReportTime,3);
		}
	}
};
struct PortFuncSet
{
	INT8U PortNo;
	INT32U Baud;	/*115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600,300*/
	INT8S DataBit; /*5,6,7,8 */
	INT8S StopBit; /*1->1λ,2->2λ,3->1.5λ*/
	INT8S Parity; /*0*/
	INT8U Function;//0,����ڣ�1��������2������
};

struct MeterStatus
{
	INT8U CommTrouble;//ͨѶ����,0��ʾ�޹��ϣ���0��ʾ�й���
	INT8U TimeOverDiff;//ʱ�䳬�0��ʾ��������0��ʾʱ�䳬��
	
	INT16U GatherFailTimes;//����ʧ�ܴ���

	INT8U SetTimeFailTimes:2;//���Уʱʧ�ܴ�����2λ��ʾ����
	INT8U VIPUnexpectedTimes:2;//�ص㻧�쳣����
	INT8U DayUnexpectedTimes:2;//�������쳣����
	INT8U MonthUnexpectedTimes:2;//�������쳣����

	
	
	time_t LastVIPRecordTime;//�ϴγ����ص㻧ʱ��
	float LastVIPData[LAST_RECORD_NUM];//��5���ص㻧����
	
	time_t LastDayRecordTime;//�ϴγ���������ʱ��
	float LastDayData[LAST_RECORD_NUM];//��5��������
	float LastTwoDayData[RECORD_NUM];//��2��������
	time_t LastMonthRecordTime;//�ϴγ���������ʱ��
	float LastMonthData[LAST_RECORD_NUM];//��5��������

	time_t LastCommTime;	

	//����ϴ�ͣ�����������,0xFFFF��ʾδ֪����Ƿ�֧��ͣ�ϵ��¼���0xFFEE��ʾ���֧��ͣ�ϵ��¼�
	//����ֵ��ʾ���֧��ͣ�ϵ��¼�
	INT16U PowerOffNum;

	/* �ñ����Ҫ������Ӧ�����Ժ���ԣ���¼����������ּ������ĳ���ڣ���ʵ�ʼ����������У��ǲ���Ҫ���ݸ��жϵģ�
   Ϊ�˲��ı����еĵ�������ļ���������������Ѹü�¼��ʧ��������status�б����λ״̬��*/
	INT8U MeterGatherPortType;   

	MeterStatus();

	
	inline void GetLastVIPTime(char time[7])
	{
		Time_tTimeToBCDTime7(LastVIPRecordTime, time);
	}
	inline void Init()
	{
		SetTimeFailTimes=0;
		GatherFailTimes=0;		
		VIPUnexpectedTimes=0;
		DayUnexpectedTimes=0;
		MonthUnexpectedTimes=0;
		LastCommTime=0;
		//suxufeng:[2010-4-29]�޸�Ϊtime_t��ʽ���������������
		LastVIPRecordTime = 0;
		LastDayRecordTime = 0;
		LastMonthRecordTime = 0;
		for (int i=0; i<LAST_RECORD_NUM; i++)
		{
			LastVIPData[i]=MAX_METER_DATA;
			LastDayData[i]=MAX_METER_DATA;
			LastMonthData[i]=MAX_METER_DATA;
		}
		PowerOffNum=0xFFFF;
	}
	inline void AddMeterStatus()
	{
		Init();
	}

	inline bool IsCommFailsMore3Days()
	{
		time_t nowtime;
		time(&nowtime);	
		if (LastCommTime==0)
		{
			return false;
		}		
		return (datediff(4,LastCommTime,nowtime)>=CONS_GATHER_FAIL_ALARM_DAYS);
	}
	inline bool ChangeData(char rTime[7],DATA_TYPE type)
	{
		//suxufeng:[2010-4-29]�޸�Ϊtime_t��ʽ���������������
		switch (type)
		{
		case DATA_TYPE_VIP:
			LastVIPRecordTime = Time_BCDTime7TotTime(rTime);
			break;
		case DATA_TYPE_DAY:
			LastDayRecordTime = Time_BCDTime7TotTime(rTime);
			break;
		case DATA_TYPE_MONTH:
			LastMonthRecordTime = Time_BCDTime7TotTime(rTime);
			break;
		}
		return true;
	}
	int CheckTime(DATA_TYPE type,const char rTime[7]);


	bool UpdateRecordTime(DATA_TYPE type,const time_t t_recordtime=0);
	int CheckMeterData(DATA_TYPE type,INT8U record[8],bool UpdateFlag=false,INT8U IntervalValue=1);
	
	KLDump& Dump(KLDump& dump) const;
private:
	int GetNextData(const float* inData,int len,float &outData);
	int GetReferenceValue(const float* inData,int len,float &outData);
	bool GetVIPRecordTime7(INT8U day,INT8U hour,INT8U recordTime[7]);
	int CheckVIPData(INT8U record[8],bool UpdateFlag=false,INT8U IntervalValue=1);
	int CheckDayData(INT8U record[8],bool UpdateFlag=false,INT8U IntervalValue=1);
	int CheckSZDayData(INT8U record[8],bool UpdateFlag,INT8U IntervalValue);

	int CheckMonthData(const INT8U record[8],bool UpdateFlag=false);

};


class KLDump;



/* RCU�豸״̬��Ϣ*/
#define CONS_COMPESE_MEMBER_MAX_COUNT	64 //���64����Աһ�����
#define CONS_SCENE_COMPOSE_MAX_COUNT 7 //���3����Ϸ���

#define CONS_TOTALPOWER_COMPESE_MEMBER_MAX_COUNT		64
#define CONS_DOOR_COMPESE_MEMBER_MAX_COUNT		4

/* ���ܲ���*/
#define CONS_SMART_GARAGE_MAX_COUNT			16


/***************/


struct SSceneComposeMember
{
	INT8U m_DeviceNo;
	INT8U m_SubNo;
	INT8U m_Function;
	INT8U m_FuncData[4];
};

struct SSceneCompose
{
	INT8U m_ComposeState;//��ϵƿ���״̬
	INT8U m_MemberNum;
	SSceneComposeMember m_SceneComposeMember[CONS_TOTALPOWER_COMPESE_MEMBER_MAX_COUNT];
};

struct SDoorSceneCompose
{
	INT8U m_ComposeState;//��ϵƿ���״̬
	INT8U m_MemberNum;
	SSceneComposeMember m_SceneComposeMember[CONS_DOOR_COMPESE_MEMBER_MAX_COUNT];
};
/*
��һ�ֽڣ�
bit0������ť״̬��0��δ���£�1���Ѱ���
Bit1�����ſ�����״̬��0��δ�忨��1�Ѳ忨
Bit2 ������Ŵż�⣺0�����Źأ�1���ſ�
Bit3 �ܵ�Դ����״̬��0���أ�1������
Bit4 �����䰴ť״̬0���أ�1������
Bit5 ϴ�·���ť״̬0���أ�1������
Bit6 �������״̬0���أ�1������
Bit7 ���Ժ�״̬0���أ�1������
�ڶ��ֽڣ�
bit0�����⣺0�����ˣ�1������
Bit1�������0��������1�������
Bit2 ��̨�Ŵţ�0����̨�Źأ�1��̨�ſ�
Bit3 ҹ����0���أ�1������
Bit4 ������0���أ�1������
Bit5 �����˷�0���أ�1������
Bit6 ���� 0���أ�1������
Bit7 ά����0���أ�1������
*/

/* RCU ��ģ��״̬*/
struct SRcuStatus
{
	//section 1,
	INT8U m_UrgentButtonState;//������ť״̬
	INT8U m_DoorCardState;//��������״̬
	INT8U m_DoorState;//���ſ���״̬
	INT8U m_PowerSwitchState;// �ܵ�Դ����״̬
	INT8U m_CleanRoomButtonState;//�����䰴ť״̬
	INT8U m_WashCloseButtonState;//ϴ�°�ť״̬
	INT8U m_DonotDisturButtonState;//������Ű�ť״̬
	INT8U m_PleaseWait;//���Ժ�
	//section 2
	INT8U m_IrdaDetect;//,1�������
	INT8U m_Service;//�������
	INT8U m_OutWindow;//��̨�Ŵ�״̬
	INT8U m_BedService;//ҹ������	
	INT8U m_SafeBox;//������״̬
	INT8U m_ExitRoom;//�����˷�
	INT8U m_Paylist;//����
	INT8U m_Fixing;//ά����
};

struct SAirConditionDevices
{
	/* �յ�����״̬*/
	INT8U m_AirSeason;
	INT8U m_AirWorkMode;
	INT8U m_AirSetTemperture;
	//INT8U m_Air_Degree_low;//�յ�����¶�
	//INT8U m_Air_Degree_high;//�յ�����¶�
	INT8U m_AirWindSpeed;//�յ�����

	/* �յ�����¶�*/
	INT8U m_AirTemperature;
	//INT8U m_AirSumerWorkTemperature;//�յ����칤���¶�
	//INT8U m_AirWinterWorkTemperature;//�յ����칤���¶�
};

/* �̿��������*/
struct STaskPlanStatus
{
	INT8U m_ActionedInDay;//�����ͬһ����ִ�й���������ִ��
};

struct SNorTaskStatus
{
	STaskPlanStatus m_PlanStatus[CONS_TASKPLAN_MAX_COUNT];	
};
/* ����������*/
struct SLuxTaskStatus
{
	time_t  LuxTasksStartTime[CONS_LUXTASK_MAX_COUNT];
	time_t  LuxTasksState[CONS_LUXTASK_MAX_COUNT];
	
};

/* �����������*/
struct SConstantLuxTaskStatus
{
	INT32U  AdjustedClVol;
};

/* ��·���*/
struct SCLStatus
{
	INT8U  m_ClleakState[CONS_CLRELAY_MAX_COUNT];
	time_t m_ClleakActionTime[CONS_CLRELAY_MAX_COUNT];
public:
	INT8U GetClLeakState(INT8U clno){return m_ClleakState[clno];}
	bool SetClLeakActionTime(INT8U clno, time_t tt);
	time_t GetClLeakActionTime(INT8U clno);
	bool SetClLeakState(INT8U clno,INT8U leakState);
};
struct SDeviceStatus
{

	SRcuStatus m_RcuStatus;//RCU��ģ��״̬
	
	INT8U m_Nightwide[4];

	/* ��������״̬*/
	INT8U m_WindowRunTime;
	/* ��ͷ�����Ȱٷֱ�*/
	INT8U m_LightLevelRate;

	SAirConditionDevices m_AirConditionDevices[CONS_DEVICE_MAX_COUNT];
	/* ��״̬*/
	INT8U m_NightLightState;//ҹ��״̬
	INT8U m_GalleryLightState;//�ȵ�״̬

	/* ����ȡ������*/
	INT8U m_DoorCardType;

	/* ������ʾ����*/
	INT8U m_DoorDisplayType;//0:RCU���ƣ�1������ģ�����

	/* �յ�ģ������*/
	INT8U m_AirConditionType;//0:�ɿؿյ���壬1�Ƿǿɿؿյ����

	/* �������*/
	struct SSceneCompose m_SceneComposes[CONS_SCENE_COMPOSE_MAX_COUNT];
	INT8U m_CurrentOpenedSceneNo;// ��ǰ�Ѵ򿪵ĳ�����

	/* �ܵ�Դ��Ͽ���*/
	struct SSceneCompose m_PowerSceneComposes;

	/* �ſ���Ͽ���*/
	struct SSceneCompose m_DoorcardInSceneComposes;//����
	struct SSceneCompose m_DoorcardOutSceneComposes;//�γ�

	/* ������Ͽ���*/
	struct SDoorSceneCompose m_DoorSceneComposes;

	/* �����������Ͽ���*/
	struct SSceneCompose m_IRInSceneComposes;//����
	struct SSceneCompose m_IROutSceneComposes;//����?

	/* ����������ж�ʱ��*/
	INT8U m_IrdaDetectTime;//����

	/* �ο�����ж�ʱ��*/
	INT8U m_OutCardDetectTime;//����

	/* ���������*/
	INT8U m_TeleNo[6];//����Ա����
	INT16U m_ArgVol;//�����ѹֵ
	INT16U m_ArgCur;//������ֵ
	INT8U  m_ClArgFlag[CONS_CLRELAY_MAX_COUNT];//��·����״̬
	time_t m_ClRelayManualCtlEndTime[CONS_CLRELAY_MAX_COUNT+1];//��·�ֶ�����ʱЧʱ��	
	INT8U m_ClRelayManualCtlValidTime[CONS_CLRELAY_MAX_COUNT+1];//��·�ֶ�����ʱЧ����
	SSmartGarageItem m_ClRelayManualCtlRecoverActionItem[CONS_CLRELAY_MAX_COUNT+1];//��·�ֶ�����ʱЧʱ�����Ļָ�����	
	SSmartGarageItem m_ClRelayManualCtlActionItem[CONS_CLRELAY_MAX_COUNT+1];//��·�ֶ�����ʱЧʱ���ڵĶ���	
	/* ������� */		
	SNorTaskStatus m_NortaskStatus[CONS_NORTASK_MAX_COUNT];

	/* ���������� */
	SLuxTaskStatus m_LuxTaskStatus;

	/* ����������� */
	SConstantLuxTaskStatus m_ConstantLuxTaskStatus;

	/* ��·���*/
	SCLStatus m_CLStatus;
	
	void Init()
	{
		m_Nightwide[3] = 0x18;
		m_Nightwide[2] = 0x00;
		m_Nightwide[1] = 0x07;
		m_Nightwide[0] = 0x00;
		for (int i=2; i<CONS_DEVICE_MAX_COUNT;i++)			
		{
			//m_AirConditionDevices[i].m_Air_Degree_low = 16;
			//m_AirConditionDevices[i].m_Air_Degree_high = 32;
			m_AirConditionDevices[i].m_AirWindSpeed = 0;
			m_AirConditionDevices[i].m_AirSeason = SEASON_SUMMER;//�ļ�
			m_AirConditionDevices[i].m_AirWorkMode = 1;	//����
			m_AirConditionDevices[i].m_AirSetTemperture = 25; //�յ������¶�
		}
		m_WindowRunTime = 0x3c;//60��
		m_NightLightState=0x0;
		m_GalleryLightState=0x0;
		m_DoorCardType = 0x0;
		m_DoorDisplayType = 0x0;
		m_IrdaDetectTime = 10;
		m_OutCardDetectTime = 30;
		
		m_RcuStatus.m_PowerSwitchState = 0;		
		m_RcuStatus.m_CleanRoomButtonState = 0x0;
		m_RcuStatus.m_DonotDisturButtonState = 0x0;
		m_RcuStatus.m_IrdaDetect = 0x0;
		memset(m_TeleNo,0x00,6);
		memset(m_NortaskStatus,0x00,sizeof(m_NortaskStatus));
		
		m_ArgVol = 0x2580;//Ĭ��ֵ258V
		m_ArgCur = 0x2000;//Ĭ������20A
	}
	bool IsNight();
};

//��status��ֵʱ��סһ��Ҫ����OnChanged�����������ɱ��治��
struct Status
{
	struct HeaderChecker Checker;
	struct MeterStatus MeterStatus[CONS_METER_MAX_COUNT];

	struct ReportStatus ReportStatus_Month[CONS_METER_COUNT_NO_VIP];
	struct ReportStatus ReportStatus_Day[CONS_METER_COUNT_NO_VIP];	

	time_t t_LastLineWasteAlarm;
	char LastGatherFailAlarm;
	char LastReportDayTime;//�ϴ��ϱ�������ʱ��
	char LastReportMonthTime;//�ϴ��ϱ�������ʱ��
	//�ϴ�������������
	INT8U LastClearLineWasteAlarm;
	INT8U CurDay;

	INT8U DayTaskStatus;
	INT8U MonthTaskStatus;
	Channel GatheredChannel;

	/* start:��Ϊ���Ժ������*/
	PortFuncSet	m_PortFunc[CONFIG_PORT_NUM];
	INT8U m_ZJYS[CONS_METER_MAX_COUNT][7];
	INT8U Phase[CONS_METER_MAX_COUNT];//�����λ
	/* end */

	SDeviceStatus deviceStatus;
	
	Status();
	static Status& Instance();
	int OnCommand(KLDump& dump,const char* cmdKey,const char* objName,INT16U id,const char *propName,const char* propVal);
	int OnSet(INT16U id,const char *propName,const char* propVal,void* pOther);
	int OnSetReportDay(INT16U id,const char *propName,const char *propVal,void *pOther);
	int OnSetReportMonth(INT16U id,const char *propName,const char *propVal,void *pOther);
	//int OnSetReportVip(INT16U id,const char *propName,const char *propVal,void *pOther);
	void OnChanged();
	int Save(bool ibForce);
	inline INT16U GetFailTimes(INT16U meterNo)
	{
		return (MeterStatus[meterNo].GatherFailTimes);
	}
	inline void ResetFailTimes(INT16U meterNo)
	{
		MeterStatus[meterNo].GatherFailTimes = 0;
	}
	inline void GetLastRecordTime(INT16U meterNo,char chTime[7],DATA_TYPE type)
	{
		//suxufeng:[2010-4-29]�޸�Ϊtime_t��ʽ���������������
		switch (type)
		{
		case DATA_TYPE_VIP:
			Time_tTimeToBCDTime7(MeterStatus[meterNo].LastVIPRecordTime,chTime);
			break;
		case DATA_TYPE_DAY:
			Time_tTimeToBCDTime7(MeterStatus[meterNo].LastDayRecordTime,chTime);
			break;
		case DATA_TYPE_MONTH:
			Time_tTimeToBCDTime7(MeterStatus[meterNo].LastMonthRecordTime,chTime);
			break;
		default:
			ASSERT(false);
		}
	}
	inline void AddMeterStatus(INT16U meterNo)
	{
		return MeterStatus[meterNo].AddMeterStatus();
	}
	inline bool ChangeMeterData(INT16U meterNo,char rTime[7],DATA_TYPE type)
	{
		return MeterStatus[meterNo].ChangeData(rTime,type);
	}

private:
	INT8U DirtyFlag;
	static bool InitFile(FILE* fp, void* ipOther);
	static bool CheckFile(FILE* fp, void* ipOther);
	static ShareMem m_ShareMem;
	bool WriteAlarm(INT16U meterNo,float data,DATA_TYPE type,const char rTime[7]);//д�����쳣�澯�ļ�
	void SetStatus(INT16U id,const string& name,const string& val);
	void GetTimeFormStr(const string& val,char tTime[7]);
};

#define STATUS Status::Instance()
#define DECLARE_STATUS(status) Status& status=STATUS


#endif
