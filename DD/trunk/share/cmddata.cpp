#include "cmddata.h"
#include "context.h"
#include "../base/klb_io.h"
#include "RtfCtrl.h"

CPollCmd::CPollCmd()
{
	m_PollRtfBS = CONTEXT.Devices.m_RtfBS;
}
CPollCmd::~CPollCmd()
{
}
int CPollCmd::OnExcRcu(Device &device,bstring& orData)
{
	return -1;
}

int CPollCmd::OnExcRtf(Devices& devices)
{
	bstring orData;
	if (m_PollRtfBS.any() == 0)
	{
		m_PollRtfBS = devices.m_RtfBS;
	}

	for (DevicesBitSet::SizeType i = m_PollRtfBS.FromPos; i<m_PollRtfBS.ToPos; i++)
	{
		 if (m_PollRtfBS.test(i) != true)
		 {
			continue;
		 }		

		 CRtfCmdItem *pRtfCmdItem = CONTEXT.m_RtfCmdItems.GetRtfCmdItem(0x0501);
		 if (devices.m_Item[i].m_Type != MD_RTF)
		 {
			continue;
		 }
		 
		 pRtfCmdItem->Exec(devices.m_Item[i],orData);
		 m_PollRtfBS.reset(i);
		 m_PollRtfBS.Stat();
		 break;
	}
	return 0;
}

//回路在整流器通讯故障，进行电压均衡
int CPollCmd::OnExcClArg(Devices& devices)
{
	bstring orData;


	for (CLRelayBitSet::SizeType i = CONTEXT.ClRelays.CLRelayBS.FromPos; i<CONTEXT.ClRelays.CLRelayBS.ToPos; i++)
	{
		if (CONTEXT.ClRelays.CLRelayBS.test(i) != true)
		 {
			continue;
		 }

		/* 判断回路下整流器是否出现通讯故障*/
		bool ret = DetectClRtfComm(CONTEXT.ClRelays.m_Item[i]);

		if (ret == false)
		{
			
			if (STATUS.deviceStatus.m_ClArgFlag[i] == false)
			{
				KL_WARN("回路%d 电压均衡处理\n",CONTEXT.ClRelays.m_Item[i].m_CLRelayNo);
				ForceClVolArg(CONTEXT.ClRelays.m_Item[i], STATUS.deviceStatus.m_ArgVol,STATUS.deviceStatus.m_ArgCur);
			}
			STATUS.deviceStatus.m_ClArgFlag[i] = true;
		}
		else
		{
			STATUS.deviceStatus.m_ClArgFlag[i] = false;
		}
	}
	return 0;	
}	

bool CPollCmd::DetectClRtfComm(S_CLRelay& clRelay)
{
	for (DevicesBitSet::SizeType i = clRelay.m_Dbs.FromPos; i<clRelay.m_Dbs.ToPos;i++)
	{
		if (clRelay.m_Dbs.test(i) != true)
		{
			continue;
		}

		if (CONTEXT.Devices.m_Item[i].m_Enable == false)
		{
			continue;
		}

		if (CONTEXT.Devices.m_Item[i].m_CommBreakTimes > 5)
		{
			return false;
		}
	}
	return true;
}

bool CPollCmd::ForceClVolArg(S_CLRelay& clRelay,INT16U Vol, INT16U Cur)
{
	CRtfCtrl clRtfCtl;
	SSmartGarageItem garageItem;

	garageItem.m_DeviceNo = clRelay.m_CLRelayNo;
	
	int ret = clRtfCtl.RtfClVolCurSetCtl(garageItem, Vol, Cur);

	if (ret !=0)
	{
		return false;
	}

	return true;
}

int CPollCmd::OnExc(Device &device,bstring& orData)
{

	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x0110);
	if (pDataItem == NULL)
	{
		KL_VERBOSE("无此数据项\n");
		CONTEXT.m_DataItems.Init();
		return -1;
	}

	if (device.m_Type == MD_DOORDISPLAY || device.m_Type == MD_RTF|| device.m_Type == MD_LUX_SENSOR)
	{
		return -1;
	}

	if (device.m_Type == MD_AIRCONDITION)//小羊羊没有用查询命令来回复风速按键，所以暂时这么处理
	{
		GetAirconditionStatus(device,orData);
		return 0;	
	}

	if (device.m_Type == MD_RELAY_8 
		||device.m_Type == MD_RELAY_2 
		|| device.m_Type == MD_LIGHT_4
		|| device.m_Type == MD_LEDV12_3)//长短电平状态查询
	{
		pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1107);
	}

	orData.clear();

	if (pDataItem->Read(device,orData) == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
}

int CPollCmd::GetAirconditionStatus(Device &device,bstring& orData)
{ // 1秒检测一次

	time_t curSecond;
	static time_t lastSecond = 0;
	
	time(&curSecond) ;
	if (curSecond - lastSecond < 1)
	{
		return 0;
	}
	lastSecond = curSecond;
	
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3001);

	if (STATUS.deviceStatus.m_AirConditionType == 1 || STATUS.deviceStatus.m_AirConditionType == 2)//modbus协议空调模块控制
	{
		bstring iData;
		INT16U dataid;
		INT8U commNo;

		orData.clear();

		/* 读空调风速*/				
		iData.push_back(0X00);
		iData.push_back(0X01);

		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			dataid = MODBUS_A_RD_WINDSPEED;//空调状态
			commNo = MODBUS_A_COMMNO;
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			dataid = MODBUS_B_RD_WINDSPEED;//空调状态
			commNo = MODBUS_B_COMMNO;
		}
		
		if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,orData) == true)
		{//把MODBUS协议数据
			if (orData[0] == 0x03)//命令码为3，数据长度为2
			{
				INT8U value = orData.at(4)&0x03;
				
				if (value < 3)
				{
					value += 1;
				}
				else
				{
					value = 0;
				}

				orData.clear();
				orData.push_back(0x01);
				orData.push_back(0x30);
				orData.push_back(value);
				
				KL_VERBOSE("空调风速为%d\n",value);
				STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWindSpeed = value;
				//return 0;
			}
		}

		/* 读空调设置温度*/				
		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 1)//制冷
			{
				dataid = 0x0036;
			}
			else if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 0)
			{
				dataid = 0x004d;//制热
			}
			else
			{
				dataid = 0x004c;//制冷
			}
		}

		if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			dataid = 0x0036;
		}

		commNo = MODBUS_A_COMMNO;
		INT8U value;
		bstring rtData;

		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			commNo = MODBUS_A_COMMNO;
			rtData.clear();
			if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,rtData) == true)
			{//把MODBUS协议数据
				if (rtData[0] == 0x03&& rtData[2]==0x2)//命令码为3，数据长度为2
				{
					value = rtData.at(4);
					STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSetTemperture = value;
					KL_VERBOSE("空调设置温度为%d\n",value);
				}
			}
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			commNo = MODBUS_B_COMMNO;
			if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,rtData) == true)
			{//把MODBUS协议数据
				if (rtData[0] == 0x03&& rtData[2]==0x36)//命令码为3，通讯地址0x0036
				{
					value = rtData.at(4);
					STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSetTemperture = value;
					KL_VERBOSE("空调设置温度为%d\n",value);
				}
			}
		}
		
	}
	else
	{
		orData.clear();
		if (pDataItem->Read(device,orData) == true)
		{
			return 0;
		}
		else
		{
			return -1;
		}

	}
	
	return 0;
}

