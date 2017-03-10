#ifndef PBZDCFG
#define PBZDCFG

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class JilianSlavesCfg
{
public:
	static int LoadJilianSlavesCfg(Context& oContext,const char* cfgFile=NAME_SET_DISTRIBUTE);
	static int SaveJilianSlavesCfg(Context& oContext,const char* cfgFile=NAME_SET_DISTRIBUTE);
};
#endif
