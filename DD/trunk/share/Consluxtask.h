#ifndef _CONSLUXTASK_H_
#define _CONSLUXTASK_H_

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

#define  CONS_STRATEGY_PLANS_MAX	12
struct SStrategy
{
	INT8U m_bValid;
	INT8U m_speedlevel;//LEVEL1~12
	INT16U m_luxdiff;
	INT16U m_stepvol;
public:
	void Init(void){memset(this,0x00,sizeof(*this));}
};

struct SConsLuxTask
{
	INT8U m_Enable;
	INT8U m_TaskNo;
	INT8U m_Priority;
	time_t  m_StartTime;
	time_t  m_EndTime;
	INT8U m_PeriodUnit;
	SPeriodDetail m_PeriodDetail;
	INT32U m_PeriodDetailValue;
	INT32U m_ConstantLux;//恒定值
	INT32U m_LuxSensorsBS;//方案相关传感器有效位，bit1对应sensor1~bit31对应sensor31，目前只用了bit1~bit16对应1~16号sensor
	DevicesBitSet m_LuxSensorsDBS;//任务中传感器设备序号位集合
	INT8U m_ClrelayNo;//回路号
	INT8U m_TimePeriodInDay[CONS_TIME_PERIOD_IN_DAY_MAX_COUNT][4];//BCD格式HHMM,如时段1为[0][0][0][1]~[0][2][0][3] 时段2为[1][0][1][1]~[1][2][1][3]
	SStrategy m_Strategys[CONS_STRATEGY_PLANS_MAX];
	INT8U m_ClearClOffFlag;
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
	bool ConsLuxTaskSchedule(void);
private:	
	int IncreaseClVol(INT16U vol,INT16U level,INT16U& oVol);
	int DecreaseClVol(INT16U vol,INT16U level,INT16U& oVol);
	INT16U GetStrategyVol(INT32U curLux);
private:
	INT32U LuxCalculate(void);


};

struct SConsLuxTasks
{
	TaskBitSet m_ConsLuxTBS;//\u914D\u7F6E\u7684\u4EFB\u52A1
	TaskBitSet m_ConsLuxValidTBS;//\u6709\u6548\u7684\u4EFB\u52A1
	SConsLuxTask m_Tasks[CONS_CONSTANTLUXTASK_MAX_COUNT];
public:
	void Init(void){memset(this,0x00,sizeof(*this));}
};

#endif
