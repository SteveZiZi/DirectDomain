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
		KL_VERBOSE("已经超过系统定义的最大告警类型数量\n");
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
		KL_VERBOSE("总线获取失败\n");
		return false;
	}
	
	if (bRule->GetRtfData(device, oPackage.data(),oPackage.size(), rtData) == 0)
		return true;
	else
	{
		KL_ERROR("[!!!!!!!!! 帧回复失败 !!!!!!!!!!]");
		return false;
	}
	
}

/* can bus 读写*/
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
		KL_VERBOSE("总线获取失败\n");
		return false;
	}
	bstring rtData;
	if (bRule->GetRtfData(device, oPackage.data(),oPackage.size(), rtData) == 0)
	{
		return true;
	}
	else
	{
		KL_ERROR("[!!!!!!!!! 帧写回复失败 !!!!!!!!!!]\n");
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
		KL_VERBOSE("总线获取失败\n");
		return false;
	}
	
	if (bRule->GetRtfData(device, oPackage.data(),oPackage.size(), rtData) == 0)
	{
		return true;
	}
	else
	{
		KL_ERROR("[!!!!!!!!! 帧写回复失败 !!!!!!!!!!]\n");
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
		KL_VERBOSE("总线获取失败\n");
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

	/* IPC共享内存数据更新*/
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

/*		if (ret != true)//由于485方式总线收到的报文头字节易出现乱码，所以增加再次抄读措施
		{
			if (device.m_CommBreakTimes <= MAX_COM_RETRY_TIME)
				device.m_CommBreakTimes++;
			rtData.clear();
			ret = Read(device,ctl,rtData);	
		}*/

		if (ret == true)
		{
			memcpy(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,rtData.data()+4,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501));
			/* 如果是54V整流器，其电压格式为XX.XX，需转换为XXX.X格式*/
			if (CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0] > 0x50)
			{
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[1] = (INT8U)((CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0] &0x0f<<4)|CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[1]>>4);
				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0] = (INT8U)((CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0]>>4)&0X0f); 	
			}

			/* 通讯掉线恢复*/
			if (device.m_CommBreakTimes>=MAX_COM_RETRY_TIME)
			{
				CONTEXT.NorTasks.OnRefresh(device.m_ClRelayNo);

				{
					KL_VERBOSE("回路%d CPR%d通讯掉线恢复\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//告警内容长度
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 0;//掉线恢复
					KL_VERBOSE("__________ALARM_CPR_ON_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
				}

				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] &=0x7f;//bit7为模块通讯故障

				/*如果是恒照回路的整流器掉线恢复，则清除下回路off状态，在恒照回路在时效范围外，回路已关闭，但此时热插拔整流器，整流器上电了，但此时恒照回路不能再次关闭的问题*/
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
				memset(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,0x00,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501)-2);//状态不清除
#endif
				if (device.m_CommBreakTimes == MAX_COM_RETRY_TIME)
				{
					KL_VERBOSE("回路%d CPR%d通讯掉线\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//告警内容长度
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 1;//掉线产生
					KL_VERBOSE("__________ALARM_CPR_OFF_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);

					CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] |=0x80;//bit7为模块通讯故障
				}
			}
		}
		/* 特殊处理，目前把所有状态字置零*/
		CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] = 0x00;
		CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[8] = 0x00;

		/* IPC共享内存数据更新*/
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

				/* 查询温度*/
				time_t tt;
				struct tm *curtime;
				time(&tt);
				curtime=localtime(&tt);

				if (curtime->tm_min%2 == 0)//每个2分钟或回路掉线恢复
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
				
					/* 查询状态*/
					rtData.clear();
					ret = ModbusRead(device,commNo,MODBUS_READ,0x0005,iData,rtData);
					if (ret == true)
					{
						INT16U value;
						value = (INT16U)(rtData.at(2)<<8|rtData.at(3));
						KL_VERBOSE("查询状态=%x\n",value);
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
			/* 通讯掉线恢复*/
			if (device.m_CommBreakTimes>=MAX_COM_RETRY_TIME)
			{
				//CONTEXT.NorTasks.OnRefresh(device.m_ClRelayNo);

				{
					KL_VERBOSE("回路%d CPR%d通讯掉线恢复\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//告警内容长度
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 0;//掉线恢复
					KL_VERBOSE("__________ALARM_CPR_ON_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
					time_t tt;
					time(&tt);
					device.m_ResumeOnlineRefreshTime = tt+60;//因为电源上电需要60秒后才能进行电压设置。

					CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] &=0x7f;//bit7为模块通讯故障
					
				}
			}
			if (device.m_ResumeOnlineRefreshTime != 0)//只有恢复通讯达60秒，才恢复任务电压，因为电源上电需要60秒后才能进行电压设置。
			{
				time_t tt;
				time(&tt);
				if (tt >device.m_ResumeOnlineRefreshTime )
				{
					KL_VERBOSE("上线程控电压恢复\n");
					CONTEXT.NorTasks.OnRefresh(device.m_ClRelayNo);
					device.m_ResumeOnlineRefreshTime = 0;
				}
				/*如果是恒照回路的整流器掉线恢复，则清除下回路off状态，在恒照回路在时效范围外，回路已关闭，但此时热插拔整流器，整流器上电了，但此时恒照回路不能再次关闭的问题*/
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
				memset(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501,0x00,sizeof(CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501)-2);//状态不清除
#endif
				if (device.m_CommBreakTimes == MAX_COM_RETRY_TIME)
				{
					KL_VERBOSE("回路%d CPR%d通讯掉线\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//告警内容长度
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 1;//掉线产生
					KL_VERBOSE("__________ALARM_CPR_OFF_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
					CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] |=0x80;//bit7为模块通讯故障
				}
			}
		}

		/* IPC共享内存数据更新*/
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
			dump<<"数据项0501 返回数据rtData:"<<rtData<<"\n";
			int vol = (int)((rtData[0])<<8|(rtData[1]));
			int cur = (int)(rtData[2]<<8|rtData[3]);
			char val[2];

			/* 电压 */
			sprintf(val,"%d",vol);
			sscanf(val,"%x",&vol);
			
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[0] = (INT8U)((vol>>8)&0xff);
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[1] = (INT8U)((vol)&0xff);

			/* 电流 */
			sprintf(val,"%d",cur);
			sscanf(val,"%x",&cur);

			KL_VERBOSE("device.m_Tn=%d,vol =%x,cur=%x",device.m_Tn,vol,cur);
				
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[2] = (cur>>8)&0xff;
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[3] = (cur)&0xff;

			/* 温度 */
			CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[4] = rtData[5];


			/* 通讯掉线恢复*/
			if (device.m_CommBreakTimes>=MAX_CAN_COM_RETRY_TIME)
			{
				CONTEXT.NorTasks.OnRefresh(device.m_ClRelayNo);
				{
					KL_VERBOSE("回路%d CPR%d通讯掉线恢复\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//告警内容长度
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 0;//掉线恢复
					KL_VERBOSE("__________ALARM_CPR_ON_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
				}

				CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] &=0x7f;//bit7为模块通讯故障
				
				/*如果是恒照回路的整流器掉线恢复，则清除下回路off状态，在恒照回路在时效范围外，回路已关闭，但此时热插拔整流器，整流器上电了，但此时恒照回路不能再次关闭的问题*/
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
					KL_VERBOSE("回路%d CPR%d通讯掉线\n",device.m_ClRelayNo,device.m_Tn);
					char bcdtime7[7];
					INT8U AlarmData[4];
					Get_CurBCDTime7(bcdtime7);
					AlarmData[0] = 3;//告警内容长度
					AlarmData[1] = device.m_ClRelayNo;
					AlarmData[2] = bcdc(device.m_Tn);
					AlarmData[3] = 1;//掉线产生
					KL_VERBOSE("__________ALARM_CPR_OFF_LINE_STATE_________\n");
					AlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),ALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
//					ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CPR_ONLINE_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
					CONTEXT.m_DevicesRegData.m_Items[device.m_Tn].mReg_0501[7] |=0x80;//bit7为模块通讯故障
				}
			}
		}

		/* 在线保持，根据规约手册，每5秒内要发在线保持命令否则整流器会关机*/
