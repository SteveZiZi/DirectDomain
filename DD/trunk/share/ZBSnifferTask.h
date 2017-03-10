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

//�ز������������
struct ZBSnifferTask
{
	INT8U Enable;//��Ч�ԣ�1��ʾ��Ч��0��Ч
	INT8U Priority;//���ȼ���Խ���ʾ���ȼ�Խ��		
	TimeSection Item[CONS_ZBSNF_TIME_SECTION_MAX_COUNT];//����ʱ�伯��
	INT8U validTimeNo;//��Ч�ĵ���ʱ�������
	//RouterBitSet robs;//����·������
	INT8U MaxTimes;//��һ�ε���������ִ�д���
	INT16U MaxRecords;//�γ��ز����������־�ļ�������¼��
	INT8U AdjustEnable;//�Ƿ���Ҫ�����ɹ��ʣ�1��Ҫ��0����Ҫ����Ϊ0�������ɹ��ʼ������Ч
	INT16U AdjustInterval;//�����ɹ��ʼ������λ������
	//TaskGatherInfo GatherInfo[CONS_ROUTER_MAX_COUNT];//�ɼ����
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
	/*time1����time2������1�������򷵻�-1����ȷ���0*/
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
