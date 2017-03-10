#ifndef INCLUDED_ALARM_FILE
#define INCLUDED_ALARM_FILE

#ifndef INCLUDED_CONTEXT
#include "Context.h"
#endif

#ifdef WIN32
#define PATH_ALARM_BASE	"./mnt"
#else
#define PATH_ALARM_BASE	"/mnt"
#endif

#define CONST_ALARM_FILE_NAME PATH_ALARM_BASE"/data/alarm/alarm.dat"
#define CONST_ALARM_FILE_LENGTH 120			//文件记录数，请以10的倍数初始化
#define CONST_ALARM_RECORD_ITEM_LENGTH 32		//以最长记录冗余即可
#define CONST_ALARM_RECORD_OFFSET 0xffff		//默认的io_Offset的值

#define CONST_VOLTAGE_LOST_ALARM_NO 0x0171		//失压事件报警
#define CONST_CURRENT_LOST_ALARM_NO 0x0172		//失流事件告警
#define CONST_CURRENT_RESERVE_ALARM_NO 0x0173	//电流反向事件告警
#define CONST_PROGRAM_ALARM_NO 0x0174			//编程事件告警
#define CONST_LINE_WASTE_ALARM_NO 0x0175		//线损超值/负值告警
#define CONST_RELAY_CHANGE_ALARM_NO 0x0176		//继电器变位事件告警
#define CONST_GATHER_FAIL_ALARM_NO 0x0177		//抄表失败告警
#define CONST_SETTIME_FAIL_ALARM_NO 0x0178		//校时失败告警
#define CONST_TIMEALARM_ALARM_NO	0x0179		//时钟异常告警
#define CONST_UPDATE_EVENT_ALARM_NO 0x017A		//升级事件告警
//#define CONST_CHANGE_METER_ALARM_NO 0x017B		//换表事件告警
#define CONST_RELAY_FAILROUTER_ALARM_NO 0x017C	//采集终端搜寻路由失败告警
#define CONST_POWEROFF_EVENT_ALARM_NO 0x017D	//停电事件告警
#define CONST_BRAKE_FAIL_ALARM_NO	0x017E		//电表拉合闸失败

/* Monitor 告警定义*/
#define ALARM_RTF_CONFIGURE_FILE_CLEAN		0x0200
#define ALARM_RTF_CONFIGURE_FILE_DOWNLOAD	0x0201
#define ALARM_RTF_VOL_SET	0x0202
#define ALARM_CL_VOL_SET	0x0203
#define ALARM_RTF_ONOFF	0x0208
#define ALARM_CL_CTRL		0x020A

#define ALARM_RELAY_CTRL	0x020D
#define ALARM_COMM_FAIL	0x020F
#define ALARM_TASK_CL_VOL_SET		0X0215
#define ALARM_TASK_CL_ONOFF_SET		0X0216
#define ALARM_CPR_ONLINE_STATE	0x020F

/* 光控任务*/
#define LUXTASK_CL_VOL_SET		0X0301
#define LUXTASK_END		0X0302
#define LUXTASK_CL_ONOFF_SET		0X0303

//2006-8-14由苏煦烽添加
//2007-8-30由苏煦烽移至此处，创建成公共读写函数，不再由WorkServer私有。
//2007-9-4由苏煦烽对读记录函数进行重新封装，支持读取最新一条未上报数据，用于主动上报，也支持读取所有符合条件的数据，用于主站抄读告警数据
struct AlarmRecordInfo
{
	INT8U AlarmData[1];
public:
	int Init(FILE* fp,INT16U iLen,INT8U iItemLen)
	{
		//INT32U Len=iLen*iItemLen;
		AlarmRecordReset(fp,iLen,iItemLen);
		return 0;
	}
	int AlarmRecordReset(FILE* fp,INT16U iLen,INT8U iItemLen);
};

class AlarmFile
{
public:
	static int GetAlarmRecord(bstring &iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//取出一条需要主动上报的记录，用于主动上报

	static int GetAlarmRecord(INT8U *iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//取出一条需要主动上报的记录，用于主动上报

	static int GetAlarmRecords(bstrings &iRecords,INT8U irCount = 0,time_t tBeginTime = 0,time_t tEndTime = 0,INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//取出数据文件中所有符合要求的数据记录，用于主站抄收告警数据

	static int GetAlarmRecords(bstrings &iRecords,INT8U irCount = 0,char *tBeginTimeBCD7 = NULL,char *tEndTimeBCD7 = NULL,INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//取出数据文件中所有符合要求的数据记录，用于主站抄收告警数据

	static int SetAlarmRecordReaded(INT16U io_Offset,INT8U iItemLen  = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U FileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//将一条已经上报的记录的标志位置为1

	static int SetAlarmRecordReaded(FILE* fp,INT16U io_Offset,INT8U iItemLen  = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U FileLen = CONST_ALARM_FILE_LENGTH);//将一条已经上报的记录的标志位置为1


	static int AddAlarmRecord(const INT8U* iRecordContent,INT8U iTrueLen,INT16U iAlarmNo,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
		INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ALARM_FILE_LENGTH,
		char *FileName = CONST_ALARM_FILE_NAME);//添加报警记录文件，一次只能添加一条记录

	static int AddAlarmRecord(const INT8U* iRecordContent,INT8U iTrueLen,INT16U iAlarmNo,const char *bcdtime,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
		INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ALARM_FILE_LENGTH,
		char *FileName = CONST_ALARM_FILE_NAME);//添加报警记录文件，一次只能添加一条记录

	//static int ConvertMeterNoToBCD(INT16U iMeterNo,INT8U *iBcdMeterNo);//将电表编号从HEX码转换成BCD码

	static int GetTrueLen(INT8U *iAlarmNo,INT8U &i_TrueLen);//在读取文件时获取该条记录的实际长度

	//static int AddAlarmRecord(bstring &iRecordContent,INT8U iTrueLen,INT16U iAlarmNo,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
	//	INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ALARM_FILE_LENGTH,
	//	char *FileName = CONST_ALARM_FILE_NAME);//添加报警记录文件
};

#endif
