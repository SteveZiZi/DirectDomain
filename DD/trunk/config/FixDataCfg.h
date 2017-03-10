#ifndef FIXDATACFG
#define FIXDATACFG

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

class FixDataCfg
{
public:
	static int LoadFixDataCfg(Context& oContext,const char* cfgFile=NAME_SET_FIXDATA);
	static int SaveFixDataCfg(Context& oContext,const char* cfgFile=NAME_SET_FIXDATA);
	//static void Get_FixTime(const char *m_Time,unsigned char* FixTime);
};
#endif
