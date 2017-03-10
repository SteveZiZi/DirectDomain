#ifndef INCLUDED_RELAY_CFG
#define INCLUDED_RELAY_CFG

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class RelayCfg
{
public:
	static int LoadRelays(Context& oContext,const char* name=NAME_SET_RELAY);
	static int SaveRelays(Context& oContext,const char* name=NAME_SET_RELAY);
	static int ReplaceRelay(const char *oldaddr,const char *newaddr);
};
#endif
