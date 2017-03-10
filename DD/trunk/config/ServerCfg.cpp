#include "ServerCfg.h"

#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../base/KLTrace.h"
#include "../base/FileBase.h"
#include "../include/Config.h"
#include "CfgBase.h"
#include "../share/IpcSharemem.h"

cfg_opt_t OPT_DEVICE_RULE_CONFIG_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("SubID","", CFGF_NOCASE),
	CFG_INT("PackageMaxLen", 0, CFGF_NONE),
	CFG_STR("Option","", CFGF_NOCASE),
	CFG_END()
};


cfg_opt_t OPT_HTTX_CONFIG_PLAN_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_BOOL("Default", cfg_false,CFGF_NOCASE),
	CFG_STR("Name", "", CFGF_NONE),
	
	CFG_SEC("DeviceRule", OPT_DEVICE_RULE_CONFIG_SETTINGS, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),

	CFG_END()
};

cfg_opt_t OPT_HTTX_CONFIG_PLANS_SETTINGS[] =
{
	CFG_SEC("Plan", OPT_HTTX_CONFIG_PLAN_SETTINGS, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),
	CFG_END()
};


cfg_opt_t OPT_CHANNEL_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
	CFG_STR("Name", "Serial", CFGF_NONE),
	CFG_STR("Type", "Com", CFGF_NONE),
	CFG_STR("Setting", "2,9600.8.1.0,Serial,NoCD", CFGF_NONE),
	CFG_INT("Priority", 0, CFGF_NONE),

	CFG_INT("ServiceTimes",1, CFGF_NONE),

	CFG_INT("PackageMaxLen",0, CFGF_NONE),
	CFG_INT("CharReadTimeoutMS",CONS_DEAULT_CHAR_READ_TIMEOUT_MS, CFGF_NONE),
	CFG_INT("RecvTimeoutMs",CONS_DEAULT_RECV_TIMEOUT_MS, CFGF_NONE),
	CFG_INT("ReadTimeoutMS",CONS_DEAULT_READ_TIMEOUT_MS, CFGF_NONE),
	CFG_INT("WriteTimeoutMS",CONS_DEAULT_WRITE_TIMEOUT_MS, CFGF_NONE),
	CFG_INT("SubReadTimeoutMS", CONS_DEAULT_SUB_READ_TIMEOUT_MS, CFGF_NONE),

	CFG_INT("IdleTimeoutS", 0, CFGF_NONE),
	CFG_INT("ConfirmTimeoutS", 30, CFGF_NONE),
	CFG_INT("ConnectTimeoutS",30, CFGF_NONE),
	CFG_INT("ConnectSubTimeoutS",30, CFGF_NONE),
	CFG_INT("ConnectInterS",5, CFGF_NONE),

	CFG_BOOL("LogonEnabled", cfg_true,CFGF_NOCASE),
	CFG_BOOL("ReplyEnabled", cfg_true,CFGF_NOCASE),
	CFG_BOOL("ReportEnabled", cfg_true,CFGF_NOCASE),
	CFG_BOOL("ConfirmNeeded", cfg_true,CFGF_NOCASE),
	CFG_BOOL("HeartBeatEnabled", cfg_true,CFGF_NOCASE),
	CFG_BOOL("AutoConnectEnabled", cfg_true,CFGF_NOCASE),

	CFG_BOOL("HexDataOutput", cfg_true,CFGF_NOCASE),
	CFG_BOOL("AscDataOutput", cfg_false,CFGF_NOCASE),

	CFG_END()
};

