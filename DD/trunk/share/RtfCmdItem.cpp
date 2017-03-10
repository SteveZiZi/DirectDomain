#include "RtfCmdItem.h"
#include "../brule/BusRuleFactory.h"
#include "../base/klb_base.h"
#include "../base/klb_io.h"
#include "ipcSharemem.h"
#include"../brule/CanbusRule.h"
#include "SysConfig.h"
#include "AlarmFile.h"

#define MAX_COM_RETRY_TIME 3
#define MAX_CAN_COM_RETRY_TIME 1

void CRtfCmdItems::Init(void)
{
	m_count = 0;

	CRtfCmdItem_0501 *pRtfCmdItem_0501 = new CRtfCmdItem_0501();
	AddRtfCmdItem(pRtfCmdItem_0501);

	CRtfCmdItem_0601 *pRtfCmdItem_0601 = new CRtfCmdItem_0601();
	AddRtfCmdItem(pRtfCmdItem_0601);

	CRtfCmdItem_0602 *pRtfCmdItem_0602 = new CRtfCmdItem_0602();
	AddRtfCmdItem(pRtfCmdItem_0602);
}
/*
	brief: get alarm type according to alarm name
	param [in] string alarmName: alarm name
	return CAlarm*  
	author:WanQingsong
	date: 2012-04-09
*/  
CRtfCmdItem* CRtfCmdItems::GetRtfCmdItem(INT16U dataId)
{
	for (int i=0; i<m_count; i++)
	{
		if (m_pRtfCmdItems[i]->GetRtfCmdItemID() == dataId)
		{
			return m_pRtfCmdItems[i];
		}
	}

	
	return NULL;
}
/*
	brief: add a alram type to Alarm types muster
	param [in] CAlarm alarm
	return bool 
	author:WanQingsong
	date: 2012-04-13
*/  
bool CRtfCmdItems::AddRtfCmdItem(CRtfCmdItem *pItem)
{
	if (m_count > CONS_CMDITEM_COUNT_MAX)
	{
		KL_VERBOSE("�Ѿ�����ϵͳ��������澯��������\n");
		return false;
	}
	m_pRtfCmdItems[m_count++] = pItem;
	return true;
}


bool CRtfCmdItem::Read(Device device,INT8U ctl,bstring &rtData)
{
	bstring oPackage;
	MakePackage(device,ctl,NULL,0,oPackage);

	BusChannelsRule* bRule= GDBusRuleFactory::GetBusRule(DBCJ_1_BUSNO_485);
	if (bRule == NULL)
	{
		KL_VERBOSE("���߻�ȡʧ��\n");
		return false;
	}
	
	if (bRule->GetRtfData(device, oPackage.data(),oPackage.size(), rtData) == 0)
		return true;
	else
	{
		KL_ERROR("[!!!!!!!!! ֡�ظ�ʧ�� !!!!!!!!!!]");
		return false;
	}
	
}

/* can bus ��д*/
bool CRtfCmdItem::Write(INT8U srcAddr,INT8U destAddr,INT8U cmd, INT8U *pDatabuf,INT8U len,bstring &rtData)
{
	INT8U rcvBuf[8] = {0};
	INT8U rtLen=0;
	destAddr = bcdc(destAddr);
	if (CCanbus::Open() != true)
	{
		KL_VERBOSE("CCanbus::Open()  == false \n");
		return false;
	}
	
	if (CCanbus::Send(srcAddr,destAddr,cmd,pDatabuf,len) == -1)
	{
		return false;
	}

	if (CCanbus::Recv(srcAddr,destAddr,rcvBuf,rtLen,1) == false)
	{
		if (CCanbus::Send(srcAddr,destAddr,cmd,pDatabuf,len) == -1)
		{
			return false;
		}

		memset(rcvBuf,0x00,sizeof(rcvBuf));
		if (CCanbus::Recv(srcAddr,destAddr,rcvBuf,rtLen,2) == false)
		{
			return false;
		}
	}
	
	if (rtLen > 0 )
	{
		rtData.append(rcvBuf,(size_t)rtLen);
	}
	return true;
	
}
	
