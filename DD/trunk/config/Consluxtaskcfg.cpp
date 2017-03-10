#include "ConsluxtaskCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../share/RegData.h"
#include "../include/StdInclude.h"
#include "../share/Server.h"
#include "../base/FileBase.h"
#include "CfgBase.h"
#include "../base/klb_base.h"
#include "../base/KLBase.h"

/*****************************************************************************************************8
struct tm {
 2                   int tm_sec;         /* seconds *
 3                   int tm_min;         /* minutes *
 4                   int tm_hour;        /* hours *
 5                   int tm_mday;        /* day of the month *
 6                   int tm_mon;         /* month *
 7                   int tm_year;        /* year *
 8                   int tm_wday;        /* day of the week *
 9                   int tm_yday;        /* day in the year *
10                   int tm_isdst;       /* daylight saving time *
11 
12               };
13 
14        tm_sec The  number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds.
15 
16        tm_min The number of minutes after the hour, in the range 0 to 59.
17 
18        tm_hour
19               The number of hours past midnight, in the range 0 to 23.
20 
21        tm_mday
22               The day of the month, in the range 1 to 31.
23 
24        tm_mon The number of months since January, in the range 0 to 11.
25 
26        tm_year
27               The number of years since 1900.
28 
29        tm_wday
30               The number of days since Sunday, in the range 0 to 6.
31 
32        tm_yday
33               The number of days since January 1, in the range 0 to 365.
34 
35        tm_isdst
36               A flag that indicates whether daylight saving time is in effect at the  time  described. The value is positive if daylight saving time is in effect, zero if it is not, and negative if the information is not available.
****************************************************************************************************/

