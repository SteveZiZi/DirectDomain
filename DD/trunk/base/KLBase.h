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
	int m_Year; ///< ��ݣ�ȡֵ����Ϊ[0,9999]
	int m_Mon; ///< �·ݣ���һ�¿�ʼ��1����һ�£� - ȡֵ����Ϊ[1,12] 
	int m_Day; ///< һ�����е����� - ȡֵ����Ϊ[1,31] 
	int m_Hour; ///< ʱ - ȡֵ����Ϊ[0,23] 
	int m_Min; ///< �� - ȡֵ����Ϊ[0,59] 
	int m_Sec; ///< ��Cȡֵ����Ϊ[0,59] 
	int m_WeekDay; ///< ���ڨCȡֵ����Ϊ[0,6]������0���������죬1��������һ���Դ�����
	int m_YearDay; ///< ��ÿ���1��1�տ�ʼ�������Cȡֵ����Ϊ[1,366]������1����1��1�գ�2����1��2�գ��Դ�����

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
