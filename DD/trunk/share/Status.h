///*Status.h系统状态，使用共享内存*/

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
#define RECORD_NUM 2 //记录上两天数
#define MAX_METER_DATA 0xF4240	//最大的电表数据，1000000度(XXXXXX.XX的BCD码最多表示这么多)
#define OVERLOAD 2	//电表电量过大倍数
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
	INT8S StopBit; /*1->1位,2->2位,3->1.5位*/
	INT8S Parity; /*0*/
	INT8U Function;//0,抄表口，1，级联，2被抄口
};

struct MeterStatus
{
	INT8U CommTrouble;//通讯故障,0表示无故障，非0表示有故障
	INT8U TimeOverDiff;//时间超差，0表示正常，非0表示时间超差
	
	INT16U GatherFailTimes;//抄表失败次数

	INT8U SetTimeFailTimes:2;//电表校时失败次数，2位表示即可
	INT8U VIPUnexpectedTimes:2;//重点户异常次数
	INT8U DayUnexpectedTimes:2;//日数据异常次数
	INT8U MonthUnexpectedTimes:2;//月数据异常次数

	
	
	time_t LastVIPRecordTime;//上次抄收重点户时间
	float LastVIPData[LAST_RECORD_NUM];//上5次重点户数据
	
	time_t LastDayRecordTime;//上次抄收日数据时间
	float LastDayData[LAST_RECORD_NUM];//上5次日数据
	float LastTwoDayData[RECORD_NUM];//上2次日数据
	time_t LastMonthRecordTime;//上次抄收月数据时间
	float LastMonthData[LAST_RECORD_NUM];//上5次月数据

	time_t LastCommTime;	

	//电表上次停电事情的数据,0xFFFF表示未知电表是否支持停上电事件，0xFFEE表示电表不支持停上电事件
	//其它值表示电表支持停上电事件
	INT16U PowerOffNum;

	/* 该标记主要是用来应付电科院测试，记录电表属于哪种集中器的抄表口，在实际集中器程序中，是不需要根据该判断的，
   为了不改变现有的电表配置文件，而担心重启会把该记录丢失，所以在status中保存该位状态。*/
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
		//suxufeng:[2010-4-29]修改为time_t格式，提高任务检查性能
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
		//suxufeng:[2010-4-29]修改为time_t格式，提高任务检查性能
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



/* RCU设备状态信息*/
#define CONS_COMPESE_MEMBER_MAX_COUNT	64 //最多64个成员一个组合
#define CONS_SCENE_COMPOSE_MAX_COUNT 7 //最多3个组合方案

#define CONS_TOTALPOWER_COMPESE_MEMBER_MAX_COUNT		64
#define CONS_DOOR_COMPESE_MEMBER_MAX_COUNT		4

/* 智能插座*/
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
	INT8U m_ComposeState;//组合灯开关状态
	INT8U m_MemberNum;
	SSceneComposeMember m_SceneComposeMember[CONS_TOTALPOWER_COMPESE_MEMBER_MAX_COUNT];
};

struct SDoorSceneCompose
{
	INT8U m_ComposeState;//组合灯开关状态
	INT8U m_MemberNum;
	SSceneComposeMember m_SceneComposeMember[CONS_DOOR_COMPESE_MEMBER_MAX_COUNT];
};
/*
第一字节：
bit0紧急按钮状态：0，未按下，1，已按下
Bit1房间门卡插入状态：0，未插卡，1已插卡
Bit2 房间大门磁检测：0，大门关，1大门开
Bit3 总电源开关状态：0：关，1：开。
Bit4 清理房间按钮状态0：关，1：开。
Bit5 洗衣服务按钮状态0：关，1：开。
Bit6 请勿打扰状态0：关，1：开。
Bit7 请稍后状态0：关，1：开。
第二字节：
bit0红外检测：0，无人，1，有人
Bit1请求服务：0，无请求，1请求服务
Bit2 阳台门磁：0，阳台门关，1阳台门开
Bit3 夜床：0：关，1：开。
Bit4 保险箱0：关，1：开。
Bit5 请求退房0：关，1：开。
Bit6 结账 0：关，1：开。
Bit7 维修中0：关，1：开。
*/

