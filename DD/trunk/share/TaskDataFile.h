#ifndef INCLUDED_TASK_DATA_FILE
#define INCLUDED_TASK_DATA_FILE

#ifndef INCLUDED_REPORT_CYC_RECORD_FILE
#include "../base/CrcReportRecordFile.h"
#endif

#ifndef INCLUDED_REG_DATA
#include "RegData.h"
#endif

struct Task;

//TODO:在未来版本中需将任务的电表个数、数据量编号、数据格式等信息保存进文件中


class TaskDataFile:public CrcReportRecordFile 
{
public:
	TaskDataFile(Task& task):m_Task(task)
	{
		SetTask(task);
	}

	static int GetTaskFileName(Task& task,char* opFileName);
	static int DeleteTaskFile(Task& task);
	inline static INT8U MakeRecordFlag(INT8U dataFlag,INT8U dataType)
	{
		return (dataFlag<<4)+(dataType & 0x0F);
	}
	inline static INT8U GetDataFlag(INT8U recordFlag)
	{
		return (recordFlag>>4);
	}
	inline static INT8U GetDataType(INT8U recordFlag)
	{
		return (recordFlag & 0x0F);
	}
	inline static bool IsRecordValid(INT8U recordFlag)
	{
		return RegData::IsValid(GetDataType(recordFlag));
	}
	static int AdjustFixData(Task& task,bstring& fixData);
	
	int GetLastClipInfo(time_t opClipTimes[],unsigned char* opFixDatas);
protected:
	bool GetFixData(FILE* fp,time_t fromTime,time_t toTime,const unsigned char* data,unsigned int dataLen,unsigned int iiOffset,bstring& oFixData);
	bool CreateHeader(unsigned char* ipHead128);
	void OnFileRebuilded();
private:
	void SetTask(Task& task);
protected:
	Task& m_Task;
};
#endif




