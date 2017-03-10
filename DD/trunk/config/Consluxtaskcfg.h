#ifndef _CONSLUXTASKCFG_H_
#define _CONSLUXTASKCFG_H_

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class ConsLuxTaskCfg
{
public:
	static int LoadConsLuxTasks(Context& oContext,const char* name=NAME_SET_CONSLUXTASK);
	static int SaveTasks(Context& oContext,const char* name=NAME_SET_CONSLUXTASK);
};
#endif

