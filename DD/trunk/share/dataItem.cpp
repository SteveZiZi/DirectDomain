#include "dataItem.h"
#include "../brule/BusRuleFactory.h"
#include "../base/klb_base.h"
#include "../base/klb_io.h"


void CDataItems::Init(void)
{
	if (m_count !=0 )
	{
		KL_VERBOSE("CDataItems::Init Finished\n");
	}
	m_count = 0;

	CDataItem_0110 *pDataItem_0110 = new CDataItem_0110();
	AddDataItem(pDataItem_0110);

	CDataItem_09XX *pDataItem_09XX = new CDataItem_09XX();
	AddDataItem(pDataItem_09XX);

	CDataItem_1000 *pDataItem_1000 = new CDataItem_1000();
	AddDataItem(pDataItem_1000);

	CDataItem_1001 *pDataItem_1001 = new CDataItem_1001();
	AddDataItem(pDataItem_1001);

	CDataItem_1004 *pDataItem_1004 = new CDataItem_1004();
	AddDataItem(pDataItem_1004);

	CDataItem_1030 *pDataItem_1030 = new CDataItem_1030();
	AddDataItem(pDataItem_1030);

	CDataItem_1031 *pDataItem_1031 = new CDataItem_1031();
	AddDataItem(pDataItem_1031);

	CDataItem_1032 *pDataItem_1032 = new CDataItem_1032();
	AddDataItem(pDataItem_1032);

	CDataItem_1035 *pDataItem_1035 = new CDataItem_1035();
	AddDataItem(pDataItem_1035);

	CDataItem_1101 *pDataItem_1101 = new CDataItem_1101();
	AddDataItem(pDataItem_1101);

	CDataItem_1102 *pDataItem_1102 = new CDataItem_1102();
	AddDataItem(pDataItem_1102);

	CDataItem_1105 *pDataItem_1105 = new CDataItem_1105();
	AddDataItem(pDataItem_1105);

	CDataItem_1106 *pDataItem_1106 = new CDataItem_1106();
	AddDataItem(pDataItem_1106);

	CDataItem_1107 *pDataItem_1107 = new CDataItem_1107();
	AddDataItem(pDataItem_1107);

	CDataItem_1133 *pDataItem_1133 = new CDataItem_1133();
	AddDataItem(pDataItem_1133);

	CDataItem_1144 *pDataItem_1144 = new CDataItem_1144();
	AddDataItem(pDataItem_1144);

	CDataItem_1210 *pDataItem_1210 = new CDataItem_1210();
	AddDataItem(pDataItem_1210);

	CDataItem_2010 *pDataItem_2010 = new CDataItem_2010();
	AddDataItem(pDataItem_2010);

	CDataItem_2011 *pDataItem_2011 = new CDataItem_2011();
	AddDataItem(pDataItem_2011);
	
	CDataItem_2110 *pDataItem_2110 = new CDataItem_2110();
	AddDataItem(pDataItem_2110);

	CDataItem_3001 *pDataItem_3001 = new CDataItem_3001();
	AddDataItem(pDataItem_3001);

	CDataItem_3002 *pDataItem_3002 = new CDataItem_3002();
	AddDataItem(pDataItem_3002);

	CDataItem_3003 *pDataItem_3003 = new CDataItem_3003();
	AddDataItem(pDataItem_3003);

	CDataItem_3004 *pDataItem_3004 = new CDataItem_3004();
	AddDataItem(pDataItem_3004);

	CDataItem_3005 *pDataItem_3005 = new CDataItem_3005();
	AddDataItem(pDataItem_3005);

	CDataItem_3007 *pDataItem_3007 = new CDataItem_3007();
	AddDataItem(pDataItem_3007);

	CDataItem_3008 *pDataItem_3008 = new CDataItem_3008();
	AddDataItem(pDataItem_3008);

	CDataItem_3009 *pDataItem_3009 = new CDataItem_3009();
	AddDataItem(pDataItem_3009);

	CDataItem_3010 *pDataItem_3010 = new CDataItem_3010();
	AddDataItem(pDataItem_3010);

	CDataItem_3011 *pDataItem_3011 = new CDataItem_3011();
	AddDataItem(pDataItem_3011);

	CDataItem_3101 *pDataItem_3101 = new CDataItem_3101();
	AddDataItem(pDataItem_3101);

	CDataItem_3102 *pDataItem_3102 = new CDataItem_3102();
	AddDataItem(pDataItem_3102);

	CDataItem_3103 *pDataItem_3103 = new CDataItem_3103();
	AddDataItem(pDataItem_3103);

	CDataItem_3104 *pDataItem_3104 = new CDataItem_3104();
	AddDataItem(pDataItem_3104);

	CDataItem_3105 *pDataItem_3105 = new CDataItem_3105();
	AddDataItem(pDataItem_3105);

	CDataItem_4001 *pDataItem_4001 = new CDataItem_4001();
	AddDataItem(pDataItem_4001);

	CDataItem_C343 *pDataItem_C343 = new CDataItem_C343();
	AddDataItem(pDataItem_C343);

}
/*
	brief: get alarm type according to alarm name
	param [in] string alarmName: alarm name
	return CAlarm*  
	author:WanQingsong
	date: 2012-04-09
*/  
CDataItem* CDataItems::GetDataItem(INT16U dataId)
{
	for (int i=0; i<m_count; i++)
	{
		if (m_pDataItems[i]->GetDataItemID() == dataId)
		{
			return m_pDataItems[i];
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
bool CDataItems::AddDataItem(CDataItem *pItem)
{
	if (m_count > CONS_DATAITEM_COUNT_MAX)
	{
		KL_VERBOSE("已经超过系统定义的最大告警类型数量\n");
		return false;
	}
	m_pDataItems[m_count++] = pItem;
	return true;
}


bool CDataItem::Read(Device device,bstring &rtData)
{
	bstring oPackage;
	MakePackage(device,0x01,m_dataId,NULL,0,oPackage);

	BusChannelsRule* bRule= GDBusRuleFactory::GetBusRule(DBCJ_1_BUSNO_485);
	if (bRule == NULL)
	{
		KL_VERBOSE("总线获取失败\n");
		return false;
	}
	
	if (bRule->GetDeviceData(device, oPackage.data(),oPackage.size(), rtData) == 0)
		return true;
	else
	{
		KL_ERROR("[!!!!!!!!! 帧回复失败 !!!!!!!!!!]");
		return false;
	}
	
}

bool CDataItem::Read(INT8U busNo,Device device,bstring &rtData)
{
	bstring oPackage;
	MakePackage(device,0x01,m_dataId,NULL,0,oPackage);

	BusChannelsRule* bRule= GDBusRuleFactory::GetBusRule(busNo);
	if (bRule == NULL)
	{
		KL_VERBOSE("总线获取失败\n");
		return false;
	}
	
	if (bRule->GetDeviceData(device, oPackage.data(),oPackage.size(), rtData) == 0)
		return true;
	else
	{
		KL_ERROR("[!!!!!!!!! 帧回复失败 !!!!!!!!!!]");
		return false;
	}
	
}


bool CDataItem::ModbusRead(Device device,INT8U commNo,INT8U ctl,INT16U dataid,bstring iData,bstring &rtData)
{
	bstring oPackage;
	MakeModbusPackage(commNo,ctl,dataid,iData.data(),iData.size(),oPackage);

	BusChannelsRule* bRule= GDBusRuleFactory::GetBusRule(DBCJ_2_BUSNO_485);
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
bool CDataItem::ModbusWrite(Device device,INT8U commNo,INT8U ctl,INT16U dataid,bstring iData,bstring &rtData)
{
	bstring oPackage;
	MakeModbusPackage(commNo,0x06,dataid,iData.data(),iData.size(),oPackage);

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
bool CDataItem::Write(Device device,INT16U dataid,bstring wtData)
{
	bstring oPackage;
	bstring rtData;
	MakePackage(device,0x04,dataid,wtData.data(),(INT8U)wtData.size(),oPackage);

	BusChannelsRule* bRule= GDBusRuleFactory::GetBusRule(DBCJ_1_BUSNO_485);
	if (bRule == NULL)
	{
		KL_VERBOSE("总线获取失败\n");
		return false;
	}
	
	if (bRule->GetDeviceData(device, oPackage.data(),oPackage.size(), rtData) == 0)
		return true;
	else
	{
		KL_ERROR("[!!!!!!!!! 帧回复失败 !!!!!!!!!!]");
		return false;
	}	
	
}

bool CDataItem::Write(Device device,INT16U dataid,bstring wtData,bstring &rtData)
{
	bstring oPackage;
	MakePackage(device,0x04,dataid,wtData.data(),(INT8U)wtData.size(),oPackage);

	BusChannelsRule* bRule= GDBusRuleFactory::GetBusRule(DBCJ_1_BUSNO_485);
	if (bRule == NULL)
	{
		KL_VERBOSE("总线获取失败\n");
		return false;
	}
	
	if (bRule->GetDeviceData(device, oPackage.data(),oPackage.size(), rtData) == 0)
		return true;
	else
	{
		KL_ERROR("[!!!!!!!!! 帧写回复失败 !!!!!!!!!!]\n");
		return false;
	}
	
}


int CDataItem::MakePackage(Device device,INT8U ctl,INT16U dataid, const INT8U* iData,const INT8U iDataLen,bstring& oPackage)
{
	unsigned char addr[6];
	oPackage.clear();

	oPackage.push_back(0x68);

	GetAddr_6_Fix0(device.m_Addr,addr);
	for (int i=0;i<6;i++)
	{
		oPackage.push_back(addr[i]);
	}
	oPackage.push_back(0x68);
	oPackage.push_back(ctl);
	oPackage.push_back(iDataLen+2);
	oPackage.push_back(INT8U(dataid&0x00FF));
	oPackage.push_back(INT8U((dataid&0xFF00)>>8));
	for (int i=0; i<iDataLen; i++)
	{
		oPackage.push_back(iData[i]);
	}

	

	INT8U checkSum=0;
	INT8U curLen=12;
	for (int i=0;i<curLen+iDataLen;i++)
	{
		checkSum+=oPackage[i];
	}
	oPackage.push_back(checkSum);
	oPackage.push_back(0x16);

	oPackage.insert(oPackage.begin(),0x7e);
	oPackage.insert(oPackage.begin(),0x7f);

	return 0;
}

int CDataItem::MakeModbusPackage(INT8U commNo,INT8U ctl,INT16U dataid, const INT8U* iData,const INT8U iDataLen,bstring& oPackage)
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

int CDataItem_1001::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1001;
	bstring wtData;

	wtData.push_back(Data[0]);//第几路
	wtData.push_back(Data[1]);//通或断

	for (int i=0; i<2; i++)
	{
		if (Write(device,dataid,wtData) == true)
		{
			return 0;
		}
	}
	return -1;
}
int CDataItem_1004::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1004;
	bstring wtData;

	wtData.push_back(Data[0]);//第几路
	wtData.push_back(Data[1]);//通或断
 	wtData.push_back(Data[2]);//持续时间

	if (Write(device,dataid,wtData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CDataItem_1030::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1030;
	bstring wtData;

	wtData.push_back(Data[0]);//第几路
	wtData.push_back(0x01);//调亮一级

	if (Write(device,dataid,wtData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

int CDataItem_1031::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1031;
	bstring wtData;

	wtData.push_back(Data[0]);//第几路
	wtData.push_back(0x01);//调暗一级

	if (Write(device,dataid,wtData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CDataItem_1032::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1032;
	bstring wtData;

	wtData.push_back(Data[0]);//第几路
	wtData.push_back(0x01);//调暗一级

	if (Write(device,dataid,wtData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CDataItem_1035::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1035;
	bstring wtData;

	wtData.push_back(Data[0]);//第几路
	wtData.push_back(Data[1]);//亮度基数

	if (Write(device,dataid,wtData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


int CDataItem_1101::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1101;
	bstring wtData;
	bstring rtData;
	INT8U subno;
	INT8U funcType;
	INT8U keyState;
	
	subno = Data[0];//按键将要控制设备的子路号
	funcType = Data[1];//按键的特殊功能
	keyState = Data[3];
	
	if (device.m_Type == MD_LIGHT_4 ||device.m_Type == MD_LEDV12_3 || device.m_Type == MD_RELAY_8)
	{
		if (funcType == FUNC_NORMAL)
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(keyState);
		}

		if (funcType == FUNC_RELAY_ON)
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(2);//继电器通
		}

		if (funcType == FUNC_RELAY_OFF)
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(3);//继电器断
		}


		if (funcType == FUNC_LIGHT_UP)
		{
			dataid = 0x1030;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_LIGHT_DOWN)
		{
			dataid = 0x1031;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_LIGHT_AUTO)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_LIGHT_AUTO_UP)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_LIGHT_AUTO_DOWN)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_WINDOW_OPEN)//窗帘开
		{
			dataid = 0x1002;
			wtData.push_back(subno);
			wtData.push_back(0x01);
			wtData.push_back(STATUS.deviceStatus.m_WindowRunTime);//运行时间
			wtData.push_back(0x01);//窗编号
		}

		if (funcType == FUNC_WINDOW_CLOSE)//窗帘关
		{
			dataid = 0x1003;
			wtData.push_back(subno);
			wtData.push_back(0x01);
			wtData.push_back(STATUS.deviceStatus.m_WindowRunTime);//运行时间
			wtData.push_back(0x01);//窗编号

		}

		if (funcType == FUNC_NIGHT_LIGHT)
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			if (STATUS.deviceStatus.m_NightLightState == 0)
			{
				STATUS.deviceStatus.m_NightLightState = 1;
				wtData.push_back(2);//继电器通
				/* 打开夜灯指示灯*/
				#ifndef WIN32		
					int fd,sw;
					fd = open("/dev/kgeio", O_RDWR );   
					if (fd == -1) 
					{	
						KL_VERBOSE(" open /dev/kgeio fail\n");
					}

					sw = NIGHTLIGHT_INDECATION_ON;
					ioctl(fd, KGESG_IO_SET_NIGHTLIGHT, &sw );
					close(fd);	
					KL_VERBOSE("夜灯指示灯打开\n");
				#endif
			}
			else
			{
				STATUS.deviceStatus.m_NightLightState = 0;
				wtData.push_back(3);//继电器断
				/* 关闭夜灯指示灯*/
				#ifndef WIN32		
					int fd,sw;
					fd = open("/dev/kgeio", O_RDWR );   
					if (fd == -1) 
					{	
						KL_VERBOSE(" open /dev/kgeio fail\n");
					}

					sw = NIGHTLIGHT_INDECATION_OFF;
					ioctl(fd, KGESG_IO_SET_NIGHTLIGHT, &sw );
					close(fd);	
					KL_VERBOSE("夜灯指示灯关闭\n");
				#endif
			}
		}
		Write(device,dataid,wtData,rtData);

		if (rtData.size() == 3)//返回继电器当前状态
		{
			return (int)(rtData.at(2));
		}
	}
	return 0;
}

int CDataItem_1102::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1102;
	bstring wtData;
	INT8U subno;
	INT8U funcType;
	INT8U keyState;
	
	subno = Data[0];//按键将要控制设备的子路号
	funcType = Data[1];//按键的特殊功能
	keyState = Data[3];
	
	if (device.m_Type == MD_LIGHT_4 || device.m_Type == MD_LEDV12_3)
	{
		if (funcType == FUNC_LIGHT_AUTO)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1032;
			wtData.push_back(subno);
			wtData.push_back(keyState);
		}
		if (funcType == FUNC_LIGHT_AUTO_UP)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1033;
			wtData.push_back(subno);
			wtData.push_back(keyState);
		}
		if (funcType == FUNC_LIGHT_AUTO_DOWN)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1034;
			wtData.push_back(subno);
			wtData.push_back(keyState);
		}

		
	}

	Write(device,dataid,wtData);
	return 0;

}

int CDataItem_1105::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1105;
	bstring wtData;
	bstring rtData;
	INT8U subno;
	INT8U funcType;
	INT8U keyState;
	
	subno = Data[0];//按键将要控制设备的子路号
	funcType = Data[1];//按键的特殊功能
	keyState = Data[3];
	
	if (device.m_Type == MD_LIGHT_4 ||device.m_Type == MD_LEDV12_3 || device.m_Type == MD_RELAY_8)
	{
		if (funcType == FUNC_NORMAL)
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(keyState);
		}

		if (funcType == FUNC_RELAY_ON)
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(2);//继电器通
		}

		if (funcType == FUNC_RELAY_OFF)
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(3);//继电器断
		}


		if (funcType == FUNC_LIGHT_UP)
		{
			dataid = 0x1030;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_LIGHT_DOWN)
		{
			dataid = 0x1031;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_LIGHT_AUTO)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_LIGHT_AUTO_UP)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_LIGHT_AUTO_DOWN)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1001;
			wtData.push_back(subno);
			wtData.push_back(0x01);
		}

		if (funcType == FUNC_WINDOW_OPEN)//窗帘开
		{
			dataid = 0x1002;
			wtData.push_back(subno);
			wtData.push_back(0x01);
			wtData.push_back(STATUS.deviceStatus.m_WindowRunTime);//运行时间
			wtData.push_back(0x01);//窗编号
		}

		if (funcType == FUNC_WINDOW_CLOSE)//窗帘关
		{
			dataid = 0x1003;
			wtData.push_back(subno);
			wtData.push_back(0x01);
			wtData.push_back(STATUS.deviceStatus.m_WindowRunTime);//运行时间
			wtData.push_back(0x01);//窗编号

		}

		
		Write(device,dataid,wtData,rtData);

		if (rtData.size() == 3)//返回继电器当前状态
		{
			return (int)(rtData.at(2));
		}
	}
	return 0;
}

