#ifndef INCLUDED_SERVER_CFG
#define INCLUDED_SERVER_CFG

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;
struct cfg_t;
struct Channel;

class ServerCfg
{
public:
	static int LoadHttxPlanInfo(Context& oContext,const char* name=NAME_SET_HTTX_PLAN);
	static int SaveHttxPlanInfo(Context& oContext,const char* name=NAME_SET_HTTX_PLAN);

	static int LoadHttxInfo(Context& oContext,const char* name=NAME_SET_HTTX);
	static int SaveHttxInfo(Context& oContext,const char* name=NAME_SET_HTTX);
	static int LoadDbcjInfo(Context& oContext,char* name=NAME_SET_GDDBCJ);
	static int SaveDbcjInfo(Context& oContext,char* name=NAME_SET_GDDBCJ);
	static int LoadTraceInfo(Context& oContext,const char* name=NAME_SET_TRACE);
	static int SaveTraceInfo(Context& oContext,const char* name=NAME_SET_TRACE);
	static int LoadPPPInfo(Context& oContext,const char* name=NAME_SET_PPP);
	static int SavePPPInfo(Context& oContext,const char* name=NAME_SET_PPP);
	static int LoadJilianInfo(Context& oContext,const char* name=NAME_SET_JILIAN);
	static int SaveJilianInfo(Context& oContext,const char* name=NAME_SET_JILIAN);
	static int LoadTermIPConfig(Context& oContext, const char *fname = CFG_FILE_INI_IPCONFIG);
	static int SaveTermIPConfig(Context& oContext, const char *fname = CFG_FILE_INI_IPCONFIG);
private:
	static void LoadChannel(cfg_t* cfgChannel,Channel& channel);
};

#endif
