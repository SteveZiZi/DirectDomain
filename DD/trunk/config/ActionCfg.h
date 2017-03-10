#ifndef INCLUDED_ACTION_CFG
#define INCLUDED_ACTION_CFG

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class ActionCfg
{
public:
	static int LoadActions(Context& oContext,const char* name=NAME_SET_ACTION);
	static int SaveActions(Context& oContext,const char* name=NAME_SET_ACTION);
};
#endif
