#include "nortask.h"
#include "dataItem.h"
#include "meter.h"
#include "context.h"
#include "RtfCtrl.h"
#include "AlarmFile.h"
#include "status.h"
#include "luxtask.h"

bool SLuxPlan::IsTimerOn(INT8U taskno,INT8U planno)
{
	time_t tt;
	time(&tt);

	KL_VERBOSE_DUMP(dump);
	dump<<"tt="<<tt<<",m_ActionTime="<<m_ActionTime<<"\n";
	if (tt >= m_ActionTime /*&& m_RunSTate != STATE_RUN*/)
	{
		return true;
	}

	return false;
}
void SLuxPlan::OnExec(INT8U taskno)
{
/*	if (m_Enable == false)
	{
		return ;
	}*/

	for(int i=0; i<m_ActionItemsNum; i++)
	{
/*		if (m_ActionItems[i].m_Function == FUNC_CL_ON)//��·��������
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
		}*/

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
				AlarmData[3] = 0x00;//�ر�
				
				AlarmData[4] = (m_KeepMins>>8)&0xff;
				AlarmData[5] = m_KeepMins&0xff;
				AlarmData[6] = 0x00;
				AlarmData[7] = 0x00;
				AlarmData[8] = 0x00;
				KL_VERBOSE("__________ALARM_LUXTASK_CL_ONOFF _________\n");
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),LUXTASK_CL_ONOFF_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);
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
					KL_VERBOSE("�Ƿ������0\n");
					return ;
				}
				AlarmData[2] = m_ActionItems[i].m_DeviceNo;//��·��
				AlarmData[3] = (INT8U)(vol>>8&0xff);
				AlarmData[4] = (INT8U)(vol&0xff);

				AlarmData[5] = (m_KeepMins>>8)&0xff;
				AlarmData[6] = m_KeepMins&0xff;
				AlarmData[7] = 0x00;
				AlarmData[8] = 0x00;
				AlarmData[9] = 0x00;
				KL_VERBOSE("__________ALARM_LUXTASK_CL_VOL_SET _________\n");
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),LUXTASK_CL_VOL_SET,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}
		}

	}
	
}

void SLuxPlan::OnExec(SSmartGarageItem & actionItem, INT8U alarmtype)
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
					KL_VERBOSE(" actionItem.m_SubNo == 0 \n");
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

bool SLuxTask::IsValidTime()
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
				return true;
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
bool SLuxTask::LuxTaskSchedule(void)
{
	/* �����ػ�·�����ֶ�����ʱ�Σ�����������ͣ����*/
	if (m_ClManualCtl == 1)
		return true;
		
	if (IsValidTime() == false)
	{
		if (m_InvalidTimeClOff == 0)
		{
		    	    KL_VERBOSE("(((((((((((((((((        ʱ����Ч������������     )))))))))))))))))\n");
			    m_RunState = LUXTASK_STOP;	
			   STATUS.deviceStatus.m_LuxTaskStatus.LuxTasksState[m_TaskNo] =	m_RunState;
			    STATUS.deviceStatus.m_LuxTaskStatus.LuxTasksStartTime[m_TaskNo] = 0;	
			    for (int i=0; i<CONS_TASKPLAN_MAX_COUNT; i++)
			    {
				   m_Plans[i].m_RunSTate = STATE_STOP;
			    }
			    if (m_Plans[0].m_Enable == true)//��ȡ��·����Ϣ
			    {
			    		CRtfCtrl rtfctl;
					rtfctl.RtfClOffCtl(m_Plans[0].m_ActionItems[0]);	
					{
						char bcdtime7[7];
						INT8U AlarmData[9];
						Get_CurBCDTime7(bcdtime7);
						AlarmData[0] = 2;//�澯���ݳ���
						AlarmData[1] = m_TaskNo;//�����
						AlarmData[2] = m_Plans[0].m_ActionItems[0].m_DeviceNo;//��·��
						
						AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),LUXTASK_END,bcdtime7,CONS_UNVALID_METER_NUMBER);
					}
			     }
				m_InvalidTimeClOff = 1;
				return true; 
		}
		return false;
	}
	m_InvalidTimeClOff = 0;
	if (m_LuxSensorsDBS.count() == 0)
	{
		return false;
	}
	/* ��·��������ֵ����*/
	INT32U lux;
	lux = LuxCalculate();
	KL_VERBOSE("LuxCalculate = %d,m_StartLuxlimit=%d,m_EndLuxlimit=%d\n",lux,m_StartLuxlimit,m_EndLuxlimit);

	/* ������ֵ��� */
	if (lux  < m_StartLuxlimit &&  m_RunState != LUXTASK_RUN)
	{
	    KL_VERBOSE("<<<<<<<         �����������      >>>>>>>>>>>>\n");
	    CalculatePlanActionTime();	
	    m_RunState = LUXTASK_RUN;	
	    STATUS.deviceStatus.m_LuxTaskStatus.LuxTasksState[m_TaskNo] =	m_RunState;	
	}

	//ϵͳ�ϵ���������
	static bool start=true;
	if (start == true)
	{
		if (STATUS.deviceStatus.m_LuxTaskStatus.LuxTasksState[m_TaskNo] == LUXTASK_RUN)
		{
			CalculatePlanActionTime();	
	    		m_RunState = LUXTASK_RUN;	
		}
	}
	/* ������ֵ��� */
	KL_VERBOSE("lux=%d,m_EndLuxlimit=%d,m_RunState=%d\n",lux,m_EndLuxlimit,m_RunState);
	if (lux  > m_EndLuxlimit && (m_RunState != LUXTASK_STOP || start==true))
	{
	   start = false;	
	    //m_Plans[1].OnExec();
	    
    	    KL_VERBOSE("(((((((((((((((((         ����������     )))))))))))))))))\n");
	    m_RunState = LUXTASK_STOP;	
	    STATUS.deviceStatus.m_LuxTaskStatus.LuxTasksState[m_TaskNo] =	m_RunState;	
	    STATUS.deviceStatus.m_LuxTaskStatus.LuxTasksStartTime[m_TaskNo] = 0;	
	    for (int i=0; i<CONS_TASKPLAN_MAX_COUNT; i++)
	    {
		   m_Plans[i].m_RunSTate = STATE_STOP;
	    }
	    if (m_Plans[0].m_Enable == true)//��ȡ��·����Ϣ
	    {
	    		CRtfCtrl rtfctl;
	
			rtfctl.RtfClOffCtl(m_Plans[0].m_ActionItems[0]);	
			{
				char bcdtime7[7];
				INT8U AlarmData[9];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 2;//�澯���ݳ���
				AlarmData[1] = m_TaskNo;//�����
				AlarmData[2] = m_Plans[0].m_ActionItems[0].m_DeviceNo;//��·��
				
				AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),LUXTASK_END,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}
	     }
	    return true; 
	}

	if ( m_RunState == LUXTASK_STOP)
		return true;

	/* ��������ִ�� */
	INT8S planno = -1;
	for (int i=0; i<CONS_TASKPLAN_MAX_COUNT; i++)
	{
		if (m_Plans[i].m_Enable == false)
		{
			continue;
		}

		/* plan\u542F\u52A8\u65F6\u95F4\u68C0\u6D4B*/
		if (LuxTaskPlanTimeOn(i) == true)
		{
			planno = i;
		}
	}

	if (planno == -1 || planno >= CONS_LUXTASKPLAN_MAX_COUNT)
		return false;
	
	KL_VERBOSE("planno=%d,m_RunSTate=%d\n",planno,m_Plans[planno].m_RunSTate);
	if (planno != -1 && m_Plans[planno].m_RunSTate != STATE_RUN)
	{
		KL_VERBOSE("���ִ�з���[%d]\n",planno);
		SetPlanRunState(planno);
		LuxTaskPlanAction(planno);
	}
	return true;
}

