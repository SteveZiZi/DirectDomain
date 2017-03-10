#include "RelayCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../share/RegData.h"
#include "../include/StdInclude.h"
#include "../share/Server.h"
#include "../base/FileBase.h"
#include "CfgBase.h"


//TODO:增加对TimeUnit的文字配置支持

cfg_opt_t OPT_RELAY_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
		CFG_INT("RelayNo",0, CFGF_NOCASE),
		CFG_STR("Addr", "000000000000",CFGF_NOCASE),
		CFG_STR("ZBXW","0", CFGF_NOCASE),
		CFG_INT("Priority",0, CFGF_NOCASE),
		CFG_STR_LIST("Meters", "{1-1206}", CFGF_NONE),
		CFG_END()
};

cfg_opt_t OPT_RELAYS_SETTINGS[] =
{
	CFG_SEC("Relay", OPT_RELAY_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
		CFG_END()
};

int RelayCfg::LoadRelays(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_RELAYS_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}   
	cfg_t* cfgRelay;
	oContext.Relays.RBS.reset();
	oContext.Relays.RBS.Stat();
	for(unsigned int i = 0; i < cfg_size(cfg, "Relay"); i++)
	{
		cfgRelay = cfg_getnsec(cfg, "Relay", i);
		INT16U RelayNo=(INT16U)cfg_getint(cfgRelay, "RelayNo");
		if (Relay::IsValidNo(RelayNo)==false)
		{
			continue;
		}
		Relay& relay =oContext.Relays.Item[RelayNo];
		relay.Enable=(cfg_getbool(cfgRelay, "Enable")==cfg_true)?1:0;
		if (relay.Enable!=1) continue;
		relay.RelayNo=RelayNo;	
		//suxufeng:[2010-10-11]晓程3106芯片无须获取相位，但是通信过程需要相位信息，因此每个终端均默认为A相
		//relay.ZBXW=MAKE_ZB_XWTYPE_VAL(cfg_getstr(cfgRelay, "ZBXW"));
		relay.ZBXW=METER_XW_A;
		relay.Priority=(INT8U)cfg_getint(cfgRelay, "Priority");

		for(unsigned t = 0; t < cfg_size(cfgRelay, "Meters"); t++)
		{
			relay.MBS.Parse(cfg_getnstr(cfgRelay, "Meters", t));
		}
	

		memset(relay.Addr,0,sizeof(relay.Addr));		
		//strncpy(relay.Addr,cfg_getstr(cfgRelay, "Addr"),sizeof(relay.Addr)-1);		
		string strs;
		strs.reserve(CONS_METER_ADDR_MAX_LEN+1);
		strs.append(cfg_getstr(cfgRelay, "Addr"));
		if(strs.length()>CONS_METER_ADDR_MAX_LEN)
		{
			KL_ERROR("%d号终端的地址长度超过%d个字符\n",relay.RelayNo,CONS_METER_ADDR_MAX_LEN);
			ASSERT(false);
		}
		strs.append(relay.Addr);
		int zc=0;
		for(int i=0;i<CONS_METER_ADDR_MAX_LEN;i++)
		{
			if((strs[i]<='0')||(strs[i]>'9'))
				zc+=1;
			else
				break;
		}
		strs.erase(0,zc);
		strcpy(relay.Addr,strs.c_str());
		oContext.Relays.RBS.set(RelayNo);
	}
	oContext.Relays.RBS.Stat();
	cfg_free(cfg);
	return 0;
}

int RelayCfg::SaveRelays(Context& oContext,const char* name)
{
	Relays& RelayCls=oContext.Relays;
	string buff;
	buff.reserve(255*sizeof(Relay));
	string sStr;
	sStr.reserve(500);
	string strs;
	strs.reserve(CONS_METER_ADDR_MAX_LEN);
	for(size_t i=0;i<RelayCls.RBS.size();i++)
	{
		if(RelayCls.Item[i].Enable==0x00)
			continue;
		if (RelayCls.RBS.test(i))
		{
			Relay &relay=RelayCls.Item[i];			
			char cTemp[200];	
			
			
			sprintf(cTemp,"%s",relay.Enable==1?"true":"false");
			buff.append("Relay\n{\n\tEnable=\"").append(cTemp).append("\"");


			sprintf(cTemp,"%d",relay.RelayNo);
			buff.append("\n\tRelayNo=").append(cTemp);

			sprintf(cTemp,"%d",relay.Priority);
			buff.append("\n\tPriority=").append(cTemp);

			strs.erase(strs.begin(),strs.end());
			strs.append(relay.Addr);
			while(strs.length()<CONS_METER_ADDR_MAX_LEN)
				strs.insert(0,1,'0');
			buff.append("\n\tAddr=\"").append(strs.c_str()).append("\"");

			
			MAKE_ZB_XWTYPE_KEY(relay.ZBXW,cTemp);
		    buff.append("\n\tZBXW=\"").append(cTemp).append("\"");
			//KL_INFO("终端 %d 保存的相位：：%d",relay.RelayNo,relay.ZBXW);
			


			sStr.erase();
			relay.MBS.ToString(sStr);
			buff.append("\n\tMeters={").append(sStr).append("}");

			buff.append("\n}\n\n");
		}
	}
	int iFlag=CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	RelayCls.OnChanged();
	//oContext.RefreshGatherInfo();
	oContext.SendSigNotifyToDaemon(Server::ParamChangedTag);
	return iFlag;
	return -1;
}

int RelayCfg::ReplaceRelay(const char *oldaddr,const char *newaddr)
{
	Relays& RelayCls=CONTEXT.Relays;
	Relay relay=RelayCls.GetRelayInfo(oldaddr);
	if (relay.IsValid())
	{
		memcpy(RelayCls.Item[relay.RelayNo].Addr,newaddr,CONS_METER_ADDR_MAX_LEN+1);
		return SaveRelays(CONTEXT);

	}
	return -1;
}

