///*Task.h����*/

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
#define ONE_HOUR_TIME_T_TIME 3600		//һСʱ��ʱ�䣬��λ����
#define ONE_DAY_TIME_T_TIME 86400	//һ���ʱ�䣬��λ����
#define FIX_DATA_NUM 5

struct TaskGatherInfo
{
	time_t LastExecTime;//���һ��ִ��ʱ��
	INT32U TotalTimes;
	INT8U Times;//�Ѿ�ִ�еĴ���
	INT8U Result;//�ɼ�������μ�GATHER_RESULT
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
		//8��18����ӣ���
		//������˲ɼ�ʱ��ʱ����Ϊ�ϴ�����ɼ����û����ն�����������ٴ�����
		Result=0;
	}
	KLDump& Dump(KLDump& dump) const;
	
};


struct TaskDataInfo
{
	INT16U MeterCount;//������
	INT16U DataCount;//����������
	time_t LastFreezeTime;//���һ�ζ���ʱ��
	time_t FirstRecordTime;//����һ�μ�¼ʱ��
	time_t LastRecordTime;//���һ�μ�¼ʱ��
	INT8U CurRecordFlag;//��ǰ��¼��־��Ϊ0��ʾδ����ɹ�����0��ʾ����ɹ�
	INT8U ValFormat;//�洢�ڼ�¼�е���ֵ���͵ĸ�ʽ
	INT8U ItemLen;//��¼���
	
public:
	//��ȡÿ����¼�ĳ���
	
	INT32U GetRecordLen() const
	{
		return GetMeterRecordLen()*MeterCount;
	}
	//��ȡÿ������������ĳ���
	//ÿ����¼�ĳ��ȣ�������ַ��6�ֽڣ������״̬��2�ֽڣ���������ʶ��1�ֽڣ���������ĸ�����ÿ�����������ݳ��ȣ�7�ֽڣ���
	INT32U GetMeterRecordLen() const
	{
		return (FLAG_DATA_LENTH+DataCount*ItemLen);
	}
	KLDump& Dump(KLDump& dump) const;
};

struct Task
{
	INT8U Enable;//��Ч�ԣ�1��ʾ��Ч��0��Ч
	INT8U TaskNo;//����ţ�0��CONS_TASK_MAX_COUNT-1
	INT8U Priority;//���ȼ���Խ���ʾ���ȼ�Խ��
	INT8U DataType;//�������ͣ��μ�DATA_TYPE����
	INT8U MaxTimes;//��һ�ε���������ִ�д���,���Դ���=MaxTimes-1
	INT8U IdleEnable;//�Ƿ�������вɼ�
	INT8U DataPeriod;//����ʱ�ڣ��μ�DATA_PERIOD,WorkServer������Ҫ�õ���

	INT8U FreezeDelayUnit;//������ʱʱ�䵥λ
	INT8U FreezeDelayVal;//������ʱʱ��ֵ

	INT8U FixDelayUnit;//������ʱʱ�䵥λ
	INT8U FixDelayVal;//������ʱʱ��ֵ
	MatchTimer FixDelayTimer;//��������ʱ��
	
	INT8U FixMethod;//������ʽ���μ�DATA_FIX_METHOD
	DataBitSet FixDBS;//��Ҫ����������������

	InterTimer FreezeTimer;//�����������

	MatchTimer GatherTimer;//�ɼ���������
	INT32U MaxRecords;//����¼����Ϊ0��ʾ������
	
	DataBitSet CfgDBS;//�����ļ��е��������������
	DataBitSet DBS;//�������������	
	MeterBitSet MBS;//���������
	MeterBitSet VaildMBS;//������Ч�ĵ��
	MeterBitSet GatheredRS485MBS;//�����Ѳɼ���485�ֱܷ���������
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
	TaskBitSet TBS;//���õ�����
	TaskBitSet ValidTBS;//��Ч������
	TaskBitSet FreezeTBS;//��Ҫ���ݶ��������

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

	inline bool IsFreezeTask(INT32U taskNo)//TODO:����
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

	//����0��ʾ�ɹ�������-1��ʾʧ��
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

