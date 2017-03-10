#include "./JilianSlavesCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../base/KLTrace.h"
#include "../base/FileBase.h"
#include "../base/Modem.h"
#include "CfgBase.h"

cfg_opt_t OPT_JILIAN_SLAVE_SETTINGS[] =
{
	CFG_BOOL("Enable",cfg_false,CFGF_NOCASE),
	CFG_STR("CzdAddrs","FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF",CFGF_NOCASE),
	CFG_STR("PbzdAddr","FFFFFFFF",CFGF_NOCASE),
	CFG_INT("CT", 1, CFGF_NOCASE),
	CFG_INT("PT", 1, CFGF_NOCASE),
	CFG_END()
};
int JilianSlavesCfg::LoadJilianSlavesCfg(Context& oContext,const char* cfgFile)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_JILIAN_SLAVE_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,cfgFile,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	oContext.JilianSlaves.Enable=(cfg_getbool(cfg, "Enable")==cfg_true)?1:0;
	oContext.JilianSlaves.SetJilianSlaveMachines(cfg_getstr(cfg,"CzdAddrs"));
	oContext.JilianSlaves.SetPbzdAddr(cfg_getstr(cfg,"PbzdAddr"));
	oContext.JilianSlaves.CT=(INT16U)cfg_getint(cfg, "CT");
	oContext.JilianSlaves.PT=(INT16U)cfg_getint(cfg, "PT");
	cfg_free(cfg);	
	return 0;
}

int JilianSlavesCfg::SaveJilianSlavesCfg(Context& oContext,const char* cfgFile)
{
	JilianSlaves& jlslaves=oContext.JilianSlaves;
	string buff;
	buff.reserve(sizeof(JilianSlaves));

	char tep_3[10];
	memset(tep_3,0,sizeof(tep_3));

	sprintf(tep_3,"%s",jlslaves.Enable==1?"true":"false");

	char tep_JilianSlaveMachines[40];
	memset(tep_JilianSlaveMachines,0,sizeof(tep_JilianSlaveMachines));
	int t=0;
	memset(tep_3,0,sizeof(tep_3));
	for(int i=0;i<4;i++)
	{
		if(i!=3)
		{
			sprintf(tep_3,"%02X%02X%02X%02X,",(INT8U)jlslaves.CzdAddrs[i*4+0],(INT8U)jlslaves.CzdAddrs[i*4+1],(INT8U)jlslaves.CzdAddrs[i*4+2],(INT8U)jlslaves.CzdAddrs[i*4+3]);
		}
		else 
		{
			sprintf(tep_3,"%02X%02X%02X%02X",(INT8U)jlslaves.CzdAddrs[i*4+0],(INT8U)jlslaves.CzdAddrs[i*4+1],(INT8U)jlslaves.CzdAddrs[i*4+2],(INT8U)jlslaves.CzdAddrs[i*4+3]);
		}

		for(int f=0;f<9;f++)
		{
			tep_JilianSlaveMachines[t]=tep_3[f];
			t+=1;
		}
		memset(tep_3,0,sizeof(tep_3));
	}
	
	buff.append("\n\tCzdAddrs=\"").append(tep_JilianSlaveMachines).append("\"");

	memset(tep_3,0,sizeof(tep_3));
	sprintf(tep_3,"%02X%02X%02X%02X",(INT8U)jlslaves.PbzdAddr[0],(INT8U)jlslaves.PbzdAddr[1],(INT8U)jlslaves.PbzdAddr[2],(INT8U)jlslaves.PbzdAddr[3]);
	buff.append("\n\tPbzdAddr=\"").append(tep_3).append("\"");

	sprintf(tep_3,"%d",jlslaves.CT);
	buff.append("\n\tCT=").append(tep_3);

	sprintf(tep_3,"%d",jlslaves.PT);
	buff.append("\n\tPT=").append(tep_3);


	int iFlag=CfgBase::SaveSetFile(buff,cfgFile,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	return iFlag;
}




