#include "ClRelayCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../share/RegData.h"
#include "../include/StdInclude.h"
#include "../share/Server.h"
#include "../base/FileBase.h"
#include "CfgBase.h"
#include "../share/ipcSharemem.h"

//TODO:增加对TimeUnit的文字配置支持
cfg_opt_t OPT_LEAKDT_INFO_SETTINGS[]=
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
	CFG_INT("ChannelNo",0, CFGF_NOCASE),
	CFG_INT("DelayMs",0, CFGF_NOCASE),	
	CFG_INT("DtVol",0, CFGF_NOCASE),	
	CFG_BOOL("CutEnable", cfg_false,CFGF_NOCASE),	
	CFG_END()
};

cfg_opt_t OPT_CLRELAY_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
		CFG_INT("CLRelayNo",0, CFGF_NOCASE),
		CFG_STR("Addr", "000000000000",CFGF_NOCASE),
		CFG_STR("ZBXW","0", CFGF_NOCASE),
		CFG_STR_LIST("Devices", "{1-16}", CFGF_NONE),
		CFG_SEC("LeakDtInfo", OPT_LEAKDT_INFO_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
		CFG_END()
};

cfg_opt_t OPT_CLRELAYS_SETTINGS[] =
{
	CFG_SEC("CLRelay", OPT_CLRELAY_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
		CFG_END()
};

int CLRelayCfg::LoadClRelays(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_CLRELAYS_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}   
	cfg_t* cfgRelay;

	oContext.ClRelays.CLRelayBS.reset();
	oContext.ClRelays.CLRelayBS.Stat();

	for(unsigned int i = 0; i < cfg_size(cfg, "CLRelay"); i++)
	{
		cfgRelay = cfg_getnsec(cfg, "CLRelay", i);
		INT16U ClRelayNo=(INT16U)cfg_getint(cfgRelay, "CLRelayNo");
		if (S_CLRelay::IsValidNo(ClRelayNo)==false)
		{
			continue;
		}
		S_CLRelay& clrelay =oContext.ClRelays.m_Item[ClRelayNo];
		clrelay.m_Enable=(cfg_getbool(cfgRelay, "Enable")==cfg_true)?1:0;
		if (clrelay.m_Enable!=1) continue;
		clrelay.m_CLRelayNo = ClRelayNo;	
		//suxufeng:[2010-10-11]晓程3106芯片无须获取相位，但是通信过程需要相位信息，因此每个终端均默认为A相
		//relay.ZBXW=MAKE_ZB_XWTYPE_VAL(cfg_getstr(cfgRelay, "ZBXW"));
		clrelay.m_ZBXW=METER_XW_A;
		
		for(unsigned t = 0; t < cfg_size(cfgRelay, "Devices"); t++)
		{
			clrelay.m_Dbs.Parse(cfg_getnstr(cfgRelay, "Devices", t));
		}

		clrelay.m_Dbs.Stat();

		for (DevicesBitSet::SizeType i = clrelay.m_Dbs.FromPos; i<clrelay.m_Dbs.ToPos; i++)
		{
			if (clrelay.m_Dbs.test(i))
			{
				oContext.Devices.m_Item[i].m_ClRelayNo = clrelay.m_CLRelayNo;
			}
		}

		memset(clrelay.m_Addr,0,sizeof(clrelay.m_Addr));		
		
		string strs;
		strs.reserve(CONS_METER_ADDR_MAX_LEN+1);
		strs.append(cfg_getstr(cfgRelay, "Addr"));
		if(strs.length()>CONS_METER_ADDR_MAX_LEN)
		{
			KL_ERROR("%d号回路终端的地址长度超过%d个字符\n",clrelay.m_CLRelayNo,CONS_METER_ADDR_MAX_LEN);
			ASSERT(false);
		}
		strs.append(clrelay.m_Addr);
		int zc=0;
		for(int i=0;i<CONS_METER_ADDR_MAX_LEN;i++)
		{
			if((strs[i]<='0')||(strs[i]>'9'))
				zc+=1;
			else
				break;
		}
		strs.erase(0,zc);
		strcpy(clrelay.m_Addr,strs.c_str());

		cfg_t* cfgLeakInfo;
		for(unsigned t = 0; t < cfg_size(cfgRelay, "LeakDtInfo"); t++)
		{
			cfgLeakInfo = cfg_getnsec(cfgRelay, "LeakDtInfo", t);	
			clrelay.m_ClLeakInfo.Init();
			clrelay.m_ClLeakInfo.m_Enable = (cfg_getbool(cfgLeakInfo, "Enable")==cfg_true)?1:0;
			clrelay.m_ClLeakInfo.m_ChannelNo = (INT8U)cfg_getint(cfgLeakInfo, "ChannelNo");
			clrelay.m_ClLeakInfo.m_DelayMs = (INT16U)cfg_getint(cfgLeakInfo, "DelayMs");
			clrelay.m_ClLeakInfo.m_DtVol = D2B((INT16U)cfg_getint(cfgLeakInfo, "DtVol"));
			clrelay.m_ClLeakInfo.m_CutEnable = (cfg_getbool(cfgLeakInfo, "CutEnable")==cfg_true)?1:0;
			oContext.ClRelays.CLRelayLeakDtBS.set(ClRelayNo);
		}
		
		oContext.ClRelays.CLRelayBS.set(ClRelayNo);
		
	}
	oContext.ClRelays.CLRelayBS.Stat();
	oContext.ClRelays.CLRelayLeakDtBS.Stat();
	IPCSHAREMEM.m_ClNum = oContext.ClRelays.CLRelayBS.count();
	IPCSHAREMEM.Update();
	cfg_free(cfg);
	return 0;
}

