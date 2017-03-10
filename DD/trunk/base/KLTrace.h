#ifndef INCLUDED_KL_TRACE
#define INCLUDED_KL_TRACE

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

typedef enum _TRACE_LEVEL
{
	TRACE_LEVEL_DISABLED=0,
	TRACE_LEVEL_ERROR=1,
	TRACE_LEVEL_WARN=2,
	TRACE_LEVEL_INFO=3,
	TRACE_LEVEL_VERBOSE=4,
	TRACE_LEVEL_MAX=TRACE_LEVEL_VERBOSE,
}TRACE_LEVEL;

typedef enum _EXPORT_WAY
{
	EXPORT_WAY_NONE=0,
	EXPORT_WAY_STDOUT=1,
	EXPORT_WAY_PUBLIC=2,
	EXPORT_WAY_PRIVATE=3,
	EXPORT_WAY_TTYP0=4,
	EXPORT_WAY_TTYP1=5,
	EXPORT_WAY_TTYP2=6,
	EXPORT_WAY_TTYP3=7,
	EXPORT_WAY_TTYP4=8,
	EXPORT_WAY_TTYP5=9,
}EXPORT_WAY;

INT8U MAKE_TRACE_LEVEL_VAL(const char* key);
int MAKE_TRACE_LEVEL_KEY(INT8U val,char* oKey);
INT8U MAKE_TRACE_WAY_VAL(const char* key);
int MAKE_TRACE_WAY_KEY(INT8U val,char* oKey);

#define TRACE_OPTION_DATA_STREAM 1	//输出数据流
#define TRACE_OPTION_SCHEDULE 2	//输出调度信息

#define TRACE_FILE_PATH_LEN	100	//输出信息文件路径字符长度
#define TRACE_FILE_MAX_SIZE 50000	//输出每个文件最大字符数

#define PUBLIC_FILE_MAX_SIZE 5242880	//公共输出文件最大字符数5M

#define PRIVATE_FILE_MAX_SIZE 2097152	//私有输出文件最大字符数2M

struct TraceSetting
{
	INT8U EnableTimeStamp;
	INT8U TraceLevel;
	INT8U TraceWay;
	INT8U DataStreamWay;
	INT8U ScheduleWay;
	char ServerName[CONS_SERVER_NAME_MAX_LEN+5];
	long MaxFileSize;
	long TotalSize;

	inline TraceSetting()
	{
		Init("");
	}
	void Init(char *i_ServerName)
	{
		EnableTimeStamp=1;
		TraceLevel=CONS_DEFAULT_TRACE_LEVEL;
		TraceWay=EXPORT_WAY_PRIVATE;
		DataStreamWay=EXPORT_WAY_NONE;
		ScheduleWay=EXPORT_WAY_NONE;
		strcpy(ServerName,i_ServerName);
		MaxFileSize=TRACE_FILE_MAX_SIZE;
	}
};

class KLTrace
{
public:
	inline KLTrace(TRACE_LEVEL traceLevel,int option ):m_traceLevel(traceLevel),m_option(option)
	{
		m_Timeoutputed=false;
	}
	void operator()(const char *pszFmt, ...);
	inline const TraceSetting& GetSetting() const
	{
		static TraceSetting sDefaultSetting;
		sDefaultSetting.Init("Command");
		sDefaultSetting.TraceLevel=TRACE_LEVEL_VERBOSE;
		sDefaultSetting.TraceWay=EXPORT_WAY_STDOUT;
		return (Setting?(*Setting):sDefaultSetting);
	}
	inline void Reset()
	{
		m_Timeoutputed=false;
	}
public:
	static char* Prefix;
	static TraceSetting* Setting;
private:
	TRACE_LEVEL m_traceLevel;
	int m_option;
	FILE* GetExportFile(INT8U i_ExportType,INT8U i_ExportWay,const char *i_ServerName,long i_iMaxFileSize,long i_TotalSize) const;
	volatile bool m_Timeoutputed;
};

#define KL_ERROR KLTrace(TRACE_LEVEL_ERROR,0)//错误
#define KL_WARN KLTrace(TRACE_LEVEL_WARN,0)//警告
#define KL_INFO KLTrace(TRACE_LEVEL_INFO,0)//信息
#define KL_VERBOSE KLTrace(TRACE_LEVEL_VERBOSE,0)//冗长的

#endif
