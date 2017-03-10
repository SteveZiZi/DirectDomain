#include "nortask.h"
#include "dataItem.h"
#include "meter.h"
#include "context.h"
#include "RtfCtrl.h"
#include "AlarmFile.h"
#include "status.h"

bool SPlan::IsTimerOn(INT8U taskno,INT8U planno)
{
	time_t tt;
	struct tm *curtime;

	time(&tt);
	curtime=localtime(&tt);
	

//	if (curtime->tm_mday == m_curday)
	if (curtime->tm_mday == STATUS.deviceStatus.m_NortaskStatus[taskno].m_PlanStatus[planno].m_ActionedInDay)
	{
		KL_VERBOSE("方案当天已执行\n");
		return false;
	}

	KL_VERBOSE("当前时间(%x:%x),方案启动时间(%x:%x)\n",cbcd(curtime->tm_hour),cbcd(curtime->tm_min),m_BeginTimeHHMM[0],m_BeginTimeHHMM[1]);

	if (cbcd(curtime->tm_hour) == m_BeginTimeHHMM[0] && cbcd(curtime->tm_min) >=m_BeginTimeHHMM[1])
	{
		KL_VERBOSE("<<<<<<<<<<                    任务调度时间到        ?>>>>>>>>>>>>>>>>>>>\n");
		m_curday = curtime->tm_mday;
		STATUS.deviceStatus.m_NortaskStatus[taskno].m_PlanStatus[planno].m_ActionedInDay = m_curday;
		return true;
	}

	return false;
}
void SPlan::OnExec(INT8U taskno)
{
/*	if (m_Enable == false)
	{
		return ;
	}*/

	for(int i=0; i<m_ActionItemsNum; i++)
	{
		if (m_ActionItems[i].m_Function == FUNC_RELAY_ON)//继电器开
		{
			CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
			if (pDataItem == NULL)
			{
				return;
			}
			
			bstring tempData;
			tempData.clear();
			tempData.push_back(m_ActionItems[i].m_SubNo);//子路号
			tempData.push_back(2);//开
			
			int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[m_ActionItems[i].m_DeviceNo],tempData);
		}

		if (m_ActionItems[i].m_Function == FUNC_RELAY_OFF)//继电器关
		{
			CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
			if (pDataItem == NULL)
			{
				return;
			}
			
			bstring tempData;
			tempData.clear();
			tempData.push_back(m_ActionItems[i].m_SubNo);//继电器关
			tempData.push_back(3);//关
			
			int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[m_ActionItems[i].m_DeviceNo],tempData);
		}

		if (m_ActionItems[i].m_Function == FUNC_RTF_ON)//整流器开
		{
			CRtfCtrl rtfCtl;
			rtfCtl.RtfSwitchSetCtl(m_ActionItems[i], 0, 0);
		}

		if (m_ActionItems[i].m_Function == FUNC_RTF_OFF)//整流器关
		{
			CRtfCtrl rtfCtl;
			rtfCtl.RtfSwitchSetCtl(m_ActionItems[i], 1, 0);
		}

		if (m_ActionItems[i].m_Function == FUNC_RTF_VOL_SET)//整流器电压设置
		{
			INT16U vol = (INT16U)(m_ActionItems[i].m_FuncData[0]<<8 | m_ActionItems[i].m_FuncData[1]);
			INT16U cur = (INT16U)(m_ActionItems[i].m_FuncData[2]<<8 | m_ActionItems[i].m_FuncData[3]);
			KL_VERBOSE("Set Clrelay no %d Vol = %x Cur = %x\n",m_ActionItems[i].m_DeviceNo,vol,cur);
 			CRtfCtrl rtfCtl;
			rtfCtl.RtfSwitchSetCtl(m_ActionItems[i], vol,cur);
		}

		if (m_ActionItems[i].m_Function == FUNC_CL_ON)//回路整流器开
		{
			CRtfCtrl rtfCtl;
			time_t tNow=time(NULL);
			if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo])
			{
				KL_VERBOSE("回路%d处于手动设置有效期\n",m_ActionItems[i].m_DeviceNo);
				//手动控制无效后的动作
				STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[m_ActionItems[i].m_DeviceNo] = m_ActionItems[i];
				continue;
			}
			else
			{
				//已移植ManualCtlOutOfDateProcess进行复位
				//STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo] = 0;

			}
			rtfCtl.RtfClOnCtl(m_ActionItems[i]);

			{
				char bcdtime7[7];
				INT8U AlarmData[9];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 8;//告警内容长度
				AlarmData[1] = taskno;
				AlarmData[2] = m_ActionItems[i].m_DeviceNo;//回路号
				AlarmData[3] = 0x01;

				INT16U hhmm =  CONTEXT.NorTasks.m_ClrelayTasks[m_ActionItems[i].m_DeviceNo].GetNextPlanStartTime();				
				AlarmData[4] = 0x00;//年
				AlarmData[5] = 0x00;//月
				AlarmData[6] = 0x00;//日
				AlarmData[7] = (INT8U)(hhmm>>8&0xff);//时
				AlarmData[8] = (INT8U)(hhmm&0xff);//分
				
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_ONOFF_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}
		}

		if (m_ActionItems[i].m_Function == FUNC_CL_OFF)//回路整流器关
		{
			CRtfCtrl rtfCtl;
			time_t tNow=time(NULL);
			if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo])
			{
				KL_VERBOSE("回路%d处于手动设置有效期\n",m_ActionItems[i].m_DeviceNo);
				//手动控制无效后的动作
				STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[m_ActionItems[i].m_DeviceNo] = m_ActionItems[i];

				continue;
			}
			else
			{
				//已移植ManualCtlOutOfDateProcess进行复位
				//STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo] = 0;
			}

			rtfCtl.RtfClOffCtl(m_ActionItems[i]);
			{
				char bcdtime7[7];
				INT8U AlarmData[9];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 8;//告警内容长度
				AlarmData[1] = taskno;
				AlarmData[2] = m_ActionItems[i].m_DeviceNo;//回路号
				AlarmData[3] = 0x00;
				
				INT16U hhmm =  CONTEXT.NorTasks.m_ClrelayTasks[m_ActionItems[i].m_DeviceNo].GetNextPlanStartTime();				
				AlarmData[4] = 0x00;//年
				AlarmData[5] = 0x00;//月
				AlarmData[6] = 0x00;//日
				AlarmData[7] = (INT8U)(hhmm>>8&0xff);//时
				AlarmData[8] = (INT8U)(hhmm&0xff);//分
				
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_ONOFF_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}
		}
		KL_VERBOSE("m_Function=%d\n",m_ActionItems[i].m_Function);
		if (m_ActionItems[i].m_Function == FUNC_CL_VOL_SET)//回路设置电压
		{
			time_t tNow=time(NULL);
			if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo])
			{
				KL_VERBOSE("回路%d处于手动设置有效期\n",m_ActionItems[i].m_DeviceNo);
				//手动控制无效后的动作
				STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[m_ActionItems[i].m_DeviceNo] = m_ActionItems[i];
				STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[m_ActionItems[i].m_DeviceNo].m_SubNo = taskno;//用子路号存储任务号 
				continue;
			}
			else
			{
				//已移植ManualCtlOutOfDateProcess进行复位
				//STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo] = 0;
			}

			CRtfCtrl rtfCtl;
			rtfCtl.RtfClOnCtl(m_ActionItems[i]);//先将回路整流器打开
					
			INT16U vol = (INT16U)(m_ActionItems[i].m_FuncData[0]<<8 | m_ActionItems[i].m_FuncData[1]);
			INT16U cur = (INT16U)(m_ActionItems[i].m_FuncData[2]<<8 | m_ActionItems[i].m_FuncData[3]);
			KL_VERBOSE("Set Clrelay no %d Vol = %x Cur = %x\n",m_ActionItems[i].m_DeviceNo,vol,cur);
 		
			rtfCtl.RtfClVolCurSetCtl(m_ActionItems[i], vol,cur);

			{
				char bcdtime7[7];
				INT8U AlarmData[10];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 9;//告警内容长度
				AlarmData[1] = taskno;
				if (taskno == 0)
				{
					KL_VERBOSE("1.非法任务号0\n");
					return ;
				}
				AlarmData[2] = m_ActionItems[i].m_DeviceNo;//回路号
				AlarmData[3] = (INT8U)(vol>>8&0xff);
				AlarmData[4] = (INT8U)(vol&0xff);

				INT16U hhmm =  CONTEXT.NorTasks.m_ClrelayTasks[m_ActionItems[i].m_DeviceNo].GetNextPlanStartTime();				
				AlarmData[5] = 0x00;//年
				AlarmData[6] = 0x00;//月
				AlarmData[7] = 0x00;//日
				AlarmData[8] = (INT8U)(hhmm>>8&0xff);//时
				AlarmData[9] = (INT8U)(hhmm&0xff);//分
				KL_VERBOSE("__________ALARM_TASK_CL_VOL_SET 2_________\n");
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_VOL_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}
		}

	}
	
}

