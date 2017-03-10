#ifndef INCLUDED_LOCALIP_CFG
#define INCLUDED_LOCALIP_CFG

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class LocalIpCfg
{
public:
	static int LoadLocalIp(Context& oContext,const char* name=CFG_LOCAL_IP);
	static int SaveLocalIp(Context& oContext,const char* name=CFG_LOCAL_IP);
};
#endif

