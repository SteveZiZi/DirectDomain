#include "TaskCfg.h"
#include "../share/Context.h"
#include "../config/confuse/confuse.h"
#include "../share/RegData.h"
#include "../include/StdInclude.h"
#include "../share/Server.h"
#include "../base/FileBase.h"
#include "CfgBase.h"


//TODO:增加对TimeUnit的文字配置支持

cfg_opt_t OPT_TASK_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
		CFG_INT("TaskNo",0, CFGF_NOCASE),
		CFG_INT("Priority",0, CFGF_NOCASE),
		CFG_INT("MaxTimes", 1, CFGF_NOCASE),
		CFG_BOOL("IdleEnable", cfg_false,CFGF_NOCASE),

		CFG_INT("FreezeTimer.StartUnit", TIME_UNIT_MINUTE, CFGF_NOCASE),
		CFG_INT("FreezeTimer.StartVal", 0, CFGF_NOCASE),
		CFG_INT("FreezeTimer.IntervalUnit", TIME_UNIT_MINUTE, CFGF_NOCASE),
		CFG_INT("FreezeTimer.IntervalVal", 15, CFGF_NOCASE),

		CFG_STR_LIST("GatherTimer.Minute", "{0,15,30,45}", CFGF_NONE),
		CFG_STR_LIST("GatherTimer.Hour", "{FF}", CFGF_NONE),
		CFG_STR_LIST("GatherTimer.Day", "{FF}", CFGF_NONE),
		CFG_STR_LIST("GatherTimer.Month", "{FF}", CFGF_NONE),

		CFG_INT("FixDelayUnit", 0, CFGF_NOCASE),
		CFG_INT("FixDelayVal", 0, CFGF_NOCASE),

		CFG_INT("FreezeDelayUnit", TIME_UNIT_INVALID, CFGF_NOCASE),
		CFG_INT("FreezeDelayVal", 0, CFGF_NOCASE),
		CFG_STR("FixMethod","CurHD", CFGF_NOCASE),
		CFG_STR_LIST("FixData", "{}", CFGF_NONE),

		CFG_STR_LIST("Meters", "{All}", CFGF_NONE),
		CFG_STR("DataType","Reg", CFGF_NOCASE),
		CFG_STR("DataPeriod","Cur", CFGF_NOCASE),
		CFG_STR_LIST("Data", "{}", CFGF_NONE),
		CFG_INT("MaxRecords",1, CFGF_NOCASE),
		CFG_END()
};

