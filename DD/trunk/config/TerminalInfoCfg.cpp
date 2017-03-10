#include "../share/Context.h"
#include "../share/SysConfig.h"
#include "../config/confuse/confuse.h"
#include "../base/KLTrace.h"
#include "../base/FileBase.h"
#include "CfgBase.h"
#include "../share/SysBase.h"
#include "../share/ipcSharemem.h"
#include "TerminalInfoCfg.h"

cfg_opt_t OPT_TERMINAL_ADDR_CONFIG_SETTINGS[] =
{	
	CFG_STR("RTUA","00,00,00,00", CFGF_NOCASE),
	CFG_END()
};


int TerminalInfoCfg::LoadTerminalConfig(Context& oContext,const char* cfgFile)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_TERMINAL_ADDR_CONFIG_SETTINGS, CFGF_NONE);
	if(cfg_parse(cfg,cfgFile) == CFG_PARSE_ERROR)
	{
		ASSERT(false);
		KL_ERROR("Parse cfg file[%s] failed\n",cfgFile);
		cfg_free(cfg);
		return -1;
	}
	SysConfig& config=oContext.SysConfig;
	config.SetRTUA(cfg_getstr(cfg,"RTUA"));
	IPCSHAREMEM.Update();

	cfg_free(cfg);
	return 0;
}

int TerminalInfoCfg::SaveTerminalConfig(Context& oContext,const char* cfgFile)
{
	FILE *fp =FileBase::OpenFile(cfgFile, "w");
	if (fp==NULL)
	{
		KL_ERROR("Open file %s failed\n",cfgFile);
		return -1;
	}
	cfg_t* cfg;
	cfg = cfg_init(OPT_TERMINAL_ADDR_CONFIG_SETTINGS, CFGF_NONE);
	SysConfig& config=oContext.SysConfig;
	char tep[40];
		
	sprintf(tep,"%02x,%02x,%02x,%02x",(INT8U)config.RTUA[0],(INT8U)config.RTUA[1],(INT8U)config.RTUA[2],(INT8U)config.RTUA[3]);
	cfg_setstr(cfg,"RTUA",tep);
	
	cfg_print(cfg, fp);
	FileBase::Close(fp);
	cfg_free(cfg);
	return 0;
}