bool CPollCmd::PollUrgentButton()
{
	
	int state;
	int fd;
	static int beforeState = 1;

#ifndef WIN32
	fd = open("/dev/kgeio", O_RDWR );   
	if (fd == -1) 
	{	
		KL_ERROR(" open /dev/kgeio fail\n");
		return false;
	}
	ioctl(fd, KGESG_IO_GET_RCUSOS, &state);
	close(fd);

	if ( state == beforeState)
	{
		return false;
	}
	beforeState = state;
	if (state == 0)
	{
		STATUS.deviceStatus.m_RcuStatus.m_UrgentButtonState = 1;
	}
	else
	{
		STATUS.deviceStatus.m_RcuStatus.m_UrgentButtonState = 0;
	}

	MapItem *mapItem = NULL;

	UrgentKeyControl(mapItem);

#endif	
	return true;
		
}
bool CPollCmd::UrgentKeyControl(MapItem *pmapItem)
{
	bstring tempData;
	
	//紧急按钮按下，请勿打扰提示灯熄灭
	if ( STATUS.deviceStatus.m_RcuStatus.m_UrgentButtonState == 1)
	{
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3103);
			if (pDataItem == NULL || pmapItem == NULL)
			{
				return false;
			}
			int ret;
			tempData.clear();
			tempData.push_back(0X00);//关闭请勿打扰背光
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[pmapItem->m_mapTn],tempData);
			STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;
			KL_WARN("紧急按钮按下，请勿打扰提示灯熄灭\n");
			return true;
		}
		else//RCU控制
		{
#ifndef WIN32			
			int fd,sw;
			fd = open("/dev/kgeio", O_RDWR );   
			if (fd == -1) 
			{	
				KL_VERBOSE(" open /dev/kgeio fail\n");
			}

			sw = DNT_LIGHT_OFF;
			ioctl(fd, KGESG_IO_SET_RCUDND, &sw );
			close(fd);
			STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;
			KL_WARN("紧急按钮按下，请勿打扰提示灯熄灭\n");
			return true;		
#endif			
		}

	}
	return false;
}
//如果是插卡取电方式，将通过RCU的门节点检测。
bool CPollCmd::PollDoorCard(Device &device,bstring& orData)
{
	//如果是插卡取电方式，将通过RCU的门节点检测。
#ifndef WIN32
	KL_VERBOSE("STATUS.deviceStatus.m_DoorCardType=%d\n",STATUS.deviceStatus.m_DoorCardType);
	if (STATUS.deviceStatus.m_DoorCardType == 1)
	{
		int fd,ret,state;
		static int DoorbeforeState=INSET_CARD_INVALID_LEVEL;
		static time_t timenow,doorcardout_30Second = 0;

		time_t time_now;
		static time_t t=0;	
		time(&time_now) ;
		if (time_now == t)
		{
			return false;
		}
		t = time_now;	
			
		fd = open("/dev/kgeio", O_RDWR );   
		if (fd == -1) 
		{	
			KL_VERBOSE(" open /dev/kgeio fail\n");
		}
		ioctl(fd, KGESG_IO_GET_RCUCARD, &state);

		KL_VERBOSE("DoorbeforeState=%d,door card state = %d\n",DoorbeforeState,state);
		close(fd);

		orData.clear();

		 if (state == INSET_CARD_VALID_LEVEL)
		 {
			doorcardout_30Second = 0;
		 }
		 
		if (DoorbeforeState!=state && state == INSET_CARD_VALID_LEVEL)
		{
			//dataid == 0x2110
			DoorbeforeState = state;
			doorcardout_30Second = 0;
			orData.push_back(0x10);
			orData.push_back(0x21);
			orData.push_back(0x01);
			KL_VERBOSE("$$$$$$$$$$$$$$门卡插入。。\n");
			return true;
		
		}
		else if  (DoorbeforeState!=state && state == INSET_CARD_INVALID_LEVEL) 
		{
			//dataid == 0x2110
			if (doorcardout_30Second == 0)
			{
				time(&doorcardout_30Second);
			}
			time(&timenow) ;
			KL_VERBOSE("门卡还有%d 才算拔卡\n",(timenow - doorcardout_30Second));
			if (timenow - doorcardout_30Second > STATUS.deviceStatus.m_OutCardDetectTime)//把出门卡?秒才算门卡拔出
			{
				DoorbeforeState = state;
				orData.push_back(0x10);
				orData.push_back(0x21);
				orData.push_back(0x00);
				KL_VERBOSE("$$$$$$$$$$$$$$$$$$$$门卡拔出\n");
				return true;
			}
		}
	}
#endif
	return false;
}

//人体红外场景处理
void CPollCmd::IrdaSceneCompose(INT8U state)
{
	if (state == 1)
	{//红外检测有人
		SSceneCompose &SceneCompose = STATUS.deviceStatus.m_IRInSceneComposes;
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//亮度
				
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
				tempData.push_back(2);//开
				
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

		}

		SceneCompose.m_ComposeState = state;
	}
	else
	{//红外检测无人
		SSceneCompose &SceneCompose = STATUS.deviceStatus.m_IROutSceneComposes;
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
		}
		SceneCompose.m_ComposeState = state;
	}
}



//人体红外检测
bool CPollCmd::IrdaDetect(Device &device,bstring& orData)
{
#ifndef WIN32
		int fd,ret,state;
		static int beforeState=0;
		static time_t timenow,IrdaDetect_30Second = 0;

		time_t time_now;
		static time_t t=0;	
		time(&time_now) ;
		if (time_now == t)
		{
			return false;
		}
		t = time_now;	
			
		fd = open("/dev/kgeio", O_RDWR );   
		if (fd == -1) 
		{	
			KL_VERBOSE(" open /dev/kgeio fail\n");
		}
		ioctl(fd, KGESG_IO_GET_RCUIR, &state);

		KL_VERBOSE("IRDA state = %d\n",state);
		close(fd);

		orData.clear();

		 if (state == 1)
		 {
			IrdaDetect_30Second = 0;
		 }
		 KL_VERBOSE("beforeState=%d,state=%d\n",beforeState,state);
		if (beforeState!=state && state == 1)
		{
			beforeState = state;
			IrdaDetect_30Second = 0;
			KL_VERBOSE("人体红外已检测到。。\n");
			STATUS.deviceStatus.m_RcuStatus.m_IrdaDetect = 1;
			IrdaSceneCompose(1);
			return true;
		
		}
		else if  (beforeState!=state && state == 0) 
		{
			//dataid == 0x2110
			if (IrdaDetect_30Second == 0)
			{
				time(&IrdaDetect_30Second);
			}
			time(&timenow) ;

			KL_VERBOSE("还有%d 秒人体红外判断为无人。。。。\n",(STATUS.deviceStatus.m_IrdaDetectTime*60-(timenow - IrdaDetect_30Second)));
			
			if (timenow - IrdaDetect_30Second > STATUS.deviceStatus.m_IrdaDetectTime*60)//把出门卡30秒才算门卡拔出
			{
				beforeState = state;
				
				KL_VERBOSE("人体红外未检测到人物活动。。\n");
				STATUS.deviceStatus.m_RcuStatus.m_IrdaDetect = 0;
				IrdaSceneCompose(0);
				
				return true;
			}
		}
#endif
	return false;
}