bool CRtfCmdItem::Write(Device device,INT8U ctl,bstring wtData)
{
	bstring oPackage;
	MakePackage(device,0x04,wtData.data(),(INT8U)wtData.size(),oPackage);

	BusChannelsRule* bRule= GDBusRuleFactory::GetBusRule(DBCJ_1_BUSNO_485);
	if (bRule == NULL)
	{
		KL_VERBOSE("���߻�ȡʧ��\n");
		return false;
	}
	bstring rtData;
	if (bRule->GetRtfData(device, oPackage.data(),oPackage.size(), rtData) == 0)
	{
		return true;
	}
	else
	{
		KL_ERROR("[!!!!!!!!! ֡д�ظ�ʧ�� !!!!!!!!!!]\n");
		return false;
	}
	
}

bool CRtfCmdItem::Write(Device device,INT8U ctl,bstring wtData,bstring &rtData)
{
	bstring oPackage;
	MakePackage(device,ctl,wtData.data(),(INT8U)wtData.size(),oPackage);

	BusChannelsRule* bRule= GDBusRuleFactory::GetBusRule(DBCJ_1_BUSNO_485);
	if (bRule == NULL)
	{
		KL_VERBOSE("���߻�ȡʧ��\n");
		return false;
	}
	
	if (bRule->GetRtfData(device, oPackage.data(),oPackage.size(), rtData) == 0)
	{
		return true;
	}
	else
	{
		KL_ERROR("[!!!!!!!!! ֡д�ظ�ʧ�� !!!!!!!!!!]\n");
		return false;
	}
	
}

int CRtfCmdItem::MakeModbusPackage(INT8U commNo,INT8U ctl,INT16U dataid, const INT8U* iData,const INT8U iDataLen,bstring& oPackage)
{
	unsigned char addr[6];
	 unsigned short crc_code;

	oPackage.clear();
	oPackage.push_back(commNo);
	oPackage.push_back(ctl);
	oPackage.push_back((dataid>>8)&0xff);
	oPackage.push_back(dataid&0xff);
	
	for (int i=0;i<iDataLen;i++)
	{
		oPackage.push_back(iData[i]);
	}

	crc_code = 0;
	for (int i = 0;i < oPackage.size();i++)
	{
		crc_code = CRC16RTU( oPackage.data(), (unsigned int )oPackage.size());
	}		
	
	oPackage.push_back(INT8U(crc_code & 0xff));
	oPackage.push_back(INT8U((crc_code >> 8) & 0xff));
	
	
	return 0;
}


bool CRtfCmdItem::ModbusRead(Device device,INT8U commNo,INT8U ctl,INT16U dataid,bstring iData,bstring &rtData)
{
	bstring oPackage;
	MakeModbusPackage(commNo,ctl,dataid,iData.data(),iData.size(),oPackage);

	BusChannelsRule* bRule= GDBusRuleFactory::GetBusRule(DBCJ_1_BUSNO_485);
	if (bRule == NULL)
	{
		KL_VERBOSE("���߻�ȡʧ��\n");
		return false;
	}
	
	if (bRule->GetDeviceData(device, oPackage.data(),oPackage.size(), rtData) == 0)
		return true;
	else
		return false;
}
int CRtfCmdItem::MakePackage(Device device,INT8U ctl, const INT8U* iData,const INT8U iDataLen,bstring& oPackage)
{
	unsigned char addr[6];
	oPackage.clear();

	GetAddr_6_Fix0(device.m_Addr,addr);
	
	oPackage.push_back(addr[0]);
	oPackage.push_back(cbcd(iDataLen+1));
	oPackage.push_back(ctl);
	for (int i=0; i<iDataLen; i++)
	{
		oPackage.push_back(iData[i]);
	}

	INT8U checkSum=0;
	INT8U curLen=3;
	for (int i=0;i<curLen+iDataLen;i++)
	{
		checkSum+=oPackage[i];
	}
	oPackage.push_back(cbcd(checkSum));
	oPackage.push_back(0x0D);

	oPackage.insert(oPackage.begin(),0x7e);
	return 0;
}