void SPlan::OnExec(SSmartGarageItem & actionItem, INT8U alarmtype)
{

	if (actionItem.m_Function == FUNC_RELAY_ON)//继电器开
	{
		CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
		if (pDataItem == NULL)
		{
			return;
		}
		
		bstring tempData;
		tempData.clear();
		tempData.push_back(actionItem.m_SubNo);//子路号
		tempData.push_back(2);//开
		
		int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[actionItem.m_DeviceNo],tempData);
	}

	if (actionItem.m_Function == FUNC_RELAY_OFF)//继电器关
	{
		CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
		if (pDataItem == NULL)
		{
			return;
		}
		
		bstring tempData;
		tempData.clear();
		tempData.push_back(actionItem.m_SubNo);//继电器关
		tempData.push_back(3);//关
		
		int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[actionItem.m_DeviceNo],tempData);
	}

	if (actionItem.m_Function == FUNC_RTF_ON)//整流器开
	{
		CRtfCtrl rtfCtl;
		rtfCtl.RtfSwitchSetCtl(actionItem, 0, 0);
	}

	if (actionItem.m_Function == FUNC_RTF_OFF)//整流器关
	{
		CRtfCtrl rtfCtl;
		rtfCtl.RtfSwitchSetCtl(actionItem, 1, 0);
	}

	if (actionItem.m_Function == FUNC_RTF_VOL_SET)//整流器电压设置
	{
		INT16U vol = (INT16U)(actionItem.m_FuncData[0]<<8 | actionItem.m_FuncData[1]);
		INT16U cur = (INT16U)(actionItem.m_FuncData[2]<<8 | actionItem.m_FuncData[3]);
		KL_VERBOSE("Set Clrelay no %d Vol = %x Cur = %x\n",actionItem.m_DeviceNo,vol,cur);
		CRtfCtrl rtfCtl;
		rtfCtl.RtfSwitchSetCtl(actionItem, vol,cur);
	}

	if (actionItem.m_Function == FUNC_CL_ON)//回路整流器开
	{
		time_t tNow=time(NULL);
		if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[actionItem.m_DeviceNo])
		{
			KL_VERBOSE("回路%d处于手动设置有效期\n",actionItem.m_DeviceNo);
			//手动控制无效后的动作
			STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[actionItem.m_DeviceNo] = actionItem;

			return ;
		}
		
		CRtfCtrl rtfCtl;
		
		rtfCtl.RtfClOnCtl(actionItem);

		{
			char bcdtime7[7];
			INT8U AlarmData[4];
			Get_CurBCDTime7(bcdtime7);
			AlarmData[0] = 2;//告警内容长度
			AlarmData[1] = actionItem.m_DeviceNo;
			AlarmData[2] = 0x01;
/*			if (alarmtype == MANUAL_CTL)
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CL_CTRL,bcdtime7,CONS_UNVALID_METER_NUMBER);
			else
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_ONOFF_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);*/
		}
	}

	if (actionItem.m_Function == FUNC_CL_OFF)//回路整流器关
	{
		time_t tNow=time(NULL);
		if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[actionItem.m_DeviceNo])
		{
			KL_VERBOSE("回路%d处于手动设置有效期\n",actionItem.m_DeviceNo);
			//手动控制无效后的动作
			STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[actionItem.m_DeviceNo] = actionItem;

			return ;
		}
		
		CRtfCtrl rtfCtl;
		
		rtfCtl.RtfClOffCtl(actionItem);
		{
			char bcdtime7[7];
			INT8U AlarmData[4];
			Get_CurBCDTime7(bcdtime7);
			AlarmData[0] = 2;//告警内容长度
			AlarmData[1] = actionItem.m_DeviceNo;
			AlarmData[2] = 0x00;

/*			if (alarmtype == MANUAL_CTL)
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CL_CTRL,bcdtime7,CONS_UNVALID_METER_NUMBER);
			else
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_ONOFF_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);*/
		}
	}

	if (actionItem.m_Function == FUNC_CL_VOL_SET)//回路设置电压
	{
		time_t tNow=time(NULL);
		if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[actionItem.m_DeviceNo])
		{
			KL_VERBOSE("回路%d处于手动设置有效期\n",actionItem.m_DeviceNo);
			//手动控制无效后的动作
			STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[actionItem.m_DeviceNo] = actionItem;

			return ;
		}
			
		INT16U vol = (INT16U)(actionItem.m_FuncData[0]<<8 | actionItem.m_FuncData[1]);
		INT16U cur = (INT16U)(actionItem.m_FuncData[2]<<8 | actionItem.m_FuncData[3]);
		KL_VERBOSE("Set Clrelay no %d Vol = %x Cur = %x\n",actionItem.m_DeviceNo,vol,cur);
		CRtfCtrl rtfCtl;
		rtfCtl.RtfClVolCurSetCtl(actionItem, vol,cur);

		{
			if (alarmtype == TASK_CTL)//由于手动操作有效时间推迟了任务执行，待有效时间结束后再执行任务
			{
				char bcdtime7[7];
				INT8U AlarmData[10];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 9;//告警内容长度

				if (actionItem.m_SubNo == 0)
				{
					KL_VERBOSE("2 actionItem.m_SubNo == 0 \n");
					return;
				}

				AlarmData[1] = actionItem.m_SubNo;//这个子路号存放的是任务号
				AlarmData[2] = actionItem.m_DeviceNo;//回路号
				AlarmData[3] = (INT8U)(vol>>8&0xff);
				AlarmData[4] = (INT8U)(vol&0xff);

				INT16U hhmm =  CONTEXT.NorTasks.m_ClrelayTasks[actionItem.m_DeviceNo].GetNextPlanStartTime();				
				AlarmData[5] = 0x00;//年
				AlarmData[6] = 0x00;//月
				AlarmData[7] = 0x00;//日
				AlarmData[8] = (INT8U)(hhmm>>8&0xff);//时
				AlarmData[9] = (INT8U)(hhmm&0xff);//分
				KL_VERBOSE("__________ALARM_TASK_CL_VOL_SET_________\n");
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_VOL_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}			

		}
	}
}

