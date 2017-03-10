#ifndef INCLUDED_METER_GATHER_INFO
#define INCLUDED_METER_GATHER_INFO

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_PORT_SET
#include "../base/PortSet.h"
#endif

#ifndef INCLUDED_DATA_ID
#include "DataID.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

struct MeterGatherInfo
{
	INT8U DataType;//参见DATA_TYPE
	INT8U Result;//采集结果，参见GATHER_RESULT
	DataBitSet DBS;//需采集的数据量掩码
	DataBitSet FailedDBS;//没有采集成功的数据量
	TaskBitSet TBS;//采集相关的任务
	TaskBitSet FailedTBS;//采集失败的任务
	INT32U TotalTimes;//总执行次数
	DataBitSet NotSupportDBS;//不支持的数据量
	time_t SchedTime;//采集调度时间

public:
	bool IsValid() const
	{
		return (DataType!=DATA_TYPE_INVALID && DBS.any());
	}

	void Init(DATA_TYPE dataType)
	{
		DataType=dataType;
		TotalTimes=0;
		NotSupportDBS.reset();
		SchedTime=0;
		Reset();

	}

	inline INT8U GetPriority() const
	{
		//数据越小
		return DataType;
	}

	KLDump& Dump(KLDump& dump) const
	{
		dump<<"DataType="<<(DATA_TYPE)DataType<<"\tPriority="<<GetPriority()<<"\tResult="<<(GATHER_RESULT)Result;
		dump<<"\tDBS="<<DBS<<"\tFailedDBS="<<FailedDBS<<"\tTBS="<<TBS<<"\tFailedTBS="<<FailedTBS<<"\tTotalTimes="<<TotalTimes<<"\tNotSupportDBS="<<NotSupportDBS;
		return dump;
	}

	void Reset()
	{
		Result=GATHER_RESULT_NOT_GATHERED;
		DBS.reset();
		FailedDBS.reset();
		TBS.reset();
		FailedTBS.reset();
	}
};

#endif