//如果是门铃按键由RCU的门节点检测。
bool CPollCmd::PollDoorBell(Device &device,bstring& orData)
{
#ifndef WIN32
	int fd;

	//KL_VERBOSE("STATUS.deviceStatus.m_DoorDisplayType = %d\n",STATUS.deviceStatus.m_DoorDisplayType);

	//	在勿扰开按门铃，请勿打扰等闪烁
	if (STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState == 1)
	{
		int state;
		static int flash_times = 30;
		static bool flag = false;

		fd = open("/dev/kgeio", O_RDWR );   
		if (fd == -1) 
		{	
			KL_ERROR(" open /dev/kgeio fail\n");
			return false;
		}
		
		ioctl(fd, KGESG_IO_GET_RCUBELL, &state);
		
		
		if ( state == 0)
		{
			flag = true;
		}

//		KL_VERBOSE("flag=%d,flash_times=%d\n",flag,flash_times);

		if (flag == true && (flash_times >= 0))
		{
			static int value = 1;
			if (flash_times %2 == 0)
			{
				
				if (value==DNT_LIGHT_OFF)
					value = DNT_LIGHT_ON;
				else
					value = DNT_LIGHT_OFF;
				ioctl(fd, KGESG_IO_SET_RCUDND, &value);
				//KL_VERBOSE("######################################\n");
			}
			
			if (flash_times == 0)
			{
				flag = false;
				flash_times = 30;
				value = DNT_LIGHT_ON;
				ioctl(fd, KGESG_IO_SET_RCUDND, &value);
			}
			else
			{
				flash_times--;
			}
			
		}
		
		close(fd);
	}
	
	//	在勿扰开或者拔卡状态下失效
	if (STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState == 1 ||STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0 )
	{
		KL_WARN("在勿扰开或者拔卡状态下失效\n");
		fd = open("/dev/kgeio", O_RDWR );   
		if (fd == -1) 
		{	
			KL_ERROR(" open /dev/kgeio fail\n");
			return false;
		}
		int sw = BELL_BG_LIGHT_OFF;
		ioctl(fd, KGESG_IO_SET_RCUBELLBG, &sw);
		close(fd);
		return false;
	}
	else
	{
		fd = open("/dev/kgeio", O_RDWR );   
		if (fd == -1) 
		{	
			KL_ERROR(" open /dev/kgeio fail\n");
			return false;
		}
		int sw = BELL_BG_LIGHT_ON;
		ioctl(fd, KGESG_IO_SET_RCUBELLBG, &sw);
		close(fd);
	}

	if (STATUS.deviceStatus.m_DoorDisplayType == 0)
	{
		int ret,state,sw;
		static int beforeState=1;
//		static time_t recordTick = 0;
//		time_t  tt = 0;
			
		fd = open("/dev/kgeio", O_RDWR );   
		if (fd == -1) 
		{	
			KL_VERBOSE(" open /dev/kgeio fail\n");
			return false;
		}

		ioctl(fd, KGESG_IO_GET_RCUBELL, &state);

		orData.clear();
//		time(&tt);
		if (beforeState!=state && state == 0)
		{
			//dataid == 0x2110
			beforeState = state;
			//sw = 1;
			sw = BELL_RING_ON;
			ioctl(fd, KGESG_IO_SET_RCUBELL, &sw);

			
//			time(&recordTick) ;
			KL_VERBOSE("门按键按下\n");
			return true;
		}
		else if  (beforeState!=state && state == 1) 
		{
			//dataid == 0x2110
			beforeState = state;
			sw = BELL_RING_OFF;
			ioctl(fd, KGESG_IO_SET_RCUBELL, &sw);
			KL_VERBOSE("门铃按键释放\n");
			return true;
		}
/*		else if  (beforeState!=state && state == 1 && (tt-recordTick)>3) //3
		{//3//延时3秒后切断门铃电源
			beforeState = state;
			sw = 1;
			ioctl(fd, KGESG_IO_SET_RCUBELL, &sw);
			KL_VERBOSE("门铃按键释放\n");
			return true;
		}*/

		close(fd);
		return false;
			
	}
#endif
	return false;
}

INT16U CDataItemCmd::Filter(INT16U dataid)
{
	return 0;
}

bool  CDataItemCmd::KeyBacklightCtrl(Device device,int state)
{
	bstring tempData;
	CDataItem *pDI = CONTEXT.m_DataItems.GetDataItem(0x1144);
	if (pDI == NULL)
	{
		KL_VERBOSE("pDI == NULL \n");
		return false;
	}
	if (device.m_MapItemCount > 0)
	{
		for (int i=0; i<device.m_MapItemCount; i++)
		{
			for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
			{
				if (device.m_MapItems[i].m_mapNbs.test(st) && device.m_MapItems[i].m_funcType == FUNC_KEYBACKLIGHT)
				{
					tempData.clear();
					tempData.push_back((INT8U)st);//按键控制的子路号
					tempData.push_back((INT8U)state);//当前的按键路数及状态值
					//根据继电器的通断状态来控制按键的背光的打开或关闭
					pDI->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);
				}
			}
		}
	}
	return true;
}

//总电源场景组合处理
void CDataItemCmd::TotalPowerCompose(INT8U state)
{
	SSceneCompose &SceneCompose = STATUS.deviceStatus.m_PowerSceneComposes;

	if (state == 1)
	{
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//亮度
				
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

		}

		SceneCompose.m_ComposeState = state;
	}
	else
	{
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
		}
		SceneCompose.m_ComposeState = state;
	}
}


bool CDataItemCmd::PowerSwitchControl(Device device)
{
	bstring tempData;

	if (STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState != 0)
	{
		STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState = 0;
		/* 关闭总电源指示灯*/
		
#ifndef WIN32		
		int fd,sw;
		fd = open("/dev/kgeio", O_RDWR );   
		if (fd == -1) 
		{	
			KL_VERBOSE(" open /dev/kgeio fail\n");
		}

		sw = POWER_INDECATION_OFF;
		ioctl(fd, KGESG_IO_SET_RCUPOWERLIGHT, &sw );
		close(fd);	
		KL_VERBOSE("1总电源指示灯关闭\n");
#endif
//		TotalPowerCompose(0);

	}
	else
	{
		STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState = 1;
		/* 打开总电源指示灯*/
#ifndef WIN32		
		int fd,sw;
		fd = open("/dev/kgeio", O_RDWR );   
		if (fd == -1) 
		{	
			KL_VERBOSE(" open /dev/kgeio fail\n");
		}

		sw = POWER_INDECATION_ON;
		ioctl(fd, KGESG_IO_SET_RCUPOWERLIGHT, &sw );
		close(fd);	
		KL_VERBOSE("总电源指示灯打开\n");
#endif
		TotalPowerCompose(1);
	}
	
//	for (int i=0; i<device.m_MapItemCount; i++)
	{
		/* 夜间总电源关闭打开夜灯*/
		//在夜间（18：00—8：00）关总电源要开夜灯指示灯，白天关总电源不开夜灯指示灯
		//KL_VERBOSE("STATUS.deviceStatus.IsNight() = %d, state =%d,m_funcType=%d\n ",STATUS.deviceStatus.IsNight(),STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState,device.m_MapItems[i].m_funcType);
		KL_VERBOSE("STATUS.deviceStatus.IsNight() = %d, state =%d,\n ",STATUS.deviceStatus.IsNight(),STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState);
		if (/*device.m_MapItems[i].m_funcType == FUNC_NIGHT_LIGHT 8*/
			 STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState == 0
			&& STATUS.deviceStatus.IsNight() == true
		)
		{
/*				CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1101);
				if (pDataItem == NULL)
				{
					return false;
				}
				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
					if (device.m_MapItems[i].m_mapNbs.test(st))
					{
						int ret;
						tempData.clear();
						tempData.push_back((INT8U)st);//按键控制的子路号
						tempData.push_back(FUNC_NORMAL);//按键特殊功能
						tempData.push_back(i);//总电源键值
						tempData.push_back(0X01);//开夜灯

						KL_WARN("夜间总电源关闭打开夜灯\n");
						STATUS.deviceStatus.m_NightLightState = 1;
						/* 打开夜灯指示灯*
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
						//对第几路夜灯进行操作
						ret = pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);
						return true;
					}
				}*/

				KL_WARN("夜间总电源关闭打开夜灯指示灯\n");
				STATUS.deviceStatus.m_NightLightState = 1;
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

		//关总电源关清理
		if ( STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState == 0)
		{
			
			if (STATUS.deviceStatus.m_DoorDisplayType == 1)
			{
				CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3104);
				if (pDataItem == NULL)
				{
					return false;
				}
				int ret;
				tempData.clear();
				tempData.push_back(0X00);//关闭清理房间提示
				
//				ret = pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);
				STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
				KL_WARN("关闭清理房间提示\n");
				//return true;
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

				sw = CLEAN_LIGHT_OFF;//关闭清理房间提示
				ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
				close(fd);
				STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
				KL_WARN("关闭清理房间提示\n");
				//return true;	
#endif			
			}
		}


	}
	return false;
}

