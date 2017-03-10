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
	INT8U m_Enable;//ִ�з���ʹ��λ
	INT16U m_KeepMins;//�ȴ�ִ�е�ʱ��
	INT8U m_ActionItemsNum;//ִ�������
	SSmartGarageItem m_ActionItems[32];//ִ�����б�
	INT8U m_RunSTate;
public:
	time_t m_ActionTime;//��������ʱ�� 
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
	INT32U m_StartLuxlimit;//������ֵ
	INT32U m_EndLuxlimit;//������ֵ
	INT32U m_LuxSensorsBS;//������ش�������Чλ��bit1��Ӧsensor1~bit31��Ӧsensor31��Ŀǰֻ����bit1~bit16��Ӧ1~16��sensor
	DevicesBitSet m_LuxSensorsDBS;//�����д������豸���λ����
 	SLuxPlan m_Plans[CONS_LUXTASKPLAN_MAX_COUNT];
 	INT8U m_InvalidTimeClOff;//ʱ����Ч��·�ر�״̬
	INT8U m_ClManualCtl;//��·�����ֶ�����
	INT8U m_ClManualCtlBackState;//�����ֶ����ƣ��ն��˹���������Ա����ֶ���ʼʱ���״̬�����ֶ����ƽ����ָ�״̬
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
