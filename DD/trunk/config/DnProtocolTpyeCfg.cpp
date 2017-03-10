#include "DnProtocolTpyeCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../share/RegData.h"
#include "../include/StdInclude.h"
#include "../share/Server.h"
#include "../base/FileBase.h"
#include "CfgBase.h"


cfg_opt_t OPT_DNPROTOCOLTYPE_SETTINGS[] =
{ 
	    CFG_STR("DnProtocolType", "CAN_A", CFGF_NOCASE),	//路由类型
	    CFG_END()
};


int DnProtocolTpye::LoadDnProtocolTpye(Context& oContext,const char* name)
{
	cfg_t *cfg;

	cfg = cfg_init(OPT_DNPROTOCOLTYPE_SETTINGS,CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	oContext.m_DnProtocolType  = GetDnProtocolTpyeType(cfg_getstr(cfg,"DnProtocolType" ));
	cfg_free(cfg);
	return 0;
}

int DnProtocolTpye::SaveDnProtocolTpye(Context& oContext,const char* name)
{
	char cTemp[200];
	string buff;
	string sStr;
	sStr.reserve(500);
	buff.reserve(3000);

	ASSERT(GetDnProtocolTpyeString(oContext.m_DnProtocolType)!=NULL);
	sprintf(cTemp,"%s",GetDnProtocolTpyeString(oContext.m_DnProtocolType));
	buff.append("\DnProtocolType=").append(cTemp);

	return CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
}

int DnProtocolTpye::GetDnProtocolTpyeType(const char *key)
{
	static T_DN_PROTOCOL_TYPE protocolType[DN_PROTOCOL_TYPE_MAX] ={
									{DN_PROTOCOL_TYPE_CAN_HDM,"CAN_HDM"},
									{DN_PROTOCOL_TYPE_RS485_A,"RS485_A"},
									{DN_PROTOCOL_TYPE_RS485_B,"RS485_B"}
									
									};
	for(size_t i=0;i<DN_PROTOCOL_TYPE_MAX;i++)
	{
		if (strcasecmp(key,protocolType[i].key)==0)
		{
			return (INT8U)protocolType[i].index;
		}
	}
	
	return DN_PROTOCOL_TYPE_CAN_HDM;
}



char *DnProtocolTpye::GetDnProtocolTpyeString(const int  type)
{
	static T_DN_PROTOCOL_TYPE protocolType[DN_PROTOCOL_TYPE_MAX] ={
									{DN_PROTOCOL_TYPE_CAN_HDM,"CAN_HDM"},
									{DN_PROTOCOL_TYPE_RS485_A,"RS485_A"},
									{DN_PROTOCOL_TYPE_RS485_B,"RS485_B"}
									};
	for(size_t i=0;i<DN_PROTOCOL_TYPE_MAX;i++)
	{
		if (type == protocolType[i].index)
			return protocolType[i].key;
	}

	return NULL;
}
