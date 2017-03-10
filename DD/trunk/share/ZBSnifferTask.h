#ifndef INCLUDED_ZBSNIFFERTASK
#define INCLUDED_ZBSNIFFERTASK

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_TIMER
#include "../base/Timer.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

#ifndef INCLUDED_TASK
#include "task.h"
#endif

#define ZBRECORDLEN 21
#define ZBRECORDHEADLEN 10

struct TimeSection
{
	MatchTimer BeginTime;
	MatchTimer EndTime;
};

//载波质量侦测任务
struct ZBSnifferTask
{
	INT8U Enable;//有效性，1表示有效，0无效
	INT8U Priority;//优先级，越大表示优先级越高		
	TimeSection Item[CONS_ZBSNF_TIME_SECTION_MAX_COUNT];//调度时间集合
	INT8U validTimeNo;//有效的调度时间的总数
	//RouterBitSet robs;//侦测的路径集合
	INT8U MaxTimes;//在一次调度中最多的执行次数
	INT16U MaxRecords;//形成载波质量侦测日志文件的最大记录数
	INT8U AdjustEnable;//是否需要调整成功率，1需要，0不需要。若为0，调整成功率间隔将无效
	INT16U AdjustInterval;//调整成功率间隔，单位：分钟
	//TaskGatherInfo GatherInfo[CONS_ROUTER_MAX_COUNT];//采集结果
public:
	ZBSnifferTask()
	{
		curTimeSec=0xff;
		memset(this,0x00,sizeof(*this));
	};
	void Init();
	bool IsNeedSniff();
	bool IsEndTime();
	bool IsFinishForRouter(INT16U RouterNo) const;
	void UpdateGatherInfo(INT16U RouterNo,bool SuccedFlag);
	void OnChanged();
private:
	INT8U curTimeSec;
	bool IsValidTime(TimeSection& ts);
	/*time1早于time2，返回1；迟于则返回-1，相等返回0*/
	int IsEarlier(TimeBitSet& time1,TimeBitSet& time2)
	{
		for (size_t i=time1.size()-1; i>0; i--)
		{
			if (!time1.test(i) && time2.test(i))
			{
				return 1;
			}
			else if (time1.test(i) && !time2.test(i))
			{
				return -1;
			}
		}
		return 0;
	};
	bool IsEqual(TimeBitSet& time1,TimeBitSet& time2);
	size_t FirstSetPos(TimeBitSet& time)
	{
		for (size_t i=0; i<time.size(); i++)
		{
			if (time.test(i))
			{
				return i;
			}
		}
		return 0xff;
	};
	//int IsValidTimeSec(TimeBitSet& bt,TimeBitSet& et,INT8U selc);
	int IsRearchTime(MatchTimer& tt);
	int CompareTime(INT32U t1, INT32U t2);
};

#endif