bool SNorTask::IsValidTime()
{
	time_t curtime;
	time(&curtime);
	if ((curtime>= m_StartTime) && (curtime<=m_EndTime) )
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*brief:set task schedule function,will decide whice task will be excuted
	param [in] void
	param [out]None 
	return void
	author:Wanqingsong
	date: 2012-02-25
*/
bool SNorTask::TaskSchedule(void)
{
	if (IsValidTime() == false)
	{
		return false;
	}

	for (int i=0; i<CONS_TASKPLAN_MAX_COUNT; i++)
	{
		if (m_Plans[i].m_Enable == false)
		{
			continue;
		}

		/* plan\u542F\u52A8\u65F6\u95F4\u68C0\u6D4B*/
		if (TaskPlanTimeOn(i) == true)
		{
			TaskPlanAction(i);
		}
		
	}

	
}

bool SNorTask::TaskPlanTimeOn(INT8U planNo)
{
	time_t tt;
	struct tm *curtime;

	time(&tt);
	curtime=localtime(&tt);

	switch(m_PeriodUnit)
	{
		case EM_PERIODUNIT_DAY:
			break;

		case EM_PERIODUNIT_WEEK:
			if (m_PeriodDetail.m_WeekBS.test(curtime->tm_wday) == true)
			{
				KL_VERBOSE("程控任务%d方案%d启动时间检测..........\n",this->m_TaskNo,planNo);
				if (m_Plans[planNo].IsTimerOn(this->m_TaskNo,planNo) == true)
				{
					return true;
				}
			}
			return false;

 		case EM_PERIODUNIT_MONTH:
			break;

		default:
			break;
	}
}

void SNorTask::TaskPlanAction(INT8U planNo)
{
	m_Plans[planNo].OnExec(this->m_TaskNo);
}

void SNorTasks::OnChange()
{
	ClrelayTaskPlan* pClrelayTaskPlan;
	SPlan plan;

	static INT8U bSysStart=true;

	if (bSysStart==true)
	{
		bSysStart = false;
		m_fresh = true;
	}

	if (m_fresh == false)
	{
		return ;
	}

	if(m_NorValidTBS.count() == 0)
		return ;

	KL_VERBOSE("void SNorTasks::OnChange()\n");
	time_t tNow=time(NULL);
	for (RelayBitSet::SizeType i = CONTEXT.ClRelays.CLRelayBS.FromPos;i != CONTEXT.ClRelays.CLRelayBS.ToPos;i++)
	{
		if (CONTEXT.ClRelays.CLRelayBS.test(i))
		{
			/* 如果回路处于手动控制期间，则刷新手动控制动作*/
			if ( STATUS.deviceStatus.m_ClRelayManualCtlEndTime[i] != 0)
			{
				if (tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[i])	
				{
					plan.OnExec(STATUS.deviceStatus.m_ClRelayManualCtlActionItem[i],RESUME_CTL);
					continue;
				}
			}


			/* 如果回路处于程序控制期间，则刷新程序控制动作*/
			pClrelayTaskPlan = m_ClrelayTasks[i].GetTaskPlan(); 
			if (pClrelayTaskPlan != NULL)
			{
				//plan.OnExec(pClrelayTaskPlan->m_ActionItem,TASK_CTL);
				KL_VERBOSE("plan.OnExec(pClrelayTaskPlan->m_ActionItem,RESUME_CTL);\n");
				plan.OnExec(pClrelayTaskPlan->m_ActionItem,RESUME_CTL);
			}
		}
	}
	m_fresh = false;
}

/*
 * brief:重新设置回路任务配置电压
    param [in] INT8U clNo:回路号
    return:void
*/
void SNorTasks::OnRefresh(INT8U clNo)
{
	ClrelayTaskPlan* pClrelayTaskPlan;
	SPlan plan;

	if (clNo > CONS_CLRELAY_MAX_COUNT)
	{
		return ;
	}
	
	if (CONTEXT.ClRelays.CLRelayBS.test(clNo))
	{
		pClrelayTaskPlan = m_ClrelayTasks[clNo].GetTaskPlan(); 
		if (pClrelayTaskPlan != NULL)
		{
			plan.OnExec(pClrelayTaskPlan->m_ActionItem,RESUME_CTL);
		}
	}
}

ClrelayTaskPlan* ClrelayTask::GetTaskPlan(void)
{
	char bcdTime7[7] = {0};
	Get_CurBCDTime7(bcdTime7);

	INT16U curTime;
	curTime = (INT16U)(bcdTime7[4]<<8 |bcdTime7[5]);

	time_t tt;
	struct tm *Nowtime;

	time(&tt);
	Nowtime=localtime(&tt);

	int i;
	for (i=0; i<CONS_CLRELAY_TASKACTION_NUM-1; i++)
	{
		if (this->m_clrelayTaskPlan[i].m_WeekBS.test(Nowtime->tm_wday) == false)//执行方案不在循环周期内
		{
			continue;
		}
		
		if (this->m_clrelayTaskPlan[i].m_Enable == true && this->m_clrelayTaskPlan[i+1].m_Enable == true)
		{
			if (this->m_clrelayTaskPlan[i].m_ActionTime <=  curTime && curTime < this->m_clrelayTaskPlan[i+1].m_ActionTime)
			{
				return &(this->m_clrelayTaskPlan[i]);
			}
		}

		if (this->m_clrelayTaskPlan[i].m_Enable == true && this->m_clrelayTaskPlan[i+1].m_Enable == false)
		{
			if (this->m_clrelayTaskPlan[i].m_ActionTime <=  curTime)
			{
				return &(this->m_clrelayTaskPlan[i]);
			}
		}
		
	}

/*	if (i == CONS_CLRELAY_TASKACTION_NUM-1)
	{
		if (curTime >= this->m_clrelayTaskPlan[CONS_CLRELAY_TASKACTION_NUM-1].m_ActionTime)
			return &(this->m_clrelayTaskPlan[0]);
	}*/

	return NULL;
	
}

INT16U ClrelayTask::GetNextPlanStartTime()
{
	char bcdTime7[7] = {0};
	Get_CurBCDTime7(bcdTime7);

	INT16U curTime;

	curTime = (INT16U)(bcdTime7[4]<<8 |bcdTime7[5]);

	ClrelayTaskPlan nextClrelayTaskPlan;
	int i;
	for (i=0; i<CONS_CLRELAY_TASKACTION_NUM-1; i++)
	{
		if (this->m_clrelayTaskPlan[i].m_Enable == true && this->m_clrelayTaskPlan[i+1].m_Enable == true)
		{
			if (this->m_clrelayTaskPlan[i].m_ActionTime <=  curTime && curTime < this->m_clrelayTaskPlan[i+1].m_ActionTime)
			{
				nextClrelayTaskPlan = this->m_clrelayTaskPlan[i+1];
			}
		}

		if (this->m_clrelayTaskPlan[i].m_Enable == true && this->m_clrelayTaskPlan[i+1].m_Enable == false)
		{
			if (this->m_clrelayTaskPlan[i].m_ActionTime <=  curTime)
			{
				nextClrelayTaskPlan = this->m_clrelayTaskPlan[0];
			}
		}
	}
	return nextClrelayTaskPlan.m_ActionTime;
}