int CDataItem_1106::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x1106;
	bstring wtData;
	INT8U subno;
	INT8U funcType;
	INT8U keyState;
	
	subno = Data[0];//按键将要控制设备的子路号
	funcType = Data[1];//按键的特殊功能
	keyState = Data[3];
	
	if (device.m_Type == MD_LIGHT_4 || device.m_Type == MD_LEDV12_3)
	{
		if (funcType == FUNC_LIGHT_AUTO)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1032;
			wtData.push_back(subno);
			wtData.push_back(keyState);
		}
		if (funcType == FUNC_LIGHT_AUTO_UP)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1033;
			wtData.push_back(subno);
			wtData.push_back(keyState);
		}
		if (funcType == FUNC_LIGHT_AUTO_DOWN)//一键控制，当短按时则开或关继电器
		{
			dataid = 0x1034;
			wtData.push_back(subno);
			wtData.push_back(keyState);
		}

		
	}

	if (Write(device,dataid,wtData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CDataItem_1107::Exec(Device &device,bstring &Data)
{
	return 0;
}
int CDataItem_1133::Exec(Device &device,bstring &Data)
{
	INT16U dataid = GetDataItemID(); ;
	bstring wtData;
	wtData.push_back(Data[0]);//键盘背光亮度

	if (Write(device,dataid,wtData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}	
}

int CDataItem_1144::Exec(Device &device,bstring &Data)
{
	INT16U dataid = GetDataItemID(); ;
	bstring wtData;
	wtData.push_back(Data[0]);//键盘路数
	wtData.push_back(Data[1]);//背光状态

	if (Write(device,dataid,wtData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}	
}


/*设置设备时间*/
int CDataItem_1210::Exec(Device &device,bstring &Data)
{
	INT16U dataid = GetDataItemID();
	bstring wtData;
	
	char bTime[7];
	Get_CurBCDTime7(bTime);
	wtData.push_back(bTime[6]);
	wtData.push_back(bTime[5]);
	wtData.push_back(bTime[4]);
	wtData.push_back(bTime[3]);
	wtData.push_back(bTime[2]);
	wtData.push_back(bTime[1]);

	if (Write(device,dataid,wtData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


/*设置继电器映射表*/
int CDataItem_2010::Exec(Device &device,bstring &Data)
{
	INT16U dataid = GetDataItemID();
	if (Write(device,dataid,Data) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

/*设置继电器映射表*/
int CDataItem_2011::Exec(Device &device,bstring &Data)
{
	INT16U dataid = GetDataItemID();

	if (Write(device,dataid,Data) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
//门卡场景组合处理
void CDataItem_2110::DoorcardCompose(INT8U state)
{
	if (state == 1)
	{
		SSceneCompose &SceneCompose = STATUS.deviceStatus.m_DoorcardInSceneComposes;
		for(int i=0; i<SceneCompose.m_MemberNum; i++)
		{
			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_LIGHT_LEVEL)
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1035);
				if (pDataItem == NULL)
				{
					return;
				}
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路

				//门卡插入，白天只量20%,晚上亮70%
				if (STATUS.deviceStatus.IsNight() == true)
				{
					tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]+5);//亮度
				}
				else
				{
					tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//亮度
				}
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			//夜间插门卡打开夜灯
			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_NIGHT_LIGHT)
			{
				if (STATUS.deviceStatus.IsNight() == true)
				{
					CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
					if (pDataItem == NULL)
					{
						return;
					}
					
					bstring tempData;
					tempData.clear();
					tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
					tempData.push_back(2);//开
					
					int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
					/* 打开夜灯指示灯*/
					#ifndef WIN32		
						int fd,sw;
						fd = open("/dev/kgeio", O_RDWR );   
						if (fd == -1) 
						{	
							KL_VERBOSE(" open /dev/kgeio fail\n");
						}

						sw = NIGHTLIGHT_INDECATION_ON;
						ioctl(fd, KGESG_IO_SET_NIGHTLIGHT, &sw );
						close(fd);	
					#endif
				}
			}
			
			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_NORMAL)
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
				tempData.push_back(2);//开
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_OFF)//总电源按键只关闭继电器
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
				tempData.push_back(3);//关
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_ON)//总电源按键只关闭继电器
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
				tempData.push_back(2);//开
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

		}

		SceneCompose.m_ComposeState = state;
	}
	else
	{
		SSceneCompose &SceneCompose = STATUS.deviceStatus.m_DoorcardOutSceneComposes;
		
		for(int i=0; i<SceneCompose.m_MemberNum; i++)
		{

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_LIGHT_LEVEL)
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1035);
				if (pDataItem == NULL)
				{
					return;
				}
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
				tempData.push_back(0);//亮度0级表示关闭
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_NORMAL)
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
				tempData.push_back(3);//关
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_OFF)//总电源按键只关闭继电器
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
				tempData.push_back(3);//关
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_ON)//总电源按键只关闭继电器
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
				tempData.push_back(2);//关
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}
		}
		SceneCompose.m_ComposeState = state;
	}
}


