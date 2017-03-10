#ifndef INCLUDED_CLRELAY_CFG
#define INCLUDED_CLRELAY_CFG

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class CLRelayCfg
{
public:
	static int LoadClRelays(Context& oContext,const char* name=NAME_SET_CLRELAY);
	static int SaveClRelays(Context& oContext,const char* name=NAME_SET_CLRELAY);
	static int ReplaceClRelay(const char *oldaddr,const char *newaddr);
};
#endif
