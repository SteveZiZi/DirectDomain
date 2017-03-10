#include "MeterCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../share/Server.h"
#include "../base/FileBase.h"
#include "CfgBase.h"

int MeterCfg::LoadMeters(Context& oContext,const char* name)
{
	cfg_opt_t OPT_METER_SETTINGS[] =
	{
		CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
		CFG_STR("MeterNo","0", CFGF_NOCASE),
		CFG_INT("ZJLS", 0, CFGF_NOCASE),
		CFG_INT("BusNo", 3, CFGF_NOCASE),
		CFG_STR("ConnectType", "ZB", CFGF_NOCASE),
		CFG_STR("Property", "", CFGF_NOCASE),
		CFG_STR("MeterType", "", CFGF_NOCASE),


		CFG_STR("MeterRule", "GD645", CFGF_NOCASE),		
	
		CFG_STR("Address", "000000000000",CFGF_NOCASE),

		CFG_INT("PortSet.Baud", 1200, CFGF_NOCASE),
		CFG_INT("PortSet.DataBit", 8, CFGF_NOCASE),
		CFG_INT("PortSet.StopBit", 1, CFGF_NOCASE),
		CFG_INT("PortSet.Parity", 2, CFGF_NOCASE),
		
		CFG_STR("BureauNo", "000000000000",CFGF_NOCASE),
		CFG_BOOL("CompressFlag", cfg_false,CFGF_NOCASE),
		
		CFG_STR("UserName", "",CFGF_NOCASE),
		CFG_STR("Password", "",CFGF_NOCASE),

		//CFG_BOOL("PowerOffFlag", cfg_false,CFGF_NOCASE),
		//CFG_BOOL("VolOverLoadFlag", cfg_false,CFGF_NOCASE),

		CFG_BOOL("BrakeFlag", cfg_false,CFGF_NOCASE),
		CFG_INT("SetRegGrade", 0,CFGF_NOCASE),
		CFG_STR("SetRegPassWord", "000000",CFGF_NOCASE),

		CFG_STR("SetTimeMethod", "", CFGF_NOCASE),//对时方式
		CFG_INT("SetTimeGrade", 0,CFGF_NOCASE),
		CFG_STR("SetTimePassword", "",CFGF_NOCASE),

		CFG_INT("ZeroMaxXLGrade", 0,CFGF_NOCASE),
		CFG_STR("ZeroMaxXLPassword", "",CFGF_NOCASE),


		CFG_INT("CT", 1, CFGF_NOCASE),
		CFG_INT("PT", 1, CFGF_NOCASE),

	
		CFG_INT("CommRetryTimes",2, CFGF_NOCASE),
		CFG_STR("PrefixChar","", CFGF_NOCASE),
		CFG_INT("PrefixCount",0, CFGF_NOCASE),
		CFG_INT("Option",0, CFGF_NOCASE),

		CFG_INT("operatecode", 0, CFGF_NOCASE),

		CFG_END()
	};
	cfg_opt_t OPT_METERS_SETTINGS[] =
	{
		CFG_SEC("Meter", OPT_METER_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
		CFG_END()
	};
	
	cfg_t* cfg;
	cfg = cfg_init(OPT_METERS_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	cfg_t* cfgMeter;
	KL_INFO("Max Meter Count is %d\n",oContext.SysConfig.MaxMeterCount);
	for(unsigned int i = 0; i < cfg_size(cfg, "Meter"); i++)
	{		
		cfgMeter = cfg_getnsec(cfg, "Meter", i);	
		const char* meterNoStr=cfg_getstr(cfgMeter, "MeterNo");
		int meterNo=atoi(meterNoStr);
		if(meterNo!=1)
		{
			if (cfg_getbool(cfgMeter, "Enable")!=cfg_true) continue;
		}
		MeterBitSet mbs;
		mbs.Parse(meterNoStr);

		for(INT16U k=0;k<(INT16U)mbs.size();k++)
		{
			if (mbs.test(k)==false)
			{
				continue;
			}
			LoadMeter(oContext,cfgMeter,k);
		}
		
	}
	cfg_free(cfg);
	oContext.Meters.MBS.Stat();
	return 0;
}

int MeterCfg::LoadMeter(Context& oContext,cfg_t* cfgMeter,INT16U meterNo)
{
	if (Meter::IsValidNo(meterNo)==false) 
	{
		return -1;
	}
	Meter& meter=oContext.Meters.Item[meterNo];
	meter.Enable=(cfg_getbool(cfgMeter, "Enable")==cfg_true)?1:0;
	meter.MeterNo=meterNo;
	//meter.ZJLS=(INT8U)cfg_getint(cfgMeter, "ZJLS");
	
	meter.ConnectType=MAKE_METER_CONNECTTYPE_VAL(cfg_getstr(cfgMeter, "ConnectType"));
	meter.MeterType=MAKE_METER_METERTYPE_VAL(cfg_getstr(cfgMeter, "MeterType"));
		        
	
	meter.Prop=MAKE_METER_PROPERTY_VAL(cfg_getstr(cfgMeter, "Property"));

	if (meter.Prop==::METER_PROPERTY_DGNZ)
	{
		meter.BusNo=1;
	}
	else
	{
       int busno = cfg_getint(cfgMeter,"BusNo");

	   if (busno >=1 && busno <=4)
		   meter.BusNo = busno;
	}

	meter.RuleID=MAKE_METER_RULE_VAL(cfg_getstr(cfgMeter, "MeterRule"));


	memset(meter.Addr,0,sizeof(meter.Addr));
	//strncpy(meter.Addr,cfg_getstr(cfgMeter, "Address"),sizeof(meter.Addr)-1);
	string strs;
	strs.reserve(CONS_METER_ADDR_MAX_LEN+1);
	//strs.append(meter.Addr);
	strs.append(cfg_getstr(cfgMeter, "Address"));
	if(strs.length()>CONS_METER_ADDR_MAX_LEN)
	{
		KL_ERROR("%d号表的电表地址长度超过%d个字符\n",meter.MeterNo,CONS_METER_ADDR_MAX_LEN);
		ASSERT(false);
	}
	int zc=0;
	for(int i=0;i<CONS_METER_ADDR_MAX_LEN;i++)
	{
		if((strs[i]<='0')||(strs[i]>'9'))
			zc+=1;
		else			
			break;
	}
	strs.erase(0,zc);
	strcpy(meter.Addr,strs.c_str());

	
	meter.PortSet.Baud=cfg_getint(cfgMeter, "PortSet.Baud");
	meter.PortSet.DataBit=(INT8S)cfg_getint(cfgMeter, "PortSet.DataBit");
	meter.PortSet.StopBit=(INT8S)cfg_getint(cfgMeter, "PortSet.StopBit");
	meter.PortSet.Parity=(INT8S)cfg_getint(cfgMeter, "PortSet.Parity");

	memset(meter.BureauNo,0,sizeof(meter.BureauNo));
	strncpy(meter.BureauNo,cfg_getstr(cfgMeter, "BureauNo"),sizeof(meter.BureauNo)-1);
	meter.CompressFlag=(cfg_getbool(cfgMeter, "CompressFlag")==cfg_true)?1:0;

	if(meter.CompressFlag==false)//非压缩
	{
		for(int i=0;i<10;i++)
		{
			if(meter.BureauNo[i]==0x00)
				meter.BureauNo[i]=0x20;//补空格				
		}
		meter.BureauNo[10]=0x00;//末尾添加字符结束符号
	}
	else 
	{
		for(int i=0;i<13;i++)
		{
			if(meter.BureauNo[i]==0x00)
				meter.BureauNo[i]=0x30;//补0
		}
		meter.BureauNo[13]=0x00;//末尾添加字符结束符号
	}
	
	
	memset(meter.User,0,sizeof(meter.User));
	strncpy(meter.User,cfg_getstr(cfgMeter, "UserName"),sizeof(meter.User)-1);
	memset(meter.Pwd,0,sizeof(meter.Pwd));
	strncpy(meter.Pwd,cfg_getstr(cfgMeter, "Password"),sizeof(meter.Pwd)-1);

	//meter.PowerOffFlag=(cfg_getbool(cfgMeter, "PowerOffFlag")==cfg_true)?1:0;
	//meter.VolOverLoadFlag=(cfg_getbool(cfgMeter, "VolOverLoadFlag")==cfg_true)?1:0;
	
	meter.BrakeFlag=(cfg_getbool(cfgMeter, "BrakeFlag")==cfg_true)?1:0;
	meter.SetRegGrade=(INT8S)cfg_getint(cfgMeter, "SetRegGrade");
	memset(meter.SetRegPwd,0,sizeof(meter.SetRegPwd));
	strncpy(meter.SetRegPwd,cfg_getstr(cfgMeter, "SetRegPassWord"),sizeof(meter.SetRegPwd)-1);


	//meter.SetTimeMethod =MAKE_METER_SETTIME_METHOD_VAL(cfg_getstr(cfgMeter, "SetTimeMethod"));
	//meter.SetTimeGrade=(INT8S)cfg_getint(cfgMeter, "SetTimeGrade");
	//memset(meter.SetTimePwd,0,sizeof(meter.SetTimePwd));
	//strncpy(meter.SetTimePwd,cfg_getstr(cfgMeter, "SetTimePassword"),sizeof(meter.SetTimePwd)-1);

	//meter.ZeroMaxXLGrade=(INT8S)cfg_getint(cfgMeter, "ZeroMaxXLGrade");
	//memset(meter.ZeroMaxXLPwd,0,sizeof(meter.ZeroMaxXLPwd));
	//strncpy(meter.ZeroMaxXLPwd,cfg_getstr(cfgMeter, "ZeroMaxXLPassword"),sizeof(meter.ZeroMaxXLPwd)-1);
	
	/*meter.RelayNo=(INT16U)cfg_getint(cfgMeter, "RelayNo");*/

	meter.CT=cfg_getint(cfgMeter, "CT");
	meter.PT=cfg_getint(cfgMeter, "PT");

	meter.CommRetryTimes=(INT16S)cfg_getint(cfgMeter, "CommRetryTimes");
	meter.PrefixChar=HexToChar(cfg_getstr(cfgMeter, "PrefixChar"),CONS_DEFAULT_METER_PREFIX_CHAR);
	meter.PrefixCount=(INT8U)cfg_getint(cfgMeter, "PrefixCount");
	meter.Option=(INT32U)cfg_getint(cfgMeter, "Option");

	meter.operatecode=(INT8U)cfg_getint(cfgMeter, "operatecode");

	oContext.Meters.MBS.set(meterNo);
	return 0;
}


int MeterCfg::SaveMeters(Context& oContext,const char* name)
{
	Meters& MeterCls=oContext.Meters;
	string buff;
	buff.reserve(1208*sizeof(Meter));
	size_t meterSize=MeterCls.MBS.size();
	//20071206刘志强暂时屏蔽
	//if (oContext.SysConfig.MaxMeterCount>0 && oContext.SysConfig.MaxMeterCount<meterSize)
	//{
	//	meterSize=oContext.SysConfig.MaxMeterCount;
	//}
	string strs;
	strs.reserve(CONS_METER_ADDR_MAX_LEN);
	for(size_t i=0;i<meterSize;i++)
	{
		if (MeterCls.Item[i].Enable==0x01)
		{
			char cTemp[32];
			sprintf(cTemp,"%s",MeterCls.Item[i].Enable==1?"true":"false");
			buff.append("Meter\n{\n\tEnable=\"").append(cTemp).append("\"");
			
			sprintf(cTemp,"%d",MeterCls.Item[i].MeterNo);
			buff.append("\n\tMeterNo=\"").append(cTemp).append("\"");
			
			//sprintf(cTemp,"%d",MeterCls.Item[i].ZJLS);
			//buff.append("\n\tZJLS=").append(cTemp);
			//
			sprintf(cTemp,"%d",MeterCls.Item[i].BusNo);
			buff.append("\n\tBusNo=").append(cTemp);

			MAKE_METER_CONNECTTYPE_KEY(MeterCls.Item[i].ConnectType,cTemp);
			buff.append("\n\tConnectType=").append(cTemp);

			MAKE_METER_METERTYPE_KEY(MeterCls.Item[i].MeterType,cTemp);
			buff.append("\n\tMeterType=\"").append(cTemp).append("\"");

			MAKE_METER_PROPERTY_KEY(MeterCls.Item[i].Prop,cTemp);
			buff.append("\n\tProperty=\"").append(cTemp).append("\"");
			
			MAKE_METER_RULE_KEY(MeterCls.Item[i].RuleID,cTemp);
			buff.append("\n\tMeterRule=\"").append(cTemp).append("\"");
			
			strs.erase(strs.begin(),strs.end());
			strs.append(MeterCls.Item[i].Addr);
			while(strs.length()<CONS_METER_ADDR_MAX_LEN)
				strs.insert(0,1,'0');
			buff.append("\n\tAddress=\"").append(strs).append("\"");


			sprintf(cTemp,"%d",MeterCls.Item[i].PortSet.Baud);
			buff.append("\n\tPortSet.Baud=").append(cTemp);
			
			sprintf(cTemp,"%d",MeterCls.Item[i].PortSet.DataBit);
			buff.append("\n\tPortSet.DataBit=").append(cTemp);
			
			sprintf(cTemp,"%d",MeterCls.Item[i].PortSet.StopBit);
			buff.append("\n\tPortSet.StopBit=").append(cTemp);
			
			sprintf(cTemp,"%d",MeterCls.Item[i].PortSet.Parity);
			buff.append("\n\tPortSet.Parity=").append(cTemp);

			buff.append("\n\tBureauNo=\"").append(MeterCls.Item[i].BureauNo).append("\"");
			sprintf(cTemp,"%s",MeterCls.Item[i].CompressFlag==1?"true":"false");
			buff.append("\n\tCompressFlag=\"").append(cTemp).append("\"");

			
			buff.append("\n\tUserName=\"").append(MeterCls.Item[i].User).append("\"");			
			buff.append("\n\tPassword=\"").append(MeterCls.Item[i].Pwd).append("\"");
			
			//sprintf(cTemp,"%s",MeterCls.Item[i].PowerOffFlag==1?"true":"false");
			//buff.append("\n\tPowerOffFlag=\"").append(cTemp).append("\"");

			//sprintf(cTemp,"%s",MeterCls.Item[i].VolOverLoadFlag==1?"true":"false");
			//buff.append("\n\tVolOverLoadFlag=\"").append(cTemp).append("\"");

			sprintf(cTemp,"%s",MeterCls.Item[i].BrakeFlag==1?"true":"false");
			buff.append("\n\tBrakeFlag=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%d",MeterCls.Item[i].SetRegGrade);
			buff.append("\n\tSetRegGrade=").append(cTemp);			
			buff.append("\n\tSetRegPassWord=\"").append(MeterCls.Item[i].SetRegPwd).append("\"");
			
			
			//MAKE_METER_SETTIME_METHOD_KEY(MeterCls.Item[i].SetTimeMethod,cTemp);
			//buff.append("\n\tSetTimeMethod=\"").append(cTemp).append("\"");
			//sprintf(cTemp,"%d",MeterCls.Item[i].SetTimeGrade);
			//buff.append("\n\tSetTimeGrade=").append(cTemp);			
			//buff.append("\n\tSetTimePassword=\"").append(MeterCls.Item[i].SetTimePwd).append("\"");
			//
			//sprintf(cTemp,"%d",MeterCls.Item[i].ZeroMaxXLGrade);
			//buff.append("\n\tZeroMaxXLGrade=").append(cTemp);			
			//buff.append("\n\tZeroMaxXLPassword=\"").append(MeterCls.Item[i].ZeroMaxXLPwd).append("\"");
			
			//sprintf(cTemp,"%d",MeterCls.Item[i].RelayNo);
			//buff.append("\n\tRelayNo=").append(cTemp);
			
			sprintf(cTemp,"%d",MeterCls.Item[i].CT);
			buff.append("\n\tCT=").append(cTemp);
			
			sprintf(cTemp,"%d",MeterCls.Item[i].PT);
			buff.append("\n\tPT=").append(cTemp);
		

			sprintf(cTemp,"%d",MeterCls.Item[i].CommRetryTimes);			
			buff.append("\n\tCommRetryTimes=").append(cTemp);

			sprintf(cTemp,"%02X",MeterCls.Item[i].PrefixChar);
			buff.append("\n\tPrefixChar=").append(cTemp);

			sprintf(cTemp,"%d",MeterCls.Item[i].PrefixCount);
			buff.append("\n\tPrefixCount=").append(cTemp);
			
			sprintf(cTemp,"%d",MeterCls.Item[i].Option);
			buff.append("\n\tOption=").append(cTemp);		

			sprintf(cTemp,"%d",MeterCls.Item[i].operatecode);
			buff.append("\n\toperatecode=").append(cTemp);	
			
			buff.append("\n}\n");
			MeterCls.Item[i].OnChanged();
		
		
		}
	}

	int iFlag=CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	MeterCls.OnChanged();
	oContext.RefreshGatherInfo();
	oContext.SendSigNotifyToDaemon(Server::ParamChangedTag);
	return iFlag;
}