/* RCU 主模块状态*/
struct SRcuStatus
{
	//section 1,
	INT8U m_UrgentButtonState;//紧急按钮状态
	INT8U m_DoorCardState;//房卡插入状态
	INT8U m_DoorState;//大门开关状态
	INT8U m_PowerSwitchState;// 总电源开关状态
	INT8U m_CleanRoomButtonState;//清理房间按钮状态
	INT8U m_WashCloseButtonState;//洗衣按钮状态
	INT8U m_DonotDisturButtonState;//请勿打扰按钮状态
	INT8U m_PleaseWait;//请稍后
	//section 2
	INT8U m_IrdaDetect;//,1检测有人
	INT8U m_Service;//请求服务
	INT8U m_OutWindow;//阳台门磁状态
	INT8U m_BedService;//夜床服务	
	INT8U m_SafeBox;//保险箱状态
	INT8U m_ExitRoom;//请求退房
	INT8U m_Paylist;//结账
	INT8U m_Fixing;//维修中
};

struct SAirConditionDevices
{
	/* 空调运行状态*/
	INT8U m_AirSeason;
	INT8U m_AirWorkMode;
	INT8U m_AirSetTemperture;
	//INT8U m_Air_Degree_low;//空调最低温度
	//INT8U m_Air_Degree_high;//空调最高温度
	INT8U m_AirWindSpeed;//空调风速

	/* 空调检测温度*/
	INT8U m_AirTemperature;
	//INT8U m_AirSumerWorkTemperature;//空调夏天工作温度
	//INT8U m_AirWinterWorkTemperature;//空调冬天工作温度
};

/* 程控任务相关*/
struct STaskPlanStatus
{
	INT8U m_ActionedInDay;//如果在同一天内执行过，将不再执行
};

struct SNorTaskStatus
{
	STaskPlanStatus m_PlanStatus[CONS_TASKPLAN_MAX_COUNT];	
};
/* 光控任务相关*/
struct SLuxTaskStatus
{
	time_t  LuxTasksStartTime[CONS_LUXTASK_MAX_COUNT];
	time_t  LuxTasksState[CONS_LUXTASK_MAX_COUNT];
	
};

/* 恒照任务相关*/
struct SConstantLuxTaskStatus
{
	INT32U  AdjustedClVol;
};

