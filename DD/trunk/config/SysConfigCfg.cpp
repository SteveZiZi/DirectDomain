#include "SysConfigCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../base/KLTrace.h"
#include "../base/FileBase.h"
#include "../base/Modem.h"
#include "CfgBase.h"
#include "../share/SysBase.h"
#include "../share/ipcSharemem.h"

cfg_opt_t OPT_SYS_CONFIG_SETTINGS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_INT("DeviceNo", 1, CFGF_NOCASE),
	CFG_INT("HardwareVerion",101, CFGF_NOCASE),
	CFG_STR("ModuleSoftVersion","2.0.1.2",CFGF_NOCASE),
//	CFG_STR("RTUA","00,00,00,00", CFGF_NOCASE),
	CFG_STR("m_ChipAddr","00,00,00,00,00,00", CFGF_NOCASE),
	CFG_INT("m_ChipChannel",0x00, CFGF_NOCASE),
	CFG_INT("m_RouterMode", 0x03, CFGF_NOCASE),
	CFG_INT("m_MSTA",30, CFGF_NOCASE), //2006-6-8刘志强+
	CFG_STR("m_PhoneNum","013312345678", CFGF_NOCASE),//
	CFG_STR("m_Default_GateWay","192.168.8.1:5001", CFGF_NOCASE),//
	CFG_STR("m_psw_Read","111111", CFGF_NOCASE),
	CFG_STR("m_psw_lowSet","111111", CFGF_NOCASE),
	CFG_STR("m_Admin_psw","111111", CFGF_NOCASE),//
	CFG_INT("m_psw_LCD",0, CFGF_NOCASE),
	CFG_STR("m_softVerion","5100-101-123-510245", CFGF_NOCASE),
	CFG_INT("m_Customs",1000, CFGF_NOCASE),
	CFG_INT("m_MutilFunMeters",200, CFGF_NOCASE),
	CFG_INT("m_VIP",6, CFGF_NOCASE),
	CFG_INT("m_TimeLock",0, CFGF_NOCASE),
	CFG_INT("m_Space",0, CFGF_NOCASE),
	CFG_INT("m_Relay",0, CFGF_NOCASE),
	CFG_STR("m_Alarm_gate","0010", CFGF_NOCASE),
	CFG_STR("m_ReadTime","04", CFGF_NOCASE),
	CFG_STR("m_Reportable","04", CFGF_NOCASE),
	CFG_STR("m_Alarm_Shield","ffffffff", CFGF_NOCASE),//suxufeng:[2010-4-26]根据新规范进行修改
	CFG_STR("m_Lose_ToDay","00FFFF", CFGF_NOCASE),
	CFG_STR("m_SideDJS", "FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF",CFGF_NOCASE),
	CFG_STR("m_SlaveDistribut", "FFFFFFFF",CFGF_NOCASE),
	CFG_INT("m_Month_trail_read",0x25, CFGF_NOCASE),
	CFG_STR("m_ReportTime","0104", CFGF_NOCASE),
	CFG_STR("m_BroadcastSetTime","011230",CFGF_NOCASE),
	CFG_INT("m_BroadcastInterval",30, CFGF_NOCASE),

	CFG_INT("MaxMeterCount",CONS_METER_MAX_COUNT, CFGF_NOCASE),
	CFG_STR("UserCode", "gddyjc",CFGF_NOCASE),
	CFG_STR("PlanKey", "gd",CFGF_NOCASE),
	CFG_STR("WirelessModemType", "GM47",CFGF_NOCASE),
	CFG_STR("AlarmCheckStartTime","4", CFGF_NOCASE),

	CFG_INT("BeatReply",1,CFGF_NOCASE),
	CFG_INT("BEATIMES",3,CFGF_NOCASE),

	CFG_STR("RelaysVersion","3.0",CFGF_NOCASE),

	CFG_INT("GPRSDialMaxTimes",255,CFGF_NOCASE),
	CFG_INT("GPRSReConnectMaxTimes",255,CFGF_NOCASE),
	CFG_STR("GPRSAutoConnectStartDate","1",CFGF_NOCASE),
	CFG_STR("GPRSAutoConnectEndDate","31",CFGF_NOCASE),
	CFG_STR("GPRSAutoConnectStartHour","0",CFGF_NOCASE),
	CFG_STR("GPRSAutoConnectEndHour","23",CFGF_NOCASE),
	CFG_INT("GPRSAutoDialInterMinute",15,CFGF_NOCASE),
	CFG_INT("GPRSAutoConnectInterMinute",1,CFGF_NOCASE),
	//CFG_INT("GPRSMonthGeneralTraffics",0,CFGF_NOCASE),
	CFG_INT("m_Check_Daydata_Type", 1, CFGF_NOCASE),
	CFG_INT("m_XIAOCHENG_PROTOCOL_Type", XC_PROTOCOL_N6_N12, CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_GPRS_INFO[] =
{
	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_BOOL("EnableFlowCtrl",cfg_true,CFGF_NOCASE),
	CFG_INT("LogOnInterS",300,CFGF_NOCASE),
	CFG_INT("LogOnRetryTimes",3,CFGF_NOCASE),
	CFG_INT("SocketConnectInterS",300,CFGF_NOCASE),
	CFG_INT("DialUpInterS",300,CFGF_NOCASE),
	CFG_BOOL("EnableMaxFlowLimit",cfg_true,CFGF_NOCASE),
	CFG_INT("GenericTrafficStatOfMonth",0,CFGF_NOCASE),

	CFG_END()
};


int SysConfigCfg::LoadConfig(Context& oContext,const char* cfgFile)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_SYS_CONFIG_SETTINGS, CFGF_NONE);
	if(cfg_parse(cfg,cfgFile) == CFG_PARSE_ERROR)
	{
		ASSERT(false);
		KL_ERROR("Parse cfg file[%s] failed\n",cfgFile);
		cfg_free(cfg);
		return -1;
	}
	SysConfig& config=oContext.SysConfig;
	config.DeviceNo=(INT32U)cfg_getint(cfg, "DeviceNo");
	config.HardwareVerion=(INT16U)cfg_getint(cfg, "HardwareVerion");
	config.Setm_ModuleSoftversion(cfg_getstr(cfg,"ModuleSoftVersion"));
	
	//2006-6-8刘志强+
