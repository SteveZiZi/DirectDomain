#ifndef _NORTASKCFG_H_
#define _NORTASKCFG_H_

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class NorTaskCfg
{
public:
	static int LoadNorTasks(Context& oContext,const char* name=NAME_SET_NORTASK);
	static int SaveTasks(Context& oContext,const char* name=NAME_SET_NORTASK);
};
#endif

