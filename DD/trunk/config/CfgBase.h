#ifndef INCLUDED_CFG_BASE
#define INCLUDED_CFG_BASE

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

struct cfg_t;

class CfgBase
{
public:
	static int ParseSetFile(cfg_t* cfg,const char* name,const char* userCode,const char* planKey,string* oCfgFile=NULL);
	static int SaveSetFile(cfg_t* cfg,const char* name,const char* userCode,const char* planKey);
	static int SaveSetFile(string& str,const char* name,const char* userCode,const char* planKey);
	static int DelSetFile(const char* name,const char* userCode,const char* planKey);

private:
	static int ParseCfg(cfg_t* cfg,const char* cfgFile);
};
#endif