cfg_opt_t OPT_HTTX_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_BOOL("Master", cfg_true,CFGF_NOCASE),
	CFG_STR_LIST("IncludeRule", "{1-255}", CFGF_NONE),//不再有效，仅为了保持兼容性
	CFG_STR_LIST("ExcludeRule", "{}", CFGF_NONE),//不再有效，仅为了保持兼容性
	CFG_STR("HttxPlan", "", CFGF_NONE),//后台通讯配置方案
	CFG_BOOL("RuleChannelEnabled", cfg_true,CFGF_NOCASE),

	CFG_INT("IdleExitTimeoutM",0, CFGF_NONE),
	CFG_INT("IdleRebootTimeoutM",0, CFGF_NONE),
	CFG_STR("CompressMethod","None",CFGF_NOCASE),
	CFG_STR("EncryptMethod", "None",CFGF_NOCASE),

	CFG_SEC("Channel",OPT_CHANNEL_SETTINGS , CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_HTTXS_SETTINGS[] =
{
	CFG_SEC("Httx", OPT_HTTX_SETTINGS, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_DBCJ_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
		CFG_SEC("Channel",OPT_CHANNEL_SETTINGS , CFGF_MULTI |  CFGF_NOCASE),
		CFG_END()
};

cfg_opt_t OPT_PPP_DIAL_SETTINGS[] =
{
	CFG_STR("Apn", CONS_DEFAULT_KEY,CFGF_NOCASE),
	CFG_STR("TeleNo", CONS_DEFAULT_KEY,CFGF_NOCASE),
	CFG_STR("User", CONS_DEFAULT_KEY,CFGF_NOCASE),
	CFG_STR("Pwd", CONS_DEFAULT_KEY,CFGF_NOCASE),
	CFG_STR("ModemInitCmd",CONS_PPP_DEFAULT_MODEM_INIT_CMD,CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_PPP_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("PortNo","VCom2", CFGF_NOCASE),
	CFG_INT("Baud", CONS_PPP_DEFAULT_BAUD, CFGF_NOCASE),
	CFG_INT("CheckInterM",CONS_PPP_DEFAULT_CHECK_INTER_M, CFGF_NOCASE),
	CFG_INT("CheckFailTimes",CONS_PPP_DEFAULT_CHECK_FAIL_TIMES, CFGF_NOCASE),

	CFG_SEC("Gprs", OPT_PPP_DIAL_SETTINGS, CFGF_NOCASE),
	CFG_SEC("Cdma", OPT_PPP_DIAL_SETTINGS, CFGF_NOCASE),

	CFG_STR("Apn", CONS_DEFAULT_KEY,CFGF_NOCASE),//不再有效，仅为了保持与以前的配置文件兼容
	CFG_STR("TeleNo", CONS_DEFAULT_KEY,CFGF_NOCASE),//不再有效，仅为了保持与以前的配置文件兼容
	CFG_STR("User", CONS_DEFAULT_KEY,CFGF_NOCASE),//不再有效，仅为了保持与以前的配置文件兼容
	CFG_STR("Pwd", CONS_DEFAULT_KEY,CFGF_NOCASE),//不再有效，仅为了保持与以前的配置文件兼容
	CFG_STR("ModemInitCmd",CONS_PPP_DEFAULT_MODEM_INIT_CMD,CFGF_NOCASE),//不再有效，仅为了保持与以前的配置文件兼容

	CFG_END()
};

//2006-6-7，由苏煦烽添加级联进程配置文件字段
cfg_opt_t OPT_JILIAN_SETTINGS[]=
{
	CFG_BOOL("Enable",cfg_true,CFGF_NOCASE),
	CFG_SEC("Channel",OPT_CHANNEL_SETTINGS , CFGF_MULTI | CFGF_NOCASE),

	CFG_END()
};

cfg_opt_t OPT_JILIANS_SETTINGS[] =
{
	CFG_SEC("Jilian", OPT_JILIAN_SETTINGS, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),
	CFG_END()
};

//在加载此信息之前必须先加载装置规约信息

int ServerCfg::LoadHttxPlanInfo(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_HTTX_CONFIG_PLANS_SETTINGS, CFGF_NONE);
	int result=CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());

	HttxPlans& plans=oContext.HttxCfgPlans;
	DeviceRuleInfos& ruleInfos=oContext.DeviceRuleInfos;
	if (result==0)
	{

		cfg_t* cfgPlan;
		cfg_t* cfgRule;
		int planCount=0;
		int defaultIndex=-1;
		for(unsigned int i = 0; i < cfg_size(cfg, "Plan"); i++)
		{
			cfgPlan = cfg_getnsec(cfg, "Plan", i);
			if (planCount>=(int)(sizeof(plans.Item)/sizeof(plans.Item[0])))
			{
				KL_WARN("Too many plans config in httxconfigplan file\n");
				break;
			}
			HttxPlan& plan=plans.Item[planCount++];
			plan.Configed=1;
			plan.Enabled=cfg_getbool(cfgPlan, "Enable");
			if (plan.Enabled && cfg_getbool(cfgPlan, "Default")==cfg_true)
			{
				if (defaultIndex>=0)
				{
					ASSERT(false);
					KL_WARN("Default Plan is already set in plan %s\n",plans.Item[defaultIndex].Key);
				}
				else
				{
					defaultIndex=(int)i;
				}
			}
			memset(plan.Key,0,sizeof(plan.Key));
			strncpy(plan.Key,cfg_title(cfgPlan),sizeof(plan.Key)-1);
			memset(plan.Name,0,sizeof(plan.Name));
			strncpy(plan.Name,cfg_getstr(cfgPlan,"Name"),sizeof(plan.Name)-1);

			int ruleCount=0;
			for(unsigned int k = 0; k < cfg_size(cfgPlan, "DeviceRule"); k++)
			{
				cfgRule = cfg_getnsec(cfgPlan, "DeviceRule", k);
				if (ruleCount>=(int)(sizeof(plan.DeviceRule)/sizeof(plan.DeviceRule[0])))
				{
					KL_WARN("Too many DeviceRule config in plan %s of httxconfigplan file\n",cfg_title(cfgPlan));
					break;
				}
				DeviceRuleConfig& config=plan.DeviceRule[ruleCount++];
				config.Configed=1;
				config.Enabled=cfg_getbool(cfgRule, "Enable");
				for(size_t t=0;t<ruleInfos.Count;t++)
				{
					DeviceRuleInfo& info=ruleInfos.Item[t];
					if (strcasecmp(info.GetKey(),cfg_title(cfgRule))==0)
					{
						config.RuleID=MAKE_DEVICE_RULE_VAL(cfg_title(cfgRule));
						config.RuleSubID=MAKE_DEVICE_RULE_SUB_VAL(cfg_getstr(cfgRule, "SubID"));
						config.PackageMaxLen=(INT32U)cfg_getint(cfgRule,"PackageMaxLen");
						memset(config.Option,0,sizeof(config.Option));
						const char* pOption=cfg_getstr(cfgRule,"Option");
						//suxufeng:[2010-6-25]默认配置为调整时间标签。除非Option为空，否则均为"FixDateLabel"
						if ((pOption != NULL) && (strcmp(pOption, "") != 0) && (strcmp(pOption, "FixDateLabel") != 0))
						{
							pOption = "FixDateLabel";
						}
						strncpy(config.Option,pOption,sizeof(config.Option)-1);
						break;
					}
				}
			}
			plan.RuleCount=ruleCount;
		}
		if (defaultIndex>=0)
		{
			plans.DefaultIndex=defaultIndex;
		}
		else
		{
			plans.DefaultIndex=0;
		}
		plans.Count=planCount;
	}
	cfg_free(cfg);
	if (plans.Count==0)
	{
		HttxPlan& plan=plans.Item[0];
		plan.Configed=0;
		plan.Enabled=1;
		strcpy(plan.Key,"Default");
		strcpy(plan.Key,"默认方案");
		int ruleCount=0;
		for(size_t t=0;t<ruleInfos.Count;t++)
		{
			DeviceRuleConfig& config=plan.DeviceRule[ruleCount++];
			config.Configed=0;
			config.Enabled=1;
			DeviceRuleInfo& info=ruleInfos.Item[t];
			config.RuleID=MAKE_DEVICE_RULE_VAL(info.GetKey());
			config.RuleSubID=DEVICE_RULE_SUB_NONE;
			memset(config.Option,0,sizeof(config.Option));
		}
		plan.RuleCount=ruleCount;

		plans.Count=1;
		plans.DefaultIndex=0;

		KL_WARN("Use default httx plan\n");
	}
	return 0;
}

int ServerCfg::SaveHttxPlanInfo(Context& oContext,const char* name)
{
	HttxPlans& plans=oContext.HttxCfgPlans;
	//DeviceRuleInfos& ruleInfos=oContext.DeviceRuleInfos;

	string buff;
	string sStr;
	sStr.reserve(500);
	buff.reserve(3000);
	for(size_t i=0;i<sizeof(plans.Item)/sizeof(plans.Item[0]);i++)
	{
		HttxPlan& plan=plans.Item[i];
		if (plan.Configed==0)
		{
			continue;
		}
		char cTemp[200];
		sprintf(cTemp,"Plan %s",plan.Key);
		buff.append(cTemp);
		sprintf(cTemp,"%s",plan.Enabled==1?"true":"false");
		buff.append("\n{\n\tEnable=\"").append(cTemp).append("\"");
		sprintf(cTemp,"%s",plans.DefaultIndex==i?"true":"false");
		buff.append("\n\tDefault=\"").append(cTemp).append("\"");
		sprintf(cTemp,"\n\tName=\"%s\"",plan.Name);
		buff.append(cTemp);
		buff.append("\n");
		for(size_t t=0;t<plan.RuleCount;t++)
		{
			DeviceRuleConfig& config=plan.DeviceRule[t];
			if (config.Configed==0)
			{
				continue;
			}
			MAKE_DEVICE_RULE_KEY(config.RuleID,cTemp);
			buff.append("\n\tDeviceRule ").append(cTemp).append("\n\t{");
		
			sprintf(cTemp,"%s",config.Enabled==1?"true":"false");
			buff.append("\n\t\tEnable=\"").append(cTemp).append("\"");

			MAKE_DEVICE_RULE_SUB_KEY(config.RuleSubID,cTemp);
			buff.append("\n\t\tSubID=\"").append(cTemp).append("\"");
			sprintf(cTemp,"\n\t\tPackageMaxLen=%d",config.PackageMaxLen);
			buff.append(cTemp);
			sStr.erase();
			buff.append("\n\t\tOption=\"").append(config.Option).append("\"");
			buff.append("\n\t}\n");
		}
		buff.append("\n}\n");
	}
	return CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
}


void ServerCfg::LoadChannel(cfg_t* cfgChannel,Channel& channel)
{
	channel.Enabled=(cfg_getbool(cfgChannel, "Enable")==cfg_true?1:0);
	memset(channel.Name,0,sizeof(channel.Name));
	strncpy(channel.Name,cfg_getstr(cfgChannel,"Name"),sizeof(channel.Name)-1);
	channel.Type=MAKE_CHANNEL_TYPE_VAL(cfg_getstr(cfgChannel, "Type"));
	memset(&channel.Setting,0,sizeof(channel.Setting));
	channel.SettingParse(channel.Type,cfg_getstr(cfgChannel,"Setting"));
	channel.Priority=(INT8U)cfg_getint(cfgChannel, "Priority");
	channel.ServiceTimes=(INT8U)cfg_getint(cfgChannel, "ServiceTimes");

	channel.PackageMaxLen=(INT32S)cfg_getint(cfgChannel, "PackageMaxLen");
	channel.CharReadTimeoutMs=(INT32U)cfg_getint(cfgChannel, "CharReadTimeoutMS");
	channel.RecvTimeoutMs=(INT32U)cfg_getint(cfgChannel, "RecvTimeoutMs");
	channel.ReadTimeoutMS=(INT32U)cfg_getint(cfgChannel, "ReadTimeoutMS");
	channel.WriteTimeoutMS=(INT32U)cfg_getint(cfgChannel, "WriteTimeoutMS");
	channel.SubReadTimeoutMS=(INT32U)cfg_getint(cfgChannel, "SubReadTimeoutMS");

	channel.IdleTimeoutS=(INT16U)cfg_getint(cfgChannel, "IdleTimeoutS");
	channel.ConfirmTimeoutS=(INT16U)cfg_getint(cfgChannel, "ConfirmTimeoutS");
	channel.ConnectTimeoutS=(INT16U)cfg_getint(cfgChannel, "ConnectTimeoutS");
	channel.ConnectSubTimeoutS=(INT16U)cfg_getint(cfgChannel, "ConnectSubTimeoutS");
	channel.ConnectInterS=(INT16U)cfg_getint(cfgChannel, "ConnectInterS");

	channel.LogonEnabled=(cfg_getbool(cfgChannel, "LogonEnabled")==cfg_true?1:0);
	channel.ReplyEnabled=(cfg_getbool(cfgChannel, "ReplyEnabled")==cfg_true?1:0);
	channel.ReportEnabled=(cfg_getbool(cfgChannel, "ReportEnabled")==cfg_true?1:0);
	channel.ConfirmNeeded=(cfg_getbool(cfgChannel, "ConfirmNeeded")==cfg_true?1:0);
	channel.HeartBeatEnabled=(cfg_getbool(cfgChannel, "HeartBeatEnabled")==cfg_true?1:0);
	channel.AutoConnectEnabled=(cfg_getbool(cfgChannel, "AutoConnectEnabled")==cfg_true?1:0);

	channel.HexDataOutput=(cfg_getbool(cfgChannel, "HexDataOutput")==cfg_true?1:0);
	channel.AscDataOutput=(cfg_getbool(cfgChannel, "AscDataOutput")==cfg_true?1:0);
}



int ServerCfg::LoadHttxInfo(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_HTTXS_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	ServerInfos& serverInfos=oContext.ServerInfos;
	cfg_t* cfgHttx;
	cfg_t* cfgChannel;
	for(unsigned int i = 0; i < cfg_size(cfg, "Httx"); i++)
	{
		cfgHttx = cfg_getnsec(cfg, "Httx", i);
		for(size_t t=0;t<sizeof(serverInfos.Httx)/sizeof(serverInfos.Httx[0]);t++)
		{
			HttxServerInfo& info=serverInfos.Httx[t];
			if (strcasecmp(cfg_title(cfgHttx),info.BaseInfo.Name)==0 || strcasecmp(cfg_title(cfgHttx),info.BaseInfo.Key)==0)
			{
				info.Configed=1;
				info.BaseInfo.Enabled=cfg_getbool(cfgHttx, "Enable");
				info.Master=cfg_getbool(cfgHttx,"Master");
				info.CompressMethod=MAKE_COMPRESS_METHOD_VAL(cfg_getstr(cfgHttx,"CompressMethod"));
				info.EncryptMethod=MAKE_ENCRYPT_METHOD_VAL(cfg_getstr(cfgHttx,"EncryptMethod"));
				info.IdleExitTimeoutM=(INT16U)cfg_getint(cfgHttx,"IdleExitTimeoutM");
				info.IdleRebootTimeoutM=(INT16U)cfg_getint(cfgHttx,"IdleRebootTimeoutM");
				if (info.IdleRebootTimeoutM>0 && info.IdleRebootTimeoutM<info.IdleExitTimeoutM)
				{
					info.IdleRebootTimeoutM=info.IdleExitTimeoutM;
				}

				memset(info.PlanKey,0,sizeof(info.PlanKey));
				strncpy(info.PlanKey,cfg_getstr(cfgHttx,"HttxPlan"),sizeof(info.PlanKey)-1);
				info.RuleChannelEnabled=(cfg_getbool(cfgHttx, "RuleChannelEnabled")==cfg_true?1:0);
				info.ChannelCount=0;
				for(unsigned int k = 0; k < cfg_size(cfgHttx, "Channel"); k++)
				{
					cfgChannel = cfg_getnsec(cfgHttx, "Channel", k);

					if (info.ChannelCount>=sizeof(info.Channels)/sizeof(info.Channels[0]))
					{
						KL_ERROR("Too many channels for HttxServer %s\n",info.BaseInfo.Name);
						break;
					}
					LoadChannel(cfgChannel,info.Channels[info.ChannelCount++]);
				}
				break;
			}
		}
	}
	cfg_free(cfg);
	return 0;
}

int ServerCfg::LoadDbcjInfo(Context& oContext,char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_DBCJ_SETTINGS, CFGF_NONE);

	name = NAME_SET_GDDBCJ;

	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}	
	if(cfg_getbool(cfg, "Enable")!=cfg_true) 
	{
		return -1;
	}
	ServerInfos& serverInfos=oContext.ServerInfos;
	DbcjServerInfo& info=serverInfos.Dbcj;
	info.Configed=1;
	info.BaseInfo.Enabled=1;
	cfg_t* cfgChannel;
	for(unsigned int i = 0; i < cfg_size(cfg, "Channel"); i++)
	{
		cfgChannel = cfg_getnsec(cfg, "Channel", i);
		if (cfg_getbool(cfgChannel, "Enable")==cfg_true)
		{
			LoadChannel(cfgChannel,info.Channel[i]);
			//info.Channel[i].Type=(CHANNEL_TYPE)i+10;

		}
	}

	cfg_free(cfg);
	return 0;
}

