#include "ContextCfg.h"

#include "SysConfigCfg.h"
#include "ServerCfg.h"
#include "MeterCfg.h"
#include "deviceCfg.h"
#include "RelayCfg.h"
#include "ClRelayCfg.h"
//#include "ZBSnifferTaskCfg.h"
#include "TaskCfg.h"
#include "nortaskcfg.h"
#include "luxtaskcfg.h"
#include "ConsluxtaskCfg.h"
//#include "RuleCfg.h"
#include "ActionCfg.h"
#include "JilianSlavesCfg.h"
#include "FixDataCfg.h"
#include "../share/SysIpc.h"
#include "../share/RegItem.h"
#include "../share/MeterRule.h"
#include "../share/DeviceRule.h"
#include "../drule/DeviceRuleFactory.h"
#include "../share/Context.h"
#include "../base/FileBase.h"
#include "localIpCfg.h"
#include "DnProtocolTpyeCfg.h"
#include "TerminalInfoCfg.h"
#include "../share/ipcStatus.h"

int ContextCfg::BaseInit(DirScan& dirScan)
{
	StatusCfg::SetRegHook();
	return 0;
}

int ContextCfg::InitContext(DirScan& dirScan,Context& oContext)
{
	
	oContext.JilianSlaves.init();
	oContext.FixData.init();
	oContext.ServerInfos.BaseInit(dirScan);
	oContext.ServerInfos.Init();
	oContext.ResetInfo.Init();
	oContext.httx_sendBuffer.Init();
	oContext.debug_buffer.Init();
	oContext.FtsDatas.Init();
	oContext.gprsInfo.Init();
	oContext.m_DevicesRegData.Init();
	//printf("\n--------%d ",oContext.httx_sendBuffer.aim);
	SysIpc::ClearIpc();

	//SysConfigCfg::LoadConfig(oContext);
	//SysConfigCfg::LoadGprsInfo(oContext);
	//ServerCfg::LoadDbcjInfo(oContext);
	//ServerCfg::LoadHttxInfo(oContext);
	//ServerCfg::LoadPPPInfo(oContext);
	//ServerCfg::LoadJilianInfo(oContext);

	oContext.MeterRuleInfos.Init();

	//2009-03-13 陈桂华加
	oContext.BusInfos.Init();
	oContext.DeviceRuleInfos.Init();

	CONTEXT.m_DataItems.Init();

	time(&oContext.ContextInitTime);
	return 0;
}

int ContextCfg::LoadContext(Context& oContext)
{
	oContext.Actions.Init();
	oContext.RegIDS.Init();

	size_t count=0;

	SysConfigCfg::LoadConfig(oContext);
	TerminalInfoCfg::LoadTerminalConfig(oContext);
	SysConfigCfg::LoadPortMap(oContext);
	SysConfigCfg::LoadGprsInfo(oContext);
	ServerCfg::LoadDbcjInfo(oContext);
	ServerCfg::LoadHttxInfo(oContext);
	ServerCfg::LoadPPPInfo(oContext);
	ServerCfg::LoadJilianInfo(oContext);
 

	oContext.Meters.Init();
	oContext.Relays.Init();
	oContext.Tasks.Init();
	oContext.HttxCfgPlans.Init();
	oContext.ZBSnifferTask.Init();
	oContext.debug_buffer.Init();
	oContext.Devices.Init();
	oContext.ClRelays.Init();
	int result=0;
	
 	DeviceRuleFactory::Instance().OnSysInit(oContext);
	
	ServerCfg::LoadHttxPlanInfo(oContext);
	result+=ActionCfg::LoadActions(oContext);//if (result!=0) KL_INFO("载入错误1\n");

	result+=ServerCfg::LoadTraceInfo(oContext);//if (result!=0) KL_INFO("载入错误3\n");
	result+=TaskCfg::LoadDataIDS(oContext);//if (result!=0) KL_INFO("载入错误4\n");
	result+=MeterCfg::LoadMeters(oContext);//if (result!=0) KL_INFO("载入错误5\n");
	result+=TaskCfg::LoadTasks(oContext);//if (result!=0) KL_INFO("载入错误8\n");

	DeviceCfg::LoadDevices(oContext);
	CLRelayCfg::LoadClRelays(oContext);
	NorTaskCfg::LoadNorTasks(oContext);//if (result!=0) KL_INFO("载入错误8\n");
	LuxTaskCfg::LoadLuxTasks(oContext);
	ConsLuxTaskCfg::LoadConsLuxTasks(oContext);	
	
	GDBusRuleFactory::Instance().OnSysInit(oContext);
	oContext.RefreshGatherInfo();


//	AdjustMeters(oContext);
//	ClearMeterReg(oContext);
	if (result!=0)
	{
		KL_ERROR("LoadContext error\n");
	}
	
	//SetLocalIp(oContext);
	ServerCfg::LoadTermIPConfig(oContext);
	DnProtocolTpye::LoadDnProtocolTpye(oContext);

	IPCSTATUS;
	return (result==0?0:-1);
}

void ContextCfg::SetLocalIp(Context& oContext)
{
	string setIpStr;

	LocalIpCfg::LoadLocalIp(oContext);
	system("ifconfig eth0 up");
	setIpStr.append("ifconfig eth0 ").append(oContext.LocalIp); 
	system(setIpStr.c_str());
}

