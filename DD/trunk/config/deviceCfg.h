#ifndef _DEVICECFG_H_
#define _DEVICECFG_H_ 

#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../share/Server.h"
#include "../base/FileBase.h"
#include "CfgBase.h"

class DeviceCfg
{
public:
	static int LoadDevices(Context& oContext,const char* name=NAME_SET_device);	
	static int LoadDevice(Context& oContext,cfg_t* cfgDevice,INT16U measureNo);
	static int SaveDevices(Context& oContext,const char* name=NAME_SET_device);
};
	




#endif