/* 门卡检测*/
static int card_state=0;//数值1表示插入，0未插入
int CDataItem_2110::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x2110; 
	bstring wtData;
	Device tempdev;

	if (Data[0] == card_state)//状态无变化
	{
		return -1;
	}

	if (Data[0] == 1)//插入
	{
		//打开键盘背光浅亮
		card_state = 1;
		STATUS.deviceStatus.m_RcuStatus.m_DoorCardState = 1;
/*		for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
		{
			 if (CONTEXT.Devices.m_DBS.test(i) != true)
			 {
				continue;
			 }
			 tempdev = CONTEXT.Devices.m_Item[i];
			 if (tempdev.m_Type ==  MD_KEYBOARD_20)
			 {
				 CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1133);
				 wtData.clear();
				 wtData.push_back(0x01);//打开
				 pDataItem->Exec(tempdev,wtData);
			 }
		}*/

		//打开廊灯
		/* if (device.m_Type ==  MD_RFID_CARD)
		 {
			for (int j=0; j<device.m_MapItemCount; j++)
			{
				if (device.m_MapItems[j].m_funcType == FUNC_GALLERY_LIGHT)//廊灯控制
				{
 					for(NoBitSet::SizeType st = device.m_MapItems[j].m_mapNbs.FromPos; st != device.m_MapItems[j].m_mapNbs.ToPos; st++)
					{
						if (device.m_MapItems[j].m_mapNbs.test(st) )
						{
							CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
							wtData.clear();
							wtData.push_back((INT8U)st);//按键控制的子路号
							wtData.push_back(2);//开
							
							 pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[j].m_mapTn],wtData);
 							 STATUS.deviceStatus.m_GalleryLightState = 1;
						}
					}
				}	
			 }
		}*/
		DoorcardCompose(1);

		/* 12路输入8路输出继电器板键盘和调光板键盘解锁*/
		for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
		{
			 if (CONTEXT.Devices.m_DBS.test(i) != true)
			 {
				continue;
			 }
			 tempdev = CONTEXT.Devices.m_Item[i];
			 if (tempdev.m_Type ==  MD_RELAY_8 
			 	|| tempdev.m_Type ==  MD_LIGHT_4 
			 	|| tempdev.m_Type ==  MD_RELAY_2
			 	|| tempdev.m_Type ==  MD_LEDV12_3)
			 {
				 CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x2011);
				 wtData.clear();
				 wtData.push_back(0x00);//打开
				 pDataItem->Exec(tempdev,wtData);
				 KL_VERBOSE("设备%d 键盘解锁\n",tempdev.m_Tn);
			 }
		}
		/* 门牌提示控制*/
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
			{
				 if (CONTEXT.Devices.m_DBS.test(i) != true)
				 {
					continue;
				 }
				 tempdev = CONTEXT.Devices.m_Item[i];
				 if (tempdev.m_Type ==  MD_DOORDISPLAY)
				 {
					//门铃背光亮
					 CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3102);
					 wtData.clear();
					 wtData.push_back(0x01);//打开
					 pDataItem->Exec(tempdev,wtData);

					 //清理房间提示灯如果没关闭则关闭
					if (STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState == 1)//清理房间灯打开时不执行此操作
					{
						pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3104);
						 wtData.clear();
						 wtData.push_back(0x00);//关闭
						 pDataItem->Exec(tempdev,wtData);
						 STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
					}
				 }
			}
		}
		else
		{
#ifndef WIN32
			//门铃背光亮
			int fd,sw;
			fd = open("/dev/kgeio", O_RDWR );   
			if (fd == -1) 
			{	
				KL_VERBOSE(" open /dev/kgeio fail\n");
			}

			//打开门铃按键背光
			sw = BELL_BG_LIGHT_ON;
			ioctl(fd, KGESG_IO_SET_RCUBELLBG, &sw );
			KL_WARN("打开门铃按键背光\n");

			if (STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState == 1)//清理房间灯打开时不执行此操作
			{
				sw = CLEAN_LIGHT_OFF;
				ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
				KL_WARN("清理房间灯关掉\n");
				STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
			}
			close(fd);
#endif
		}
	}
	else//拔卡
	{
		//关闭键盘背光，关掉所有继电器开关
		card_state = 0;
		STATUS.deviceStatus.m_RcuStatus.m_DoorCardState = 0;
		CDataItem *pDataItem;

		DoorcardCompose(0);

		/* 12路输入8路输出继电器板键盘加锁*/
		for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
		{
			 if (CONTEXT.Devices.m_DBS.test(i) != true)
			 {
				continue;
			 }
			 tempdev = CONTEXT.Devices.m_Item[i];

			 if (tempdev.m_Type ==  MD_RELAY_8
			 	||tempdev.m_Type ==  MD_RELAY_2
			 	||tempdev.m_Type ==  MD_LEDV12_3
			 	||tempdev.m_Type ==  MD_LIGHT_4
			 	)
			 {
				//12路8路输出键盘加锁
				 CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x2011);
				 wtData.clear();
				 wtData.push_back(0x01);//加锁
				 pDataItem->Exec(tempdev,wtData);
				 KL_VERBOSE("设备%d 键盘加锁\n",tempdev.m_Tn);
			 }
		}


