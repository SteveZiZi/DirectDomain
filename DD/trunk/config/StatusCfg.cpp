#include "StatusCfg.h"

#include "../share/Status.h"
#include "../share/Context.h"
#include "../share/DataID.h"
#include "../share/MeterRule.h"
#include "../share/DeviceRule.h"
#include "../share/RegItem.h"
#include "../share/RegFile.h"
#include "../drule/DeviceRuleFactory.h"

#include "ContextCfg.h"

bool StatusCfg::m_bNeedSaveStatus=false;
StatusCookie StatusCfg::m_Cookie;

int StatusCfg::OnSetPowerSource(INT8U newVal)
{
	if (newVal>POWER_SOURCE_MAX_VAL)
	{
		return -1;
	}
	/*INT8U& val=status.DeviceStatus.PowerSource;*/
	INT8U val=0;
	if (val==newVal)
	{
		return 1;
	}
	OnStatusChanged();
	val=newVal;
	OnNotify(val==POWER_SOURCE_NORMAL?NOTIFY_TYPE_DEVICE_POWEROFF_RESUME:NOTIFY_TYPE_DEVICE_POWEROFF_HAPPEN,0,NULL);
	return 0;
}
int StatusCfg::SetMeterFlag(INT16U meterNo,INT8U& flag,INT8U newVal,INT32S happenNotify,INT32S resumeNotify)
{
	if (flag==newVal)
	{
		return 1;
	}
	OnStatusChanged();
	flag=newVal;
	OnNotify(flag==0?resumeNotify:happenNotify,meterNo,NULL);
	return 0;
}

//TODO:根据事件的配置在外层进行筛选
int StatusCfg::OnNotify(INT32U iiNotifyID,INT32S iiObjID,void* ipOther)
{
	KL_INFO("OnNotify(%08X,%d) Called\n",iiNotifyID,iiObjID);
	int result=0;
	DeviceRuleFactory& factory=DeviceRuleFactory::Instance();
	for(INT32U i=0;i<factory.Count();i++)
	{
		DeviceRuleItem* pItem=factory.Item(i);
		if (pItem)
		{
			if (pItem->Rule->OnNotify(iiNotifyID,iiObjID,NULL)!=0)
			{
				KL_WARN("Failed OnNofity(%08X,%d,%p) for Rule %s\n",iiNotifyID,iiObjID,ipOther,pItem->Rule->GetDeviceRuleInfo()->GetKey());
				result=-1;
			}
		}
	}
	return result;
}

void StatusCfg::SetRegHook()
{
	RegUpdateIntercept& intercept=RegItemShareFile::GetIntercept();
	intercept.BeforeUpdate=BeforeUpdateReg;
	intercept.AfterUpdate=AfterUpdateReg;
	intercept.BeforeWrite=NULL;
	intercept.AfterWrite=AfterWriteReg;
}
void StatusCfg::BeforeUpdateReg(unsigned short isObjID)
{
	m_Cookie.Reset();
}
void StatusCfg::AfterUpdateReg(unsigned short isObjID)
{
	if (m_Cookie.MeterProgramCount>0)
	{
		OnNotify(NOTIFY_TYPE_METER_PROGRAM,isObjID,NULL);
	}
	if (m_Cookie.MeterTimeRegCount>0)//电表日期或时间被采集过，需要判断电表时钟异常
	{
		MeterTimeCheck(isObjID);
	}
}

//通过检查电表日期以及时间能量寄存器，并与能量寄存器的采集时间进行比较，进行电表时钟异常判断
void StatusCfg::MeterTimeCheck(unsigned short isObjID)
{
	RegItem regItem;
	char meterDateTime[7];
	char deviceDateTime[7];
	DECLARE_CONTEXT(context);
	DECLARE_REG_FILE(regFile);
	if (regFile.Read(isObjID,0x1071C010,context.RegIDS ,regItem)!=0)
	{
		return;
	}
	if (regItem.Data.GetTime(meterDateTime)!=0)
	{
		return;
	}
	memcpy(deviceDateTime,regItem.Time,sizeof(deviceDateTime));

	if (regFile.Read(isObjID,0x1071C011,context.RegIDS ,regItem)!=0)
	{
		return;
	}
	char temp[7];
	if (regItem.Data.GetTime(temp)!=0)
	{
		return;
	}
	memcpy(meterDateTime+4,temp+4,3);
	memcpy(deviceDateTime+4,regItem.Time+4,3);

	if (IsBcd7Time((unsigned char*)meterDateTime)==false)
	{
		return;
	}
	if (IsBcd7Time((unsigned char*)deviceDateTime)==false)
	{
		return;
	}
	time_t meterT;
	time_t deviceT;
	Time_BCDTime7TotTime(meterDateTime,&meterT);
	Time_BCDTime7TotTime(deviceDateTime,&deviceT);
	//time_t trueDiff=(meterT>deviceT)?(meterT-deviceT):(deviceT-meterT) ;
	//KL_VERBOSE_DUMP(dump);
	//dump<<"Meter "<<isObjID<<" time unnormal check:MeterTime="<<meterT<<",DeviceTime="<<deviceT<<",truediff is "<<(INT32U)trueDiff <<",maxdiff is " <<(INT32U)maxDiff <<"\n";
	//DECLARE_STATUS(status);
	//SetMeterFlag(isObjID,status.MeterStatus[isObjID].TimeOverDiff ,(trueDiff>maxDiff)?1:0,NOTIFY_TYPE_METER_TIMEUNNORMAL_HAPPEN,NOTIFY_TYPE_METER_TIMEUNNORMAL_RESUME);//电表时钟异常
}