cfg_opt_t OPT_TASKES_SETTINGS[] =
{
	CFG_SEC("Task", OPT_TASK_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
		CFG_END()
};

int TaskCfg::LoadDataIDS(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_TASKES_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	cfg_t* cfgTask;
	for(unsigned int i = 0; i < cfg_size(cfg, "Task"); i++)
	{
		cfgTask = cfg_getnsec(cfg, "Task", i);
		INT8U dataType=MAKE_DATA_TYPE_VAL(cfg_getstr(cfgTask, "DataType"));
		if (IsRegDataType(dataType)==true)
		{
			for(unsigned t = 0; t < cfg_size(cfgTask, "Data"); t++)
			{
				oContext.RegIDS.AddParse(cfg_getnstr(cfgTask, "Data", t));
			}
		}
	}
	cfg_free(cfg);
	return 0;
}

int TaskCfg::LoadTasks(Context& oContext,const char* name)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_TASKES_SETTINGS, CFGF_NONE);
	if (CfgBase::ParseSetFile(cfg,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey())!=0)
	{
		cfg_free(cfg);
		return -1;
	}   
	cfg_t* cfgTask;

	for(unsigned int i = 0; i < cfg_size(cfg, "Task"); i++)
	{
		cfgTask = cfg_getnsec(cfg, "Task", i);
		INT8U taskNo=(INT8U)cfg_getint(cfgTask, "TaskNo");
		if (Task::IsValidNo(taskNo)==false)
		{
			continue;
		}
		Task& task =oContext.Tasks.Item[taskNo];
		task.Enable=(cfg_getbool(cfgTask, "Enable")==cfg_true)?1:0;
		task.TaskNo=taskNo;
		task.Priority=(INT8U)cfg_getint(cfgTask, "Priority");
		task.MaxTimes=(INT8U)cfg_getint(cfgTask, "MaxTimes");
		//保障每个任务要重试5次
		if(task.MaxTimes<2)
		{
			task.MaxTimes=2;
		}
		task.IdleEnable=(cfg_getbool(cfgTask, "IdleEnable")==cfg_true)?1:0;

		task.DataType=MAKE_DATA_TYPE_VAL(cfg_getstr(cfgTask, "DataType"));

		task.FreezeTimer.StartUnit=(INT8S)cfg_getint(cfgTask, "FreezeTimer.StartUnit");
		task.FreezeTimer.StartVal=(INT8S)cfg_getint(cfgTask, "FreezeTimer.StartVal");
		task.FreezeTimer.IntervalUnit=(INT8S)cfg_getint(cfgTask, "FreezeTimer.IntervalUnit");
		task.FreezeTimer.IntervalVal=(INT8S)cfg_getint(cfgTask, "FreezeTimer.IntervalVal");

		//补抄延迟时间
		task.FixDelayUnit=(INT8S)cfg_getint(cfgTask, "FixDelayUnit");
		task.FixDelayVal=(INT8S)cfg_getint(cfgTask, "FixDelayVal");

		task.FreezeDelayUnit=(INT8S)cfg_getint(cfgTask, "FreezeDelayUnit");
		task.FreezeDelayVal=(INT8S)cfg_getint(cfgTask, "FreezeDelayVal");

		for(unsigned t = 0; t < cfg_size(cfgTask, "GatherTimer.Minute"); t++)
		{
			task.GatherTimer.MinuteBS.Parse(cfg_getnstr(cfgTask, "GatherTimer.Minute", t),0,59);
		}
		for(unsigned t = 0; t < cfg_size(cfgTask, "GatherTimer.Hour"); t++)
		{
			task.GatherTimer.HourBS.Parse(cfg_getnstr(cfgTask, "GatherTimer.Hour", t),0,23);
		}
		for(unsigned t = 0; t < cfg_size(cfgTask, "GatherTimer.Day"); t++)
		{
			task.GatherTimer.DayBS.Parse(cfg_getnstr(cfgTask, "GatherTimer.Day", t),1,31);
		}
		for(unsigned t = 0; t < cfg_size(cfgTask, "GatherTimer.Month"); t++)
		{
			task.GatherTimer.MonthBS.Parse(cfg_getnstr(cfgTask, "GatherTimer.Month", t),1,12);
		}

		if (IsRegDataType(task.DataType)==true)
		{
			for(unsigned t = 0; t < cfg_size(cfgTask, "Data"); t++)
			{
				oContext.RegIDS.SetBitSet(cfg_getnstr(cfgTask, "Data", t),task.DBS);
			}
		}
		else if (IsActionDataType(task.DataType)==true)
		{
			for(unsigned t = 0; t < cfg_size(cfgTask, "Data"); t++)
			{
				oContext.Actions.SetBitSet(cfg_getnstr(cfgTask, "Data", t),task.DBS);
			}
		}

		task.DBS.Stat();
		task.CfgDBS=task.DBS;
		task.CfgDBS.Stat();


		for(unsigned t = 0; t < cfg_size(cfgTask, "Meters"); t++)
		{
			task.MBS.Parse(cfg_getnstr(cfgTask, "Meters", t));
		}
		//将不存在的电表去掉，一定要保证meter.ini比task.ini先加载
		/*for (size_t j=0; j<task.MBS.size(); j++)
		{
			if (!oContext.Meters.MBS.test(j))
			{
				task.MBS.reset(j);
			}
		}*/
		task.MBS.Stat();
		task.FixMethod=MAKE_DATA_FIX_METHOD_VAL(cfg_getstr(cfgTask, "FixMethod"));
		task.FixDBS.reset();
		if (task.FixMethod==DATA_FIX_METHOD_CURHD)
		{
			unsigned int regID;
			for(size_t t=task.DBS.FromPos;t<task.DBS.size();t++)
			{
				if (task.DBS.test(t)==false)
				{
					continue;
				}
				regID=oContext.RegIDS.Item[t];
				if (DataID::GetDataType(regID)==DataID::DataHD && DataID::GetTimeProp(regID)==DataID::Now)//当前行度
				{
					task.FixDBS.set(t);
				}
			}
		}
		else if (task.FixMethod==DATA_FIX_METHOD_ALL)
		{
			task.FixDBS=task.DBS;
		}
		else if (task.FixMethod==DATA_FIX_METHOD_CUSTOM)
		{
			for(unsigned t = 0; t < cfg_size(cfgTask, "FixData"); t++)
			{
				oContext.RegIDS.SetBitSet(cfg_getnstr(cfgTask, "FixData", t),task.FixDBS);
			}
		}
		task.FixDBS.Stat();
		task.DataInfo.MeterCount=(INT16U)task.MBS.count();
		task.DataInfo.DataCount=(INT16U)task.DBS.count();
		task.DataInfo.ValFormat=CONS_TASK_FILE_DEFAULT_VAL_FORMAT;//TODO:
		task.DataInfo.ItemLen=1+RegData::GetValFormatLen(task.DataInfo.ValFormat);//标志位+数据
		task.DataPeriod=MAKE_DATA_PERIOD_VAL(cfg_getstr(cfgTask, "DataPeriod"));
		task.MaxRecords=(INT32U)cfg_getint(cfgTask, "MaxRecords");


		oContext.Tasks.TBS.set(taskNo);
	}
	cfg_free(cfg);
	oContext.Tasks.TBS.Stat();
	return 0;
}