/*		for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
		{
			 if (CONTEXT.Devices.m_DBS.test(i) != true)
			 {
				continue;
			 }
			 tempdev = CONTEXT.Devices.m_Item[i];
			 if (tempdev.m_Type ==  MD_KEYBOARD_20)
			 {
				 pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1133);
				 wtData.clear();
				 wtData.push_back(0x00);//键盘背光关闭
				 pDataItem->Exec(tempdev,wtData);
			 }
		}*/

		//拔卡时如果清理房间按钮按下，则清理房间提示灯亮，其他门牌提示灯熄灭，否则全都关掉
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
			{
				 if (CONTEXT.Devices.m_DBS.test(i) != true)
				 {
					continue;
				 }
				 tempdev = CONTEXT.Devices.m_Item[i];
				 if (tempdev.m_Type ==  MD_DOORDISPLAY)
				 {
					
						 //关闭门铃灯
						pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3102);
						 wtData.clear();
						 wtData.push_back(0x00);//关闭
						 pDataItem->Exec(tempdev,wtData);
					
						//关闭请勿打扰灯
						if (STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState != 1)
						{
							pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3103);
							 wtData.clear();
							 wtData.push_back(0x00);//关闭
							 pDataItem->Exec(tempdev,wtData);
							 STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;
						}
						
						//关闭清理房间灯
						if (STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState != 1)//清理房间灯打开时不执行此操作
						{
							pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3104);
							 wtData.clear();
							 wtData.push_back(0x00);//关闭
							 pDataItem->Exec(tempdev,wtData);
							 STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
						}

						//关闭总电源指示灯
						//关闭夜灯
					}
				}
			}
			else
			{
#ifndef WIN32
					int fd,sw;
					fd = open("/dev/kgeio", O_RDWR );   
					if (fd == -1) 
					{	
						KL_VERBOSE(" open /dev/kgeio fail\n");
					}

					//关闭门铃按键背光
					sw = BELL_BG_LIGHT_OFF;
					ioctl(fd, KGESG_IO_SET_RCUBELLBG, &sw );
					KL_WARN("关闭门铃按键背光\n");

//					if (STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState != 1)
					{
						sw = DNT_LIGHT_OFF;
						ioctl(fd, KGESG_IO_SET_RCUDND, &sw );
						 STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;
						KL_WARN("关闭请勿打扰灯\n");
					}

					//关闭清理房间灯
					if (STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState != 1)//清理房间灯打开时不执行此操作
					{
						sw = CLEAN_LIGHT_OFF;
						ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
						KL_WARN("清理房间灯关掉\n");
						 STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
					}
					//关闭总电源指示灯
					sw = POWER_INDECATION_OFF;
					ioctl(fd, KGESG_IO_SET_RCUPOWERLIGHT, &sw );
					KL_VERBOSE("2总电源指示灯关闭\n");						
					//关闭夜灯
					sw = NIGHTLIGHT_INDECATION_OFF;
					ioctl(fd, KGESG_IO_SET_NIGHTLIGHT, &sw );
					KL_VERBOSE("夜灯关闭\n");	
					close(fd);
#endif

		 }

		//关闭廊灯
		 /*if (device.m_Type ==  MD_RFID_CARD)
		 {
			for (int j=0; j<device.m_MapItemCount; j++)
			{
				if (device.m_MapItems[j].m_funcType == FUNC_GALLERY_LIGHT)//空调风机控制
				{
 					for(NoBitSet::SizeType st = device.m_MapItems[j].m_mapNbs.FromPos; st != device.m_MapItems[j].m_mapNbs.ToPos; st++)
					{
						if (device.m_MapItems[j].m_mapNbs.test(st) )
						{
							CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
							wtData.clear();
							wtData.push_back((INT8U)st);//按键控制的子路号
							wtData.push_back(3);//关
							
							 pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[j].m_mapTn],wtData);
							 STATUS.deviceStatus.m_GalleryLightState = 0;
						}
					}
				}	
			 }
		}*/
	}
	return 0;
}