//	config.SetRTUA(cfg_getstr(cfg,"RTUA"));
//	IPCSHAREMEM.Update();

//	KL_VERBOSE("%d", CONTEXT.SysConfig.m_RTUA);

	//2009-03-11 陈桂华添加
	config.SetChipAddr(cfg_getstr(cfg,"m_ChipAddr"));
	config.SetConnChannel((INT8U)cfg_getint(cfg,"m_ChipChannel"));
	//config.Setm_ModuleSoftversion(cfg_getstr(cfg,"ModuleSoftVersion"));
	config.SetRouterWorkMode((INT8U)cfg_getint(cfg,"m_RouterMode"));

	config.m_MSTA=(INT16U)cfg_getint(cfg,"m_MSTA");

	config.Setm_PhoneNum(cfg_getstr(cfg,"m_PhoneNum"));
	config.Setm_Default_GateWay(cfg_getstr(cfg,"m_Default_GateWay"));

	config.Setm_psw_Read(cfg_getstr(cfg,"m_psw_Read"));
	config.Setm_psw_lowSet(cfg_getstr(cfg,"m_psw_lowSet"));
	config.Setm_Admin_psw(cfg_getstr(cfg,"m_Admin_psw"));
	if (0 != strncmp(config.Setm_softVerion(cfg_getstr(cfg,"m_softVerion")), CONS_SOFT_VERSION, strlen(CONS_SOFT_VERSION)))
	{
		//suxufeng:[2010-4-20]两次启动的版本不同，说明集中器已经被升级了。
		oContext.m_DeviceStatus.OnSystemUpdate = true;
	}

	config.LCDPassword=cfg_getint(cfg,"m_psw_LCD"); 

	config.m_Customs=(INT16U)cfg_getint(cfg,"m_Customs"); 
	config.m_MutilFunMeters=(INT16U)cfg_getint(cfg,"m_MutilFunMeters");
	config.m_VIP=(INT8U)cfg_getint(cfg,"m_VIP");
	config.m_TimeLock=(INT8U)cfg_getint(cfg,"m_TimeLock");
	config.m_Space=(INT8U)cfg_getint(cfg,"m_Space");
	config.m_Relay=(INT8U)cfg_getint(cfg,"m_Relay");
	config.Setm_Alarm_gate(cfg_getstr(cfg,"m_Alarm_gate"));

	config.Setm_ReadTime(cfg_getstr(cfg,"m_ReadTime"));

	config.Setm_Reportable(cfg_getstr(cfg,"m_Reportable"));

	config.Setm_Alarm_Shield(cfg_getstr(cfg,"m_Alarm_Shield"));

	config.Setm_Lose_ToDay(cfg_getstr(cfg,"m_Lose_ToDay"));

	config.m_Month_trail_read=(INT16U)cfg_getint(cfg,"m_Month_trail_read");

	config.Setm_ReportTime(cfg_getstr(cfg,"m_ReportTime"));

	config.Setm_BroadcastSetTime(cfg_getstr(cfg,"m_BroadcastSetTime"));
	config.m_BroadcastInterval=(INT8U)cfg_getint(cfg,"m_BroadcastInterval");

	//config.CRC_Times=(INT32U)cfg_getint(cfg,"CRC_Times");

	//2006-6-8刘志强+
	config.MaxMeterCount=(INT16U)cfg_getint(cfg,"MaxMeterCount");
	config.SetUserCode(cfg_getstr(cfg, "UserCode"));
	config.SetPlanKey(cfg_getstr(cfg, "PlanKey"));
	config.WirelessModemType=MAKE_MODEM_TYPE_VAL(cfg_getstr(cfg, "WirelessModemType"));

	config.SetalarmCheckStartTime(cfg_getstr(cfg,"AlarmCheckStartTime"));
	
	config.BeatReply=(INT8U)cfg_getint(cfg,"BeatReply");

	config.BEATIMES=(INT8U)cfg_getint(cfg,"BEATIMES");

	sprintf(config.RelaysVersion, "%s", cfg_getstr(cfg,"RelaysVersion"));

	if (strlen(config.RelaysVersion) < 3)
	{
		sprintf(config.RelaysVersion, "%s", "3.0");
	}

	//config.GPRSDialMaxTimes = (INT8U)cfg_getint(cfg,"GPRSDialMaxTimes");

	//config.GPRSReConnectMaxTimes = (INT8U)cfg_getint(cfg,"GPRSReConnectMaxTimes");

	//char *endHour = cfg_getstr(cfg,"GPRSAutoConnectEndHour");
	//INT32U temp = 0;
	//sscanf(endHour,"%x",&temp);
	//config.GPRSAutoConnectEndHour = (INT8U)temp;
	////sscanf(endHour,"%x",config.GPRSAutoConnectEndHour);

	//char *startDate = cfg_getstr(cfg,"GPRSAutoConnectStartDate");
	//temp = 0;
	//sscanf(startDate,"%x",&temp);
	//config.GPRSAutoConnectStartDate = (INT8U)temp;
	////sscanf(startDate,"%x",&config.GPRSAutoConnectStartDate);

	//char *startHour = cfg_getstr(cfg,"GPRSAutoConnectStartHour");
	//temp = 0;
	//sscanf(startHour,"%x",&temp);
	//config.GPRSAutoConnectStartHour = (INT8U)temp;
	////sscanf(startHour,"%x",&config.GPRSAutoConnectStartHour);

	//char *endDate = cfg_getstr(cfg,"GPRSAutoConnectEndDate");
	//temp = 0;
	//sscanf(endDate,"%x",&temp);
	//config.GPRSAutoConnectEndDate = (INT8U)temp;
	////sscanf(endDate,"%x",&config.GPRSAutoConnectEndDate);

	//config.GPRSAutoDialInterMinute = (INT8U)cfg_getint(cfg,"GPRSAutoDialInterMinute");

	//config.GPRSAutoConnectInterMinute = (INT8U)cfg_getint(cfg,"GPRSAutoConnectInterMinute");

	////config.GPRSMonthGeneralTraffics = (INT32U)cfg_getint(cfg,"GPRSMonthGeneralTraffics");
	config.m_Check_Daydata_Type = (INT8U)cfg_getint(cfg,"m_Check_Daydata_Type");
	config.m_XC_Protocol_Type = (INT8U)cfg_getint(cfg,"m_XIAOCHENG_PROTOCOL_Type");
	cfg_free(cfg);
	return 0;
}

