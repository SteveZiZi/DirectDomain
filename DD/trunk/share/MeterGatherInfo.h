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
	INT8U DataType;//�μ�DATA_TYPE
	INT8U Result;//�ɼ�������μ�GATHER_RESULT
	DataBitSet DBS;//��ɼ�������������
	DataBitSet FailedDBS;//û�вɼ��ɹ���������
	TaskBitSet TBS;//�ɼ���ص�����
	TaskBitSet FailedTBS;//�ɼ�ʧ�ܵ�����
	INT32U TotalTimes;//��ִ�д���
	DataBitSet NotSupportDBS;//��֧�ֵ�������
	time_t SchedTime;//�ɼ�����ʱ��

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
		//����ԽС
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

