#ifndef INCLUDED_KL_BASE
#define INCLUDED_KL_BASE

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif
#ifndef INCLUDED_KL_TRACE
#include "KLTrace.h"
#endif

#include "klb_base.h"
#include "klb_io.h"

struct S_DATE
{
	int m_Year; ///< 年份，取值区间为[0,9999]
	int m_Mon; ///< 月份（从一月开始，1代表一月） - 取值区间为[1,12] 
	int m_Day; ///< 一个月中的日期 - 取值区间为[1,31] 
	int m_Hour; ///< 时 - 取值区间为[0,23] 
	int m_Min; ///< 分 - 取值区间为[0,59] 
	int m_Sec; ///< 秒–取值区间为[0,59] 
	int m_WeekDay; ///< 星期–取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推
	int m_YearDay; ///< 从每年的1月1日开始的天数–取值区间为[1,366]，其中1代表1月1日，2代表1月2日，以此类推

	S_DATE(void){memset(this, 0x00, sizeof(*this));}
};

inline int KLSystem(const char* command)
{
#ifdef WIN32
	KL_INFO("Exec Command %s\n",command);
	return 0;
	//return system(command);
#else
	int ret=system(command);
	usleep(1000);
	KL_VERBOSE("Return %d for Command [%s]\n",ret,command);
	return ret;
#endif
}
//bool GetGlobleLock(int id);
//bool GetGlobleLock(const char* name);

int GetProcessName(PID_T pid,char* opName);

bool IsProcessRunning(PID_T pid,const char* pName=NULL);

int GetFlashSize(int* freezeM,int* totalM);

int GetNetIP(const char* ifname,char* opIP);

bool isValidTime(time_t& inputtime);

time_t MakeTime(const S_DATE &date);

time_t GetStandardTime(const char *pdate, const char *ptime);

bool ConvertBcdStrToDecStr(char *pBcd, INT8U Len,string &str);

void GetBaseDateString(time_t basetime,string &strDate, string &strTime);

#endif
