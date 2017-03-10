#include "nortask.h"
#include "dataItem.h"
#include "meter.h"
#include "context.h"
#include "RtfCtrl.h"
#include "AlarmFile.h"
#include "status.h"
#include "Consluxtask.h"
#include "RtfCtrl.h"
#include "ipcStatus.h"

bool SConsLuxTask::IsValidTime()
{
	time_t tt;
	time(&tt);
	if ((tt>= m_StartTime) && (tt<=m_EndTime) )
	{
		//return true;
	}
	else
	{
		return false;
	}

	
	struct tm *curtime;

	curtime=localtime(&tt);

	switch(m_PeriodUnit)
	{
		case EM_PERIODUNIT_DAY:
			break;

		case EM_PERIODUNIT_WEEK:
			if (m_PeriodDetail.m_WeekBS.test(curtime->tm_wday) == true)
			{
				int i=0;
				for (i; i<CONS_TIME_PERIOD_IN_DAY_MAX_COUNT; i++)
				{
					int curMins = curtime->tm_hour*60 + curtime->tm_min;
					int beginMins = bcdc(m_TimePeriodInDay[i][0])*60 + bcdc(m_TimePeriodInDay[i][1]);
					int endMins = bcdc(m_TimePeriodInDay[i][2])*60 + bcdc(m_TimePeriodInDay[i][3]);
					KL_VERBOSE("beginMins=%d,curMins=%d,endMins=%d\n",beginMins,curMins,endMins);
					if ( curMins>= beginMins && curMins <  endMins)
					{
						if ((m_TimePeriodInDay[i][0] == m_TimePeriodInDay[i][2]) && (m_TimePeriodInDay[i][1] == m_TimePeriodInDay[i][3]))
						{
							continue;
						}
						else
						{
							return true;
						}
					}
				}
				return false;	
			}
			else
			{
				return false;
			}

 		case EM_PERIODUNIT_MONTH:
			break;

		default:
			break;
	}
	return false;
}
/*brief:set task schedule function,will decide whice task will be excuted
	param [in] void
	param [out]None 
	return void
	author:Wanqingsong
	date: 2012-02-25
*/
bool SConsLuxTask::ConsLuxTaskSchedule(void)
{
	static bool clOff=false;
	static bool bstart = true;
	if (IsValidTime() == false)
	{
		if (m_ClearClOffFlag==true && clOff == true)
		{	
			clOff = false;	
			m_ClearClOffFlag = false;
		}
		
		if (clOff==false)
		{
			CRtfCtrl clCtl;
			SSmartGarageItem command;
			command.m_DeviceNo = m_ClrelayNo;
			clCtl.RtfClOffCtl(command);
			clOff = true;
			KL_VERBOSE("恒照时效无效，关闭回路\n");
		}
		return false;
	}

	if (clOff == true || bstart==true)
	{
		CRtfCtrl clCtl;
		SSmartGarageItem command;
		command.m_DeviceNo = m_ClrelayNo;
		clCtl.RtfClOnCtl(command);
		bstart=false;
	}

	clOff = false;
	
	static INT32U bak_lux=0;
	INT32U lux = LuxCalculate();

	if (lux == bak_lux)//如果调光后lux值没变表示整流器电压还没有调光完成，待电压调成功后再继续调光
		return true;

	CRtfCtrl clCtl;
	INT16U clVol;
	INT16U clAdjustedVol;

	if (clCtl.GetRtfClVol(m_ClrelayNo, clVol) == false)
		return false;

	
	INT16U adjustvol = GetStrategyVol(lux);

	if (adjustvol == 0)//lux差值在不需要调光范围
		return true;

	KL_VERBOSE("回路电压为%x，调整电压值为%x\n",clVol,adjustvol);
	int ret=-1;
	if (lux < m_ConstantLux)
	{
		KL_VERBOSE("电压上调\n");
		ret = IncreaseClVol(clVol,adjustvol,clAdjustedVol);
	}

	if (lux > m_ConstantLux)
	{
		KL_VERBOSE("电压下调\n");
		ret = DecreaseClVol(clVol,adjustvol,clAdjustedVol);
	}

	if (ret == 0)
	{
		KL_VERBOSE("恒照调压后的电压为%x\n",clAdjustedVol);
		STATUS.deviceStatus.m_ConstantLuxTaskStatus.AdjustedClVol = clAdjustedVol;
		bak_lux = lux;
	}


	return true;
}