/* 空调风机控制*/
int CDataItem_3001::Exec(Device &device,bstring &Data)
{
	return 0;
}

/* 空调数码管背光显示*/
int CDataItem_3002::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3002; 
	bstring wtData;
	Device tempdev;
	
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_AIRCONDITION)
		 {
			wtData.clear();
			wtData.push_back(Data[0]);
			Write(tempdev,dataid,wtData);
			return 0;
		}
	}
	return 0;
}

/* 读空调模块温度*/
int CDataItem_3003::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3003; 
	bstring wtData;
	Device tempdev;
	
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_AIRCONDITION)
		 {
			 if (Read(tempdev,Data) == true)
				return 0;
		}
	}
	return -1;
}

/* 设置空调工作模式*/
int CDataItem_3004::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3004; 
	bstring wtData;
	Device tempdev;
	
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_AIRCONDITION)
		 {
			wtData.clear();
			wtData.push_back(Data[0]);
			Write(tempdev,dataid,wtData);
			return 0;
		}
	}
	return 0;
}

/* 设置空调季节模式*/
int CDataItem_3005::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3005; 
	bstring wtData;
	Device tempdev;
	
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_AIRCONDITION)
		 {
			wtData.clear();
			wtData.push_back(Data[0]);
			Write(tempdev,dataid,wtData);
			return 0;
		}
	}
	return -1;
}

