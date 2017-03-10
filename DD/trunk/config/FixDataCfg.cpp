#include "./FixDataCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../base/KLTrace.h"
#include "../base/FileBase.h"
#include "../base/Modem.h"
#include "CfgBase.h"

cfg_opt_t OPT_FIXDATA_SETTINGS[] =
{
	CFG_BOOL("Enable",cfg_false,CFGF_NOCASE),
		CFG_STR("FixMethod", "Cur",CFGF_NOCASE),

		CFG_STR_LIST("FixTimer.Minute", "{0,15,30,45}", CFGF_NONE),
		CFG_STR_LIST("FixTimer.Hour", "{FF}", CFGF_NONE),
		CFG_STR_LIST("FixTimer.Day", "{FF}", CFGF_NONE),
		CFG_STR_LIST("FixTimer.Month", "{FF}", CFGF_NONE),

		CFG_BOOL("IncreasingFlag", cfg_false, CFGF_NOCASE),
		CFG_BOOL("ReportFlag", cfg_false, CFGF_NOCASE),
		CFG_END()
};

int FixDataCfg::LoadFixDataCfg(Context& oContext,const char* cfgFile)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_FIXDATA_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,cfgFile,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	oContext.FixData.Enable=(cfg_getbool(cfg, "Enable")==cfg_true)?1:0;
	oContext.FixData.FixMethod=MAKE_DATA_PERIOD_VAL(cfg_getstr(cfg,"FixMethod"));
	//Get_FixTime(cfg_getstr(cfg,"FixTime"),oContext.FixData.FixTime);

	for(unsigned t = 0; t < cfg_size(cfg, "FixTimer.Minute"); t++)
	{
		oContext.FixData.FixTimer.MinuteBS.Parse(cfg_getnstr(cfg, "FixTimer.Minute", t),0,59);
	}
	for(unsigned t = 0; t < cfg_size(cfg, "FixTimer.Hour"); t++)
	{
		oContext.FixData.FixTimer.HourBS.Parse(cfg_getnstr(cfg, "FixTimer.Hour", t),0,23);
	}
	for(unsigned t = 0; t < cfg_size(cfg, "FixTimer.Day"); t++)
	{
		oContext.FixData.FixTimer.DayBS.Parse(cfg_getnstr(cfg, "FixTimer.Day", t),1,31);
	}
	for(unsigned t = 0; t < cfg_size(cfg, "FixTimer.Month"); t++)
	{
		oContext.FixData.FixTimer.MonthBS.Parse(cfg_getnstr(cfg, "FixTimer.Month", t),1,12);
	}
	oContext.FixData.IncreasingFlag=(cfg_getbool(cfg, "IncreasingFlag")==cfg_true)?1:0;
	oContext.FixData.ReportFlag=(cfg_getbool(cfg, "ReportFlag")==cfg_true)?1:0;
	cfg_free(cfg);	
	return 0;
}
//void FixDataCfg::Get_FixTime(const char *m_Time,unsigned char* FixTime)
//{
//unsigned char r1,r2,r3,r4;
//r1=Char_No1Hex(*(m_Time+0));
//r2=Char_No1Hex(*(m_Time+1));
//r3=Char_No1Hex(*(m_Time+2));
//r4=Char_No1Hex(*(m_Time+3));

//FixTime[0]=(r1<<4)+r2;
//FixTime[1]=(r3<<4)+r4;

//}

int FixDataCfg::SaveFixDataCfg(Context& oContext,const char* cfgFile)
{
	FixData& fd=oContext.FixData;
	string buff;
	buff.reserve(300);
	char cTemp[20];	
	string sStr;

	sprintf(cTemp,"%s",fd.Enable==1?"true":"false");
	buff.append("Enable=\"").append(cTemp).append("\"");


	MAKE_DATA_PERIOD_KEY(fd.FixMethod,cTemp);
	buff.append("\nFixMethod=\"").append(cTemp).append("\"");

	sStr.erase();
	oContext.FixData.FixTimer.MinuteBS.ToString(sStr,0,59);
	buff.append("\nFixTimer.Minute={").append(sStr).append("}");
	sStr.erase();
	oContext.FixData.FixTimer.HourBS.ToString(sStr,0,23);
	buff.append("\nFixTimer.Hour={").append(sStr).append("}");
	sStr.erase();
	oContext.FixData.FixTimer.DayBS.ToString(sStr,1,31);
	buff.append("\nFixTimer.Day={").append(sStr).append("}");
	sStr.erase();
	oContext.FixData.FixTimer.MonthBS.ToString(sStr,1,12);
	buff.append("\nFixTimer.Month={").append(sStr).append("}");

	sprintf(cTemp,"%s",fd.IncreasingFlag==1?"true":"false");
	buff.append("\nIncreasingFlag=\"").append(cTemp).append("\"");

	sprintf(cTemp,"%s",fd.ReportFlag==1?"true":"false");
	buff.append("\nReportFlag=\"").append(cTemp).append("\"");


	buff.append("\n");

	int iFlag=CfgBase::SaveSetFile(buff,cfgFile,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	return iFlag;
}