/*		INT8U data[5] = {0};
		data[0] = rtData[0];
		data[1] = rtData[1];
		data[2] = 0x00;//限流20A
		data[3] = 0xc8;
		data[4] = 0x55;
		KL_VERBOSE("测量点%d在线保持\n",device.m_Tn);
		ret = Write(0xAA,addr[0],CAN_CMD_SET_PARAM, data,sizeof(data),rtData);*/


		
		/* IPC共享内存数据更新*/
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
			KL_VERBOSE("设置整流器0601数据标识内容失败\n");
			return -1;
		}
		if (rtData.at(3) != 0)//写确认
		{
			KL_VERBOSE("设置整流器0601数据标识内容失败\n");
			return -1;
		}
		else
		{
			KL_VERBOSE("设置整流器0601数据标识内容成功\n");
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
			KL_VERBOSE("设置整流器0601数据标识内容失败\n");
			return -1;
		}
		else
		{
			KL_VERBOSE("设置整流器0601数据标识内容成功\n");
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
		databuf[4] = 0xAA;//开机

		KL_INFO_DUMP(dump);
		bstring tempdata;
		tempdata.append(databuf,sizeof(databuf));
		KL_VERBOSE("测量点号%d\n",device.m_Tn);
		dump<<"设置数据项0601数据为"<<tempdata<<"\n";

		for (int i=0; i<3; i++)
		{
			rtData.clear();
			
			ret = Write(0xAA,addr[0],CAN_CMD_SET_PARAM, databuf,sizeof(databuf),rtData);
			if (ret ==true && rtData.size() == 8)
			{
				dump<<"设置数据项0601返回数据为"<<rtData<<"\n";
				/* 当设置的电压值与返回的电压值相差超过10V，将再次设置电压*/
				INT16U retVol = (INT16U)(rtData[0]<<8|rtData[1]);
				if (abs(vol-retVol) > 100)
				{
					KL_ERROR("电压设置差值为%dV,电压设置不成功，重试.....\n",abs(vol-retVol));
					continue;
				}
				else
				{
					return 0;	
				}
			}
		}
	}
	KL_ERROR("设置整流器0601数据标识内容失败\n");
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
			KL_VERBOSE("设置整流器0602数据标识内容失败\n");
			return -1;
		}
		if (rtData.at(2) != 0x84)//写确认
		{
			KL_VERBOSE("设置整流器0602数据标识内容失败\n");
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

		/* 开关机命令设置*/
		if (Data[0] == 0x00)
		{
			iData.push_back(0x00);
			iData.push_back(0x00);//开机
		}
		else if (Data[0] == 0x01)
		{
			iData.push_back(0x00);
			iData.push_back(0x01);//关机
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
			KL_VERBOSE("设置整流器0602数据标识内容失败\n");
			return -1;
		}
		else
		{
			KL_VERBOSE("设置整流器0602数据标识内容成功\n");
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

		

		/* 获取整流器当前电压信息，发送开、关、机保持命令时需要把电压值设置下去*/
		INT8U sdata[1] = {0x00};
		INT8U databuf[5] = {0x00};
		ret = Write(0xAA,addr[0],CAN_CMD_READ_STATE, sdata,sizeof(sdata),rtData);
		if (ret ==true && rtData.size() == 8)
		{
			databuf[0] = rtData[0];
			databuf[1] = rtData[1];
			databuf[2] = 0x00;//限流20A
			databuf[3] = 0xc8;
		}

		/* 开关机命令设置*/
		if (Data[0] == 0x00)
		{
			databuf[4] = 0xaa;//开机
		}
		else if (Data[0] == 0x01)
		{
			databuf[4] = 0x00;//关机
		}
		else
		{
			databuf[4] = 0x55;//保持
		}
		
		KL_INFO_DUMP(dump);
		bstring tempdata;
		tempdata.append(databuf);
		dump<<"设置数据项0602数据为"<<tempdata<<"\n";

		rtData.clear();		
		ret = Write(0xAA,addr[0],CAN_CMD_SET_PARAM, databuf,sizeof(databuf),rtData);

		rtData.clear();		
		ret = Write(0xAA,addr[0],CAN_CMD_SET_PARAM, databuf,sizeof(databuf),rtData);

		if (ret ==true)
		{
			dump<<"设置数据项0602返回数据为"<<rtData<<"\n";
			return 0;	
		}
		else
		{
			dump<<"设置数据项0602设置失败"<<rtData<<"\n";
			return -1;	
		}
	}
}