bool CDataItemCmd::CleanRoomKeyControl(MapItem mapItem)
{
	bstring tempData;
	//门卡拔出无效
	if (STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0)
	{
		return false;
	}

	if (STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState != 0)
	{
		STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;//关闭清理房间按钮
	}
	else
	{
		STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 1;//按下了清理房间按钮
	}
	
	//按下清理房间按钮，门口显示的清理房间指示灯亮。
	if ( STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState == 1)
	{
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			KL_WARN("按下清理房间按钮，门口显示的清理房间指示灯亮\n");
			CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3104);
			if (pDataItem == NULL)
			{
				return false;
			}
			int ret;
			tempData.clear();
			tempData.push_back(0X01);//打开清理房间提示
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			
			KL_WARN("按下清理房间按钮，请勿打扰指示灯关闭 \n");
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3103);
			if (pDataItem == NULL)
			{
				return false;
			}

			tempData.clear();
			tempData.push_back(0X00);//关闭请勿打扰背光
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;

			KL_WARN("按下清理房间按钮，门铃背光开启\n");
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3102);
			if (pDataItem == NULL)
			{
				return false;
			}

			tempData.clear();
			tempData.push_back(0X01);//门铃背光开启
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			return true;
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
			KL_WARN("按下清理房间按钮，门口显示的清理房间指示灯亮\n");
			sw = CLEAN_LIGHT_ON;//打开清理房间提示
			ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
			
			KL_WARN("按下清理房间按钮，请勿打扰指示灯关闭 \n");
			sw = DNT_LIGHT_OFF;//关闭请勿打扰背光
			ioctl(fd, KGESG_IO_SET_RCUDND, &sw );
			STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;


			KL_WARN("按下清理房间按钮，门铃背光开启\n");
			sw = BELL_BG_LIGHT_ON;
			ioctl(fd, KGESG_IO_SET_RCUBELLBG, &sw );
			
			close(fd);
			return true;			
#endif			
		}
	}
	else
	{
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3104);
			if (pDataItem == NULL)
			{
				return false;
			}
			int ret;
			tempData.clear();
			tempData.push_back(0X00);//关闭清理房间提示
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("关闭清理房间提示\n");
			return true;
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

			sw = CLEAN_LIGHT_OFF;//关闭清理房间提示
			ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
			close(fd);
			KL_WARN("关闭清理房间提示\n");
			return true;	
#endif			
		}

	}

	return false;
}
//门铃按键控制
bool CDataItemCmd::BellKeyControl(MapItem mapItem)
{
	bstring tempData;

	
	//请勿打扰开启，如果有人按门铃，请勿打扰指示灯闪3秒。
	if ( STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState == 1)
	{
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3105);
			if (pDataItem == NULL)
			{
				return false;
			}
			int ret;
			tempData.clear();
			tempData.push_back(0X03);//门铃背光闪3秒
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("请勿打扰开启，如果有人按门铃，请勿打扰指示灯闪3秒\n");
			return true;
		}
		else
		{
			//RCU控制
			KL_WARN("请勿打扰开启，如果有人按门铃，请勿打扰指示灯闪3秒\n");
			return true;
		}
	}
	return false;
}

bool CDataItemCmd::DonotDisturbKeyControl(MapItem mapItem)
{
	bstring tempData;

	//门卡拔出无效
	if (STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0)
	{
		return false;
	}

	if (STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState != 0)
	{
		STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;//未按
	}
	else
	{
		STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 1;//按下
	}
	
	//按下请勿打扰按键
	if ( STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState == 1)
	{
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			//房间内按请勿打扰按钮，关闭门铃按键背光
			CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3102);
			if (pDataItem == NULL)
			{
				return false;
			}
			int ret;
			tempData.clear();
			tempData.push_back(0X00);//关闭门铃背光
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("房间内按请勿打扰按钮，关闭门铃按键背光\n");
			
			//房间内按请勿打扰按钮，门口显示请勿打扰指示灯亮
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3103);
			if (pDataItem == NULL)
			{
				return false;
			}
			tempData.clear();
			tempData.push_back(0X01);//打开请勿打扰背光
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("房间内按请勿打扰按钮，门口显示请勿打扰指示灯亮\n");

			//房间内按请勿打扰按钮，清理房间灯关掉
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3104);
			if (pDataItem == NULL)
			{
				return false;
			}
			tempData.clear();
			tempData.push_back(0X00);//关闭清理房间背光
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
			KL_WARN("房间内按请勿打扰按钮，清理房间灯关掉\n");
			return true;
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

			//房间内按请勿打扰按钮，关闭门铃按键背光
			sw = BELL_BG_LIGHT_OFF;
			ioctl(fd, KGESG_IO_SET_RCUBELLBG, &sw );
			KL_WARN("房间内按请勿打扰按钮，关闭门铃按键背光\n");

			//房间内按请勿打扰按钮，门口显示请勿打扰指示灯亮
			sw = DNT_LIGHT_ON;
			ioctl(fd, KGESG_IO_SET_RCUDND, &sw );
			KL_WARN("房间内按请勿打扰按钮，门口显示请勿打扰指示灯亮\n");

			//房间内按请勿打扰按钮，清理房间灯关掉
			sw = CLEAN_LIGHT_OFF;
			ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
			STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
			KL_WARN("房间内按请勿打扰按钮，清理房间灯关掉\n");
			close(fd);
#endif			
			return true;
		}
	}
	else
	{
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3103);
			if (pDataItem == NULL)
			{
				return false;
			}
			int ret;
			tempData.clear();
			tempData.push_back(0X00);//关闭请勿打扰背光
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("房间内按请勿打扰按钮，门口显示请勿打扰指示灯亮\n");
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

			//房间内按请勿打扰按钮，门口显示请勿打扰指示灯亮
			sw = DNT_LIGHT_OFF;
			ioctl(fd, KGESG_IO_SET_RCUDND, &sw );
			close(fd);
			KL_WARN("房间内按请勿打扰按钮，门口显示请勿打扰指示灯亮\n");
#endif						
		}
	}
	return false;
}


//门牌号显示
void CDataItemCmd::OpenDoorNoDisplay()
{
	/* 门牌提示控制*/
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }
		 Device tempdev = CONTEXT.Devices.m_Item[i];
		 if (tempdev.m_Type ==  MD_DOORDISPLAY)
		 {
		 	if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		 	{
				//房号背光亮
				 CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3101);
				 bstring wtData;
				 wtData.clear();
				 wtData.push_back(0x01);//打开
				 pDataItem->Exec(tempdev,wtData);
		 	}
			else
			{
#ifndef WIN32		
				//房号背光亮
				//TODO
#endif	
			}
		 }
	}
	return;	
}

bool CDataItemCmd::IsBedLightOn()
{
	int count = 0;
	SSceneCompose &SceneCompose = STATUS.deviceStatus.m_PowerSceneComposes;

	for (int i=0; i<SceneCompose.m_MemberNum; i++)
	{
		if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_LEFT_BED_LIGHT
			|| SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RIGHT_BED_LIGHT)
		{
			INT8U no = SceneCompose.m_SceneComposeMember[i].m_SubNo;

			count++;

			CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x0900);
			if (pDataItem == NULL)
			{
				return false;
			}
			bstring tempData;
			tempData.clear();
			tempData.push_back(no);//第几路
			
			int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			if (ret == 0&&tempData.at(2) == 1)//继电器打开
			{
				KL_WARN("床头灯已开\n");
				return true;
			}
		}
	}

	if (count == 0)//如果没有配床头灯则也返回true
	{
		return true;
	}
}

bool CDataItemCmd::OpenBedLight()
{
	SSceneCompose &SceneCompose = STATUS.deviceStatus.m_PowerSceneComposes;

	for (int i=0; i<SceneCompose.m_MemberNum; i++)
	{
		if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_LEFT_BED_LIGHT
			|| SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RIGHT_BED_LIGHT)
		{
			//如果床头灯的继电器是开状态则不需要再开
			CDataItem * pDataItem; 
			bstring tempData;
			int ret; 
			tempData.clear();
			
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1035);
			if (pDataItem == NULL)
			{
				return false;
			}
			tempData.clear();
			tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
			tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//亮度
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
		}
	}
	return true;
}