int CLRelayCfg::SaveClRelays(Context& oContext,const char* name)
{
	S_CLRelays& clRelays=oContext.ClRelays;
	string buff;
	buff.reserve(255*sizeof(S_CLRelay));
	string sStr;
	sStr.reserve(500);
	string strs;
	strs.reserve(CONS_METER_ADDR_MAX_LEN);
	for(size_t i=0;i<clRelays.CLRelayBS.size();i++)
	{
		if(clRelays.m_Item[i].m_Enable==0x00)
			continue;
		if (clRelays.CLRelayBS.test(i))
		{
			S_CLRelay &clrelay=clRelays.m_Item[i];			
			char cTemp[200];	
			
			
			sprintf(cTemp,"%s",clrelay.m_Enable==1?"true":"false");
			buff.append("CLRelay\n{\n\tEnable=\"").append(cTemp).append("\"");


			sprintf(cTemp,"%d",clrelay.m_CLRelayNo);
			buff.append("\n\tCLRelayNo=").append(cTemp);

			strs.erase(strs.begin(),strs.end());
			strs.append(clrelay.m_Addr);
			while(strs.length()<CONS_METER_ADDR_MAX_LEN)
				strs.insert(0,1,'0');
			buff.append("\n\tAddr=\"").append(strs).append("\"");

			
			MAKE_ZB_XWTYPE_KEY(clrelay.m_ZBXW,cTemp);
		    buff.append("\n\tZBXW=\"").append(cTemp).append("\"");
			//KL_INFO("终端 %d 保存的相位：：%d",relay.RelayNo,relay.ZBXW);
			


			sStr.erase();
			clrelay.m_Dbs.ToString(sStr);
			buff.append("\n\tDevices={").append(sStr).append("}");

			sprintf(cTemp,"%s",clrelay.m_ClLeakInfo.m_Enable==1?"true":"false");

			if (clrelay.m_ClLeakInfo.m_Enable==1)
			{
				oContext.ClRelays.CLRelayLeakDtBS.set(clrelay.m_CLRelayNo);
			}
			else
			{
				oContext.ClRelays.CLRelayLeakDtBS.reset(clrelay.m_CLRelayNo);
			}
			
			buff.append("\n\tLeakDtInfo\n\t{\n\t\tEnable=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%d",clrelay.m_ClLeakInfo.m_ChannelNo);
			buff.append("\n\t\tChannelNo=").append(cTemp);
			sprintf(cTemp,"%d",clrelay.m_ClLeakInfo.m_DelayMs);
			buff.append("\n\t\tDelayMs=").append(cTemp);
			sprintf(cTemp,"%x",clrelay.m_ClLeakInfo.m_DtVol);
			buff.append("\n\t\tDtVol=").append(cTemp);
			sprintf(cTemp,"%s",clrelay.m_ClLeakInfo.m_CutEnable==1?"true":"false");
			buff.append("\n\t\tCutEnable=").append(cTemp);
			buff.append("\n\t}");

			buff.append("\n}\n\n");
		}
	}

	oContext.ClRelays.CLRelayLeakDtBS.Stat();
	IPCSHAREMEM.m_ClNum = oContext.ClRelays.CLRelayBS.count();
	IPCSHAREMEM.Update();
	int iFlag=CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	clRelays.OnChanged();
	//oContext.RefreshGatherInfo();
	oContext.SendSigNotifyToDaemon(Server::ParamChangedTag);
	return iFlag;
}

int CLRelayCfg::ReplaceClRelay(const char *oldaddr,const char *newaddr)
{
	S_CLRelays& clRelays = CONTEXT.ClRelays;
	S_CLRelay clrelay = CONTEXT.ClRelays.GetCLRelayInfo(oldaddr);
	if (clrelay.IsValid())
	{
		memcpy(clRelays.m_Item[clrelay.m_CLRelayNo].m_Addr,newaddr,CONS_METER_ADDR_MAX_LEN+1);
		return SaveClRelays(CONTEXT);

	}
	return -1;
}