int SysConfigCfg::SaveConfig(Context& oContext,const char* cfgFile)
{
	FILE *fp =FileBase::OpenFile(cfgFile, "w");
	if (fp==NULL)
	{
		KL_ERROR("Open file %s failed\n",cfgFile);
		return -1;
	}
	cfg_t* cfg;
	cfg = cfg_init(OPT_SYS_CONFIG_SETTINGS, CFGF_NONE);
	SysConfig& config=oContext.SysConfig;
	cfg_setint(cfg,"DeviceNo",(long)config.DeviceNo);
	cfg_setint(cfg,"HardwareVerion",config.HardwareVerion);
	cfg_setstr(cfg,"ModuleSoftVersion",config.ModuleSoftversion);
	//2006-6-8 以下由刘志强+
	char tep[40];
		
	//sprintf(tep,"%x%x,%x%x,%x%x,%x%x",Char_No1Hex((config.RTUA[0])>>4),Char_No1Hex(config.RTUA[0]),Char_No1Hex((config.RTUA[1])>>4),Char_No1Hex(config.RTUA[1]),Char_No1Hex((config.RTUA[2])>>4),Char_No1Hex(config.RTUA[2]),Char_No1Hex((config.RTUA[3])>>4),Char_No1Hex(config.RTUA[3]));
//	sprintf(tep,"%02x,%02x,%02x,%02x",(INT8U)config.RTUA[0],(INT8U)config.RTUA[1],(INT8U)config.RTUA[2],(INT8U)config.RTUA[3]);
//	cfg_setstr(cfg,"RTUA",tep);//2006-6-8刘志强+
	
	//09-03-11 陈桂华设置无线rouble地址
	sprintf(tep,"%02x,%02x,%02x,%02x,%02x,%02x",(INT8U)config.m_ChipAddr[0],(INT8U)config.m_ChipAddr[1],(INT8U)config.m_ChipAddr[2],(INT8U)config.m_ChipAddr[3],
									             (INT8U)config.m_ChipAddr[4],(INT8U)config.m_ChipAddr[5]);
	cfg_setstr(cfg,"m_ChipAddr",tep);

	cfg_setint(cfg,"m_ChipChannel",config.ChipChannel);
    //cfg_setstr(cfg,"ModuleSoftVersion",config.ModuleSoftversion);

	cfg_setint(cfg,"m_RouterMode",config.m_RouterMode);
	cfg_setint(cfg,"m_MSTA",config.m_MSTA);	


	cfg_setstr(cfg,"m_PhoneNum",config.m_PhoneNum);  	
	cfg_setstr(cfg,"m_Default_GateWay",config.m_Default_GateWay);

	cfg_setstr(cfg,"m_psw_Read",config.m_psw_Read);	

	cfg_setstr(cfg,"m_psw_lowSet",config.m_psw_lowSet);	
	cfg_setstr(cfg,"m_Admin_psw",config.m_Admin_psw);

	cfg_setint(cfg,"m_psw_LCD",config.LCDPassword);
	
	cfg_setstr(cfg,"m_softVerion",config.m_softVerion);
	cfg_setint(cfg,"m_Customs",config.m_Customs);
	cfg_setint(cfg,"m_MutilFunMeters",config.m_MutilFunMeters);
	cfg_setint(cfg,"m_VIP",config.m_VIP);
	cfg_setint(cfg,"m_TimeLock",config.m_TimeLock);
	cfg_setint(cfg,"m_Space",config.m_Space);
	cfg_setint(cfg,"m_Relay",config.m_Relay);

	char malarmgate[6];
	//sprintf(malarmgate,"%x%x.%x%x",0x0F&((config.m_Alarm_gate[0])>>4),0x0F&(config.m_Alarm_gate[0]),0x0F&(config.m_Alarm_gate[1]>>4),0x0F&(config.m_Alarm_gate[1]));
	//变通的解决办法2006-11-23

	sprintf(malarmgate,"%02x.%02x",(INT8U)config.m_Alarm_gate[0],(INT8U)config.m_Alarm_gate[1]);
	if((malarmgate[0]==0x30)&&(malarmgate[1]==0x30)&&(malarmgate[3]==0x30)&&(malarmgate[4]==0x30))
		malarmgate[3]=0x31;
	cfg_setstr(cfg,"m_Alarm_gate",malarmgate);
	
	char m_rt[3];
	//sprintf(m_rt,"%x%x",(config.m_ReadTime>>4)&0x0F,(config.m_ReadTime)&0x0F);
	sprintf(m_rt,"%02x",(INT8U)config.m_ReadTime);
	cfg_setstr(cfg,"m_ReadTime",m_rt);
	
	memset(m_rt,0,3);
	//sprintf(m_rt,"%x%x",(config.m_Reportable>>4)&0x0F,(config.m_Reportable)&0x0F);
	sprintf(m_rt,"%02x",(INT8U)config.m_Reportable);
	cfg_setstr(cfg,"m_Reportable",m_rt);

	char m_ra[9];
	memset(m_ra,0,9);
	//sprintf(m_rt,"%x%x",(config.m_Alarm_Shield>>4)&0x0F,(config.m_Alarm_Shield)&0x0F);
	sprintf(m_ra,"%02x%02x%02x%02x",(INT8U)config.m_Alarm_Shield[0],(INT8U)config.m_Alarm_Shield[1],
		(INT8U)config.m_Alarm_Shield[2],(INT8U)config.m_Alarm_Shield[3]);
	cfg_setstr(cfg,"m_Alarm_Shield",m_ra);

	char lose[7];
	memset(lose,0,7);
	//sprintf(lose,"%x%x%x%x%x%x",(config.m_Lose_ToDay[0]>>4)&0x0F,(config.m_Lose_ToDay[0])&0x0F,(config.m_Lose_ToDay[1]>>4)&0x0F,(config.m_Lose_ToDay[1])&0x0F,(config.m_Lose_ToDay[2]>>4)&0x0F,(config.m_Lose_ToDay[2])&0x0F);
	sprintf(lose,"%02x%02x%02x",(INT8U)config.m_Lose_ToDay[0],(INT8U)config.m_Lose_ToDay[1],(INT8U)config.m_Lose_ToDay[2]);
	cfg_setstr(cfg,"m_Lose_ToDay",lose);

	//char tep_JilianSlaveMachines[40];
	//memset(tep_JilianSlaveMachines,0,sizeof(tep_JilianSlaveMachines));
	//char tep_3[10];
	//int t=0;
	//memset(tep_3,0,sizeof(tep_3));
	//for(int i=0;i<4;i++)
	//{
	//	//if(i!=3)
	//	//	sprintf(tep_3,"%x%x%x%x%x%x%x%x,",Char_No1Hex((config.CzdAddrs[i*4+0])>>4),Char_No1Hex(config.CzdAddrs[i*4+0]),Char_No1Hex((config.CzdAddrs[i*4+1])>>4),Char_No1Hex(config.CzdAddrs[i*4+1]),Char_No1Hex((config.CzdAddrs[i*4+2])>>4),Char_No1Hex(config.CzdAddrs[i*4+2]),Char_No1Hex((config.CzdAddrs[i*4+3])>>4),Char_No1Hex(config.CzdAddrs[i*4+3]));
	//	//else 
	//	//	sprintf(tep_3,"%x%x%x%x%x%x%x%x",Char_No1Hex((config.CzdAddrs[i*4+0])>>4),Char_No1Hex(config.CzdAddrs[i*4+0]),Char_No1Hex((config.CzdAddrs[i*4+1])>>4),Char_No1Hex(config.CzdAddrs[i*4+1]),Char_No1Hex((config.CzdAddrs[i*4+2])>>4),Char_No1Hex(config.CzdAddrs[i*4+2]),Char_No1Hex((config.CzdAddrs[i*4+3])>>4),Char_No1Hex(config.CzdAddrs[i*4+3]));

	//	if(i!=3)
	//		sprintf(tep_3,"%02x%02x%02x%02x,",(INT8U)config.CzdAddrs[i*4+0],(INT8U)config.CzdAddrs[i*4+1],(INT8U)config.CzdAddrs[i*4+2],(INT8U)config.CzdAddrs[i*4+3]);
	//	else 
	//		sprintf(tep_3,"%02x%02x%02x%02x",(INT8U)config.CzdAddrs[i*4+0],(INT8U)config.CzdAddrs[i*4+1],(INT8U)config.CzdAddrs[i*4+2],(INT8U)config.CzdAddrs[i*4+3]);

	//	for(int f=0;f<9;f++)
	//	{
	//		tep_JilianSlaveMachines[t]=tep_3[f];
	//		t+=1;
	//	}
	//	memset(tep_3,0,sizeof(tep_3));
	//}
 //
	//cfg_setstr(cfg,"CzdAddrs",tep_JilianSlaveMachines);

	//sprintf(tep_3,"%x%x%x%x%x%x%x%x",Char_No1Hex((config.PbzdAddr[0])>>4),
	//	Char_No1Hex(config.PbzdAddr[0]),Char_No1Hex((config.PbzdAddr[1])>>4),
	//	Char_No1Hex(config.PbzdAddr[1]),Char_No1Hex((config.PbzdAddr[2])>>4),
	//	Char_No1Hex(config.PbzdAddr[2]),Char_No1Hex((config.PbzdAddr[3])>>4),
	//	Char_No1Hex(config.PbzdAddr[3]));
	//cfg_setstr(cfg,"CzdAddrs",tep_3);

	//sprintf(tep_3,"%02x%02x%02x%02x",(INT8U)config.PbzdAddr[0],(INT8U)config.PbzdAddr[1],(INT8U)config.PbzdAddr[2],(INT8U)config.PbzdAddr[3]);
	//cfg_setstr(cfg,"PbzdAddr",tep_3);

	cfg_setint(cfg,"m_Month_trail_read",config.m_Month_trail_read);
	char m_rtime[5];
	//sprintf(m_rtime,"%x%x%x%x",(config.m_ReportTime>>12)&0x0F,(config.m_ReportTime>>8)&0x0F,(config.m_ReportTime>>4)&0x0F,(config.m_ReportTime)&0x0F);
	sprintf(m_rtime,"%04x",config.m_ReportTime);
	cfg_setstr(cfg,"m_ReportTime",m_rtime);
	char m_bstime[7];
	sprintf(m_bstime,"%02X%02X%02X",config.m_BroadcastSetTime[0],config.m_BroadcastSetTime[1],config.m_BroadcastSetTime[2]);
	cfg_setstr(cfg,"m_BroadcastSetTime",m_bstime);
	cfg_setint(cfg,"m_BroadcastInterval",config.m_BroadcastInterval);
	//cfg_setint(cfg,"CRC_Times",config.CRC_Times);
	//2006-6-8刘志强+
	cfg_setint(cfg,"MaxMeterCount",config.MaxMeterCount);
	cfg_setstr(cfg,"UserCode",config.GetUserCode());
	cfg_setstr(cfg,"PlanKey",config.GetPlanKey());
	char key[CONS_KEY_MAX_LEN];
	MAKE_MODEM_TYPE_KEY(config.WirelessModemType,key);
	cfg_setstr(cfg,"WirelessModemType",key);

	memset(m_rt,0,3);
	//sprintf(m_rt,"%x%x",(config.AlarmCheckStartTime>>4)&0x0F,(config.AlarmCheckStartTime)&0x0F);
	sprintf(m_rt,"%02x",(INT8U)config.AlarmCheckStartTime);
	cfg_setstr(cfg,"AlarmCheckStartTime",m_rt);

	cfg_setint(cfg,"BeatReply",config.BeatReply);
	cfg_setint(cfg,"BEATIMES",config.BEATIMES);
	cfg_setstr(cfg,"RelaysVersion",config.RelaysVersion);

	//cfg_setint(cfg,"GPRSDialMaxTimes",config.GPRSDialMaxTimes);
	//cfg_setint(cfg,"GPRSReConnectMaxTimes",config.GPRSReConnectMaxTimes);

	//char startDate[3]={0x00,0x00,0x00};
	//sprintf(startDate,"%x",config.GPRSAutoConnectStartDate);
	////INT8U StartDate = bcdc(config.GPRSAutoConnectStartDate);
	////sscanf(startDate,"%d",&StartDate);
	//cfg_setstr(cfg,"GPRSAutoConnectStartDate",startDate);

	//char endDate[3]={0x00,0x00,0x00};
	//sprintf(endDate,"%x",config.GPRSAutoConnectEndDate);
	////INT8U EndDate = bcdc(config.GPRSAutoConnectEndDate);
	////sscanf(endDate,"%d",&EndDate);
	//cfg_setstr(cfg,"GPRSAutoConnectEndDate",endDate);

	//char startHour[3]={0x00,0x00,0x00};
	//sprintf(startHour,"%x",config.GPRSAutoConnectStartHour);
	////INT8U StartHour = bcdc(config.GPRSAutoConnectStartHour);
	////sscanf(startHour,"%d",&StartHour);
	//cfg_setstr(cfg,"GPRSAutoConnectStartHour",startHour);

	//char endHour[3]={0x00,0x00,0x00};
	//sprintf(endHour,"%x",config.GPRSAutoConnectEndHour);
	////INT8U EndHour = bcdc(config.GPRSAutoConnectEndHour);
	////sscanf(endHour,"%d",&EndHour);
	//cfg_setstr(cfg,"GPRSAutoConnectEndHour",endHour);

	//cfg_setint(cfg,"GPRSAutoDialInterMinute",config.GPRSAutoDialInterMinute);
	//cfg_setint(cfg,"GPRSAutoConnectInterMinute",config.GPRSAutoConnectInterMinute);
	////cfg_setint(cfg,"GPRSMonthGeneralTraffics",config.GPRSMonthGeneralTraffics);
	cfg_setint(cfg,"m_Check_Daydata_Type",config.m_Check_Daydata_Type);
	cfg_setint(cfg,"m_XIAOCHENG_PROTOCOL_Type",config.m_XC_Protocol_Type);
	cfg_print(cfg, fp);
	FileBase::Close(fp);
	cfg_free(cfg);
	return 0;
}