/* 回路相关*/
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

	SRcuStatus m_RcuStatus;//RCU主模块状态
	
	INT8U m_Nightwide[4];

	/* 窗帘控制状态*/
	INT8U m_WindowRunTime;
	/* 床头灯亮度百分比*/
	INT8U m_LightLevelRate;

	SAirConditionDevices m_AirConditionDevices[CONS_DEVICE_MAX_COUNT];
	/* 灯状态*/
	INT8U m_NightLightState;//夜灯状态
	INT8U m_GalleryLightState;//廊灯状态

	/* 房卡取电类型*/
	INT8U m_DoorCardType;

	/* 门牌显示类型*/
	INT8U m_DoorDisplayType;//0:RCU控制，1是门牌模块控制

	/* 空调模块类型*/
	INT8U m_AirConditionType;//0:可控空调面板，1是非可控空调面板

	/* 场景组合*/
	struct SSceneCompose m_SceneComposes[CONS_SCENE_COMPOSE_MAX_COUNT];
	INT8U m_CurrentOpenedSceneNo;// 当前已打开的场景号

	/* 总电源组合控制*/
	struct SSceneCompose m_PowerSceneComposes;

	/* 门卡组合控制*/
	struct SSceneCompose m_DoorcardInSceneComposes;//插入
	struct SSceneCompose m_DoorcardOutSceneComposes;//拔出

	/* 大门组合控制*/
	struct SDoorSceneCompose m_DoorSceneComposes;

	/* 人体红外检测组合控制*/
	struct SSceneCompose m_IRInSceneComposes;//有人
	struct SSceneCompose m_IROutSceneComposes;//无人?

	/* 人体红外检测判断时间*/
	INT8U m_IrdaDetectTime;//分钟

	/* 拔卡检测判断时间*/
	INT8U m_OutCardDetectTime;//分钟

	/* 整流器相关*/
	INT8U m_TeleNo[6];//管理员号码
	INT16U m_ArgVol;//均衡电压值
	INT16U m_ArgCur;//电流限值
	INT8U  m_ClArgFlag[CONS_CLRELAY_MAX_COUNT];//回路均衡状态
	time_t m_ClRelayManualCtlEndTime[CONS_CLRELAY_MAX_COUNT+1];//回路手动控制时效时间	
	INT8U m_ClRelayManualCtlValidTime[CONS_CLRELAY_MAX_COUNT+1];//回路手动控制时效分钟
	SSmartGarageItem m_ClRelayManualCtlRecoverActionItem[CONS_CLRELAY_MAX_COUNT+1];//回路手动控制时效时间过后的恢复动作	
	SSmartGarageItem m_ClRelayManualCtlActionItem[CONS_CLRELAY_MAX_COUNT+1];//回路手动控制时效时间内的动作	
	/* 任务相关 */		
	SNorTaskStatus m_NortaskStatus[CONS_NORTASK_MAX_COUNT];

	/* 光控任务相关 */
	SLuxTaskStatus m_LuxTaskStatus;

	/* 恒照任务相关 */
	SConstantLuxTaskStatus m_ConstantLuxTaskStatus;

	/* 回路相关*/
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
			m_AirConditionDevices[i].m_AirSeason = SEASON_SUMMER;//夏季
			m_AirConditionDevices[i].m_AirWorkMode = 1;	//制冷
			m_AirConditionDevices[i].m_AirSetTemperture = 25; //空调设置温度
		}
		m_WindowRunTime = 0x3c;//60秒
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
		
		m_ArgVol = 0x2580;//默认值258V
		m_ArgCur = 0x2000;//默认限制20A
	}
	bool IsNight();
};

//对status赋值时记住一定要调用OnChanged，否则可能造成保存不了
struct Status
{
	struct HeaderChecker Checker;
	struct MeterStatus MeterStatus[CONS_METER_MAX_COUNT];

	struct ReportStatus ReportStatus_Month[CONS_METER_COUNT_NO_VIP];
	struct ReportStatus ReportStatus_Day[CONS_METER_COUNT_NO_VIP];	

	time_t t_LastLineWasteAlarm;
	char LastGatherFailAlarm;
	char LastReportDayTime;//上次上报日数据时间
	char LastReportMonthTime;//上次上报月数据时间
	//上次清除线损的日期
	INT8U LastClearLineWasteAlarm;
	INT8U CurDay;

	INT8U DayTaskStatus;
	INT8U MonthTaskStatus;
	Channel GatheredChannel;

	/* start:仅为电科院测试用*/
	PortFuncSet	m_PortFunc[CONFIG_PORT_NUM];
	INT8U m_ZJYS[CONS_METER_MAX_COUNT][7];
	INT8U Phase[CONS_METER_MAX_COUNT];//电表相位
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
		//suxufeng:[2010-4-29]修改为time_t格式，提高任务检查性能
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
	bool WriteAlarm(INT16U meterNo,float data,DATA_TYPE type,const char rTime[7]);//写数据异常告警文件
	void SetStatus(INT16U id,const string& name,const string& val);
	void GetTimeFormStr(const string& val,char tTime[7]);
};

#define STATUS Status::Instance()
#define DECLARE_STATUS(status) Status& status=STATUS


#endif