void CDataItemCmd:: IORelayShortKey(Device device,INT16U bitStat,bstring idata)
{
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1101);
	if (pDataItem == NULL)
	{
		return;
	}

	/* 12路输入端短电平扫描处理*/
	for (INT8U i=0; i<CONS_IORELAY_INPUTPORT_MAX_COUNT; i++)
	{
		if (bitStat&(0x01<<i))
		{
			INT8U subno = i+1;
			idata.clear();
			idata.push_back(subno);
			idata.push_back(0x01);//state,默认继电器反转
			SoftKeyProcess(device,subno,pDataItem,idata);
		}
	}
}

void CDataItemCmd::IORelayLongKey(Device device,INT16U bitStat,bstring idata)
{
	static INT16U backup_longkey_state[CONS_DEVICE_MAX_COUNT]={0};
	
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1102);
	if (pDataItem == NULL)
	{
		return;
	}
	
	/* 12路输入端长电平扫描处理*/
	for (INT8U i=0; i<CONS_IORELAY_INPUTPORT_MAX_COUNT; i++)
	{
		if (bitStat&(0x01<<i))
		{
			INT8U subno = i+1;
			idata.clear();
			idata.push_back(subno);
			idata.push_back(0x01);//state,默认继电器反转
			SoftLongKeyProcess(device,subno,pDataItem,idata);
		}
		else
		{
			if (backup_longkey_state[device.m_Tn]&(0x01<<i))
			{
				INT8U subno = i+1;
				idata.clear();
				idata.push_back(subno);
				idata.push_back(0x00);//state,默认继电器反转
				SoftLongKeyProcess(device,subno,pDataItem,idata);
			}
		}
	}
	backup_longkey_state[device.m_Tn] = bitStat;
}

int CDataItemCmd::SoftKeyProcess(Device device,INT8U subno,CDataItem *pDataItem,bstring idata)
{
	/* 按任意键，总电源解锁，关闭总电源指示灯*/
	#ifndef WIN32		
		int fd,sw;
		fd = open("/dev/kgeio", O_RDWR );   
		if (fd == -1) 
		{	
			KL_VERBOSE(" open /dev/kgeio fail\n");
		}

		sw = POWER_INDECATION_OFF;
		ioctl(fd, KGESG_IO_SET_RCUPOWERLIGHT, &sw );
		close(fd);	
		KL_VERBOSE("3总电源指示灯关闭\n");
		 STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState =0;
	#endif
						
	if (device.m_MapItemCount > 0)
	{
		bstring tempData;
		
		for (int i=0; i<device.m_MapItemCount; i++)
		{
			if (device.m_MapItems[i].m_subno == subno)//配置文件是否存在此路号
			{

				//在总电源开的情况下，夜间时按任意键先点亮左右床灯逐渐调亮到20%，并打开总电源按键的指示灯，再按任意键执行相对应的按键功能；
				if ( STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState == 1
					&& STATUS.deviceStatus.IsNight() == true
				)
				{
					if (IsBedLightOn() == true)//床头灯已打开则不许要打开
					{
						;
					}
					else //打开左右床头灯
					{
						OpenBedLight(); 
						return 0;
					}
				}

				/* 当门卡拔出，只有紧急按钮有效，其他按钮均无效*/
				if (STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0 && device.m_MapItems[i].m_funcType != FUNC_URGENT_KEY)
				{
					KL_VERBOSE(" 当门卡拔出，只有紧急按钮有效，其他按钮均无效\n");
					return 0;
				}
				
				/* 如果该端口已设置为电平关联控制，则退出处理*/
				if (device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_TRIGLE
					||device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_HIGH_LEVEL
					||device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_LOW_LEVEL)
				{
					return 0;
				}
				
				/* 如果为总开关按键，则对其做特殊处理*/
				if (device.m_MapItems[i].m_funcType == FUNC_POWER_KEY)
				{
					if (PowerSwitchControl(device) == true)
					{
						return 0;
					}
				}

				/* 如果为紧急开关按键，则对其做特殊处理*/
/*				if (device.m_MapItems[i].m_funcType == FUNC_URGENT_KEY)
				{
					if (UrgentKeyControl(device.m_MapItems[i]) == true)
					{
						return 0;
					}
				}*/

				/* 如果为清理房间按键，则对其做特殊处理*/
				if (device.m_MapItems[i].m_funcType == FUNC_KEY_CLEANROOM)
				{
					if (CleanRoomKeyControl(device.m_MapItems[i]) == true)
					{
						return 0;
					}					
				}

				/* 如果为洗衣服按键*/
				if (device.m_MapItems[i].m_funcType == FUNC_KEY_WASH_CLOSE)
				{
					if (STATUS.deviceStatus.m_RcuStatus.m_WashCloseButtonState ==0)
					{
						STATUS.deviceStatus.m_RcuStatus.m_WashCloseButtonState = 1;
					}
					else
					{
						STATUS.deviceStatus.m_RcuStatus.m_WashCloseButtonState = 0;
					}
				}


				/* 如果为请勿打扰按键，则对其做特殊处理*/
				if (device.m_MapItems[i].m_funcType == FUNC_KEY_DONOT_DISTURB)
				{
					if (DonotDisturbKeyControl(device.m_MapItems[i]) == true)
					{
						return 0;
					}	
				}

				/* 如果为门铃按键，则对其做特殊处理*/
				if (device.m_MapItems[i].m_funcType == FUNC_KEY_BELL)
				{
					if (DonotDisturbKeyControl(device.m_MapItems[i]) == true)
					{
						return 0;
					}	
				}
				/* 如果组合灯光亮度设置*/
				if (device.m_MapItems[i].m_funcType == FUNC_SCENE_COMPOSE1)
				{
					SceneCompose(1);
					return 0;
				}
				else if (device.m_MapItems[i].m_funcType == FUNC_SCENE_COMPOSE2)
				{
					SceneCompose(2);
					return 0;
				}
				else if (device.m_MapItems[i].m_funcType == FUNC_SCENE_COMPOSE3)
				{
					SceneCompose(3);
					return 0;
				}
				else if (device.m_MapItems[i].m_funcType == FUNC_SCENE_COMPOSE4)
				{
					SceneCompose(4);
					return 0;
				}
				else if (device.m_MapItems[i].m_funcType == FUNC_SCENE_COMPOSE5)
				{
					SceneCompose(5);
					return 0;
				}
				else if (device.m_MapItems[i].m_funcType == FUNC_SCENE_COMPOSE6)
				{
					SceneCompose(6);
					return 0;
				}
				else if (device.m_MapItems[i].m_funcType == FUNC_SCENE_COMPOSE7)
				{
					SceneCompose(7);
					return 0;
				}
				
				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
					if (device.m_MapItems[i].m_mapNbs.test(st))
					{
						int ret;
						tempData.clear();
						tempData.push_back((INT8U)st);//按键控制的子路号
						tempData.push_back(device.m_MapItems[i].m_funcType);//按键特殊功能
						tempData.append(idata);//当前的按键路数及状态值
							//对第几路进行操作
						ret = pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);

						//根据继电器的通断状态来控制按键的背光的打开或关闭
						//KeyBacklightCtrl(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],ret);
					}
				}
			}
			
		}
	}
	return 0;
}

bool CDataItemCmd::IsBelongToOtherSceneComposeMember(SSceneComposeMember member,INT8U refScenePlanNo)
{
	SSceneCompose &SceneCompose = STATUS.deviceStatus.m_SceneComposes[refScenePlanNo-1];	

	for(int i=0; i<SceneCompose.m_MemberNum; i++)
	{
		if (SceneCompose.m_SceneComposeMember[i].m_DeviceNo == member.m_DeviceNo
			&& SceneCompose.m_SceneComposeMember[i].m_SubNo == member.m_SubNo
			&& SceneCompose.m_SceneComposeMember[i].m_Function == member.m_Function
			)
		{
			return true;
		}
	}
	
	return false;
}