int CRtfCmdItem_0501::Exec(Device &device,bstring &Data)
{
	INT8U ctl = 0x03; 
	bstring rtData;
	bool ret;

	/* IPC�����ڴ����ݸ���*/
	IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_DeviceNo = device.m_Tn;
	IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_DeviceType = device.m_Type;
	IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_ClNo = device.m_ClRelayNo;

	KL_VERBOSE("CRtfCmdItem_0501::Exec device tn = %d\n",device.m_Tn);
	if ( CONTEXT.m_DnProtocolType == DN_PROTOCOL_TYPE_RS485_A)
	{
		for (int n=0; n<MAX_COM_RETRY_TIME; n++)
		{
			rtData.clear();
			ret = Read(device,ctl,rtData);
			if (ret == true)
			{
				break;
			}
			else
			{
				if (device.m_CommBreakTimes <= MAX_COM_RETRY_TIME)
				{
					device.m_CommBreakTimes++;
				}
				else
				{
					break;
				}
			}
		}

/*		if (ret != true)//����485��ʽ�����յ��ı���ͷ�ֽ��׳������룬���������ٴγ�����ʩ
		{
			if (device.m_CommBreakTimes <= MAX_COM_RETRY_TIME)
				device.m_CommBreakTimes++;
			rtData.clear();
			ret = Read(device,ctl,rtData);	
		}*/

		if (ret == true)
		{
			memcpy(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,rtData.data()+4,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501));
			/* �����54V�����������ѹ��ʽΪXX.XX����ת��ΪXXX.X��ʽ*/
			if (CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0] > 0x50)
			{
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[1] = (INT8U)((CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0] &0x0f<<4)|CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[1]>>4);
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0] = (INT8U)((CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0]>>4)&0X0f); 	
			}

			/* ͨѶ���߻ָ�*/
			if (device.m_CommBreakTimes>=MAX_COM_RETRY_TIME)
			{
				CONTEXT.NorTasks.OnRefresh(device.m_ClRelayNo);

				{
					KL_VERBOSE("��·%d CPR%dͨѶ���߻ָ�\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//�澯���ݳ���
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 0;//���߻ָ�
					KL_VERBOSE("__________ALARM_CPR_ON_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
				}

				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] &=0x7f;//bit7Ϊģ��ͨѶ����

				/*����Ǻ��ջ�·�����������߻ָ���������»�·off״̬���ں��ջ�·��ʱЧ��Χ�⣬��·�ѹرգ�����ʱ�Ȳ�����������������ϵ��ˣ�����ʱ���ջ�·�����ٴιرյ�����*/
				for (TaskBitSet::SizeType j = CONTEXT.ConsLuxTasks.m_ConsLuxValidTBS.FromPos; j<CONTEXT.ConsLuxTasks.m_ConsLuxValidTBS.ToPos; j++)
				{
					if (CONTEXT.ConsLuxTasks.m_ConsLuxValidTBS.test(j) == false)
					{
						continue;
					}
					SConsLuxTask &task = CONTEXT.ConsLuxTasks.m_Tasks[j];
					if (task.m_ClrelayNo == device.m_ClRelayNo)
					{
						task.m_ClearClOffFlag = true;
					}
				}
			}
			
			device.m_CommBreakTimes=0;
			KL_VERBOSE("Tn=%d ,CommBreakTimes = %d\n",device.m_Tn,device.m_CommBreakTimes);
			
		}
		else
		{
			KL_VERBOSE("Tn=%d ,CommBreakTimes = %d\n",device.m_Tn,device.m_CommBreakTimes);
			if (device.m_CommBreakTimes >= MAX_COM_RETRY_TIME)
			{
#ifndef WIN32			
				memset(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,0x00,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501)-2);//״̬�����
#endif
				if (device.m_CommBreakTimes == MAX_COM_RETRY_TIME)
				{
					KL_VERBOSE("��·%d CPR%dͨѶ����\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//�澯���ݳ���
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 1;//���߲���
					KL_VERBOSE("__________ALARM_CPR_OFF_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);

					CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] |=0x80;//bit7Ϊģ��ͨѶ����
				}
			}
		}
		/* ���⴦��Ŀǰ������״̬������*/
		CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] = 0x00;
		CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[8] = 0x00;

		/* IPC�����ڴ����ݸ���*/
		memcpy(IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_Reg0501,CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501));
		IPCSHAREMEM.Update();
	}
	else if ( CONTEXT.m_DnProtocolType == DN_PROTOCOL_TYPE_RS485_B)
	{
		INT8U commNo;
		bstring iData;
		INT8U addr[6] ={0};
		static INT8U temperateRefresh[CONS_DEVICE_MAX_COUNT]={1};

		GetAddr_6_Fix0(device.m_Addr,addr);
		commNo = bcdc(addr[0]);
		iData.push_back(0X00);
		iData.push_back(0X03);
		
		for (int n=0; n<MAX_COM_RETRY_TIME; n++)
		{
			rtData.clear();
			ret = ModbusRead(device,commNo,MODBUS_READ,0x0000,iData,rtData);
			if (ret == true)
			{
				INT16U value;
				value = (INT16U)(rtData.at(2)<<8|rtData.at(3));
				value = D2B(value);
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0] = (INT8U)((value>>8)&0xff);
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[1] = (INT8U)(value&0xff);

				value = (INT16U)(rtData.at(4)<<8|rtData.at(5));
				value = D2B(value);
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[2] = (INT8U)((value>>8)&0xff);
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[3] = (INT8U)(value&0xff);

				//CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[4] = 0x00;
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[5] = 0x00;
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[6] = 0x00;
//				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] = 0x00;			
				//CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[8] = 0x00;	

				/* ��ѯ�¶�*/
				time_t tt;
				struct tm *curtime;
				time(&tt);
				curtime=localtime(&tt);

				if (curtime->tm_min%2 == 0)//ÿ��2���ӻ��·���߻ָ�
				{
					memset(temperateRefresh,0x01,sizeof(temperateRefresh));
				}

				if (device.m_CommBreakTimes>=MAX_COM_RETRY_TIME)
				{
					temperateRefresh[device.m_Tn] = 1;
				}
				if (temperateRefresh[device.m_Tn] == 1)
				{
					iData.clear();
					iData.push_back(0X00);
					iData.push_back(0X01);
					rtData.clear();
					ret = ModbusRead(device,commNo,MODBUS_READ,0x000b,iData,rtData);
					if (ret == true)
					{
						INT16U value;
						value = (INT16U)(rtData.at(2)<<8|rtData.at(3));
						value = D2B(value);
						CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[4] = bcdc((INT8U)(value>>4&0xff));
					}
					temperateRefresh[device.m_Tn] = 0;
				
					/* ��ѯ״̬*/
					rtData.clear();
					ret = ModbusRead(device,commNo,MODBUS_READ,0x0005,iData,rtData);
					if (ret == true)
					{
						INT16U value;
						value = (INT16U)(rtData.at(2)<<8|rtData.at(3));
						KL_VERBOSE("��ѯ״̬=%x\n",value);
						if (value&0x0008)
						{
							CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] |= 0x02; 
						}
						else
						{
							CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] &= 0xfd; 
						}
							
					}
					
				}
				
				break;
			}
			else
			{
				if (device.m_CommBreakTimes <= MAX_COM_RETRY_TIME)
				{
					device.m_CommBreakTimes++;
				}
				else
				{
					break;
				}
			}
		}

		if (ret == true)
		{
			KL_VERBOSE("CRtfCmdItem_0501read sucess\n");
			//memcpy(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,rtData.data()+2,4);
			/* ͨѶ���߻ָ�*/
			if (device.m_CommBreakTimes>=MAX_COM_RETRY_TIME)
			{
				//CONTEXT.NorTasks.OnRefresh(device.m_ClRelayNo);

				{
					KL_VERBOSE("��·%d CPR%dͨѶ���߻ָ�\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//�澯���ݳ���
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 0;//���߻ָ�
					KL_VERBOSE("__________ALARM_CPR_ON_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
					time_t tt;
					time(&tt);
					device.m_ResumeOnlineRefreshTime = tt+60;//��Ϊ��Դ�ϵ���Ҫ60�����ܽ��е�ѹ���á�

					CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] &=0x7f;//bit7Ϊģ��ͨѶ����
					
				}
			}
			if (device.m_ResumeOnlineRefreshTime != 0)//ֻ�лָ�ͨѶ��60�룬�Żָ������ѹ����Ϊ��Դ�ϵ���Ҫ60�����ܽ��е�ѹ���á�
			{
				time_t tt;
				time(&tt);
				if (tt >device.m_ResumeOnlineRefreshTime )
				{
					KL_VERBOSE("���߳̿ص�ѹ�ָ�\n");
					CONTEXT.NorTasks.OnRefresh(device.m_ClRelayNo);
					device.m_ResumeOnlineRefreshTime = 0;
				}
				/*����Ǻ��ջ�·�����������߻ָ���������»�·off״̬���ں��ջ�·��ʱЧ��Χ�⣬��·�ѹرգ�����ʱ�Ȳ�����������������ϵ��ˣ�����ʱ���ջ�·�����ٴιرյ�����*/
				for (TaskBitSet::SizeType j = CONTEXT.ConsLuxTasks.m_ConsLuxValidTBS.FromPos; j<CONTEXT.ConsLuxTasks.m_ConsLuxValidTBS.ToPos; j++)
				{
					if (CONTEXT.ConsLuxTasks.m_ConsLuxValidTBS.test(j) == false)
					{
						continue;
					}
					SConsLuxTask &task = CONTEXT.ConsLuxTasks.m_Tasks[j];
					if (task.m_ClrelayNo == device.m_ClRelayNo)
					{
						task.m_ClearClOffFlag = true;
					}
				}
			}
			device.m_CommBreakTimes=0;
			KL_VERBOSE("Tn=%d ,CommBreakTimes = %d\n",device.m_Tn,device.m_CommBreakTimes);
			
		}
		else
		{
			KL_VERBOSE("Tn=%d ,CommBreakTimes = %d\n",device.m_Tn,device.m_CommBreakTimes);
			if (device.m_CommBreakTimes >= MAX_COM_RETRY_TIME)
			{
#ifndef WIN32			
				memset(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,0x00,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501)-2);//״̬�����
#endif
				if (device.m_CommBreakTimes == MAX_COM_RETRY_TIME)
				{
					KL_VERBOSE("��·%d CPR%dͨѶ����\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//�澯���ݳ���
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 1;//���߲���
					KL_VERBOSE("__________ALARM_CPR_OFF_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
					CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] |=0x80;//bit7Ϊģ��ͨѶ����
				}
			}
		}

		/* IPC�����ڴ����ݸ���*/
		memcpy(IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_Reg0501,CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501));
		IPCSHAREMEM.Update();
	}
	else//CANBUS Device
	{
		unsigned char addr[6];
		GetAddr_6_Fix0(device.m_Addr,addr);
		INT8U databuf[1] = {0x00};

		rtData.clear();
		ret = Write(0xAA,addr[0],CAN_CMD_READ_STATE, databuf,sizeof(databuf),rtData);
		if (ret ==true && rtData.size() == 8)
		{
			KL_INFO_DUMP(dump);
			dump<<"������0501 ��������rtData:"<<rtData<<"\n";
			int vol = (int)((rtData[0])<<8|(rtData[1]));
			int cur = (int)(rtData[2]<<8|rtData[3]);
			char val[2];

			/* ��ѹ */
			sprintf(val,"%d",vol);
			sscanf(val,"%x",&vol);
			
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0] = (INT8U)((vol>>8)&0xff);
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[1] = (INT8U)((vol)&0xff);

			/* ���� */
			sprintf(val,"%d",cur);
			sscanf(val,"%x",&cur);

			KL_VERBOSE("device.m_Tn=%d,vol =%x,cur=%x",device.m_Tn,vol,cur);
				
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[2] = (cur>>8)&0xff;
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[3] = (cur)&0xff;

			/* �¶� */
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[4] = rtData[5];


			/* ͨѶ���߻ָ�*/
			if (device.m_CommBreakTimes>=MAX_CAN_COM_RETRY_TIME)
			{
				CONTEXT.NorTasks.OnRefresh(device.m_ClRelayNo);
				{
					KL_VERBOSE("��·%d CPR%dͨѶ���߻ָ�\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//�澯���ݳ���
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 0;//���߻ָ�
					KL_VERBOSE("__________ALARM_CPR_ON_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
				}

				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] &=0x7f;//bit7Ϊģ��ͨѶ����
				
				/*����Ǻ��ջ�·�����������߻ָ���������»�·off״̬���ں��ջ�·��ʱЧ��Χ�⣬��·�ѹرգ�����ʱ�Ȳ�����������������ϵ��ˣ�����ʱ���ջ�·�����ٴιرյ�����*/
				for (TaskBitSet::SizeType j = CONTEXT.ConsLuxTasks.m_ConsLuxValidTBS.FromPos; j<CONTEXT.ConsLuxTasks.m_ConsLuxValidTBS.ToPos; j++)
				{
					if (CONTEXT.ConsLuxTasks.m_ConsLuxValidTBS.test(j) == false)
					{
						continue;
					}
					SConsLuxTask &task = CONTEXT.ConsLuxTasks.m_Tasks[j];
					if (task.m_ClrelayNo == device.m_ClRelayNo)
					{
						task.m_ClearClOffFlag = true;
					}
				}
			}
			
			device.m_CommBreakTimes=0;
		}
		else
		{
			if (device.m_CommBreakTimes <= MAX_CAN_COM_RETRY_TIME)
				device.m_CommBreakTimes++;
			
			if (device.m_CommBreakTimes>=MAX_CAN_COM_RETRY_TIME)
			{
#ifndef WIN32			
				memset(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,0x00,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501));
#endif 
				if (device.m_CommBreakTimes == MAX_CAN_COM_RETRY_TIME)
				{
					KL_VERBOSE("��·%d CPR%dͨѶ����\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//�澯���ݳ���
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 1;//���߲���
					KL_VERBOSE("__________ALARM_CPR_OFF_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
					CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] |=0x80;//bit7Ϊģ��ͨѶ����
				}
			}
		}

		/* ���߱��֣����ݹ�Լ�ֲᣬÿ5����Ҫ�����߱������������������ػ�*/
/*		INT8U data[5] = {0};
		data[0] = rtData[0];
		data[1] = rtData[1];
		data[2] = 0x00;//����20A
		data[3] = 0xc8;
		data[4] = 0x55;
		KL_VERBOSE("������%d���߱���\n",device.m_Tn);
		ret = Write(0xAA,addr[0],CAN_CMD_SET_PARAM, data,sizeof(data),rtData);*/


		
		/* IPC�����ڴ����ݸ���*/
		memcpy(IPCSHAREMEM.m_IpcDevicesRegData[device.m_Tn].m_Reg0501,CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501));
		IPCSHAREMEM.Update();
	}
	return 0;
}

