///*Task.h任务*/

#ifndef INCLUDED_TASK
#define INCLUDED_TASK

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_TIMER
#include "../base/Timer.h"
#endif

#ifndef INCLUDED_METER
#include "Meter.h"
#endif

#ifndef INCLUDED_METER_GATHER_INFO
#include "MeterGatherInfo.h"
#endif

#ifndef INCLUDED_DATA_ID
#include "DataID.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

class KLDump;
#define FLAG_DATA_LENTH 9
#define TASK_VIP 2
#define SET_TIME_TASK 6

#define VIP_REG_ID 0x10000000
#define ONE_HOUR_TIME_T_TIME 3600		//一小时的时间，单位是秒
#define ONE_DAY_TIME_T_TIME 86400	//一天的时间，单位是秒
#define FIX_DATA_NUM 5

struct TaskGatherInfo
{
	time_t LastExecTime;//最后一次执行时间
	INT32U TotalTimes;
	INT8U Times;//已经执行的次数
	INT8U Result;//采集结果，参见GATHER_RESULT
public:
	inline void Init()
	{
		LastExecTime=0;
		TotalTimes=0;
		Result=GATHER_RESULT_NOT_GATHERED;
		Reset();
	}
	inline void Reset()
	{
		Times=0;
		//8月18日添加，：
		//解决到了采集时间时，因为上次任务采集结果没有清空而造成任务不能再次启动
		Result=0;
	}
	KLDump& Dump(KLDump& dump) const;
	
};


struct TaskDataInfo
{
	INT16U MeterCount;//电表个数
	INT16U DataCount;//数据量个数
	time_t LastFreezeTime;//最近一次冻结时间
	time_t FirstRecordTime;//最早一次记录时间
	time_t LastRecordTime;//最近一次记录时间
	INT8U CurRecordFlag;//当前记录标志，为0表示未冻结成功，非0表示冻结成功
	INT8U ValFormat;//存储在记录中的数值类型的格式
	INT8U ItemLen;//记录项长度
	
public:
	//获取每条记录的长度
	
	INT32U GetRecordLen() const
	{
		return GetMeterRecordLen()*MeterCount;
	}
	//获取每个电表的数据项的长度
	//每条记录的长度：（电表地址（6字节）＋电表状态（2字节）＋补抄标识（1字节）＋数据项的个数×每个数据项数据长度（7字节））
	INT32U GetMeterRecordLen() const
	{
		return (FLAG_DATA_LENTH+DataCount*ItemLen);
	}
	KLDump& Dump(KLDump& dump) const;
};

struct Task
{
	INT8U Enable;//有效性，1表示有效，0无效
	INT8U TaskNo;//任务号，0至CONS_TASK_MAX_COUNT-1
	INT8U Priority;//优先级，越大表示优先级越高
	INT8U DataType;//数据类型，参见DATA_TYPE定义
	INT8U MaxTimes;//在一次调度中最多的执行次数,重试次数=MaxTimes-1
	INT8U IdleEnable;//是否允许空闲采集
	INT8U DataPeriod;//数据时期，参见DATA_PERIOD,WorkServer进程需要用到的

	INT8U FreezeDelayUnit;//冻结延时时间单位
	INT8U FreezeDelayVal;//冻结延时时间值

	INT8U FixDelayUnit;//补抄延时时间单位
	INT8U FixDelayVal;//补抄延时时间值
	MatchTimer FixDelayTimer;//补抄调度时间
	
	INT8U FixMethod;//补数方式，参见DATA_FIX_METHOD
	DataBitSet FixDBS;//需要补数的数据量掩码

	InterTimer FreezeTimer;//冻结调度设置

	MatchTimer GatherTimer;//采集调度设置
	INT32U MaxRecords;//最大记录数，为0表示不冻结
	
