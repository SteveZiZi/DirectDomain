#ifndef _NORTASK_H_
#define _NORTASK_H_

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



enum EM_PERIODUNIT
{
	EM_PERIODUNIT_DAY=0,
	EM_PERIODUNIT_WEEK=1,	
	EM_PERIODUNIT_MONTH= 2
};

struct ClrelayTaskPlan
{
	INT8U m_Enable;
	INT16U m_ActionTime;
	SSmartGarageItem m_ActionItem;
	TimeBitSet m_WeekBS;
};

struct ClrelayTask
{
	INT8U m_clrelayNo;
	ClrelayTaskPlan m_clrelayTaskPlan[CONS_CLRELAY_TASKACTION_NUM];
public:
	ClrelayTaskPlan* GetTaskPlan(void);
	INT16U GetNextPlanStartTime();
};

struct SPeriodDetail
{
	TimeBitSet m_HourBS;
	TimeBitSet m_DayBS;
	TimeBitSet m_WeekBS;
	TimeBitSet m_MonthBS;
};

struct SPlan
{
	INT8U m_Enable;
	INT8U m_BeginTimeHHMM[2];
	INT8U m_ActionItemsNum;
	SSmartGarageItem m_ActionItems[32];
	INT8U m_curday;//\u4E00\u5929\u53EA\u6267\u884C\u4E00\u6B21
public:
	bool IsTimerOn(INT8U taskno,INT8U planno);
	void OnExec(INT8U taskno);
	void OnExec(SSmartGarageItem & actionItem, INT8U alarmtype);
};

struct SNorTask
{
	INT8U m_Enable;//\u6709\u6548\u6027\uFF0C1\u8868\u793A\u6709\u6548\uFF0C0\u65E0\u6548
	INT8U m_TaskNo;//\u4EFB\u52A1\u53F7\uFF0C0\u81F3CONS_TASK_MAX_COUNT-1
	INT8U m_Priority;//\u4F18\u5148\u7EA7\uFF0C\u8D8A\u5927\u8868\u793A\u4F18\u5148\u7EA7\u8D8A\u9AD8
	time_t  m_StartTime;//\u4EFB\u52A1\u6709\u6548\u8D77\u59CB\u65F6\u95F4
	time_t  m_EndTime;//\u4EFB\u52A1\u6709\u6548\u7ED3\u675F\u65F6\u95F4
	INT8U m_PeriodUnit;
	SPeriodDetail m_PeriodDetail;
	INT32U m_PeriodDetailValue;
 	SPlan m_Plans[CONS_TASKPLAN_MAX_COUNT];
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
	bool TaskSchedule(void);
private:
	bool TaskPlanTimeOn(INT8U planNo);
	void TaskPlanAction(INT8U planNo);
};

struct SNorTasks
{
	TaskBitSet m_NorTBS;//\u914D\u7F6E\u7684\u4EFB\u52A1
	TaskBitSet m_NorValidTBS;//\u6709\u6548\u7684\u4EFB\u52A1
	SNorTask m_Tasks[CONS_NORTASK_MAX_COUNT+1];
	ClrelayTask m_ClrelayTasks[CONS_CLRELAY_MAX_COUNT];
	bool m_fresh;
public:
	void Init(void){memset(this,0x00,sizeof(*this));}
	void ResetClrelayTasks(void){memset(&m_ClrelayTasks[0],0x00,sizeof(m_ClrelayTasks));}
	void OnChange();
	void ReFresh(){m_fresh = true;}
	void OnRefresh(INT8U clNo);
};

#endif


