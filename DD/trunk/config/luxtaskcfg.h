#ifndef _LUXTASKCFG_H_
#define _LUXTASKCFG_H_

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class LuxTaskCfg
{
public:
	static int LoadLuxTasks(Context& oContext,const char* name=NAME_SET_LUXTASK);
	static int SaveTasks(Context& oContext,const char* name=NAME_SET_LUXTASK);
};
#endif

