#ifndef INCLUDED_TASK_CFG
#define INCLUDED_TASK_CFG

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class TaskCfg
{
public:
	static int LoadDataIDS(Context& oContext,const char* name=NAME_SET_TASK);
	static int LoadTasks(Context& oContext,const char* name=NAME_SET_TASK);
	static int SaveTasks(Context& oContext,const char* name=NAME_SET_TASK);
};
#endif