/* 空调单管阀控制*/
int CDataItem_3007::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3007; 
	bstring wtData;
	Device tempdev;
	Device iorelay;
	
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_AIRCONDITION)
		 {
			for (int j=0; j<tempdev.m_MapItemCount; j++)
			{
				if (tempdev.m_MapItems[j].m_funcType == FUNC_SINGLE_BRAKE)//空调风机控制
				{
 					for(NoBitSet::SizeType st = tempdev.m_MapItems[j].m_mapNbs.FromPos; st != tempdev.m_MapItems[j].m_mapNbs.ToPos; st++)
					{
						if (tempdev.m_MapItems[j].m_mapNbs.test(st) )
						{
							wtData.clear();
							wtData.push_back(Data[0]);//开或关
							wtData.push_back((INT8U)st);//按键控制的子路号 
							iorelay = CONTEXT.Devices.m_Item[tempdev.m_MapItems[j].m_mapTn];
		 					Write(CONTEXT.Devices.m_Item[tempdev.m_MapItems[j].m_mapTn],dataid,wtData);
							return 0;
						}
					}
				}	
			 }
		}
	}
	return -1;
}

/* 空调双管阀控制*/
int CDataItem_3008::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3008; 
	bstring wtData;
	Device tempdev;
	Device iorelay;
	
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_AIRCONDITION)
		 {
			for (int j=0; j<tempdev.m_MapItemCount; j++)
			{
				if (tempdev.m_MapItems[j].m_funcType == FUNC_AIRCDI_DOUBLE_COLD_BRAKE
					||tempdev.m_MapItems[j].m_funcType == FUNC_AIRCDI_DOUBLE_HOT_BRAKE
				)//空调风机控制
				{
					for(NoBitSet::SizeType st = tempdev.m_MapItems[j].m_mapNbs.FromPos; st != tempdev.m_MapItems[j].m_mapNbs.ToPos; st++)
					{
						if (tempdev.m_MapItems[j].m_mapNbs.test(st) )
						{
							wtData.clear();
							wtData.push_back(Data[0]);//关、热或冷
							wtData.push_back((INT8U)st);//按键控制的子路号 
							iorelay = CONTEXT.Devices.m_Item[tempdev.m_MapItems[j].m_mapTn];
		 					Write(CONTEXT.Devices.m_Item[tempdev.m_MapItems[j].m_mapTn],dataid,wtData);
							return 0;
						}
					}
				}	
			 }
		}
	}
	return 0;
}

