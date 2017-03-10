#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../base/KLTrace.h"
#include "../base/FileBase.h"
#include "../include/Config.h"
#include "CfgBase.h"
#include "localIpCfg.h"

cfg_opt_t OPT_LOCALIP_SETTINGS[] =
{
	CFG_STR("local_ip", "192.168.000.100",CFGF_NOCASE),
	CFG_END()
};

int LocalIpCfg::LoadLocalIp(Context &oContext,const char *cfgFile)
{

	cfg_t* cfg = cfg_init(OPT_LOCALIP_SETTINGS, CFGF_NONE);
	if(cfg_parse(cfg, cfgFile) == CFG_PARSE_ERROR)
	{
		ASSERT(false);
		KL_ERROR("Parse cfg file[%s] failed\n",cfgFile);
		cfg_free(cfg);
		return -1;
	}
	memset(oContext.LocalIp,0x00,sizeof(oContext.LocalIp));
	strncpy(oContext.LocalIp,cfg_getstr(cfg,"local_ip"),sizeof(oContext.LocalIp));
	return 0;
}

int LocalIpCfg::SaveLocalIp(Context &oContext,const char *cfgFile)
{
	string buff;
	buff.reserve(3000);

	FILE *fp =FileBase::OpenFile(cfgFile, "w");
	if (fp==NULL)
	{
		KL_ERROR("Open file %s failed\n",cfgFile);
		return -1;
	}
	
	cfg_t* cfg;
	cfg = cfg_init(OPT_LOCALIP_SETTINGS, CFGF_NONE);
	
	cfg_setstr(cfg,"local_ip",oContext.LocalIp);

	cfg_print(cfg, fp);
	FileBase::Close(fp);
	cfg_free(cfg);
	
	return 0;
}


