#include "ActionCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../base/KLTrace.h"
#include "../base/FileBase.h"
#include "../base/Modem.h"
#include "CfgBase.h"

cfg_opt_t OPT_ACTION_SETTINGS[] =
{
	CFG_STR("Type","", CFGF_NOCASE),
	CFG_STR("ChinaName", "",CFGF_NOCASE),
	CFG_STR("Param", "",CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_ACTIONS_SETTINGS[] =
{
	CFG_SEC("Action", OPT_ACTION_SETTINGS, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),
	CFG_END()
};



int ActionCfg::LoadActions(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_ACTIONS_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		oContext.Actions.Default();
		return -1;
	}
	cfg_t* cfgAction;
	Actions& actions=oContext.Actions;
	actions.Init();
	for(unsigned int i = 0; i < cfg_size(cfg, "Action"); i++)
	{
		cfgAction = cfg_getnsec(cfg, "Action", i);
		const char* key=cfg_title(cfgAction);
		if (key==NULL || strlen(key)==0)
		{
			continue;
		}
		INT8U type=MAKE_ACTION_TYPE_VAL(cfg_getstr(cfgAction, "Type"));
		const char* name=cfg_getstr(cfgAction, "ChinaName");
		const char* param=cfg_getstr(cfgAction, "Param");
		actions.Add(type,key,name,param);
	}
	cfg_free(cfg);
	return 0;
}

int ActionCfg::SaveActions(Context& oContext,const char* name)
{
	Actions& actions=oContext.Actions;
	string buff;
	buff.reserve(3000);
	for(size_t t=0;t<actions.Count;t++)
	{
		Action& info=actions.Item[t];
		char cTemp[200];
		sprintf(cTemp,"Action %s\n{\n",info.Key);
		buff.append(cTemp);

		MAKE_ACTION_TYPE_KEY(info.Type ,cTemp);
		buff.append("\tType=").append(cTemp);

		buff.append("\n\tChinaName=\"").append(info.Name).append("\"");
		buff.append("\n\tParam=\"").append(info.Param).append("\"");

		buff.append("\n}\n\n");
	}
	return CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
}