//关闭已开的场景
//关闭原来已打开的场景,但新场景要打开的成员在已打开的场景中打开了
//则不需要关闭
//INT8U planNo:原场景号
//INT8U refScenePlanNo:新场景号
void CDataItemCmd::CloseOtherSceneCompose(INT8U planNo,INT8U refScenePlanNo)
{
	SSceneCompose &SceneCompose = STATUS.deviceStatus.m_SceneComposes[planNo-1];

	if (SceneCompose.m_ComposeState == 1)
	{
		for(int i=0; i<SceneCompose.m_MemberNum; i++)
		{
			//判断该场景成员是否与其他某普通场景组合的成员配置一致
			//如果一致就不需要关闭，避免关闭了马上又要打开
			if (IsBelongToOtherSceneComposeMember(SceneCompose.m_SceneComposeMember[i],refScenePlanNo) == true)
			{
				continue;
			}
			
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

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_NORMAL
				||SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_ON)
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
		}
		SceneCompose.m_ComposeState = 0;
		STATUS.deviceStatus.m_CurrentOpenedSceneNo = 0;//清除已开场景的设备号记录
	}
}
//场景组合处理
void CDataItemCmd::SceneCompose(INT8U planNo)
{
	SSceneCompose &SceneCompose = STATUS.deviceStatus.m_SceneComposes[planNo-1];

	//有已打开的场景且现在要控制的场景号不是已打开的场景号
	if (STATUS.deviceStatus.m_CurrentOpenedSceneNo != 0 && planNo != STATUS.deviceStatus.m_CurrentOpenedSceneNo) 
	{
		//打开新场景，关闭原来已打开的场景,但新场景要打开的成员在已打开的场景中打开了
		//则不需要关闭
		CloseOtherSceneCompose(STATUS.deviceStatus.m_CurrentOpenedSceneNo,planNo);
	}
	
	if (SceneCompose.m_ComposeState == 0)
	{
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//亮度
				
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
				tempData.push_back(2);//开
				
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

		SceneCompose.m_ComposeState = 1;
		STATUS.deviceStatus.m_CurrentOpenedSceneNo = planNo;
	}
	else
	{
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

		}
		SceneCompose.m_ComposeState = 0;
		STATUS.deviceStatus.m_CurrentOpenedSceneNo = 0;
	}
}



int CDataItemCmd::SoftLongKeyProcess(Device device,INT8U subno,CDataItem *pDataItem,bstring idata)
{
	if (device.m_MapItemCount > 0)
	{
		bstring tempData;

		for (int i=0; i<device.m_MapItemCount; i++)
		{
			if (device.m_MapItems[i].m_subno == subno)//配置文件是否存在此路号
			{

				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
					if (device.m_MapItems[i].m_mapNbs.test(st))
					{
						int ret;
						tempData.clear();
						tempData.push_back((INT8U)st);//按键控制的子路号
						tempData.push_back(device.m_MapItems[i].m_funcType);//按键特殊功能
						tempData.append(idata);//当前的按键路数及状态值
						//对第几路进行操作
						ret = pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);

						//根据继电器的通断状态来控制按键的背光的打开或关闭
						//KeyBacklightCtrl(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],ret);
					}
				}
			}

		}
	}
	return 0;
}

int CDataItemCmd::OnExc(Device &device,bstring& orData)
{
	if (orData.size()<2)
	{
		return -1;
	}
	INT16U dataid = orData[0] + orData[1]*256;
	
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(dataid);

	if (pDataItem == NULL)
	{
		return -1;
	}

	if (dataid == 0x0110)
	{
		return 0;
	}

	orData.erase(0,2);//去掉dataid

	if (orData.size() == 0)
	{
		return 0;
	}	
	
	INT8U subno=0;

	if (dataid == 0x1000 && orData.size() == 8 )//继电器板状态，8个字节的状态字内容
	{
		memcpy(device.m_StatusData,orData.data(),8);
		return 0;
	}

	if (dataid == 0x2110 )//门卡
	{
		pDataItem = CONTEXT.m_DataItems.GetDataItem(dataid);
		pDataItem->Exec(device,orData);
		return 0;
	}
	
	if (dataid ==  0x3001&&STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 1)//空调风机控制,房卡插入才允许设置
	{
		KL_VERBOSE("device.m_Tn = %d,\n",device.m_Tn);
		KL_VERBOSE("空调风机控制设置前风速为%d,设置后风速为%d\n",m_WindSpeed[device.m_Tn],orData[0]);
//		STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWindSpeed = orData[0];
		if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWindSpeed  == orData[0])
		{
			return 0;
		}
		STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWindSpeed = orData[0];
		
		pDataItem = CONTEXT.m_DataItems.GetDataItem(0X3011);
		pDataItem->Exec(device,orData);
		return 0;
	}

	if (dataid ==  0x1107)//输入输出继电器长短电平处理
	{
		INT16U low_word = orData.at(0) + orData.at(1)*256;
		INT16U high_word = orData.at(2) + orData.at(3)*256;
		
		IORelayShortKey(device,low_word,orData);
		IORelayLongKey(device,high_word,orData);
		return 0;
	}
	
	//按键处理
	//device是否有映射设备要控制，如果有则接下来对映射设备进行命令发送
	if ((dataid == 0x1101||dataid == 0x1104) && (orData.size()>0))//按键路数//短按键
	{
		subno = orData[0];
	}

	if ((dataid == 0x1102||dataid == 0x1105)&& (orData.size()>0))//按键路数//短按键
	{
		subno = orData[0];
	}
		
	SoftKeyProcess(device,subno,pDataItem,orData);
	return 0;
}


int CLoadMapCmd::OnExc(Device &device,bstring& orData)
{
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x2010);
	bstring mapData;
	
	if (pDataItem == NULL)
	{
		KL_VERBOSE("无此数据项\n");
		return -1;
	}
	
	//device是否有映射设备要控制，如果有则接下来对映射设备进行命令发送
	if (device.m_MapItemCount > 0)
	{
		INT8U num=0;
		for (int i=0; i<device.m_MapItemCount; i++)
		{
			if (device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_HIGH_LEVEL
				||device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_LOW_LEVEL
				|| device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_TRIGLE)
			{
				num++;
			}
		}
		mapData.push_back(num);//映射个数
		for (int i=0; i<device.m_MapItemCount; i++)
		{
			if (device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_TRIGLE)
			{
				mapData.push_back(device.m_MapItems[i].m_subno);//输入的路号
				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
						if (device.m_MapItems[i].m_mapNbs.test(st))
						{
							mapData.push_back((INT8U)st);//映射输出的路号
							mapData.push_back((INT8U)0X00);//吸合电平触发
							mapData.push_back((INT8U)0X00);//无效值
						}
				}
			}

			if (device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_HIGH_LEVEL)
			{
				mapData.push_back(device.m_MapItems[i].m_subno);//输入的路号
				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
						if (device.m_MapItems[i].m_mapNbs.test(st))
						{
							mapData.push_back((INT8U)st);//映射输出的路号
							mapData.push_back((INT8U)0X01);//电平触发
							mapData.push_back((INT8U)0X01);//高电平触发
						}
				}
			}

			if (device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_LOW_LEVEL)
			{
				mapData.push_back(device.m_MapItems[i].m_subno);//输入的路号
				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
						if (device.m_MapItems[i].m_mapNbs.test(st))
						{
							mapData.push_back((INT8U)st);//映射输出的路号
							mapData.push_back((INT8U)0X01);//电平触发
							mapData.push_back((INT8U)0X00);//低电平触发
						}
				}
			}

		}
			
	}
	
	return pDataItem->Exec(device,mapData);
}
/*
int CPollPowerCmd::GetPowerDeviceAndState(Device &odevice)
{
	Device device;
	bstring orData;
	/* 设备处理*
	for (DevicesBitSet::SizeType n =  CONTEXT.Devices.m_DBS.FromPos; n< CONTEXT.Devices.m_DBS.ToPos; n++)
	{
		 if ( CONTEXT.Devices.m_DBS.test(n) != true)
		 {
			continue;
		 }
		 device = CONTEXT.Devices.m_Item[n];
		if (device.m_Type != MD_LIGHT_4 && device.m_Type != MD_RELAY_8)
		{
			continue;
		}

		//device是否有映射设备要控制，如果有则接下来对映射设备进行命令发送
		if (device.m_MapItemCount > 0)
		{
			for (int i=0; i<device.m_MapItemCount; i++)
			{
				if ( device.m_MapItems[i].m_funcType ==FUNC_POWER_KEY)
				{
					odevice = device;//总电源开关设备
					
					CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x0900+device.m_MapItems[i].m_subno);
					if (pDataItem->Read(device,orData) == true)
					{
						if (orData[2] == 0)//总电源关
						{
							return 0;
						}
						else
						{
							return 1;//总电源开
						}
					 }
				}
			}
		} 
		
	}
	return -1;
}

int CPollPowerCmd::OnExc(Device &device,bstring& orData)
{
	if (GetPowerDeviceAndState(device) == -1)
	{
		return -1;
	}

	char bTime[7];
	Get_CurBCDTime7(bTime);
	bTime[4]=hour;
	bTime[5]=minute;
	

	

	
	return -1;
	
	
}
*/