int TaskCfg::SaveTasks(Context& oContext,const char* name)
{
	Tasks& TaskCls=oContext.Tasks;
	TaskCls.Item[SET_TIME_TASK].Enable=false;
	string buff;
	string sStr;
	buff.reserve(2000);
	sStr.reserve(500);

	for(size_t i=0;i<TaskCls.TBS.size();i++)
	{
		if (TaskCls.TBS.test(i))
		{
			char cTemp[200];
			sprintf(cTemp,"%s",TaskCls.Item[i].Enable==1?"true":"false");
			buff.append("Task\n{\n\tEnable=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%d",TaskCls.Item[i].Priority);
			buff.append("\n\tPriority=").append(cTemp);
			sprintf(cTemp,"%d",TaskCls.Item[i].TaskNo);
			buff.append("\n\tTaskNo=").append(cTemp);
			sprintf(cTemp,"%d",TaskCls.Item[i].MaxTimes);
			buff.append("\n\tMaxTimes=").append(cTemp);
			sprintf(cTemp,"%s",TaskCls.Item[i].IdleEnable==1?"true":"false");
			buff.append("\n\tIdleEnable=\"").append(cTemp).append("\"");
			sprintf(cTemp,"%d",TaskCls.Item[i].FreezeTimer.StartUnit);
			buff.append("\n\tFreezeTimer.StartUnit=").append(cTemp);
			sprintf(cTemp,"%d",TaskCls.Item[i].FreezeTimer.StartVal);
			buff.append("\n\tFreezeTimer.StartVal=").append(cTemp);
			sprintf(cTemp,"%d",TaskCls.Item[i].FreezeTimer.IntervalUnit);
			buff.append("\n\tFreezeTimer.IntervalUnit=").append(cTemp);
			sprintf(cTemp,"%d",TaskCls.Item[i].FreezeTimer.IntervalVal);
			buff.append("\n\tFreezeTimer.IntervalVal=").append(cTemp);

			sStr.erase();
			TaskCls.Item[i].GatherTimer.MinuteBS.ToString(sStr,0,59);
			buff.append("\n\tGatherTimer.Minute={").append(sStr).append("}");
			sStr.erase();
			TaskCls.Item[i].GatherTimer.HourBS.ToString(sStr,0,23);
			buff.append("\n\tGatherTimer.Hour={").append(sStr).append("}");
			sStr.erase();
			TaskCls.Item[i].GatherTimer.DayBS.ToString(sStr,1,31);
			buff.append("\n\tGatherTimer.Day={").append(sStr).append("}");
			sStr.erase();
			TaskCls.Item[i].GatherTimer.MonthBS.ToString(sStr,1,12);
			buff.append("\n\tGatherTimer.Month={").append(sStr).append("}");

			sprintf(cTemp,"%d",TaskCls.Item[i].FreezeDelayUnit);
			buff.append("\n\tFreezeDelayUnit=").append(cTemp);
			sprintf(cTemp,"%d",TaskCls.Item[i].FreezeDelayVal);
			buff.append("\n\tFreezeDelayVal=").append(cTemp);

			/*sStr.erase();
			TaskCls.Item[i].FixDelayTimer.MinuteBS.ToString(sStr,0,59);
			buff.append("\n\tFixDelayTimer.Minute={").append(sStr).append("}");
			sStr.erase();
			TaskCls.Item[i].FixDelayTimer.HourBS.ToString(sStr,0,23);
			buff.append("\n\tFixDelayTimer.Hour={").append(sStr).append("}");
			sStr.erase();
			TaskCls.Item[i].FixDelayTimer.DayBS.ToString(sStr,1,31);
			buff.append("\n\tFixDelayTimer.Day={").append(sStr).append("}");
			sStr.erase();
			TaskCls.Item[i].FixDelayTimer.MonthBS.ToString(sStr,1,12);
			buff.append("\n\tFixDelayTimer.Month={").append(sStr).append("}");*/


			sprintf(cTemp,"%d",TaskCls.Item[i].FixDelayUnit);
			buff.append("\n\tFixDelayUnit=").append(cTemp);
			sprintf(cTemp,"%d",TaskCls.Item[i].FixDelayVal);
			buff.append("\n\tFixDelayVal=").append(cTemp);

			MAKE_DATA_FIX_METHOD_KEY(TaskCls.Item[i].FixMethod,cTemp);
			buff.append("\n\tFixMethod=\"").append(cTemp).append("\"");
			if (TaskCls.Item[i].FixMethod==DATA_FIX_METHOD_CUSTOM)
			{
				sStr.erase();
				oContext.RegIDS.GetItemsString(sStr,TaskCls.Item[i].FixDBS,8,2);
				buff.append("\n\tFixData=\n\t{\n").append(sStr).append("\n\t}");
			}

			sStr.erase();
			TaskCls.Item[i].MBS.ToString(sStr);
			buff.append("\n\tMeters={").append(sStr).append("}");
			MAKE_DATA_TYPE_KEY(TaskCls.Item[i].DataType,cTemp);
			buff.append("\n\tDataType=\"").append(cTemp).append("\"");
			MAKE_DATA_PERIOD_KEY(TaskCls.Item[i].DataPeriod,cTemp);
			buff.append("\n\tDataPeriod=\"").append(cTemp).append("\"");
			sStr.erase();
			if (IsActionDataType(TaskCls.Item[i].DataType)==false)
			{
				oContext.RegIDS.GetItemsString(sStr,TaskCls.Item[i].DBS,8,2);
			}
			else
			{
				oContext.Actions.GetItemsString(sStr,TaskCls.Item[i].DBS,8,2);
			}
			
			buff.append("\n\tData=\n\t{\n").append(sStr).append("\n\t}");

			sprintf(cTemp,"%d",TaskCls.Item[i].MaxRecords);
			buff.append("\n\tMaxRecords=").append(cTemp);
			buff.append("\n}\n\n");
		}
	}
	int iFlag=CfgBase::SaveSetFile(buff,name,oContext.SysConfig.GetUserCode(),oContext.SysConfig.GetPlanKey());
	TaskCls.OnChanged();
	oContext.RefreshGatherInfo();
	oContext.SendSigNotifyToDaemon(Server::ParamChangedTag);
	return iFlag;
}