void ContextCfg::AdjustMeters(Context& oContext)
{

}
//如果电表未指明用户名和密码，则根据电表规约给出默认的用户名和密码
void ContextCfg::AdjustMeter(Meter& meter,MeterRule& rule)
{
	MeterRuleInfo* pRuleInfo=rule.GetMeterRuleInfo();
	ASSERT(pRuleInfo);
	if (pRuleInfo)
	{
		if (strlen(meter.User)==0 && strlen(pRuleInfo->DefUser)>0)
		{
			ASSERT(sizeof(meter.User)==sizeof(pRuleInfo->DefUser));
			memcpy(meter.User,pRuleInfo->DefUser,sizeof(meter.User));
		}
		if (strlen(meter.Pwd)==0 && strlen(pRuleInfo->DefPwd)>0)
		{
			ASSERT(sizeof(meter.Pwd)==sizeof(pRuleInfo->DefPwd));
			memcpy(meter.Pwd,pRuleInfo->DefPwd,sizeof(meter.Pwd));
		}
		if (meter.CommRetryTimes<0)
		{
			meter.CommRetryTimes=pRuleInfo->DefCommRetryTimes;
		}
		meter.PortSet.PartMerge(pRuleInfo->DefPortSet);
	}
}
void ContextCfg::ClearMeterReg(Context& oContext)
{
	MeterBitSet& validMBS=oContext.Meters.ValidMBS;

	for(MeterBitSet::SizeType m=0;m<validMBS.size();m++)
	{
		if (validMBS.test(m)==false)
		{
			RegItems::DeleteMeterRegs(m);//删除无效电表对应的能量寄存器文件
			continue;
		}
		Meter& meter=oContext.Meters.Item[m];
		DataBitSet clearDBS=meter.RegDBS;
		clearDBS.flip();
		RegItems::ClearMeterRegs(m,clearDBS);
	}
}

int ContextCfg::MemReset(Context& oContext)//内存复位，成功返回0
{
	int result= ExecScript(oContext,NAME_SCRIPT_MEM_RESET);
	if (result==0)
	{
		StatusCfg::OnMemReset();
	}
	KL_WARN("Mem Reset succeed\n");
	return result;
}

int ContextCfg::DataReset(Context& oContext)//数据区复位，成功返回0
{
	int result= ExecScript(oContext,NAME_SCRIPT_DATA_RESET);
	if (result==0)
	{
		StatusCfg::OnDataReset();
	}
	KL_WARN("Data Reset succeed\n");
	return result;
}
int ContextCfg::ClearLogs(Context& oContext)
{
	int result = ExecScript(oContext,NAME_SCRIPT_CLEAR_LOGS);
	KL_WARN("Logs has cleared\n");
	return result;
}
int ContextCfg::FactoryReset(Context& oContext)
{
	int result = ExecScript(oContext,NAME_SCRIPT_FACTORY_RESET);
	KL_WARN("Resuming to factory settings\n");
	return result;
}
int ContextCfg::ParamReset(Context& oContext)//参数区复位，成功返回0
{
	int result= ExecScript(oContext,NAME_SCRIPT_PARAM_RESET);
	if (result==0)
	{
		StatusCfg::OnParamReset();
	}
	KL_WARN("Param Reset succeed\n");
	return result;
}
int ContextCfg::ParamChanged(Context& oContext)//参数改变
{
	StatusCfg::OnParamChanged();
	KL_WARN("Param Changed \n");
	return 0;
}

int ContextCfg::TimeChanged(Context& oContext)//时间改变
{
	StatusCfg::OnTimeChanged();
	KL_WARN("Time Changed \n");
	return 0;
}
int ContextCfg::DataFileRebuilded(Context& oContext)
{
	StatusCfg::OnDataFileRebuilded();
	KL_WARN("DataFile Rebuilded\n");
	return 0;
}

int ContextCfg::SoftwareReset(Context& oContext)//软件复位（DYJC软件重启，操作系统不重启）
{
	StatusCfg::OnSoftwareReset();
	ContextCfg::ExecScript(oContext, NAME_SCRIPT_SOFT_RESET);
	KL_WARN("Send software reset request succeed\n");
	oContext.ResetInfo.SoftwareResetFlag=1;
	return 0;
}
int ContextCfg::HardwareReset(Context& oContext)//硬件复位（操作系统由看门狗重启）
{
	StatusCfg::OnHardwareReset();
	KL_WARN("Send hardware reset request succeed\n");
	oContext.ResetInfo.HardwareResetFlag=1;
	KLSystem("reboot");
	return 0;
}
int ContextCfg::SetPowerSource(Context& oContext,INT8U newVal)
{
	KL_WARN_DUMP(dump);
	dump<<"Set Power Source to "<<(POWER_SOURCE)newVal<<"\n";
	return StatusCfg::OnSetPowerSource(newVal);
}
INT8U ContextCfg::GetMeterSetTimeTrueMethod(Context& oContext,INT16U meterNo,INT8U settingMethod)
{
	return 0;
}

int ContextCfg::Eth0IpConfig(Context& oContext)
{
	ContextCfg::ExecScript(oContext, NAME_SCRIPT_ETH0_IPCONFIG);
	KL_WARN("设置Eth0网络参数 \n");
	return 0;
}
int ContextCfg::ExecScript(Context& context,const char* scriptName)
{
	char pathFile[CONS_PATH_FILE_MAX_LEN];
	if (FileBase::GetScriptFile(scriptName,context.SysConfig.GetUserCode(),context.SysConfig.GetPlanKey(),pathFile)==0)
	{
		#ifndef WIN32
		KLSystem(pathFile);
		#endif
	}
	return -1;
}