int CPollAirConditionCmd::OnExc(void)
{
	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true || CONTEXT.Devices.m_Item[i].m_Type !=  MD_AIRCONDITION)
		 {
			continue;
		 }
		 
		PollAirConditionHandle( CONTEXT.Devices.m_Item[i]);
	}
	return 0;
}
int CPollAirConditionCmd::PollAirConditionHandle(Device device)
{
	bstring orData;
	CDataItem *pDataItem =NULL;
	/* 空调对时*/
	if (STATUS.deviceStatus.m_AirConditionType == 0)
	{
		pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1210);
		if (pDataItem == NULL)
		{
			KL_VERBOSE("无此数据项\n");
			return -1;
		}
		pDataItem->Exec(device,orData);
	}

	//读空调温度
	pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3003);
	if (pDataItem == NULL)
	{
		KL_VERBOSE("无此数据项\n");
		return -1;
	}

	orData.clear();
	bstring iData;

	if (STATUS.deviceStatus.m_AirConditionType == 1 || STATUS.deviceStatus.m_AirConditionType == 2)//modbus协议空调模块
	{
		iData.push_back(0X00);
		iData.push_back(0X01);
		INT8U commNo;
		INT16U dataid;

		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			commNo = MODBUS_A_COMMNO;
			dataid = MODBUS_A_RD_TEMPERATER;
			if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,orData) == true)
			{//把MODBUS协议数据
				if (orData[0] == 0x03&& orData[2]==0x2)//命令码为3，数据长度为2
				{
					temperature = orData.at(4);
					STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirTemperature = temperature;
					KL_VERBOSE("房间温度为%d\n",temperature);
					AirConditonHandle(device);
					return 0;
				}
			}
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			commNo = MODBUS_B_COMMNO;
			dataid = MODBUS_B_RD_TEMPERATER;
			if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,orData) == true)
			{//把MODBUS协议数据
				if (orData[0] == 0x03&& orData[2]==0x33)//命令码为3
				{
					temperature = orData.at(4);
					STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirTemperature = temperature;
					KL_VERBOSE("房间温度为%d\n",temperature);
					AirConditonHandle(device);
					return 0;
				}
			}
		}
	}
	else
	{
		if (pDataItem->Read(device,orData) == true)
		{
			orData.erase(0,2);//去掉dataid
			temperature = orData.at(0);
			STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirTemperature = temperature;
			KL_VERBOSE("房间温度为%d\n",temperature);
			AirConditonHandle(device);
			return 0;
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

int CPollAirConditionCmd::AirConditonHandle(Device device)
{
	INT8U setValue;
	bstring orData;
	bstring iData;
	bstring rtData;
	CDataItem *pDataItem;

		if (STATUS.deviceStatus.m_AirConditionType == 1 || STATUS.deviceStatus.m_AirConditionType == 2)//modbus协议空调模块控制
		{
			/* 设置空调工作模式*/
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3004);
			if (pDataItem == NULL)
			{
				KL_VERBOSE("无此数据项\n");
				return -1;
			}
			iData.clear();
			iData.push_back(0x00);
			if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 1)//制冷
			{
				iData.push_back(0x00);
			}
			else if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 0)
			{
				iData.push_back(0x01);//制热
			}
			else
			{
				iData.push_back(0x02);//通风
			}
			
			rtData.clear();

			INT8U commNo;
			INT16U dataid;

			if (STATUS.deviceStatus.m_AirConditionType == 1)
			{
				commNo = MODBUS_A_COMMNO;
				dataid = MODBUS_A_WR_MODE;
			}
			else if (STATUS.deviceStatus.m_AirConditionType == 2)
			{
				commNo = MODBUS_B_COMMNO;
				dataid = MODBUS_B_WR_MODE;
			}
			
			if (pDataItem->ModbusWrite(device,commNo,MODBUS_WRITE,dataid,iData,rtData) == true)
			{
				//if (rtData[0] == 0x06&& rtData[2]==0x31)//命令码为3，数据长度为2
				{
					KL_VERBOSE("空调工作模式%d 设置成功\n",iData[1]);
				}
			}
		}
		else
		{
			/* 设置空调工作模式*/
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3004);
			if (pDataItem == NULL)
			{
				KL_VERBOSE("无此数据项\n");
				return -1;
			}
			orData.clear();
			orData.push_back(STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode);
			pDataItem->Exec(device,orData);
			
			/* 设置空调季节模式*/
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3005);
			if (pDataItem == NULL)
			{
				KL_VERBOSE("无此数据项\n");
				return -1;
			}
			orData.clear();
			orData.push_back(STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSeason);
			pDataItem->Exec(device,orData);
		}

		/* 读空调设置温度*/
		if (GetAirConditonSettingTemperature(device,setValue)	 == false)
		{
			return -1;
		}
		STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSetTemperture = setValue;
	

		//冬季模式，当房间温度低于设置温度1度，热阀将打开
		if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSeason == SEASON_WINNER)
		{
			if (temperature < (setValue -1))
			{
				/* 打开热单阀*/
				KL_VERBOSE("打开单阀\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3007);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("无此数据项\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x01);//打开阀
				pDataItem->Exec(device,orData);

				/* 打开双管热阀*/
				KL_VERBOSE("打开双阀\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3008);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("无此数据项\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x02);//打开阀
				pDataItem->Exec(device,orData);
			}
			else
			{
				KL_VERBOSE("关闭单阀\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3007);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("无此数据项\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x00);//关闭阀
				pDataItem->Exec(device,orData);

				//关闭双管热阀
				KL_VERBOSE("关闭双阀\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3008);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("无此数据项\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x00);//关闭阀
				pDataItem->Exec(device,orData);

			}
		}

		//夏季模式，当房间温度高于设置温度1度，冷阀将打开
		if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSeason == SEASON_SUMMER)
		{
			KL_VERBOSE("房间温度是%d 度，设置温度是%d 度\n",temperature,setValue);
			if (temperature > (setValue +1))
			{
				/* 打开单管阀*/
				KL_VERBOSE("打开单管阀\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3007);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("无此数据项\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x01);//打开阀
				pDataItem->Exec(device,orData);

				/* 打开双管冷阀*/
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3008);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("无此数据项\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x01);//打开阀
				pDataItem->Exec(device,orData);
				
			}
			else
			{
				//关闭单管阀
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3007);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("无此数据项\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x00);//关闭阀
				pDataItem->Exec(device,orData);

				//关闭双管冷阀
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3008);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("无此数据项\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x00);//关闭阀
				pDataItem->Exec(device,orData);
			}

		}

	return 0;
	
}
/* 获取空调设置温度*/
bool CPollAirConditionCmd::GetAirConditonSettingTemperature(Device device,INT8U &value)
{
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3009);
	if (pDataItem == NULL)
	{
		KL_VERBOSE("无此数据项\n");
		return false;
	}

	bstring orData;

	if (STATUS.deviceStatus.m_AirConditionType == 1 || STATUS.deviceStatus.m_AirConditionType == 2)//modbus协议空调模块控制
	{
		bstring iData;
		INT16U dataid;
		INT8U commNo;

		orData.clear();
			
		iData.push_back(0X00);
		iData.push_back(0X01);

		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 1)//制冷
			{
				dataid = 0x0036;
			}
			else if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 0)
			{
				dataid = 0x004d;//制热
			}
			else
			{
				dataid = 0x004c;//制冷
			}
		}

		if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			dataid = 0x0036;
		}

		commNo = MODBUS_A_COMMNO;

		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			commNo = MODBUS_A_COMMNO;
			if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,orData) == true)
			{//把MODBUS协议数据
				if (orData[0] == 0x03&& orData[2]==0x2)//命令码为3，数据长度为2
				{
					value = orData.at(4);
					KL_VERBOSE("空调设置温度为%d\n",value);
					return true;
				}
			}
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			commNo = MODBUS_B_COMMNO;
			if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,orData) == true)
			{//把MODBUS协议数据
				if (orData[0] == 0x03&& orData[2]==0x36)//命令码为3，通讯地址0x0036
				{
					value = orData.at(4);
					KL_VERBOSE("空调设置温度为%d\n",value);
					return true;
				}
			}
		}
			
		
	}
	else
	{
		//读空调温度
		if (pDataItem->Read(device,orData) == true)
		{
			orData.erase(0,2);//去掉dataid
			value = orData[0];
			KL_VERBOSE("空调设置温度为%d\n",value);
			return true;
		}
		else
		{
			return false;
		}
	}
}

