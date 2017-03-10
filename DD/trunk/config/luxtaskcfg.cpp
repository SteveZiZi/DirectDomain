#include "../share/luxtask.h"
#include "luxtaskCfg.h"
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

cfg_opt_t OPT_LUXTASK_PERIOD_SETTINGS[] =
{
	CFG_STR_LIST("day", "{FF}", CFGF_NONE),
	CFG_STR_LIST("weekday", "{FF}", CFGF_NONE),
	CFG_STR_LIST("month", "{FF}", CFGF_NONE),
	CFG_END()
};



cfg_opt_t OPT_LUXTASK_ACTIONITEM_SETTINGS[] =
{
	CFG_INT("deviceno",0, CFGF_NOCASE),
	CFG_INT("subno",0, CFGF_NOCASE),
	CFG_INT("funcno",0, CFGF_NOCASE),
	CFG_INT("data",0, CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_LUXTASK_PLAN_SETTINGS[] =
{
	CFG_INT("keepMins", 0, CFGF_NOCASE),
	CFG_INT("actionItemsNum",0, CFGF_NOCASE),
	CFG_SEC("ActionItem", OPT_LUXTASK_ACTIONITEM_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_LUXTASK_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
	CFG_INT("TaskNo",0, CFGF_NOCASE),
	CFG_INT("Priority",0, CFGF_NOCASE),
	CFG_STR("StartDate", "2009-10-14", CFGF_NONE),
	CFG_STR("StartTime", "00:00", CFGF_NONE),
	CFG_STR("EndDate", "2009-10-14", CFGF_NONE),
	CFG_STR("EndTime", "00:00", CFGF_NONE),
	CFG_INT("PeriodUnit", 0, CFGF_NOCASE),
	CFG_SEC("PeriodDetail", OPT_LUXTASK_PERIOD_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
	CFG_INT("StartLuxlimit", 0, CFGF_NOCASE),
	CFG_INT("EndLuxlimit", 0, CFGF_NOCASE),
	CFG_INT("LuxSensorsBS", 0, CFGF_NOCASE),
	CFG_SEC("Plan", OPT_LUXTASK_PLAN_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),

	CFG_END()
};

cfg_opt_t OPT_LUXTASKS_SETTINGS[] =
{
	CFG_SEC("nLuxTask", OPT_LUXTASK_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
	CFG_END()
};


int LuxTaskCfg::LoadLuxTasks(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_LUXTASKS_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}   
	cfg_t* cfgTask;
	
	oContext.LuxTasks.Init();//清除任务数据

	for(unsigned int i = 0; i < cfg_size(cfg, "nLuxTask"); i++)
	{
		cfgTask = cfg_getnsec(cfg, "nLuxTask", i);
		INT8U taskNo=(INT8U)cfg_getint(cfgTask, "TaskNo");

		SLuxTask& ntask =oContext.LuxTasks.m_Tasks[taskNo];
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

		ntask.m_StartLuxlimit = (INT16U)cfg_getint(cfgTask, "StartLuxlimit");
		ntask.m_EndLuxlimit = (INT16U)cfg_getint(cfgTask, "EndLuxlimit");
		ntask.m_LuxSensorsBS  = (INT32U)cfg_getint(cfgTask, "LuxSensorsBS");
		for (int j=0;j<32;j++)// 1字节8bit
		{
			if (ntask.m_LuxSensorsBS&(0x00000001<<j))
			{
				ntask.m_LuxSensorsDBS.set(j+CONS_DEVICE_LUXSENSOR_STARTNO);
			}
		}
		ntask.m_LuxSensorsDBS.Stat();
		/* Plan */
		cfg_t* cfgPlanSec;
		for(unsigned int k = 0; k < cfg_size(cfgTask, "Plan");k++)
		{
			SLuxPlan& plan =  ntask.m_Plans[k];
			//cfgPlanSec = cfg_getnsec(cfgPlan, "Plan", k);
			cfgPlanSec = cfg_getnsec(cfgTask, "Plan", k);
			plan.m_Enable = true;
			plan.m_KeepMins = (INT16U)cfg_getint(cfgPlanSec, "keepMins");
			plan.m_ActionItemsNum = (INT8U)cfg_getint(cfgPlanSec, "actionItemsNum");

			/* ActionItem */
			cfg_t* cfgActionItemSec;
			INT8U deviceNo;
			//for(unsigned int d = 0; d < cfg_size(cfgPlan, "ActionItem");d++)
			for(unsigned int d = 0; d < cfg_size(cfgPlanSec, "ActionItem");d++)
			{
				 cfgActionItemSec = cfg_getnsec(cfgPlanSec, "ActionItem", d);
			    	 SSmartGarageItem& actionItem = plan.m_ActionItems[d];
				 actionItem.m_DeviceNo = (INT8U)cfg_getint(cfgActionItemSec, "deviceno");
				 deviceNo = actionItem.m_DeviceNo;
 				 actionItem.m_SubNo = (INT8U)cfg_getint(cfgActionItemSec, "subno");
				 actionItem.m_Function = (INT8U)cfg_getint(cfgActionItemSec, "funcno");
				 INT32U data = (INT32U)cfg_getint(cfgActionItemSec, "data");
				 actionItem.m_FuncData[0] = (INT8U)(data>>24);
				 actionItem.m_FuncData[1] = (INT8U)(data>>16);
				 actionItem.m_FuncData[2] = (INT8U)(data>>8);
				 actionItem.m_FuncData[3] = (INT8U)(data);

			}
			
		}
		if (ntask.m_Enable == true)
		{
//			oContext.NorTasks.m_NorTBS.set(taskNo);
			oContext.LuxTasks.m_LuxValidTBS.set(taskNo);
		}
		else
		{
			oContext.LuxTasks.m_LuxValidTBS.reset(taskNo);
		}
		oContext.LuxTasks.m_LuxTBS.set(taskNo);
	}
	cfg_free(cfg);
	oContext.LuxTasks.m_LuxTBS.Stat();
	oContext.LuxTasks.m_LuxValidTBS.Stat();

	return 0;
}

int LuxTaskCfg::SaveTasks(Context& oContext,const char* name)
{
	SLuxTasks& TaskCls=oContext.LuxTasks;
	string buff;
	string sStr;
	buff.reserve(2000);
	sStr.reserve(500);

	for(size_t i=1;i<TaskCls.m_LuxTBS.size();i++)
	{
		if (TaskCls.m_LuxTBS.test(i))
		{
			KL_VERBOSE("SaveTasks::taskno=%d\n",i);
			char cTemp[200];
			sprintf(cTemp,"%s",TaskCls.m_Tasks[i].m_Enable==1?"true":"false");
			buff.append("nLuxTask\n{\n\tEnable=\"").append(cTemp).append("\"");

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

			sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_StartLuxlimit);
			buff.append("\n\tStartLuxlimit=").append(cTemp);

			sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_EndLuxlimit);
			buff.append("\n\tEndLuxlimit=").append(cTemp);

			sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_LuxSensorsBS);
			buff.append("\n\tLuxSensorsBS=").append(cTemp);

			for (int j=0;j<32;j++)// 1字节8bit
			{
				if (TaskCls.m_Tasks[i].m_LuxSensorsBS&(0x00000001<<j))
				{
					TaskCls.m_Tasks[i].m_LuxSensorsDBS.set(j+CONS_DEVICE_LUXSENSOR_STARTNO);
				}
			}
			TaskCls.m_Tasks[i].m_LuxSensorsDBS.Stat();

				
			INT8U deviceNo;
			for (int k=0; k<CONS_LUXTASKPLAN_MAX_COUNT; k++)
			{
				if (TaskCls.m_Tasks[i].m_Plans[k].m_Enable == true)
				{
					buff.append("\n\tPlan\n\t{");
					sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_Plans[k].m_KeepMins);
					buff.append("\n\t\tkeepMins=").append(cTemp);
					sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_Plans[k].m_ActionItemsNum);
					buff.append("\n\t\tactionItemsNum=").append(cTemp);

					for (int n=0; n<TaskCls.m_Tasks[i].m_Plans[k].m_ActionItemsNum;n++)
					{
						buff.append("\n\t\tActionItem\n\t\t{");
						sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_Plans[k].m_ActionItems[n].m_DeviceNo);
						deviceNo = TaskCls.m_Tasks[i].m_Plans[k].m_ActionItems[n].m_DeviceNo;
						buff.append("\n\t\t\tdeviceno=").append(cTemp);

						sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_Plans[k].m_ActionItems[n].m_SubNo);
						buff.append("\n\t\t\tsubno=").append(cTemp);
						
						sprintf(cTemp,"%d",TaskCls.m_Tasks[i].m_Plans[k].m_ActionItems[n].m_Function);
						buff.append("\n\t\t\tfuncno=").append(cTemp);
						
						INT32U dataVal=0;	

						dataVal = (INT32U)(TaskCls.m_Tasks[i].m_Plans[k].m_ActionItems[n].m_FuncData[0]<<24
							        |TaskCls.m_Tasks[i].m_Plans[k].m_ActionItems[n].m_FuncData[1]<<16 
							        | TaskCls.m_Tasks[i].m_Plans[k].m_ActionItems[n].m_FuncData[2]<<8 
							        |TaskCls.m_Tasks[i].m_Plans[k].m_ActionItems[n].m_FuncData[3]);
						sprintf(cTemp,"%d",dataVal);	
						buff.append("\n\t\t\tdata=").append(cTemp);

						buff.append("\n\t\t}");
					}
					buff.append("\n\t}");
		
				}
				
			}
			buff.append("\n}\n");
		}
	}

	int iFlag=CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	return iFlag;
}