INT16U SConsLuxTask::GetStrategyVol(INT32U curLux)
{

	INT32U luxdiff = abs((int)(curLux - m_ConstantLux));

	for (int i=0; i<CONS_STRATEGY_PLANS_MAX; i++)
	{
		if (m_Strategys[i].m_bValid == true)
		{
			KL_VERBOSE("luxdiff=%d,m_Strategys[%d].m_luxdiff=%d，m_m_stepvol=%x\n",luxdiff,i,m_Strategys[i].m_luxdiff,m_Strategys[i].m_stepvol);
			if (luxdiff <= m_Strategys[i].m_luxdiff)
				return m_Strategys[i].m_stepvol;

			if (i == CONS_STRATEGY_PLANS_MAX-1)//已遍历到最大项的最后一个，则直接返回最后一个项的值
				return m_Strategys[i].m_stepvol;
		}

		if (i>0 && m_Strategys[i].m_bValid == false)//无效项表示有效项已遍历完，返回最后一个有效项的值
			return m_Strategys[i-1].m_stepvol;
	}
	return 0;
}

	
INT32U SConsLuxTask::LuxCalculate(void)
{
	INT32U taskLuxValue=0;
	Device tempdev;

	for (DevicesBitSet::SizeType i =  m_LuxSensorsDBS.FromPos; i<m_LuxSensorsDBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true || m_LuxSensorsDBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 taskLuxValue +=  (tempdev.m_LuxValue * IPCSTATUS.m_DeviceCfgData[tempdev.m_Tn].m_LuxRate)/100;
//		 taskLuxValue +=  tempdev.m_LuxValue;
		 KL_VERBOSE("SConsLuxTask::LuxCalculate:TN = %d,Luxvalue=%d,LuxRate=%d\n",i,tempdev.m_LuxValue,IPCSTATUS.m_DeviceCfgData[tempdev.m_Tn].m_LuxRate);
	}
	INT32U callux = (INT32U)(taskLuxValue/m_LuxSensorsDBS.count());
	KL_VERBOSE("taskLuxValueTotal=%d,Sensors num=%d,LuxCalculate=%d\n",taskLuxValue,m_LuxSensorsDBS.count(),callux);
	return callux;
}
int SConsLuxTask::IncreaseClVol(INT16U vol,INT16U stepvol,INT16U& oVol)
{
	INT16U tmpVol;
	CRtfCtrl clCtl;
	INT16U cur;
	SSmartGarageItem SmartGarageItem;
	SmartGarageItem.m_DeviceNo = m_ClrelayNo;
	cur = 0;
	tmpVol = D2B(B2D(vol) + B2D(stepvol));
	oVol = tmpVol;
	return clCtl.RtfClVolCurSetCtl(SmartGarageItem, tmpVol, cur);
}

int SConsLuxTask::DecreaseClVol(INT16U vol,INT16U stepvol,INT16U& oVol)
{
	INT16U tmpVol;
	CRtfCtrl clCtl;
	INT16U cur;
	SSmartGarageItem SmartGarageItem;
	SmartGarageItem.m_DeviceNo = m_ClrelayNo;
	cur = 0;
	tmpVol = D2B(abs(B2D(vol) - B2D(stepvol)));
	oVol = tmpVol;	
	return clCtl.RtfClVolCurSetCtl(SmartGarageItem, tmpVol, cur);
}

