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
		KL_VERBOSE("����������ִ��\n");
		return false;
	}

	KL_VERBOSE("��ǰʱ��(%x:%x),��������ʱ��(%x:%x)\n",cbcd(curtime->tm_hour),cbcd(curtime->tm_min),m_BeginTimeHHMM[0],m_BeginTimeHHMM[1]);

	if (cbcd(curtime->tm_hour) == m_BeginTimeHHMM[0] && cbcd(curtime->tm_min) >=m_BeginTimeHHMM[1])
	{
		KL_VERBOSE("<<<<<<<<<<                    �������ʱ�䵽        ?>>>>>>>>>>>>>>>>>>>\n");
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
		if (m_ActionItems[i].m_Function == FUNC_RELAY_ON)//�̵�����
		{
			CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
			if (pDataItem == NULL)
			{
				return;
			}
			
			bstring tempData;
			tempData.clear();
			tempData.push_back(m_ActionItems[i].m_SubNo);//��·��
			tempData.push_back(2);//��
			
			int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[m_ActionItems[i].m_DeviceNo],tempData);
		}

		if (m_ActionItems[i].m_Function == FUNC_RELAY_OFF)//�̵�����
		{
			CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
			if (pDataItem == NULL)
			{
				return;
			}
			
			bstring tempData;
			tempData.clear();
			tempData.push_back(m_ActionItems[i].m_SubNo);//�̵�����
			tempData.push_back(3);//��
			
			int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[m_ActionItems[i].m_DeviceNo],tempData);
		}

		if (m_ActionItems[i].m_Function == FUNC_RTF_ON)//��������
		{
			CRtfCtrl rtfCtl;
			rtfCtl.RtfSwitchSetCtl(m_ActionItems[i], 0, 0);
		}

		if (m_ActionItems[i].m_Function == FUNC_RTF_OFF)//��������
		{
			CRtfCtrl rtfCtl;
			rtfCtl.RtfSwitchSetCtl(m_ActionItems[i], 1, 0);
		}

		if (m_ActionItems[i].m_Function == FUNC_RTF_VOL_SET)//��������ѹ����
		{
			INT16U vol = (INT16U)(m_ActionItems[i].m_FuncData[0]<<8 | m_ActionItems[i].m_FuncData[1]);
			INT16U cur = (INT16U)(m_ActionItems[i].m_FuncData[2]<<8 | m_ActionItems[i].m_FuncData[3]);
			KL_VERBOSE("Set Clrelay no %d Vol = %x Cur = %x\n",m_ActionItems[i].m_DeviceNo,vol,cur);
 			CRtfCtrl rtfCtl;
			rtfCtl.RtfSwitchSetCtl(m_ActionItems[i], vol,cur);
		}

		if (m_ActionItems[i].m_Function == FUNC_CL_ON)//��·��������
		{
			CRtfCtrl rtfCtl;
			time_t tNow=time(NULL);
			if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo])
			{
				KL_VERBOSE("��·%d�����ֶ�������Ч��\n",m_ActionItems[i].m_DeviceNo);
				//�ֶ�������Ч��Ķ���
				STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[m_ActionItems[i].m_DeviceNo] = m_ActionItems[i];
				continue;
			}
			else
			{
				//����ֲManualCtlOutOfDateProcess���и�λ
				//STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo] = 0;

			}
			rtfCtl.RtfClOnCtl(m_ActionItems[i]);

			{
				char bcdtime7[7];
				INT8U AlarmData[9];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 8;//�澯���ݳ���
				AlarmData[1] = taskno;
				AlarmData[2] = m_ActionItems[i].m_DeviceNo;//��·��
				AlarmData[3] = 0x01;

				INT16U hhmm =  CONTEXT.NorTasks.m_ClrelayTasks[m_ActionItems[i].m_DeviceNo].GetNextPlanStartTime();				
				AlarmData[4] = 0x00;//��
				AlarmData[5] = 0x00;//��
				AlarmData[6] = 0x00;//��
				AlarmData[7] = (INT8U)(hhmm>>8&0xff);//ʱ
				AlarmData[8] = (INT8U)(hhmm&0xff);//��
				
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_ONOFF_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}
		}

		if (m_ActionItems[i].m_Function == FUNC_CL_OFF)//��·��������
		{
			CRtfCtrl rtfCtl;
			time_t tNow=time(NULL);
			if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo])
			{
				KL_VERBOSE("��·%d�����ֶ�������Ч��\n",m_ActionItems[i].m_DeviceNo);
				//�ֶ�������Ч��Ķ���
				STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[m_ActionItems[i].m_DeviceNo] = m_ActionItems[i];

				continue;
			}
			else
			{
				//����ֲManualCtlOutOfDateProcess���и�λ
				//STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo] = 0;
			}

			rtfCtl.RtfClOffCtl(m_ActionItems[i]);
			{
				char bcdtime7[7];
				INT8U AlarmData[9];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 8;//�澯���ݳ���
				AlarmData[1] = taskno;
				AlarmData[2] = m_ActionItems[i].m_DeviceNo;//��·��
				AlarmData[3] = 0x00;
				
				INT16U hhmm =  CONTEXT.NorTasks.m_ClrelayTasks[m_ActionItems[i].m_DeviceNo].GetNextPlanStartTime();				
				AlarmData[4] = 0x00;//��
				AlarmData[5] = 0x00;//��
				AlarmData[6] = 0x00;//��
				AlarmData[7] = (INT8U)(hhmm>>8&0xff);//ʱ
				AlarmData[8] = (INT8U)(hhmm&0xff);//��
				
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_ONOFF_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}
		}
		KL_VERBOSE("m_Function=%d\n",m_ActionItems[i].m_Function);
		if (m_ActionItems[i].m_Function == FUNC_CL_VOL_SET)//��·���õ�ѹ
		{
			time_t tNow=time(NULL);
			if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo])
			{
				KL_VERBOSE("��·%d�����ֶ�������Ч��\n",m_ActionItems[i].m_DeviceNo);
				//�ֶ�������Ч��Ķ���
				STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[m_ActionItems[i].m_DeviceNo] = m_ActionItems[i];
				STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[m_ActionItems[i].m_DeviceNo].m_SubNo = taskno;//����·�Ŵ洢����� 
				continue;
			}
			else
			{
				//����ֲManualCtlOutOfDateProcess���и�λ
				//STATUS.deviceStatus.m_ClRelayManualCtlEndTime[m_ActionItems[i].m_DeviceNo] = 0;
			}

			CRtfCtrl rtfCtl;
			rtfCtl.RtfClOnCtl(m_ActionItems[i]);//�Ƚ���·��������
					
			INT16U vol = (INT16U)(m_ActionItems[i].m_FuncData[0]<<8 | m_ActionItems[i].m_FuncData[1]);
			INT16U cur = (INT16U)(m_ActionItems[i].m_FuncData[2]<<8 | m_ActionItems[i].m_FuncData[3]);
			KL_VERBOSE("Set Clrelay no %d Vol = %x Cur = %x\n",m_ActionItems[i].m_DeviceNo,vol,cur);
 		
			rtfCtl.RtfClVolCurSetCtl(m_ActionItems[i], vol,cur);

			{
				char bcdtime7[7];
				INT8U AlarmData[10];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 9;//�澯���ݳ���
				AlarmData[1] = taskno;
				if (taskno == 0)
				{
					KL_VERBOSE("1.�Ƿ������0\n");
					return ;
				}
				AlarmData[2] = m_ActionItems[i].m_DeviceNo;//��·��
				AlarmData[3] = (INT8U)(vol>>8&0xff);
				AlarmData[4] = (INT8U)(vol&0xff);

				INT16U hhmm =  CONTEXT.NorTasks.m_ClrelayTasks[m_ActionItems[i].m_DeviceNo].GetNextPlanStartTime();				
				AlarmData[5] = 0x00;//��
				AlarmData[6] = 0x00;//��
				AlarmData[7] = 0x00;//��
				AlarmData[8] = (INT8U)(hhmm>>8&0xff);//ʱ
				AlarmData[9] = (INT8U)(hhmm&0xff);//��
				KL_VERBOSE("__________ALARM_TASK_CL_VOL_SET 2_________\n");
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_VOL_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}
		}

	}
	
}