/* 读空调设置度数*/
int CDataItem_3009::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3009; 
	bstring wtData;
	Device tempdev;
	
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_AIRCONDITION)
		 {
			 if (Read(tempdev,Data) == true)
				return 0;
		 }
	}
	return -1;
}

/* 设置空调度数*/
int CDataItem_3010::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3010; 
	bstring wtData;
	Device tempdev;
	
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_AIRCONDITION)
		 {
		 	wtData.clear();
			wtData.push_back(Data[0]);
			Write(tempdev,dataid,wtData);
			return 0;
		 }
	}
	return -1;
}


/* 设置空调风机速度*/
int CDataItem_3011::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3011; 
	bstring wtData;
	Device tempdev;
	Device iorelay;
	
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_AIRCONDITION)
		 {
			for (int j=0; j<tempdev.m_MapItemCount; j++)
			{
				if (tempdev.m_MapItems[j].m_funcType == FUNC_AIRCDI_WIND_SPEED)//空调风机控制
				{
					wtData.clear();
					wtData.push_back(Data[0]);//风机档位
					for(NoBitSet::SizeType st = tempdev.m_MapItems[j].m_mapNbs.FromPos; st != tempdev.m_MapItems[j].m_mapNbs.ToPos; st++)
					{
						if (tempdev.m_MapItems[j].m_mapNbs.test(st) )
						{
							wtData.push_back((INT8U)st);//风机档位对应的继电器控制
							
						}
					}
					iorelay = CONTEXT.Devices.m_Item[tempdev.m_MapItems[j].m_mapTn];
					Write(iorelay,dataid,wtData);
					return 0;
				}	
			 }
		}
	}
	return 0;
}