int ServerCfg::SaveDbcjInfo(Context& oContext,char* name)
{
	ServerInfos& serverInfos=oContext.ServerInfos;
	string buff;
	string sStr;
	sStr.reserve(500);
	buff.reserve(3000);
	DbcjServerInfo *pInfo=&serverInfos.Dbcj;

	name = NAME_SET_GDDBCJ;

	if (pInfo->Configed==0)
	{
		return -1;
	}
	
	char cTemp[200];
	sprintf(cTemp,"%s",pInfo->BaseInfo.Enabled==1?"true":"false");
	buff.append("Enable=\"").append(cTemp).append("\"");
	for(size_t i=0;i<sizeof(serverInfos.Dbcj.Channel)/sizeof(serverInfos.Dbcj.Channel[0]);i++)
	{
		Channel *pChannel=&pInfo->Channel[i];
		buff.append("\nChannel\n{");
		sprintf(cTemp,"%s",pChannel->IsValid()==CHANNEL_TYPE_UNKNOWN?"false":"true");
		buff.append("\n\tEnable=\"").append(cTemp).append("\"");
		buff.append("\n\tName=\"").append(pChannel->Name).append("\"");
		MAKE_CHANNEL_TYPE_KEY(pChannel->Type,cTemp);
		buff.append("\n\tType=\"").append(cTemp).append("\"");
		sStr.erase();
		pChannel->SettingToString(sStr);
		buff.append("\n\tSetting=\"").append(sStr).append("\"");

		sprintf(cTemp,"%s",pChannel->AutoConnectEnabled==1?"true":"false");
		buff.append("\n\tAutoConnectEnabled=\"").append(cTemp).append("\"");
		sprintf(cTemp,"%d",pChannel->CharReadTimeoutMs);
		buff.append("\n\tCharReadTimeoutMS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->ReadTimeoutMS);
		buff.append("\n\tReadTimeoutMS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->WriteTimeoutMS);
		buff.append("\n\tWriteTimeoutMS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->SubReadTimeoutMS);
		buff.append("\n\tSubReadTimeoutMS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->ConfirmTimeoutS);

		buff.append("\n\tConfirmTimeoutS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->ConnectTimeoutS);
		buff.append("\n\tConnectTimeoutS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->ConnectSubTimeoutS);
		buff.append("\n\tConnectSubTimeoutS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->ConnectInterS);
		buff.append("\n\tConnectInterS=").append(cTemp);

		sprintf(cTemp,"%s",pChannel->HexDataOutput==1?"true":"false");
		buff.append("\n\tHexDataOutput=\"").append(cTemp).append("\"");
		sprintf(cTemp,"%s",pChannel->AscDataOutput==1?"true":"false");
		buff.append("\n\tAscDataOutput=\"").append(cTemp).append("\"");
		buff.append("\n}");
	}
	return CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	return -1;
}

int ServerCfg::LoadTraceInfo(Context& oContext,const char* name)
{
	cfg_opt_t OPT_TRACE_SETTINGS[] =
	{
		CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
		CFG_STR("Level", "Warn", CFGF_NOCASE),
		CFG_STR("TraceWay","None", CFGF_NOCASE),
		CFG_STR("DataStreamWay","None", CFGF_NOCASE),
		CFG_STR("ScheduleWay","None", CFGF_NOCASE),
		CFG_INT("MaxFileSize", 20000, CFGF_NOCASE),//默认20K
		CFG_INT("TotalSize", 2000000, CFGF_NOCASE),//默认2M
		CFG_END()
	};

	cfg_opt_t OPT_TRACES_SETTINGS[] =
	{
		CFG_SEC("Trace", OPT_TRACE_SETTINGS, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),
		CFG_END()
	};
	ServerInfos& serverInfos=oContext.ServerInfos;
	cfg_t* cfg;
	cfg = cfg_init(OPT_TRACES_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	cfg_t* cfgTrace;
	//const char dbcjname[]={"dbcj3"};
	for(unsigned int i = 0; i < cfg_size(cfg, "Trace"); i++)
	{
		cfgTrace = cfg_getnsec(cfg, "Trace", i);
		if (cfg_getbool(cfgTrace, "Enable")==cfg_true)
		{
			ServerInfo** baseInfos=serverInfos.GetBaseInfos();
			for(size_t i=0;i<serverInfos.InfoCount;i++)
			{
				ServerInfo* pInfo=baseInfos[i];				
				if (strcmp(cfg_title(cfgTrace),pInfo->Name)==0 || strcmp(cfg_title(cfgTrace),pInfo->Key)==0)
				{
					pInfo->TraceSetting.TraceLevel=MAKE_TRACE_LEVEL_VAL(cfg_getstr(cfgTrace, "Level"));
					pInfo->TraceSetting.TraceWay=MAKE_TRACE_WAY_VAL(cfg_getstr(cfgTrace, "TraceWay"));
					pInfo->TraceSetting.DataStreamWay=MAKE_TRACE_WAY_VAL(cfg_getstr(cfgTrace, "DataStreamWay"));
					pInfo->TraceSetting.ScheduleWay=MAKE_TRACE_WAY_VAL(cfg_getstr(cfgTrace, "ScheduleWay"));
					pInfo->TraceSetting.MaxFileSize=cfg_getint(cfgTrace, "MaxFileSize");
					pInfo->TraceSetting.TotalSize=cfg_getint(cfgTrace, "TotalSize");
				}
				//else if (strcmp(cfg_title(cfgTrace),dbcjname)==0)
				//{				
				//	pInfo->TraceSetting.TraceLevel=MAKE_TRACE_LEVEL_VAL(cfg_getstr(cfgTrace, "Level"));
				//	pInfo->TraceSetting.TraceWay=MAKE_TRACE_WAY_VAL(cfg_getstr(cfgTrace, "TraceWay"));
				//	pInfo->TraceSetting.DataStreamWay=MAKE_TRACE_WAY_VAL(cfg_getstr(cfgTrace, "DataStreamWay"));
				//	pInfo->TraceSetting.ScheduleWay=MAKE_TRACE_WAY_VAL(cfg_getstr(cfgTrace, "ScheduleWay"));
				//	pInfo->TraceSetting.MaxFileSize=cfg_getint(cfgTrace, "MaxFileSize");

				//}
			}
		}
	}
	cfg_free(cfg);
	return 0;
}

int ServerCfg::SaveTraceInfo(Context& oContext,const char* name)
{
	ServerInfos& serverInfos=oContext.ServerInfos;
	string buff;
	buff.reserve(1200);
	buff.append("#Level:Disabled=0,Error=1,Warn=2,Info=3,Verbose=4\n\n");

	ServerInfo** baseInfos=serverInfos.GetBaseInfos();
	for(size_t t=0;t<ServerInfos::InfoCount;t++)
	{
		ServerInfo* pInfo=baseInfos[t];
		struct TraceSetting& setting=pInfo->TraceSetting;
		
		char temp[CONS_KEY_MAX_LEN];
		sprintf(temp,"%d",setting.TraceLevel);
		buff.append("Trace ").append(pInfo->Name).append("\n{\n");
		buff.append("\tEnable=true");
		MAKE_TRACE_LEVEL_KEY(setting.TraceLevel,temp);
		buff.append("\n\tLevel=\"").append(temp).append("\"");
		MAKE_TRACE_WAY_KEY(setting.TraceWay,temp);
		buff.append("\n\tTraceWay=\"").append(temp);
		MAKE_TRACE_WAY_KEY(setting.DataStreamWay,temp);
		buff.append("\"\n\tDataStreamWay=\"").append(temp);
		MAKE_TRACE_WAY_KEY(setting.ScheduleWay,temp);
		buff.append("\"\n\tScheduleWay=\"").append(temp);
		sprintf(temp,"%d",(INT32S)setting.MaxFileSize);
		buff.append("\"\n\tMaxFileSize=").append(temp);
		sprintf(temp,"%d",(INT32S)setting.TotalSize);
		buff.append("\n\tTotalSize=").append(temp);
		buff.append("\n}\n\n");
	}
	return CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	//return 0;
}
int ServerCfg::SaveHttxInfo(Context& oContext,const char* name)
{
	ServerInfos& serverInfos=oContext.ServerInfos;
	string buff;
	string sStr;
	sStr.reserve(500);
	buff.reserve(3000);
	for(size_t i=0;i<sizeof(serverInfos.Httx)/sizeof(serverInfos.Httx[0]);i++)
	{
		HttxServerInfo *pInfo=&serverInfos.Httx[i];
		if (pInfo->Configed==0)
		{
			continue;
		}
		char cTemp[200];
		sprintf(cTemp,"Httx httx%d",i+1);
		buff.append(cTemp);

		sprintf(cTemp,"%s",pInfo->BaseInfo.Enabled==1?"true":"false");
		buff.append("\n{\n\tEnable=\"").append(cTemp).append("\"");

		sprintf(cTemp,"%s",pInfo->Master==1?"true":"false");
		buff.append("\n\tMaster=\"").append(cTemp).append("\"");

		MAKE_COMPRESS_METHOD_KEY(pInfo->CompressMethod,cTemp);
		buff.append("\n\tCompressMethod=\"").append(cTemp).append("\"");
		MAKE_ENCRYPT_METHOD_KEY(pInfo->EncryptMethod,cTemp);
		buff.append("\n\tEncryptMethod=\"").append(cTemp).append("\"");
		sprintf(cTemp,"%d",pInfo->IdleExitTimeoutM);
		buff.append("\n\tIdleExitTimeoutM=").append(cTemp);
		sprintf(cTemp,"%d",pInfo->IdleRebootTimeoutM);
		buff.append("\n\tIdleRebootTimeoutM=").append(cTemp);
		buff.append("\n\tHttxPlan=\"").append(pInfo->PlanKey).append("\"");
		sprintf(cTemp,"%s",pInfo->RuleChannelEnabled==1?"true":"false");
		buff.append("\n\tRuleChannelEnabled=\"").append(cTemp).append("\""); 
		for(size_t t=0;t<pInfo->ChannelCount;t++)
		{
			Channel *pChannel=&pInfo->Channels[t];
			buff.append("\n\tChannel\n\t{");
			sprintf(cTemp,"%s",pChannel->Enabled!=0?"true":"false");
			buff.append("\n\t\tEnable=\"").append(cTemp).append("\"");
			//buff.append("\n\t\tName=\"").append(pChannel->Name).append("\"");
			if(pChannel->Name[0]==0x00)
			{
				KL_VERBOSE("没有通道类型\n");
				//cTemp = "Com";
				memset(cTemp,0x00,sizeof(cTemp));
				strncpy(cTemp,"Serial",sizeof("Serial"));
				//buff.append("\n\t\tName=\"").append(pChannel->Name).append("\"");
				buff.append("\n\t\tName=\"").append(cTemp).append("\"");
			}
			else
			{
				buff.append("\n\t\tName=\"").append(pChannel->Name).append("\"");
			}
			MAKE_CHANNEL_TYPE_KEY(pChannel->Type,cTemp);
			if(cTemp[0]==0x00)
			{
				KL_VERBOSE("没有通道类型\n");
				//cTemp = "Com";
				memset(cTemp,0x00,sizeof(cTemp));
				strncpy(cTemp,"Com",sizeof("Com"));
				buff.append("\n\t\tType=\"").append(cTemp).append("\"");
				sStr.erase();
				///pChannel->SettingToString(sStr);
				sStr="2,9600.8.1.0,Serial,NoCD";
				buff.append("\n\t\tSetting=\"").append(sStr).append("\"");
			}
			else
			{
				buff.append("\n\t\tType=\"").append(cTemp).append("\"");
				sStr.erase();
				pChannel->SettingToString(sStr);
				buff.append("\n\t\tSetting=\"").append(sStr).append("\"");
			}
			sprintf(cTemp,"%d",pChannel->Priority);
			buff.append("\n\t\tPriority=").append(cTemp);
			sprintf(cTemp,"%s",pChannel->AutoConnectEnabled==1?"true":"false");
			buff.append("\n\t\tAutoConnectEnabled=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%d",pChannel->CharReadTimeoutMs);
			buff.append("\n\t\tCharReadTimeoutMS=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->RecvTimeoutMs);
			buff.append("\n\t\tRecvTimeoutMs=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->ReadTimeoutMS);
			buff.append("\n\t\tReadTimeoutMS=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->WriteTimeoutMS);
			buff.append("\n\t\tWriteTimeoutMS=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->SubReadTimeoutMS);
			buff.append("\n\t\tSubReadTimeoutMS=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->ConfirmTimeoutS);
			buff.append("\n\t\tConfirmTimeoutS=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->ConnectTimeoutS);
			buff.append("\n\t\tConnectTimeoutS=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->ConnectSubTimeoutS);
			buff.append("\n\t\tConnectSubTimeoutS=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->ConnectInterS);
			buff.append("\n\t\tConnectInterS=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->PackageMaxLen);
			buff.append("\n\t\tPackageMaxLen=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->ServiceTimes);
			buff.append("\n\t\tServiceTimes=").append(cTemp);
			sprintf(cTemp,"%d",pChannel->IdleTimeoutS);
			buff.append("\n\t\tIdleTimeoutS=").append(cTemp);
			sprintf(cTemp,"%s",pChannel->LogonEnabled==1?"true":"false");
			buff.append("\n\t\tLogonEnabled=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%s",pChannel->ReplyEnabled==1?"true":"false");
			buff.append("\n\t\tReplyEnabled=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%s",pChannel->ReportEnabled==1?"true":"false");
			buff.append("\n\t\tReportEnabled=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%s",pChannel->ConfirmNeeded==1?"true":"false");
			buff.append("\n\t\tConfirmNeeded=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%s",pChannel->HeartBeatEnabled==1?"true":"false");
			buff.append("\n\t\tHeartBeatEnabled=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%s",pChannel->HexDataOutput==1?"true":"false");
			buff.append("\n\t\tHexDataOutput=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%s",pChannel->AscDataOutput==1?"true":"false");
			buff.append("\n\t\tAscDataOutput=\"").append(cTemp).append("\"");
			buff.append("\n\t}");
		}
		buff.append("\n}\n\n");
	}
	return CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
}

int ServerCfg::LoadPPPInfo(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_PPP_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	PPPServerInfo& pppInfo=oContext.ServerInfos.PPP;
	pppInfo.Setting.Enabled=(cfg_getbool(cfg, "Enable")==cfg_true?1:0);
	//2007-12-3苏煦烽添加，没有这一行的话，就算PPP的配置是FALSE，PPP进程也照样能够启动。
	pppInfo.BaseInfo.Enabled = pppInfo.Setting.Enabled;

	Channel& channel=pppInfo.Setting.ModemChannel;

	int PortNo = 0;
	sscanf(cfg_getstr(cfg,"PortNo"), "VCom%d", &PortNo);
	channel.Setting.Com.PortNo=(INT8U)PortNo;

	if (cfg_getint(cfg,"Baud")>0)
	{
		channel.Setting.Com.PortSet.Baud=cfg_getint(cfg,"Baud");
	}
	pppInfo.Setting.CheckInterM=(INT16U)cfg_getint(cfg,"CheckInterM");
	pppInfo.Setting.CheckFailTimes=(INT16U)cfg_getint(cfg,"CheckFailTimes");

	for(int i=0;i<2;i++)
	{
		cfg_t* cfgDial = cfg_getsec(cfg, i==0?"Gprs":"Cdma");
		PPPDialSetting* pDialSetting=(i==0?&pppInfo.Setting.Gprs:&pppInfo.Setting.Cdma);

		memset(pDialSetting->Apn,0,sizeof(pDialSetting->Apn));
		strncpy(pDialSetting->Apn,cfg_getstr(cfgDial,"Apn"),sizeof(pDialSetting->Apn)-1);

		memset(pDialSetting->TeleNo,0,sizeof(pDialSetting->TeleNo));
		strncpy(pDialSetting->TeleNo,cfg_getstr(cfgDial,"TeleNo"),sizeof(pDialSetting->TeleNo)-1);

		memset(pDialSetting->User,0,sizeof(pDialSetting->User));
		strncpy(pDialSetting->User,cfg_getstr(cfgDial,"User"),sizeof(pDialSetting->User)-1);
		memset(pDialSetting->Pwd,0,sizeof(pDialSetting->Pwd));
		strncpy(pDialSetting->Pwd,cfg_getstr(cfgDial,"Pwd"),sizeof(pDialSetting->Pwd)-1);

		memset(pDialSetting->ModemInitCmd,0,sizeof(pDialSetting->ModemInitCmd));
		strncpy(pDialSetting->ModemInitCmd,cfg_getstr(cfgDial,"ModemInitCmd"),sizeof(pDialSetting->ModemInitCmd)-1);
	}
	return 0;
}
int ServerCfg::SavePPPInfo(Context& oContext,const char* name)
{
	string buff;
	buff.reserve(3000);
	char cTemp[200];

	PPPServerInfo& pppInfo=oContext.ServerInfos.PPP;
	buff.append("Enable=\"").append(pppInfo.Setting.Enabled==0?"false":"true").append("\"");
	Channel& channel=pppInfo.Setting.ModemChannel;
	sprintf(cTemp,"VCom%d",channel.Setting.Com.PortNo);
	buff.append("\nPortNo=").append(cTemp);
	sprintf(cTemp,"%d",channel.Setting.Com.PortSet.Baud);
	buff.append("\nBaud=").append(cTemp);
	sprintf(cTemp,"%d",pppInfo.Setting.CheckInterM);
	buff.append("\nCheckInterM=").append(cTemp);
	sprintf(cTemp,"%d",pppInfo.Setting.CheckFailTimes);
	buff.append("\nCheckFailTimes=").append(cTemp);
	for(int i=0;i<2;i++)
	{
		PPPDialSetting* pDialSetting=(i==0?&pppInfo.Setting.Gprs:&pppInfo.Setting.Cdma);
		buff.append("\n\n").append(i==0?"Gprs":"Cdma").append("\n{\n\t");
		buff.append("Apn=\"").append(pDialSetting->Apn).append("\"\n\t");
		buff.append("TeleNo=\"").append(pDialSetting->TeleNo).append("\"\n\t");
		buff.append("User=\"").append(pDialSetting->User).append("\"\n\t");
		buff.append("Pwd=\"").append(pDialSetting->Pwd).append("\"\n\t");
		char temp_ModemInitCmd[CONS_MODEM_INIT_CMD_MAX_LEN+1];
		sprintf(temp_ModemInitCmd,"%s,\\\"%s\\\"","AT+CGDCONT=1,\\\"IP\\\"",pDialSetting->Apn);
		buff.append("ModemInitCmd=\"").append(temp_ModemInitCmd).append("\"\n}");
	}
	return CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
}

//2006-6-7，由苏煦烽添加
int ServerCfg::LoadJilianInfo(Context& oContext,const char* name)
{
	cfg_t *cfg;
	cfg = cfg_init(OPT_JILIANS_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	ServerInfos& serverInfos=oContext.ServerInfos;
	cfg_t* cfgJilian;
	cfg_t* cfgChannel;
	for(unsigned int i = 0; i < cfg_size(cfg, "Jilian"); i++)
	{
		cfgJilian = cfg_getnsec(cfg, "Jilian", i);
		//if (cfg_getbool(cfgJilian, "Enable")==cfg_true)
		//{
			for(size_t t=0;t<sizeof(serverInfos.Jilian)/sizeof(serverInfos.Jilian[0]);t++)
			{
				JilianServerInfo& info=serverInfos.Jilian[t];
				info.Configed=1;
				info.BaseInfo.Enabled=cfg_getbool(cfgJilian, "Enable");
				if (strcasecmp(cfg_title(cfgJilian),info.BaseInfo.Name)==0 || strcasecmp(cfg_title(cfgJilian),info.BaseInfo.Key)==0)
				{
					//info.BaseInfo.Enabled=cfg_getbool(cfgJilian, "Enable");
					//info.BaseInfo.Enabled=1;
					if (cfg_size(cfgJilian, "Channel")==1)
					{
						cfgChannel = cfg_getnsec(cfgJilian, "Channel",0);
						LoadChannel(cfgChannel,info.Channel);
					}
					break;
				}
			}
		//}
	}
	cfg_free(cfg);
	return 0;
}

//2006-6-7，由苏煦烽添加
int ServerCfg::SaveJilianInfo(Context& oContext,const char* name)
{
	ServerInfos& serverInfos=oContext.ServerInfos;
	string buff;
	string sStr;
	sStr.reserve(500);
	buff.reserve(3000);
	for(size_t i=0;i<sizeof(serverInfos.Jilian)/sizeof(serverInfos.Jilian[0]);i++)
	{
		JilianServerInfo *pInfo=&serverInfos.Jilian[i];
		if (pInfo->Configed==0)
		{
			continue;
		}
		char cTemp[200];
		sprintf(cTemp,"Jilian jilian%d",i+1);
		buff.append(cTemp);
		sprintf(cTemp,"%s",pInfo->BaseInfo.Enabled==1?"true":"false");
		buff.append("\n{\n\tEnable=\"").append(cTemp).append("\"");
		//MAKE_JILIAN_MASTER_MOD_KEY(pInfo->JilianMasterMod,cTemp);
		//buff.append("\n\tJilianMasterMod=\"").append(cTemp).append("\"");

		Channel *pChannel=&pInfo->Channel;
		buff.append("\n\tChannel\n\t{");
		sprintf(cTemp,"%s",pChannel->IsValid()==CHANNEL_TYPE_UNKNOWN?"false":"true");
		buff.append("\n\t\tEnable=\"").append(cTemp).append("\"");

		//下边4行刘志强添加200711270150

		MAKE_CHANNEL_TYPE_KEY(pChannel->Type,cTemp);
		buff.append("\n\t\tType=\"").append(cTemp).append("\"");
		sprintf(cTemp,"%d",pChannel->Priority);
		buff.append("\n\t\tPriority=").append(cTemp);
		//
		if(pChannel->Name[0]==0x00)
		{
			memset(cTemp,0x00,sizeof(cTemp));
			strncpy(cTemp,"JilianCom",sizeof("JilianCom"));
			buff.append("\n\t\tName=\"").append(cTemp).append("\"");
		}
		else
		{
			buff.append("\n\t\tName=\"").append(pChannel->Name).append("\"");
		}

		//buff.append("\n\t\tType=\"").append(cTemp).append("\"");
		sStr.erase();
		pChannel->SettingToString(sStr);
		buff.append("\n\t\tSetting=\"").append(sStr).append("\"");

		//buff.append("\n\t\tName=\"").append(pChannel->Name).append("\"");
		//buff.append("\n\t\tType=\"").append(cTemp).append("\"");
		//sStr.erase();
		//pChannel->SettingToString(sStr);
		//buff.append("\n\t\tSetting=\"").append(sStr).append("\"");

		sprintf(cTemp,"%s",pChannel->AutoConnectEnabled==1?"true":"false");
		buff.append("\n\t\tAutoConnectEnabled=\"").append(cTemp).append("\"");
		//sprintf(cTemp,"%d",pChannel->CharReadTimeoutMs);
		//buff.append("\n\t\tCharReadTimeoutMS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->RecvTimeoutMs);
		buff.append("\n\t\tRecvTimeoutMs=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->WriteTimeoutMS);
		buff.append("\n\t\tWriteTimeoutMS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->ReadTimeoutMS);
		buff.append("\n\t\tReadTimeoutMS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->SubReadTimeoutMS);
		buff.append("\n\t\tSubReadTimeoutMS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->IdleTimeoutS);
		buff.append("\n\t\tIdleTimeoutS=").append(cTemp);

		sprintf(cTemp,"%d",pChannel->ConfirmTimeoutS);
		buff.append("\n\t\tConfirmTimeoutS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->ConnectTimeoutS);
		buff.append("\n\t\tConnectTimeoutS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->ConnectSubTimeoutS);
		buff.append("\n\t\tConnectSubTimeoutS=").append(cTemp);
		sprintf(cTemp,"%d",pChannel->ConnectInterS);
		buff.append("\n\t\tConnectInterS=").append(cTemp);

		//sprintf(cTemp,"%d",pChannel->ServiceTimes);
		//buff.append("\n\t\tServerceTimes=").append(cTemp);
		sprintf(cTemp,"%s",pChannel->ReplyEnabled==1?"true":"false");
		buff.append("\n\t\tReplyEnabled=\"").append(cTemp).append("\"");
		//sprintf(cTemp,"%d",pChannel->ReportEnabled==1?"true":"false");
		//buff.append("\n\t\tReportEnabled=\"").append(cTemp).append("\"");
		//sprintf(cTemp,"%d",pChannel->ConfirmNeeded==1?"true":"false");
		//buff.append("\n\t\tConfirmNeeded=\"").append(cTemp).append("\"");
		//sprintf(cTemp,"%d",pChannel->HeartBeatEnabled==1?"true":"false");
		//buff.append("\n\t\tIHeartBeatEnabled=\"").append(cTemp).append("\"");

		sprintf(cTemp,"%s",pChannel->HexDataOutput==1?"true":"false");
		buff.append("\n\t\tHexDataOutput=\"").append(cTemp).append("\"");
		sprintf(cTemp,"%s",pChannel->AscDataOutput==1?"true":"false");
		buff.append("\n\t\tAscDataOutput=\"").append(cTemp).append("\"");
		buff.append("\n\t}");
		buff.append("\n}\n\n");
	}
	return CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
}

/** @brief IP控制数据块*/
cfg_opt_t OPT_IPINFO_INFO[] =
{
	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_STR("IP_eth0", "192.168.0.100", CFGF_NOCASE),
	CFG_STR("NETMASK_eth0", "255.255.255.0", CFGF_NOCASE),
	CFG_STR("GW_eth0", "192.168.0.1", CFGF_NOCASE),
	CFG_STR("MAC_eth0", "00:01:02:B4:56:58", CFGF_NOCASE),
	CFG_INT("ProxyType", 0, CFGF_NOCASE),
	CFG_STR("ProxyIP", "192.168.9.1", CFGF_NOCASE),//徐冠+将GPRS的设置集中写
	CFG_INT("ProxyPort", 10000, CFGF_NOCASE),
	CFG_INT("ProxyLink", 0, CFGF_NOCASE),
	CFG_STR("UserName", "", CFGF_NOCASE),
	CFG_STR("PassWord", "", CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadTermIPConfig
 *  @brief 加载IP信息配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int ServerCfg::LoadTermIPConfig(Context& oContext, const char *fname)
{
	cfg_t* cfg;
	int ip0,ip1,ip2,ip3;

	cfg = cfg_init(OPT_IPINFO_INFO, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,fname,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	struct S_IpConfig &IpInfo = oContext.m_IpConfig;

	sscanf(cfg_getstr(cfg,"IP_eth0"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_TermIP[0] = ip0; IpInfo.m_TermIP[1] = ip1; IpInfo.m_TermIP[2] = ip2; IpInfo.m_TermIP[3] = ip3; 
	sscanf(cfg_getstr(cfg,"NETMASK_eth0"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_NetMask[0] = ip0; IpInfo.m_NetMask[1] = ip1; IpInfo.m_NetMask[2] = ip2; IpInfo.m_NetMask[3] = ip3; 
	sscanf(cfg_getstr(cfg,"GW_eth0"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_GateWay[0] = ip0; IpInfo.m_GateWay[1] = ip1; IpInfo.m_GateWay[2] = ip2; IpInfo.m_GateWay[3] = ip3;
	strcpy((char *)IpInfo.m_MAC, cfg_getstr(cfg, "MAC_eth0"));

	IPCSHAREMEM.m_Eth0Config.m_TermIP[0] = IpInfo.m_TermIP[0];
	IPCSHAREMEM.m_Eth0Config.m_TermIP[1] = IpInfo.m_TermIP[1];
	IPCSHAREMEM.m_Eth0Config.m_TermIP[2] = IpInfo.m_TermIP[2];
	IPCSHAREMEM.m_Eth0Config.m_TermIP[3] = IpInfo.m_TermIP[3];

	IPCSHAREMEM.m_Eth0Config.m_NetMask[0] = IpInfo.m_NetMask[0];
	IPCSHAREMEM.m_Eth0Config.m_NetMask[1] = IpInfo.m_NetMask[1];
	IPCSHAREMEM.m_Eth0Config.m_NetMask[2] = IpInfo.m_NetMask[2];
	IPCSHAREMEM.m_Eth0Config.m_NetMask[3] = IpInfo.m_NetMask[3];

	IPCSHAREMEM.m_Eth0Config.m_GateWay[0] = IpInfo.m_GateWay[0];
	IPCSHAREMEM.m_Eth0Config.m_GateWay[1] = IpInfo.m_GateWay[1];
	IPCSHAREMEM.m_Eth0Config.m_GateWay[2] = IpInfo.m_GateWay[2];
	IPCSHAREMEM.m_Eth0Config.m_GateWay[3] = IpInfo.m_GateWay[3];


	IpInfo.m_ProxyType = (INT8U)cfg_getint(cfg, "ProxyType");
	sscanf(cfg_getstr(cfg,"ProxyIP"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_ProxyIP[0] = ip0; IpInfo.m_ProxyIP[1] = ip1; IpInfo.m_ProxyIP[2] = ip2; IpInfo.m_ProxyIP[3] = ip3; 
	IpInfo.m_ProxyPort = (INT16U)cfg_getint(cfg, "ProxyPort");
	IpInfo.m_ProxyLink = (INT8U)cfg_getint(cfg, "ProxyLink");
	memset(IpInfo.m_UserName, 0x00, sizeof(IpInfo.m_UserName));
	strncpy((char *)IpInfo.m_UserName, cfg_getstr(cfg, "UserName"), 20);
	memset(IpInfo.m_Password, 0x00, sizeof(IpInfo.m_Password));
	strncpy((char *)IpInfo.m_Password, cfg_getstr(cfg, "PassWord"), 20);

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveTermIPConfig
 *  @brief 保存IP信息配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] cfgFile:文件名
 *  @return 0:successful, -1:failed
 */
int ServerCfg::SaveTermIPConfig(Context& oContext, const char *fname)
{
	char buffer[64];
	string strFileInfo;

	struct S_IpConfig &IpInfo = oContext.m_IpConfig;

	strFileInfo.append("#ipconfig.ini\r\n");
	strFileInfo.append("#function: ip config\r\n");

	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_TermIP[0], IpInfo.m_TermIP[1], IpInfo.m_TermIP[2], IpInfo.m_TermIP[3]);
	strFileInfo.append("\r\nIP_eth0 = ").append(buffer).append(" #集中器本地网口IP");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_NetMask[0], IpInfo.m_NetMask[1], IpInfo.m_NetMask[2], IpInfo.m_NetMask[3]);
	strFileInfo.append("\r\nNETMASK_eth0 = ").append(buffer).append(" #子网掩码");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_GateWay[0], IpInfo.m_GateWay[1], IpInfo.m_GateWay[2], IpInfo.m_GateWay[3]);
	strFileInfo.append("\r\nGW_eth0 = ").append(buffer).append(" #网关");
	strFileInfo.append("\r\nMAC_eth0 = ").append((char *)IpInfo.m_MAC).append(" #MAC地址");

	sprintf(buffer, "%d", IpInfo.m_ProxyType);
	strFileInfo.append("\r\nProxyType = ").append(buffer).append(" #0～3依次表示:不使用代理,http connect代理,socks4代理,socks5代理");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_ProxyIP[0], IpInfo.m_ProxyIP[1], IpInfo.m_ProxyIP[2], IpInfo.m_ProxyIP[3]);
	strFileInfo.append("\r\nProxyIP = ").append(buffer).append(" #代理服务器IP");
	sprintf(buffer, "%d", IpInfo.m_ProxyPort);
	strFileInfo.append("\r\nProxyPort = ").append(buffer);
	sprintf(buffer, "%d", IpInfo.m_ProxyLink);
	strFileInfo.append("\r\nProxyLink = ").append(buffer).append(" #0～1依次表示：无需验证、需要用户名/密码");
	strFileInfo.append("\r\nUserName = \"").append((char *)IpInfo.m_UserName).append("\"");
	strFileInfo.append("\r\nPassWord = \"").append((char *)IpInfo.m_Password).append("\"");

	strFileInfo.append("\r\n\r\n");

	return CfgBase::SaveSetFile(strFileInfo,fname,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
}