void SPlan::OnExec(SSmartGarageItem & actionItem, INT8U alarmtype)
{

	if (actionItem.m_Function == FUNC_RELAY_ON)//�̵�����
	{
		CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
		if (pDataItem == NULL)
		{
			return;
		}
		
		bstring tempData;
		tempData.clear();
		tempData.push_back(actionItem.m_SubNo);//��·��
		tempData.push_back(2);//��
		
		int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[actionItem.m_DeviceNo],tempData);
	}

	if (actionItem.m_Function == FUNC_RELAY_OFF)//�̵�����
	{
		CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
		if (pDataItem == NULL)
		{
			return;
		}
		
		bstring tempData;
		tempData.clear();
		tempData.push_back(actionItem.m_SubNo);//�̵�����
		tempData.push_back(3);//��
		
		int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[actionItem.m_DeviceNo],tempData);
	}

	if (actionItem.m_Function == FUNC_RTF_ON)//��������
	{
		CRtfCtrl rtfCtl;
		rtfCtl.RtfSwitchSetCtl(actionItem, 0, 0);
	}

	if (actionItem.m_Function == FUNC_RTF_OFF)//��������
	{
		CRtfCtrl rtfCtl;
		rtfCtl.RtfSwitchSetCtl(actionItem, 1, 0);
	}

	if (actionItem.m_Function == FUNC_RTF_VOL_SET)//��������ѹ����
	{
		INT16U vol = (INT16U)(actionItem.m_FuncData[0]<<8 | actionItem.m_FuncData[1]);
		INT16U cur = (INT16U)(actionItem.m_FuncData[2]<<8 | actionItem.m_FuncData[3]);
		KL_VERBOSE("Set Clrelay no %d Vol = %x Cur = %x\n",actionItem.m_DeviceNo,vol,cur);
		CRtfCtrl rtfCtl;
		rtfCtl.RtfSwitchSetCtl(actionItem, vol,cur);
	}

	if (actionItem.m_Function == FUNC_CL_ON)//��·��������
	{
		time_t tNow=time(NULL);
		if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[actionItem.m_DeviceNo])
		{
			KL_VERBOSE("��·%d�����ֶ�������Ч��\n",actionItem.m_DeviceNo);
			//�ֶ�������Ч��Ķ���
			STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[actionItem.m_DeviceNo] = actionItem;

			return ;
		}
		
		CRtfCtrl rtfCtl;
		
		rtfCtl.RtfClOnCtl(actionItem);

		{
			char bcdtime7[7];
			INT8U AlarmData[4];
			Get_CurBCDTime7(bcdtime7);
			AlarmData[0] = 2;//�澯���ݳ���
			AlarmData[1] = actionItem.m_DeviceNo;
			AlarmData[2] = 0x01;
/*			if (alarmtype == MANUAL_CTL)
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CL_CTRL,bcdtime7,CONS_UNVALID_METER_NUMBER);
			else
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_ONOFF_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);*/
		}
	}

	if (actionItem.m_Function == FUNC_CL_OFF)//��·��������
	{
		time_t tNow=time(NULL);
		if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[actionItem.m_DeviceNo])
		{
			KL_VERBOSE("��·%d�����ֶ�������Ч��\n",actionItem.m_DeviceNo);
			//�ֶ�������Ч��Ķ���
			STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[actionItem.m_DeviceNo] = actionItem;

			return ;
		}
		
		CRtfCtrl rtfCtl;
		
		rtfCtl.RtfClOffCtl(actionItem);
		{
			char bcdtime7[7];
			INT8U AlarmData[4];
			Get_CurBCDTime7(bcdtime7);
			AlarmData[0] = 2;//�澯���ݳ���
			AlarmData[1] = actionItem.m_DeviceNo;
			AlarmData[2] = 0x00;

/*			if (alarmtype == MANUAL_CTL)
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CL_CTRL,bcdtime7,CONS_UNVALID_METER_NUMBER);
			else
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_TASK_CL_ONOFF_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);*/
		}
	}

	if (actionItem.m_Function == FUNC_CL_VOL_SET)//��·���õ�ѹ
	{
		time_t tNow=time(NULL);
		if ( tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[actionItem.m_DeviceNo])
		{
			KL_VERBOSE("��·%d�����ֶ�������Ч��\n",actionItem.m_DeviceNo);
			//�ֶ�������Ч��Ķ���
			STATUS.deviceStatus.m_ClRelayManualCtlRecoverActionItem[actionItem.m_DeviceNo] = actionItem;

			return ;
		}
			
		INT16U vol = (INT16U)(actionItem.m_FuncData[0]<<8 | actionItem.m_FuncData[1]);
		INT16U cur = (INT16U)(actionItem.m_FuncData[2]<<8 | actionItem.m_FuncData[3]);
		KL_VERBOSE("Set Clrelay no %d Vol = %x Cur = %x\n",actionItem.m_DeviceNo,vol,cur);
		CRtfCtrl rtfCtl;
		rtfCtl.RtfClVolCurSetCtl(actionItem, vol,cur);

		{
			if (alarmtype == TASK_CTL)//�����ֶ�������Чʱ���Ƴ�������ִ�У�����Чʱ���������ִ������
			{
				char bcdtime7[7];
				INT8U AlarmData[10];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 9;//�澯���ݳ���

				if (actionItem.m_SubNo == 0)
				{
					KL_VERBOSE("2 actionItem.m_SubNo == 0 \n");
					return;
				}

				AlarmData[1] = actionItem.m_SubNo;//�����·�Ŵ�ŵ��������
				AlarmData[2] = actionItem.m_DeviceNo;//��·��
				AlarmData[3] = (INT8U)(vol>>8&0xff);
				AlarmData[4] = (INT8U)(vol&0xff);

				INT16U hhmm =  CONTEXT.NorTasks.m_ClrelayTasks[actionItem.m_DeviceNo].GetNextPlanStartTime();				
				AlarmData[5] = 0x00;//��
				AlarmData[6] = 0x00;//��
				AlarmData[7] = 0x00;//��
				AlarmData[8] = (INT8U)(hhmm>>8&0xff);//ʱ
				AlarmData[9] = (INT8U)(hhmm&0xff);//��
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
				KL_VERBOSE("�̿�����%d����%d����ʱ����..........\n",this->m_TaskNo,planNo);
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
			/* �����·�����ֶ������ڼ䣬��ˢ���ֶ����ƶ���*/
			if ( STATUS.deviceStatus.m_ClRelayManualCtlEndTime[i] != 0)
			{
				if (tNow < STATUS.deviceStatus.m_ClRelayManualCtlEndTime[i])	
				{
					plan.OnExec(STATUS.deviceStatus.m_ClRelayManualCtlActionItem[i],RESUME_CTL);
					continue;
				}
			}


			/* �����·���ڳ�������ڼ䣬��ˢ�³�����ƶ���*/
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
 * brief:�������û�·�������õ�ѹ
    param [in] INT8U clNo:��·��
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
		if (this->m_clrelayTaskPlan[i].m_WeekBS.test(Nowtime->tm_wday) == false)//ִ�з�������ѭ��������
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

