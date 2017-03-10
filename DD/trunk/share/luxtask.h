#ifndef _LUXTASK_H_
#define _LUXTASK_H_

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_TIMER
#include "../base/Timer.h"
#endif

#include "Meter.h"
#include "SysBase.h"
#include "nortask.h"

enum LUXTASKSTATE
{
	LUXTASK_STOP=0,
	LUXTASK_RUN=1
};

enum LUXTASKPLANSTATE
{
	STATE_STOP=0,
	STATE_RUN=1
};
struct SLuxPlan
{
	INT8U m_Enable;//执行方案使能位
	INT16U m_KeepMins;//等待执行的时间
	INT8U m_ActionItemsNum;//执行项个数
	SSmartGarageItem m_ActionItems[32];//执行项列表
	INT8U m_RunSTate;
public:
	time_t m_ActionTime;//方案启动时间 
public:
	bool IsTimerOn(INT8U taskno,INT8U planno);
	void OnExec(INT8U taskno);
	void OnExec(SSmartGarageItem & actionItem, INT8U alarmtype);
};

struct SLuxTask
{
	INT8U m_Enable;
	INT8U m_TaskNo;
	INT8U m_Priority;
	time_t  m_StartTime;
	time_t  m_EndTime;
	INT8U m_PeriodUnit;
	SPeriodDetail m_PeriodDetail;
	INT32U m_PeriodDetailValue;
	INT8U m_RunState;//Run;Stop;Pause
	INT32U m_StartLuxlimit;//启动阈值
	INT32U m_EndLuxlimit;//结束阈值
	INT32U m_LuxSensorsBS;//方案相关传感器有效位，bit1对应sensor1~bit31对应sensor31，目前只用了bit1~bit16对应1~16号sensor
	DevicesBitSet m_LuxSensorsDBS;//任务中传感器设备序号位集合
 	SLuxPlan m_Plans[CONS_LUXTASKPLAN_MAX_COUNT];
 	INT8U m_InvalidTimeClOff;//时间无效回路关闭状态
	INT8U m_ClManualCtl;//回路处于手动控制
	INT8U m_ClManualCtlBackState;//由于手动控制，终端了光控任务，所以备份手动开始时光控状态，待手动控制结束恢复状态
public:
	void Init(void){memset(this,0x00,sizeof(*this));}
	bool IsValidTime();
	/*brief:set task schedule function,will decide whice task will be excuted
		param [in] void
		param [out]None 
		return void
		author:Wanqingsong
		date: 2012-02-25
	*/
	bool LuxTaskSchedule(void);
	void ResetTaskState(void){m_RunState=LUXTASK_STOP;}
	bool ResetPlanRunState(INT8U planno);
	void ResetPlansRunState(void);
	
private:
	void CalculatePlanActionTime();
	bool LuxTaskPlanTimeOn(INT8U planNo);
	void LuxTaskPlanAction(INT8U planNo);
	INT32U LuxCalculate(void);
	bool SetPlanRunState(INT8U planno);
	};

struct SLuxTasks
{
	TaskBitSet m_LuxTBS;//\u914D\u7F6E\u7684\u4EFB\u52A1
	TaskBitSet m_LuxValidTBS;//\u6709\u6548\u7684\u4EFB\u52A1
	SLuxTask m_Tasks[CONS_LUXTASK_MAX_COUNT+1];
public:
	void Init(void){memset(this,0x00,sizeof(*this));}
};

#endif