//门牌显示门号背光灯控制
int CDataItem_3101::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3101; 
	bstring wtData;

 	wtData.clear();
	wtData.push_back(Data[0]);
	Write(device,dataid,wtData);
	return 0;
}
//门牌显示门铃背光灯控制
int CDataItem_3102::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3102; 
	bstring wtData;

 	wtData.clear();
	wtData.push_back(Data[0]);
	Write(device,dataid,wtData);
	return 0;
}
//门牌显示请勿打扰背光灯控制
int CDataItem_3103::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3103; 
	bstring wtData;

 	wtData.clear();
	wtData.push_back(Data[0]);
	Write(device,dataid,wtData);
	return 0;
}

//门牌显示清理房间背光灯控制
int CDataItem_3104::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3104; 
	bstring wtData;

 	wtData.clear();
	wtData.push_back(Data[0]);
	Write(device,dataid,wtData);
	return 0;
}

//门铃背光闪烁
int CDataItem_3105::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x3105; 
	bstring wtData;

	
 	wtData.clear();
	wtData.push_back(Data[0]);
	Write(device,dataid,wtData);
	return 0;

}

//光感器LUX值读取
int CDataItem_4001::Exec(Device &device,bstring &Data)
{
	bstring wtData;

	
 	wtData.clear();
	wtData.push_back(Data[0]);
	INT8U commNo;
	bstring orData,iData;
	INT8U addr[6] ={0};
	GetAddr_6_Fix0(device.m_Addr,addr);
	commNo = bcdc(addr[0]);
	iData.push_back(0X00);
	iData.push_back(0X04);

	if (ModbusRead(device,commNo,MODBUS_READ,0x0000,iData,orData) == true)
	{
		if (orData.size() == 10)
		{
			//device.m_LuxValue = (INT32U)((orData[6]*256 + orData[7])*(orData[8]*256 + orData[9])/100);
			device.m_LuxValue = (INT32U)((orData[6]*256 + orData[7])*(orData[8]*256 + orData[9])/10);//由于传感器精度问题放大10倍
		}
	}

	char time7[7] = {0};
	static char min=0;
	Get_CurBCDTime7(time7);
	if (time7[5] == min)
		return 0;
		
	min = time7[5]; 
	KL_ERROR("device.m_LuxValue = %d\n",device.m_LuxValue);
	return 0;

}
int CDataItem_09XX::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0x0900; 
	bstring wtData;

	dataid += Data[0];

	SetDataItemID(dataid);
	Data.clear();

	Read(device, Data);

	//由于SetDataItemID修改了默认ID，所以需还原默认id，不然GetItemDataid会跟初始值不一致，导致GetDataItem(0x0900)返回NULL
	SetDataItemID(0x0900);
	return 0;

}

//抄读电表9010
int CDataItem_C343::Exec(Device &device,bstring &Data)
{
	INT16U dataid = 0xc343; 
	bstring wtData;
	INT8U busno;

	
	if (device.m_Addr[0] == 0x31)
		busno = DBCJ_1_BUSNO_485;
	else if (device.m_Addr[0] == 0x32)
		busno = DBCJ_2_BUSNO_485;
	
	 if (device.m_Type == MD_AIRCONDITION)
	 {
		 if (Read(busno,device,Data) == true)
			return 0;
	}
	return -1;
}