bool SLuxTask::LuxTaskPlanTimeOn(INT8U planNo)
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
				KL_VERBOSE("�������%d����%d����ʱ����..........\n",this->m_TaskNo,planNo);
				if (m_Plans[planNo].IsTimerOn(this->m_TaskNo,planNo) == true)
				{
					KL_VERBOSE("������񷽰�%dʱ�䵽\n",planNo);
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

void SLuxTask::LuxTaskPlanAction(INT8U planNo)
{
	m_Plans[planNo].OnExec(this->m_TaskNo);
}

INT32U SLuxTask::LuxCalculate(void)
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
		 //taskLuxValue +=  (tempdev.m_LuxValue * tempdev.m_LuxRate)/100;
		 taskLuxValue +=  tempdev.m_LuxValue;
		 KL_VERBOSE("TN = %d,Luxvalue=%d,LuxRate=%d\n",i,tempdev.m_LuxValue,tempdev.m_LuxRate);
	}
	return (INT32U)(taskLuxValue/m_LuxSensorsDBS.count());
}

void SLuxTask::CalculatePlanActionTime(void)
{
	time_t curtime;
	time(&curtime);

	if ( STATUS.deviceStatus.m_LuxTaskStatus.LuxTasksStartTime[m_TaskNo] == 0)//�շ�ֵ����������װ�����й���������
	{
		STATUS.deviceStatus.m_LuxTaskStatus.LuxTasksStartTime[m_TaskNo] = curtime;
	}

	KL_VERBOSE_DUMP(dump);
	for (int i=0; i<CONS_LUXTASKPLAN_MAX_COUNT; i++)
	{
		
		if (m_Plans[i].m_Enable == false)//�����false��ʾ�Ѿ���������
		{
			break;
		}

		INT16U wait_mins = 0;
		for(int j=0; j<i; j++)//��ǰ������ִ������ʱ��Ϊǰ�������������ĵ�ʱ��֮������õ�
		{
			wait_mins += m_Plans[j].m_KeepMins;
		}
		
		m_Plans[i].m_ActionTime = wait_mins*60+STATUS.deviceStatus.m_LuxTaskStatus.LuxTasksStartTime[m_TaskNo];
		dump<<"planno="<<i<<",ActionTime="<<m_Plans[i].m_ActionTime<<"\n";
	}
	
		
}

bool SLuxTask::SetPlanRunState(INT8U planno)
{
	//��ĳһplan����Ϊ����״̬������plan״̬������Ϊֹͣ״̬
	if (planno >= CONS_LUXTASKPLAN_MAX_COUNT)
	{
		return false;
	}

	for (int i=0; i<CONS_LUXTASKPLAN_MAX_COUNT; i++)
	{
		if (i == planno)
		{
			m_Plans[i].m_RunSTate = STATE_RUN;
		}
		else
		{
			m_Plans[i].m_RunSTate = STATE_STOP;
		}
	}
}

bool SLuxTask::ResetPlanRunState(INT8U planno)
{
	if (planno >= CONS_LUXTASKPLAN_MAX_COUNT)
	{
		return false;
	}

	m_Plans[planno].m_RunSTate = STATE_STOP;
	return true;
}

void SLuxTask::ResetPlansRunState(void)
{
	for (int i=0; i<CONS_LUXTASKPLAN_MAX_COUNT; i++)
	{
		m_Plans[i].m_RunSTate = STATE_STOP;
	}
	return;
}


