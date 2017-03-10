#ifndef FIXDATA_CLASS
#define FIXDATA_CLASS

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_TIMER
#include "../base/Timer.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

struct FixData
{
public:
	INT8U Enable;
	INT8U FixMethod;
	MatchTimer FixTimer;//采集调度设置
	INT8U IncreasingFlag;
	INT8U ReportFlag;
	void init();
};
#endif