void CPollAirConditionCmd::SetAirConditonTemperature(Device device,INT8U tmp)
{
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3010);
	if (pDataItem == NULL)
	{
		KL_VERBOSE("无此数据项\n");
		return ;
	}

	if (STATUS.deviceStatus.m_AirConditionType == 1 || STATUS.deviceStatus.m_AirConditionType == 2)
	{
		bstring iData;
		bstring orData;
		INT16U dataid;
		INT8U commNo;

		orData.clear();
			
		iData.push_back(0X00);
		iData.push_back(tmp);

		if (STATUS.deviceStatus.m_AirConditionType == 1)//MODBUS_A
		{
			 if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 0)
			{
				dataid = 0x004d;//制热
			}
			else
			{
				dataid = 0x0036;//制冷
			}
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)//MODBUS_B
		{
			 if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 0)
			{
				dataid = 0x004c;//制热
			}
			else
			{
				dataid = 0x004d;//制冷
			}
		}

		commNo = MODBUS_A_COMMNO;

		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			commNo = MODBUS_A_COMMNO;
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			commNo = MODBUS_B_COMMNO;
		}
			
		if (pDataItem->ModbusWrite(device,commNo,MODBUS_WRITE,dataid,iData,orData) == true)
		{//把MODBUS协议数据
		
			KL_VERBOSE("空调设置温度成功\n");
			return;
		}
	}
	else
	{
		bstring Data;
		Data.push_back(tmp);
		if (device.m_Enable == false)
		{
			return;
		}
		KL_VERBOSE("空调设置温度为%d\n",tmp);
		pDataItem->Exec(device,Data);
	}
}


int CCheckTimeCmd::OnExc(Device &device,bstring& orData)
{
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1210);
	if (pDataItem == NULL)
	{
		KL_VERBOSE("无此数据项\n");
		return -1;
	}

	if (device.m_Type != MD_KEYBOARD_20 && device.m_Type != MD_RFID_CARD)
	{
		return -1;
	}
	
	if (pDataItem->Exec(device,orData) == 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
}

/* 打开廊灯*/
/*
	param[in]Device device:门磁测量点所在的设备
	param[in]int min:门磁测量点设备中门磁点所映射的亮灯的点亮持续时间
*/
void CPollDoorCmd::OpenGalleryLight(Device device ,int min)
{
	SDoorSceneCompose &SceneCompose = STATUS.deviceStatus.m_DoorSceneComposes;

	for(int i=0; i<SceneCompose.m_MemberNum; i++)
	{
		if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_GALLERY_LIGHT)
		{
			CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1004);
			if (pDataItem == NULL)
			{
				return;
			}
			bstring tempData;
			tempData.clear();
			tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//第几路
			tempData.push_back(0x01);//通
			tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//持续打开几分钟
			
			int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
		}
	}
}

bool CPollDoorCmd::GetDoorCheckState()
{
	Device device;
/*	for (DevicesBitSet::SizeType i =  CONTEXT.Devices.m_DBS.FromPos; i<CONTEXT.Devices.m_DBS.ToPos; i++)
	{
		 if (CONTEXT.Devices.m_DBS.test(i) != true)
		 {
			continue;
		 }

		 if (CONTEXT.Devices.m_Item[i].m_Type ==  MD_RELAY_8)
		 {
		 	
		 	for (int j=0; j<CONTEXT.Devices.m_Item[i].m_MapItemCount; j++)
			{
				if (CONTEXT.Devices.m_Item[i].m_MapItems[j].m_funcType == FUNC_DOOR_CHECK)//大门门磁检测
				{

					int subno = CONTEXT.Devices.m_Item[i].m_MapItems[j].m_subno;
					INT16U dataid = 0x0900 + subno;
					device = CONTEXT.Devices.m_Item[i];//门磁测量点模块

					CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(dataid);
					if (pDataItem == NULL)
					{
						KL_VERBOSE("无此数据项\n");
						return false;
					}

					bstring Data;

					if (pDataItem->Read(device,Data) == true)
					{
						if (Data[0] == 0 && STATUS.deviceStatus.m_RcuStatus.m_DoorState == 0)//门磁检测有效房门开，门节点电平为0为有效
						{
							STATUS.deviceStatus.m_RcuStatus.m_DoorState = 1;
							//如果门卡未插入，开门将打开廊灯持续亮1分钟
							OpenGalleryLight(device,1);//打开廊灯亮1分钟
						}
						else if (Data[0] == 1 && STATUS.deviceStatus.m_DoorState == 1)
						{
							STATUS.deviceStatus.m_RcuStatus.m_DoorState = 0;
						}
						return true;
					}
					else
					{
						return false;
					}
				}
				
		 	}
			return true;
		}
	}*/
#ifndef WIN32
	int state,fd;

#ifndef WIN32	
	fd = open("/dev/kgeio", O_RDWR );   
	if (fd == -1) 
	{	
		KL_VERBOSE(" open /dev/kgeio fail\n");
	}
	ioctl(fd, KGESG_IO_GET_RCUDOOR, &state);

	close(fd);	
#endif 
	KL_VERBOSE("门磁检测:state=%d,m_DoorState=%d\n",state,STATUS.deviceStatus.m_RcuStatus.m_DoorState);
/*	if (state == 1 && STATUS.deviceStatus.m_RcuStatus.m_DoorState == 0)//门磁检测有效房门开，门节点电平为0为有效
	{
		STATUS.deviceStatus.m_RcuStatus.m_DoorState = 1;
		KL_WARN("大门打开\n");
		//如果门卡未插入，开门将打开廊灯持续亮1分钟
		if (STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0)
		{
			KL_VERBOSE("打开廊灯亮1分钟\n");
			OpenGalleryLight(device,1);//打开廊灯亮1分钟
		}
		
		return true;
	}*/
	if (state == 1 
	    && STATUS.deviceStatus.m_RcuStatus.m_DoorState == 0
	    &&STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0)//门磁检测有效房门开，门节点电平为0为有效
	{
		STATUS.deviceStatus.m_RcuStatus.m_DoorState = 1;
		KL_WARN("大门打开\n");
		//如果门卡未插入，开门将打开廊灯持续亮1分钟
		KL_VERBOSE("打开廊灯亮1分钟\n");
		OpenGalleryLight(device,1);//打开廊灯亮1分钟
		return true;
	}
	else if (state == 0 && STATUS.deviceStatus.m_RcuStatus.m_DoorState == 1)
	{
		STATUS.deviceStatus.m_RcuStatus.m_DoorState = 0;
		KL_WARN("大门关闭\n");
		return true;
	}
	else
	{
		return	false;
	}
#endif
	return false;
}

int CPollDoorCmd::OnExc(void)
{
	GetDoorCheckState();
	return 0;
}