	DataBitSet CfgDBS;//配置文件中的数据量编号掩码
	DataBitSet DBS;//数据量编号掩码	
	MeterBitSet MBS;//电表编号掩码
	MeterBitSet VaildMBS;//任务有效的电表
	MeterBitSet GatheredRS485MBS;//任务已采集的485总分表电表编号掩码
	TaskDataInfo DataInfo;
	TaskGatherInfo GatherInfo[CONS_METER_MAX_COUNT];

public:
	static bool IsValidNo(INT32U TaskNo)
	{
		return TaskNo<CONS_TASK_MAX_COUNT;
	}
	bool IsValid() const;
	inline bool IsFreeze() const
	{
		return (FreezeTimer.IsValid() && MaxRecords>0);
	}
	void Init(INT8U taskNo);
	void MakeMeterGatherInfo(bool bForIdle,Meter& iMeter,MeterGatherInfo& ioInfo);
	KLDump& BaseDump(KLDump& dump) const;
	KLDump& Dump(KLDump& dump) const;
	int OnCommand(KLDump& dump,const char* cmdKey,const char* objName,INT8U id,const char *propName,const char* propVal);
	void StatBitSet()
	{
		DBS.Stat();
		MBS.Stat();
	}
	bool IsFinishForMeter(INT16U meterNo)const;
	void ResetFailMeter();
	void MakeTaskGatherInfo(Meter& iMeter,MeterGatherInfo& iInfo);
	bool CanDelayFreeze(time_t& tStdTime,time_t& tInitTime,time_t& tCheckTime);
	void UpdateGatherInfo(INT16U meterNo,INT8U result);
	void RefreshGatherInfo();
	void ResetFailMeter(MeterBitSet& iMBS);
	
	int FindMeterData(INT8U dataType,INT16U meterNo,INT32U dataIndex,INT8U fsjg,size_t* opMeterPosNo,size_t* opDataPosNo);
};

struct Tasks
{
	TaskBitSet TBS;//配置的任务
	TaskBitSet ValidTBS;//有效的任务
	TaskBitSet FreezeTBS;//需要数据冻结的任务

	Task Item[CONS_TASK_MAX_COUNT];
	time_t ChangeTime;
	time_t WorkServerRefreshTime;//WorkServer
	bool bStartUpMeterReadFlag;
	//TaskBitSet m_FinishedTask;
private:
	int FixHistoricalData(Task &task);
public:
	void Init();

	bool VerifyTask(INT32U taskNo);

	void ResetTaskGatherInfo(MeterBitSet& iMBS,TaskBitSet& iTBS,bool bForWait);

	void MakeMeterGatherInfo(bool bForIdle,TaskBitSet& iTBS,Meter& iMeter,MeterGatherInfo& ioInfo);	

	void MakeFailedTBS(Meter& iMeter,MeterGatherInfo& ioInfo);

	int MakeFinishedTasks(MeterBitSet& iMBS,TaskBitSet& gahterTBS,TaskBitSet& oFinishTBS);
	int MakeFinishedTasks(MeterBitSet& iMBS,Task& task,TaskBitSet& oFinishTBS);

	int FindMeterData(INT8U dataType,INT16U meterNo,INT32U dataIndex,INT8U fsjg,size_t& oTaskNo,size_t* opMeterPosNo,size_t* opDataPosNo);

	inline bool IsFreezeTask(INT32U taskNo)//TODO:整理
	{
		return Task::IsValidNo(taskNo) && FreezeTBS.test(taskNo);
	}




	void AutoFixDayData();
	float Get_diffvalue(bstrings& daydatas);
	void AutoFixMonthData();
	time_t GetDate_month(time_t t_curtime,int intermonth=1);
	void AutoFixVIPData();
	bool Sub_FixData(float& i_beg_data,float& i_end_data,int i_interhour,INT8U*  meteraddr,INT8U DataType,float& o_diff_value,bstrings& o_vipdatas);
	bool Sub_FixData(float beg_data[],float end_data[],int i_interhour,INT8U*  meteraddr,INT8U DataType,bstrings& o_vipdatas);

	//返回0表示成功，返回-1表示失败
	int FixHistoricalDayData();
	int FixHistoricalMonthData();
	int FixHistoricalVIPData();
	bool GetTaskByType(Task &task,DATA_TYPE datype);

	void OnChanged();
	KLDump& BaseDump(KLDump& dump) const;
	KLDump& Dump(KLDump& dump) const;

	int OnCommand(KLDump& dump,const char* cmdKey,const char* objName,INT8U id,const char *propName,const char* propVal);
};

#endif