cfg_opt_t OPT_STRATEGY_PLAN_SETTINGS[] =
{
	CFG_INT("speedlevel", 1, CFGF_NOCASE),
	CFG_INT("luxdiff", 0, CFGF_NOCASE),
	CFG_INT("stepvol", 0, CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_CONSLUXTASK_PERIOD_SETTINGS[] =
{
	CFG_STR_LIST("day", "{FF}", CFGF_NONE),
	CFG_STR_LIST("weekday", "{FF}", CFGF_NONE),
	CFG_STR_LIST("month", "{FF}", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_PERIODTIME_SETTINGS[] =
{
	CFG_STR("begintime", "00:00", CFGF_NONE),
	CFG_STR("endtime", "00:00", CFGF_NONE),	
	CFG_END()
};
cfg_opt_t OPT_CONSLUXTASK_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
	CFG_INT("TaskNo",0, CFGF_NOCASE),
	CFG_INT("Priority",0, CFGF_NOCASE),
	CFG_STR("StartDate", "2009-10-14", CFGF_NONE),
	CFG_STR("StartTime", "00:00", CFGF_NONE),
	CFG_STR("EndDate", "2009-10-14", CFGF_NONE),
	CFG_STR("EndTime", "00:00", CFGF_NONE),
	CFG_INT("PeriodUnit", 0, CFGF_NOCASE),
	CFG_SEC("PeriodDetail", OPT_CONSLUXTASK_PERIOD_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
	CFG_SEC("PeriodTime", OPT_PERIODTIME_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
	CFG_INT("ConstantLux", 0, CFGF_NOCASE),
	CFG_INT("LuxSensorsBS", 0, CFGF_NOCASE),
	CFG_INT("ClrelayNo", 0, CFGF_NOCASE),
	CFG_SEC("Strategy", OPT_STRATEGY_PLAN_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_CONSLUXTASKS_SETTINGS[] =
{
	CFG_SEC("nConsLuxTask", OPT_CONSLUXTASK_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
	CFG_END()
};


int ConsLuxTaskCfg::LoadConsLuxTasks(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_CONSLUXTASKS_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}   
	cfg_t* cfgTask;
	
	oContext.ConsLuxTasks.Init();//清除任务数据

	for(unsigned int i = 0; i < cfg_size(cfg, "nConsLuxTask"); i++)
	{
		cfgTask = cfg_getnsec(cfg, "nConsLuxTask", i);
		INT8U taskNo=(INT8U)cfg_getint(cfgTask, "TaskNo");

		SConsLuxTask& ntask =oContext.ConsLuxTasks.m_Tasks[taskNo];
		ntask.m_Enable=(cfg_getbool(cfgTask, "Enable")==cfg_true)?1:0;
		ntask.m_TaskNo=taskNo;
		ntask.m_Priority=(INT8U)cfg_getint(cfgTask, "Priority");

		ntask.m_StartTime = GetStandardTime(cfg_getstr(cfgTask,"StartDate"), cfg_getstr(cfgTask,"StartTime"));
		ntask.m_EndTime = GetStandardTime(cfg_getstr(cfgTask,"EndDate"), cfg_getstr(cfgTask,"EndTime"));

		ntask.m_PeriodUnit = (INT8U)cfg_getint(cfgTask, "PeriodUnit");
		
		/* PeriodDetail */
		cfg_t* cfg_PeriodDetail;
		cfg_t* cfgPeriodDetailCtx = cfg_getnsec(cfgTask, "PeriodDetail", 0);

		for(unsigned t = 0; t < cfg_size(cfgPeriodDetailCtx, "day"); t++)
		{
			ntask.m_PeriodDetail.m_DayBS.Parse(cfg_getnstr(cfgPeriodDetailCtx, "day", t),1,31);
		}

		for(unsigned t = 0; t < cfg_size(cfgPeriodDetailCtx, "weekday"); t++)
		{
			ntask.m_PeriodDetail.m_WeekBS.Parse(cfg_getnstr(cfgPeriodDetailCtx, "weekday", t),0,6);
		}

		for(unsigned t = 0; t < cfg_size(cfgPeriodDetailCtx, "month"); t++)
		{
			ntask.m_PeriodDetail.m_MonthBS.Parse(cfg_getnstr(cfgPeriodDetailCtx, "month", t),1,12);
		}
		/* Period */
		cfg_t* cfgPeriodSec;
		for(unsigned int k = 0; k < cfg_size(cfgTask, "PeriodTime");k++)
		{
			cfgPeriodSec = cfg_getnsec(cfgTask, "PeriodTime", k);
			char *pTime = cfg_getstr(cfgPeriodSec,"begintime");

			int hour,min;
			sscanf(pTime,"%x:%x",&hour,&min);
			ntask.m_TimePeriodInDay[k][0] = (INT8U)hour;
			ntask.m_TimePeriodInDay[k][1] = (INT8U)min;

			pTime = cfg_getstr(cfgPeriodSec,"endtime");
			hour,min;
			sscanf(pTime,"%x:%x",&hour,&min);
			ntask.m_TimePeriodInDay[k][2] = (INT8U)hour;
			ntask.m_TimePeriodInDay[k][3] = (INT8U)min;
		}
		ntask.m_ConstantLux = (INT16U)cfg_getint(cfgTask, "ConstantLux");
		ntask.m_LuxSensorsBS  = (INT32U)cfg_getint(cfgTask, "LuxSensorsBS");
		for (int j=0;j<32;j++)// 1字节8bit
		{
			if (ntask.m_LuxSensorsBS&(0x00000001<<j))
			{
				ntask.m_LuxSensorsDBS.set(j+CONS_DEVICE_LUXSENSOR_STARTNO);
			}
		}
		ntask.m_LuxSensorsDBS.Stat();

		ntask.m_ClrelayNo = (INT8U)cfg_getint(cfgTask, "ClrelayNo");
		if (ntask.m_Enable == true)
		{
			oContext.ConsLuxTasks.m_ConsLuxValidTBS.set(taskNo);
		}
		else
		{
			oContext.ConsLuxTasks.m_ConsLuxValidTBS.reset(taskNo);
		}

		cfg_t* cfgStrategyItem;
		for(unsigned t = 0; t < cfg_size(cfgTask, "Strategy"); t++)
		{
			if (t  == CONS_STRATEGY_PLANS_MAX)
			{
				break;
			}

			cfgStrategyItem = cfg_getnsec(cfgTask, "Strategy", t);	
			ntask.m_Strategys[t].Init();
			ntask.m_Strategys[t].m_bValid = true;
			ntask.m_Strategys[t].m_speedlevel = (INT8U)cfg_getint(cfgStrategyItem, "speedlevel");
			ntask.m_Strategys[t].m_luxdiff = (INT16U)cfg_getint(cfgStrategyItem, "luxdiff");
			ntask.m_Strategys[t].m_stepvol = D2B((INT16U)cfg_getint(cfgStrategyItem, "stepvol"));
		}
		oContext.ConsLuxTasks.m_ConsLuxTBS.set(taskNo);
	}
	cfg_free(cfg);
	oContext.ConsLuxTasks.m_ConsLuxTBS.Stat();
	oContext.ConsLuxTasks.m_ConsLuxValidTBS.Stat();

	return 0;
}

int ConsLuxTaskCfg::SaveTasks(Context& oContext,const char* name)
{
	SConsLuxTasks& TaskCls=oContext.ConsLuxTasks;
	string buff;
	string sStr;
	buff.reserve(2000);
	sStr.reserve(500);

	for(size_t i=0;i<TaskCls.m_ConsLuxTBS.size();i++)
	{
		if (TaskCls.m_ConsLuxTBS.test(i))
		{
			KL_VERBOSE("SaveTasks::taskno=%d\n",i);
			char cTemp[200];
			sprintf(cTemp,"%s",TaskCls.m_Tasks[i].m_Enable==1?"true":"false");
			buff.append("nConsLuxTask\n{\n\tEnable=\"").append(cTemp).append("\"");

			sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_TaskNo);
			buff.append("\n\tTaskNo=").append(cTemp);
			
			sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_Priority);
			buff.append("\n\tPriority=").append(cTemp);

			string StdDate,StdTime;
			GetBaseDateString(TaskCls.m_Tasks[i].m_StartTime,StdDate,StdTime);
			buff.append("\n\tStartDate=").append(StdDate);
			buff.append("\n\tStartTime=").append(StdTime);	
			
			StdDate.clear();
			StdTime.clear();
			GetBaseDateString(TaskCls.m_Tasks[i].m_EndTime,StdDate,StdTime);
			buff.append("\n\tEndDate=").append(StdDate);
			buff.append("\n\tEndTime=").append(StdTime);	

			sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_PeriodUnit);
			buff.append("\n\tPeriodUnit=").append(cTemp);

			buff.append("\n\tPeriodDetail\n\t{");
			string sStr;
			sStr.erase();
			TaskCls.m_Tasks[i].m_PeriodDetail.m_DayBS.ToString(sStr,1,31);
			buff.append("\n\t\tday={").append(sStr).append("}");
			sStr.erase();
			TaskCls.m_Tasks[i].m_PeriodDetail.m_WeekBS.ToString(sStr,0,6);
			buff.append("\n\t\tweekday={").append(sStr).append("}");
			sStr.erase();
			TaskCls.m_Tasks[i].m_PeriodDetail.m_MonthBS.ToString(sStr,1,12);
			buff.append("\n\t\tmonth={").append(sStr).append("}");
			buff.append("\n\t}");

			for (int n=0; n<CONS_TIME_PERIOD_IN_DAY_MAX_COUNT;n++)
			{
				if (TaskCls.m_Tasks[i].m_TimePeriodInDay[n][0]==0x00
					&&TaskCls.m_Tasks[i].m_TimePeriodInDay[n][1]==0x00
					&&TaskCls.m_Tasks[i].m_TimePeriodInDay[n][0]==0x00
					&&TaskCls.m_Tasks[i].m_TimePeriodInDay[n][0]==0x00
					)
				{//全为零表示有效的时段已加载完成
					break;
				}
				buff.append("\n\PeriodTime\n\t{");
				sprintf(cTemp,"%x:%x",TaskCls.m_Tasks[i].m_TimePeriodInDay[n][0],TaskCls.m_Tasks[i].m_TimePeriodInDay[n][1]);
				buff.append("\n\t\tbegintime=").append(cTemp);
				sprintf(cTemp,"%x:%x",TaskCls.m_Tasks[i].m_TimePeriodInDay[n][2],TaskCls.m_Tasks[i].m_TimePeriodInDay[n][3]);
				buff.append("\n\t\tendtime=").append(cTemp);
				buff.append("\n\t}");
			}

			sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_ConstantLux);
			buff.append("\n\tConstantLux=").append(cTemp);

			sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_LuxSensorsBS);
			buff.append("\n\tLuxSensorsBS=").append(cTemp);

			sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_ClrelayNo);
			buff.append("\n\tClrelayNo=").append(cTemp);

			for (int j=0;j<32;j++)// 1字节8bit
			{
				if (TaskCls.m_Tasks[i].m_LuxSensorsBS&(0x00000001<<j))
				{
					TaskCls.m_Tasks[i].m_LuxSensorsDBS.set(j+CONS_DEVICE_LUXSENSOR_STARTNO);
				}
			}
			TaskCls.m_Tasks[i].m_LuxSensorsDBS.Stat();

			for (int j=0; j<CONS_STRATEGY_PLANS_MAX; j++)
			{
				SStrategy& Item = TaskCls.m_Tasks[i].m_Strategys[j];
				if (Item.m_bValid == true)
				{
			
					sprintf(cTemp,"%d",Item.m_speedlevel);
					buff.append("\n\tStrategy\n\t{\n\t\tspeedlevel=\"").append(cTemp).append("\"");

					sprintf(cTemp,"%d",Item.m_luxdiff);
					buff.append("\n\t\tluxdiff=\"").append(cTemp).append("\"");

					sprintf(cTemp,"%x",Item.m_stepvol);
					buff.append("\n\t\tstepvol=\"").append(cTemp).append("\"");

					buff.append("\n\t}\n");
				}
			}
			
 			buff.append("\n}\n");
		}
	}

	int iFlag=CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	return iFlag;
}