int SysConfigCfg::LoadGprsInfo(Context &oContext,const char *cfgFile)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_GPRS_INFO, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,cfgFile,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	GprsInfo &gprs_info = oContext.gprsInfo;
	gprs_info.EnableFlowCtrl = (cfg_getbool(cfg, "EnableFlowCtrl")==cfg_true);
	gprs_info.LogOnInterS = (INT16U)cfg_getint(cfg,"LogOnInterS");
	gprs_info.LogOnRetryTimes = (INT8U)cfg_getint(cfg,"LogOnRetryTimes");
	gprs_info.SocketConnectInterS = (INT16U)cfg_getint(cfg,"SocketConnectInterS");
	gprs_info.DialUpInterS = (INT16U)cfg_getint(cfg,"SocketConnectInterS");
	gprs_info.EnableMaxFlowLimit = (cfg_getbool(cfg, "EnableMaxFlowLimit")==cfg_true);
	gprs_info.GenericTrafficStatOfMonth = (INT32U)cfg_getint(cfg,"GenericTrafficStatOfMonth");
	return 0;
}

int SysConfigCfg::SaveGprsInfo(Context &oContext,const char *cfgFile)
{
	string buff;
	buff.reserve(3000);
	char cTemp[200];

	GprsInfo& gprs_info=oContext.gprsInfo;
	buff.append("EnableFlowCtrl=\"").append(gprs_info.EnableFlowCtrl==true?"true":"false").append("\"");

	sprintf(cTemp,"%d",gprs_info.LogOnInterS);
	buff.append("\nLogOnInterS=").append(cTemp);

	sprintf(cTemp,"%d",gprs_info.LogOnRetryTimes);
	buff.append("\nLogOnRetryTimes=").append(cTemp);

	sprintf(cTemp,"%d",gprs_info.SocketConnectInterS);
	buff.append("\nSocketConnectInterS=").append(cTemp);

	sprintf(cTemp,"%d",gprs_info.DialUpInterS);
	buff.append("\nDialUpInterS=").append(cTemp);

	buff.append("\nEnableMaxFlowLimit=\"").append(gprs_info.EnableMaxFlowLimit==true?"true":"false").append("\"");

	sprintf(cTemp,"%d",gprs_info.GenericTrafficStatOfMonth);
	buff.append("\nGenericTrafficStatOfMonth=").append(cTemp);

	return CfgBase::SaveSetFile(buff,cfgFile,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
}

/** @brief 系统配置参数结构体 */
cfg_opt_t OPT_PORTMAP_SETTINGS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_INT("VCom0",  0, CFGF_NOCASE),
	CFG_INT("VCom1",  0, CFGF_NOCASE),
	CFG_INT("VCom2",  0, CFGF_NOCASE),
	CFG_INT("VCom3",  0, CFGF_NOCASE),
	CFG_INT("VCom4",  0, CFGF_NOCASE),
	CFG_INT("VCom5",  0, CFGF_NOCASE),
	CFG_INT("VCom6",  0, CFGF_NOCASE),
	CFG_INT("VCom7",  0, CFGF_NOCASE),
	CFG_INT("VCom8",  2, CFGF_NOCASE),
	CFG_INT("VCom9",  9, CFGF_NOCASE),
	CFG_INT("VCom10", 10, CFGF_NOCASE),
	CFG_INT("VCom11", 0, CFGF_NOCASE),
	CFG_INT("VCom12", 0, CFGF_NOCASE),
	CFG_INT("VCom13", 0, CFGF_NOCASE),
	CFG_INT("VCom14", 0, CFGF_NOCASE),
	CFG_INT("VCom15", 0, CFGF_NOCASE),
	CFG_INT("VCom16", 0, CFGF_NOCASE),
	CFG_INT("VCom17", 0, CFGF_NOCASE),
	CFG_INT("VCom18", 0, CFGF_NOCASE),
	CFG_INT("VCom19", 0, CFGF_NOCASE),
	CFG_INT("VCom20", 6, CFGF_NOCASE),
	CFG_INT("VCom21", 2, CFGF_NOCASE),
	CFG_INT("VCom22", 11, CFGF_NOCASE),
	CFG_INT("VCom23", 12, CFGF_NOCASE),
	CFG_INT("VCom24", 13, CFGF_NOCASE),
	CFG_INT("VCom25", 0, CFGF_NOCASE),
	CFG_INT("VCom26", 0, CFGF_NOCASE),
	CFG_INT("VCom27", 0, CFGF_NOCASE),
	CFG_INT("VCom28", 0, CFGF_NOCASE),
	CFG_INT("VCom29", 0, CFGF_NOCASE),
	CFG_INT("VCom30", 0, CFGF_NOCASE),
	CFG_INT("VCom31", 7, CFGF_NOCASE),
	
	CFG_END()
};