void StatusCfg::AfterWriteReg(const RegItem& irRegItem,const RegItem& irOldRegItem)
{
	//DECLARE_CONTEXT(context);
	//DECLARE_STATUS(status);
	INT32U regID=irRegItem.RegID;
	//if (regID==0x1071C021)//电网状态字
	//{
	//	BitSet8 bs(irRegItem.Data.Data[0]);
	//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].ACutVol,bs.test(0)?1:0,NOTIFY_TYPE_METER_ACUTVOL_HAPPEN,NOTIFY_TYPE_METER_ACUTVOL_RESUME);//A相断相
	//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].BCutVol,bs.test(1)?1:0,NOTIFY_TYPE_METER_BCUTVOL_HAPPEN,NOTIFY_TYPE_METER_BCUTVOL_RESUME);//B相断相
	//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].CCutVol,bs.test(2)?1:0,NOTIFY_TYPE_METER_CCUTVOL_HAPPEN,NOTIFY_TYPE_METER_CCUTVOL_RESUME);//C相断相

	//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].CutVol,status.MeterStatus[irRegItem.ObjID].IsCutVol()?1:0,NOTIFY_TYPE_METER_CUTVOL_HAPPEN,NOTIFY_TYPE_METER_CUTVOL_RESUME);//断相
	//	
	//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].AOverVol,bs.test(4)?1:0,NOTIFY_TYPE_METER_AOVERVOL_HAPPEN,NOTIFY_TYPE_METER_AOVERVOL_RESUME);//A相过压
	//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].BOverVol,bs.test(5)?1:0,NOTIFY_TYPE_METER_BOVERVOL_HAPPEN,NOTIFY_TYPE_METER_BOVERVOL_RESUME);//B相过压
	//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].COverVol,bs.test(6)?1:0,NOTIFY_TYPE_METER_COVERVOL_HAPPEN,NOTIFY_TYPE_METER_COVERVOL_RESUME);//C相过压

	//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].OverVol,status.MeterStatus[irRegItem.ObjID].IsOverVol()?1:0,NOTIFY_TYPE_METER_OVERVOL_HAPPEN,NOTIFY_TYPE_METER_OVERVOL_RESUME);//过压

	//}
	//else if (regID==0x1071C020)//电表状态字
	//{
	//	BitSet8 bs(irRegItem.Data.Data[0]);
	//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].BatteryLowVol,bs.test(2)?1:0,NOTIFY_TYPE_METER_BATTERYLOWVOL_HAPPEN,NOTIFY_TYPE_METER_BATTERYLOWVOL_RESUME);//电池欠压
	//}
	//else 
	if(regID==0x1071B210 || regID==0x1071B212)//最近一次编程时间或者编程次数
	{
		if (irRegItem.Data.IsValid()==true && irOldRegItem.Data.IsValid()==true)
		{
			if (memcmp(irRegItem.Data.Data,irOldRegItem.Data.Data,sizeof(irRegItem.Data.Data))!=0)
			{
				m_Cookie.MeterProgramCount++;
			}
		}
	}
	else if(regID==0x1071C010 || regID==0x1071C011)//日期或者时间
	{
		m_Cookie.MeterTimeRegCount++;
	}
	else if(regID==0x10050000)//总功率因数
	{
		//double val;
		//if (irRegItem.Data.GetVal(&val)==0)
		//{
		//	double diff=context.EventCfg.MeterZGlysMinVal-fabs(val);
		//	bool bHappen=(diff>CONS_FLOAT_PART_DIFF);
		//	SetMeterFlag(irRegItem.ObjID,status.MeterStatus[irRegItem.ObjID].ZGlyxLower,bHappen?1:0,NOTIFY_TYPE_METER_ZGLYX_LOWER_HAPPEN,NOTIFY_TYPE_METER_ZGLYX_LOWER_RESUME);//总功率因素低于限值
		//}
	}
}


