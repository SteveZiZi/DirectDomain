#ifndef INCLUDED_ImpAlarm_FILE
#define INCLUDED_ImpAlarm_FILE

#ifndef INCLUDED_CONTEXT
#include "Context.h"
#endif

#ifdef WIN32
#define PATH_ImpAlarm_BASE	"./mnt"
#else
#define PATH_ImpAlarm_BASE	"/mnt"
#endif

#define CONST_ImpAlarm_FILE_NAME PATH_ImpAlarm_BASE"/data/alarm/impalarm.dat"
#define CONST_ImpAlarm_FILE_LENGTH 120			//文件记录数，请以10的倍数初始化
#define CONST_ImpAlarm_RECORD_ITEM_LENGTH 32		//以最长记录冗余即可
#define CONST_ImpAlarm_RECORD_OFFSET 0xffff		//默认的io_Offset的值

/* Monitor 告警定义*/
#define IMPALARM_CPR_ONLINE_STATE	0x020F
#define IMPALARM_CL_LEAK_STATE	0x0101//漏电超限状态
//2006-8-14由苏煦烽添加
//2007-8-30由苏煦烽移至此处，创建成公共读写函数，不再由WorkServer私有。
//2007-9-4由苏煦烽对读记录函数进行重新封装，支持读取最新一条未上报数据，用于主动上报，也支持读取所有符合条件的数据，用于主站抄读告警数据
struct ImpAlarmRecordInfo
{
	INT8U ImpAlarmData[1];
public:
	int Init(FILE* fp,INT16U iLen,INT8U iItemLen)
	{
		//INT32U Len=iLen*iItemLen;
		ImpAlarmRecordReset(fp,iLen,iItemLen);
		return 0;
	}
	int ImpAlarmRecordReset(FILE* fp,INT16U iLen,INT8U iItemLen);
};

class ImpAlarmFile
{
public:
	static int GetAlarmRecord(bstring &iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//取出一条需要主动上报的记录，用于主动上报

	static int GetAlarmRecord(INT8U *iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//取出一条需要主动上报的记录，用于主动上报

	static int GetAlarmRecords(bstrings &iRecords,INT8U irCount = 0,time_t tBeginTime = 0,time_t tEndTime = 0,INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//取出数据文件中所有符合要求的数据记录，用于主站抄收告警数据

	static int GetAlarmRecords(bstrings &iRecords,INT8U irCount = 0,char *tBeginTimeBCD7 = NULL,char *tEndTimeBCD7 = NULL,INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//取出数据文件中所有符合要求的数据记录，用于主站抄收告警数据

	static int SetAlarmRecordReaded(INT16U io_Offset,INT8U iItemLen  = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//将一条已经上报的记录的标志位置为1

	static int SetAlarmRecordReaded(FILE* fp,INT16U io_Offset,INT8U iItemLen  = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH);//将一条已经上报的记录的标志位置为1


	static int AddAlarmRecord(const INT8U* iRecordContent,INT8U iTrueLen,INT16U iImpAlarmNo,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
		INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH,
		char *FileName = CONST_ImpAlarm_FILE_NAME);//添加报警记录文件，一次只能添加一条记录

	static int AddAlarmRecord(const INT8U* iRecordContent,INT8U iTrueLen,INT16U iImpAlarmNo,const char *bcdtime,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
		INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH,
		char *FileName = CONST_ImpAlarm_FILE_NAME);//添加报警记录文件，一次只能添加一条记录

	//static int ConvertMeterNoToBCD(INT16U iMeterNo,INT8U *iBcdMeterNo);//将电表编号从HEX码转换成BCD码

	static int GetTrueLen(INT8U *iImpAlarmNo,INT8U &i_TrueLen);//在读取文件时获取该条记录的实际长度

	//static int AddImpAlarmRecord(bstring &iRecordContent,INT8U iTrueLen,INT16U iImpAlarmNo,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
	//	INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH,
	//	char *FileName = CONST_ImpAlarm_FILE_NAME);//添加报警记录文件
};

#endif