/** @fn C_IniSys::LoadPortMap
 *  @brief 加载系统配置参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int SysConfigCfg::LoadPortMap(Context &oContext, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_PORTMAP_SETTINGS, CFGF_NONE);
	if(cfg_parse(cfg, fname) == CFG_PARSE_ERROR)
	{
		ASSERT(false);
		KL_ERROR("Parse cfg file[%s] failed\n",fname);
		cfg_free(cfg);
		return -1;
	}

	SysConfig& config=oContext.SysConfig;

	for (size_t i = 0; i < SYSTEM_SERIAL_PORT_MAX; i++)
	{
		char CfgItem[12];
		sprintf(CfgItem, "VCom%d", i);
		config.m_PortMap[i] = (INT8U)cfg_getint(cfg, CfgItem);
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SavePortMap
 *  @brief 保存系统配置参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int SysConfigCfg::SavePortMap(Context &oContext, const char *fname)
{
#if 0	
	string buff;
	buff.reserve(3000);
	char cTemp[200];

	SysConfig& config=oContext.SysConfig;

	for (size_t i = 0; i < SYSTEM_SERIAL_PORT_MAX; i++)
	{
		char CfgItem[12]={0};
		sprintf(CfgItem, "VCom%d", i );
		buff.append(CfgItem);
		memset(CfgItem,0x00,sizeof(CfgItem));
		sprintf(CfgItem,"=%d",config.m_PortMap[i]);
		buff.append(CfgItem);
		buff.append("\n");
	}

	cfg_free(cfg);
#endif
	return 0;
}



