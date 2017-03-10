#ifndef INCLUDED_METER_CFG
#define INCLUDED_METER_CFG

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;
struct cfg_t;

class MeterCfg
{
public:
	static int LoadMeters(Context& oContext,const char* name=NAME_SET_METER);
	static int SaveMeters(Context& oContext,const char* name=NAME_SET_METER);
private:
	static int LoadMeter(Context& oContext,cfg_t* cfgMeter,INT16U meterNo);
};
#endif