int CRtfCmdItem_0601::Exec(Device &device,bstring &Data)
{
	INT8U ctl = 0x06; 
	bstring wtData = Data;
	bstring rtData;
	bool ret=false;

	if ( CONTEXT.m_DnProtocolType == DN_PROTOCOL_TYPE_RS485_A)
	{
		for(int n=0; n<3; n++)
		{
			rtData.clear();
			ret = Write(device,ctl,wtData,rtData);
			if (ret == true)
				break;
		}
		if (ret == false)
		{
			KL_VERBOSE("����������0601���ݱ�ʶ����ʧ��\n");
			return -1;
		}
		if (rtData.at(3) != 0)//дȷ��
		{
			KL_VERBOSE("����������0601���ݱ�ʶ����ʧ��\n");
			return -1;
		}
		else
		{
			KL_VERBOSE("����������0601���ݱ�ʶ���ݳɹ�\n");
			return 0;
		}
	}
	else if ( CONTEXT.m_DnProtocolType == DN_PROTOCOL_TYPE_RS485_B)
	{

		INT8U commNo;
		bstring iData;
		INT8U addr[6] ={0};
		GetAddr_6_Fix0(device.m_Addr,addr);
		commNo = bcdc(addr[0]);
		INT16U value = (INT16U)(wtData.at(0)<<8|wtData.at(1));
		INT16U vol = B2D(value);
		iData.push_back((vol>>8)&0xff);
		iData.push_back(vol&0xff);

		for(int n=0; n<3; n++)
		{
			rtData.clear();
			ret = ModbusRead(device,commNo,MODBUS_WRITE,0x0000,iData,rtData);
			if (ret == true)
				break;
		}
		
		if (ret == false)
		{
			KL_VERBOSE("����������0601���ݱ�ʶ����ʧ��\n");
			return -1;
		}
		else
		{
			KL_VERBOSE("����������0601���ݱ�ʶ���ݳɹ�\n");
			return 0;
		}
	}
	else
	{
		unsigned char addr[6];
		GetAddr_6_Fix0(device.m_Addr,addr);
		char val[2];
		INT16U vol,cur;
		val[0] = Data[1];
		val[1] = Data[0];
		vol = BcdToDec(2,(INT8U*)val);

//		val[0] = Data[3];
//		val[1] = Data[2];
//		cur = BcdToDec(2,(INT8U*)val);

		INT8U databuf[5] = {0x00};
		databuf[0] = (vol>>8)&0xff;
		databuf[1] = vol&0xff;
		//databuf[2] = (cur>>8)&0xff;
		//databuf[3] = cur&0xff;
		databuf[2] = 0x00;
		databuf[3] = 0xC8;
		databuf[4] = 0xAA;//����

		KL_INFO_DUMP(dump);
		bstring tempdata;
		tempdata.append(databuf,sizeof(databuf));
		KL_VERBOSE("�������%d\n",device.m_Tn);
		dump<<"����������0601����Ϊ"<<tempdata<<"\n";

		for (int i=0; i<3; i++)
		{
			rtData.clear();
			
			ret = Write(0xAA,addr[0],CAN_CMD_SET_PARAM, databuf,sizeof(databuf),rtData);
			if (ret ==true && rtData.size() == 8)
			{
				dump<<"����������0601��������Ϊ"<<rtData<<"\n";
				/* �����õĵ�ѹֵ�뷵�صĵ�ѹֵ����10V�����ٴ����õ�ѹ*/
				INT16U retVol = (INT16U)(rtData[0]<<8|rtData[1]);
				if (abs(vol-retVol) > 100)
				{
					KL_ERROR("��ѹ���ò�ֵΪ%dV,��ѹ���ò��ɹ�������.....\n",abs(vol-retVol));
					continue;
				}
				else
				{
					return 0;	
				}
			}
		}
	}
	KL_ERROR("����������0601���ݱ�ʶ����ʧ��\n");
	return -1;
}

