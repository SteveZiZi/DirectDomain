#ifndef INCLUDED_SYS_CONFIG_CFG
#define INCLUDED_SYS_CONFIG_CFG

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class SysConfigCfg
{
public:
	static int LoadConfig(Context& oContext,const char* cfgFile=FILE_DYJC_SYS_CONFIG);
	static int SaveConfig(Context& oContext,const char* cfgFile=FILE_DYJC_SYS_CONFIG);
	static int LoadGprsInfo(Context &oContext,const char *cfgFile=NAME_SET_GPRS);
	static int SaveGprsInfo(Context &oContext,const char *cfgFile=NAME_SET_GPRS);
	static int LoadPortMap(Context &oContext, const char *fname = CFG_FILE_INI_PORTMAP);
	static int SavePortMap(Context &oContext, const char *fname= CFG_FILE_INI_PORTMAP);
};

#endif