int CRtfCmdItem_0602::Exec(Device &device,bstring &Data)
{
	INT8U ctl = 0x04; 
	bstring wtData = Data;
	bstring rtData;
	bool ret=false;

	if ( CONTEXT.m_DnProtocolType == DN_PROTOCOL_TYPE_RS485_A)
	{
		
		for(int n=0; n<3; n++)
		{
			rtData.clear();
			ret = Write(device,ctl,wtData,rtData);
			if (ret == true)
				break;
		}
		if (ret == false)
		{
			KL_VERBOSE("����������0602���ݱ�ʶ����ʧ��\n");
			return -1;
		}
		if (rtData.at(2) != 0x84)//дȷ��
		{
			KL_VERBOSE("����������0602���ݱ�ʶ����ʧ��\n");
			return -1;
		}
		return 0;
	}
	else if ( CONTEXT.m_DnProtocolType == DN_PROTOCOL_TYPE_RS485_B)
	{
		INT8U commNo;
		bstring iData;
		INT8U addr[6] ={0};
		GetAddr_6_Fix0(device.m_Addr,addr);
		commNo = bcdc(addr[0]);

		/* ���ػ���������*/
		if (Data[0] == 0x00)
		{
			iData.push_back(0x00);
			iData.push_back(0x00);//����
		}
		else if (Data[0] == 0x01)
		{
			iData.push_back(0x00);
			iData.push_back(0x01);//�ػ�
		}
		
		for(int n=0; n<3; n++)
		{
			rtData.clear();
			ret = ModbusRead(device,commNo,MODBUS_WRITE,0x0005,iData,rtData);
			if (ret == true)
				break;
		}
		if (ret == false)
		{
			KL_VERBOSE("����������0602���ݱ�ʶ����ʧ��\n");
			return -1;
		}
		else
		{
			KL_VERBOSE("����������0602���ݱ�ʶ���ݳɹ�\n");
		}
		return 0;
	}
	else
	{
		unsigned char addr[6];
		GetAddr_6_Fix0(device.m_Addr,addr);
		INT16U vol,cur;
		vol = 0;
		cur = 0;
/*		char val[2];
		INT16U vol,cur;
		val[0] = Data[0];
		val[1] = Data[1];
		vol = BcdToDec(2,(INT8U*)val);

		val[0] = Data[2];
		val[1] = Data[3];
		cur = BcdToDec(2,(INT8U*)val);*/

		

		/* ��ȡ��������ǰ��ѹ��Ϣ�����Ϳ����ء�����������ʱ��Ҫ�ѵ�ѹֵ������ȥ*/
		INT8U sdata[1] = {0x00};
		INT8U databuf[5] = {0x00};
		ret = Write(0xAA,addr[0],CAN_CMD_READ_STATE, sdata,sizeof(sdata),rtData);
		if (ret ==true && rtData.size() == 8)
		{
			databuf[0] = rtData[0];
			databuf[1] = rtData[1];
			databuf[2] = 0x00;//����20A
			databuf[3] = 0xc8;
		}

		/* ���ػ���������*/
		if (Data[0] == 0x00)
		{
			databuf[4] = 0xaa;//����
		}
		else if (Data[0] == 0x01)
		{
			databuf[4] = 0x00;//�ػ�
		}
		else
		{
			databuf[4] = 0x55;//����
		}
		
		KL_INFO_DUMP(dump);
		bstring tempdata;
		tempdata.append(databuf);
		dump<<"����������0602����Ϊ"<<tempdata<<"\n";

		rtData.clear();		
		ret = Write(0xAA,addr[0],CAN_CMD_SET_PARAM, databuf,sizeof(databuf),rtData);

		rtData.clear();		
		ret = Write(0xAA,addr[0],CAN_CMD_SET_PARAM, databuf,sizeof(databuf),rtData);

		if (ret ==true)
		{
			dump<<"����������0602��������Ϊ"<<rtData<<"\n";
			return 0;	
		}
		else
		{
			dump<<"����������0602����ʧ��"<<rtData<<"\n";
			return -1;	
		}
	}
}


