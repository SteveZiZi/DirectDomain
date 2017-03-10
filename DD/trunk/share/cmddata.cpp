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

//��·��������ͨѶ���ϣ����е�ѹ����
int CPollCmd::OnExcClArg(Devices& devices)
{
	bstring orData;


	for (CLRelayBitSet::SizeType i = CONTEXT.ClRelays.CLRelayBS.FromPos; i<CONTEXT.ClRelays.CLRelayBS.ToPos; i++)
	{
		if (CONTEXT.ClRelays.CLRelayBS.test(i) != true)
		 {
			continue;
		 }

		/* �жϻ�·���������Ƿ����ͨѶ����*/
		bool ret = DetectClRtfComm(CONTEXT.ClRelays.m_Item[i]);

		if (ret == false)
		{
			
			if (STATUS.deviceStatus.m_ClArgFlag[i] == false)
			{
				KL_WARN("��·%d ��ѹ���⴦��\n",CONTEXT.ClRelays.m_Item[i].m_CLRelayNo);
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
		KL_VERBOSE("�޴�������\n");
		CONTEXT.m_DataItems.Init();
		return -1;
	}

	if (device.m_Type == MD_DOORDISPLAY || device.m_Type == MD_RTF|| device.m_Type == MD_LUX_SENSOR)
	{
		return -1;
	}

	if (device.m_Type == MD_AIRCONDITION)//С����û���ò�ѯ�������ظ����ٰ�����������ʱ��ô����
	{
		GetAirconditionStatus(device,orData);
		return 0;	
	}

	if (device.m_Type == MD_RELAY_8 
		||device.m_Type == MD_RELAY_2 
		|| device.m_Type == MD_LIGHT_4
		|| device.m_Type == MD_LEDV12_3)//���̵�ƽ״̬��ѯ
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
{ // 1����һ��

	time_t curSecond;
	static time_t lastSecond = 0;
	
	time(&curSecond) ;
	if (curSecond - lastSecond < 1)
	{
		return 0;
	}
	lastSecond = curSecond;
	
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3001);

	if (STATUS.deviceStatus.m_AirConditionType == 1 || STATUS.deviceStatus.m_AirConditionType == 2)//modbusЭ��յ�ģ�����
	{
		bstring iData;
		INT16U dataid;
		INT8U commNo;

		orData.clear();

		/* ���յ�����*/				
		iData.push_back(0X00);
		iData.push_back(0X01);

		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			dataid = MODBUS_A_RD_WINDSPEED;//�յ�״̬
			commNo = MODBUS_A_COMMNO;
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			dataid = MODBUS_B_RD_WINDSPEED;//�յ�״̬
			commNo = MODBUS_B_COMMNO;
		}
		
		if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,orData) == true)
		{//��MODBUSЭ������
			if (orData[0] == 0x03)//������Ϊ3�����ݳ���Ϊ2
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
				
				KL_VERBOSE("�յ�����Ϊ%d\n",value);
				STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWindSpeed = value;
				//return 0;
			}
		}

		/* ���յ������¶�*/				
		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 1)//����
			{
				dataid = 0x0036;
			}
			else if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 0)
			{
				dataid = 0x004d;//����
			}
			else
			{
				dataid = 0x004c;//����
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
			{//��MODBUSЭ������
				if (rtData[0] == 0x03&& rtData[2]==0x2)//������Ϊ3�����ݳ���Ϊ2
				{
					value = rtData.at(4);
					STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSetTemperture = value;
					KL_VERBOSE("�յ������¶�Ϊ%d\n",value);
				}
			}
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			commNo = MODBUS_B_COMMNO;
			if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,rtData) == true)
			{//��MODBUSЭ������
				if (rtData[0] == 0x03&& rtData[2]==0x36)//������Ϊ3��ͨѶ��ַ0x0036
				{
					value = rtData.at(4);
					STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSetTemperture = value;
					KL_VERBOSE("�յ������¶�Ϊ%d\n",value);
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
	
	//������ť���£����������ʾ��Ϩ��
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
			tempData.push_back(0X00);//�ر�������ű���
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[pmapItem->m_mapTn],tempData);
			STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;
			KL_WARN("������ť���£����������ʾ��Ϩ��\n");
			return true;
		}
		else//RCU����
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
			KL_WARN("������ť���£����������ʾ��Ϩ��\n");
			return true;		
#endif			
		}

	}
	return false;
}
//����ǲ忨ȡ�緽ʽ����ͨ��RCU���Žڵ��⡣
bool CPollCmd::PollDoorCard(Device &device,bstring& orData)
{
	//����ǲ忨ȡ�緽ʽ����ͨ��RCU���Žڵ��⡣
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
			KL_VERBOSE("$$$$$$$$$$$$$$�ſ����롣��\n");
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
			KL_VERBOSE("�ſ�����%d ����ο�\n",(timenow - doorcardout_30Second));
			if (timenow - doorcardout_30Second > STATUS.deviceStatus.m_OutCardDetectTime)//�ѳ��ſ�?������ſ��γ�
			{
				DoorbeforeState = state;
				orData.push_back(0x10);
				orData.push_back(0x21);
				orData.push_back(0x00);
				KL_VERBOSE("$$$$$$$$$$$$$$$$$$$$�ſ��γ�\n");
				return true;
			}
		}
	}
#endif
	return false;
}

//������ⳡ������
void CPollCmd::IrdaSceneCompose(INT8U state)
{
	if (state == 1)
	{//����������
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//����
				
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(2);//��
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_ON)//�ܵ�Դ����ֻ�رռ̵���
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(2);//��
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}
			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_OFF)//�ܵ�Դ����ֻ�رռ̵���
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(3);//��
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

		}

		SceneCompose.m_ComposeState = state;
	}
	else
	{//����������
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(0);//����0����ʾ�ر�
				
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(3);//��
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_OFF)//�ܵ�Դ����ֻ�رռ̵���
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(3);//��
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}
		}
		SceneCompose.m_ComposeState = state;
	}
}



//���������
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
			KL_VERBOSE("��������Ѽ�⵽����\n");
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

			KL_VERBOSE("����%d ����������ж�Ϊ���ˡ�������\n",(STATUS.deviceStatus.m_IrdaDetectTime*60-(timenow - IrdaDetect_30Second)));
			
			if (timenow - IrdaDetect_30Second > STATUS.deviceStatus.m_IrdaDetectTime*60)//�ѳ��ſ�30������ſ��γ�
			{
				beforeState = state;
				
				KL_VERBOSE("�������δ��⵽��������\n");
				STATUS.deviceStatus.m_RcuStatus.m_IrdaDetect = 0;
				IrdaSceneCompose(0);
				
				return true;
			}
		}
#endif
	return false;
}



//��������尴����RCU���Žڵ��⡣
bool CPollCmd::PollDoorBell(Device &device,bstring& orData)
{
#ifndef WIN32
	int fd;

	//KL_VERBOSE("STATUS.deviceStatus.m_DoorDisplayType = %d\n",STATUS.deviceStatus.m_DoorDisplayType);

	//	�����ſ������壬������ŵ���˸
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
	
	//	�����ſ����߰ο�״̬��ʧЧ
	if (STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState == 1 ||STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0 )
	{
		KL_WARN("�����ſ����߰ο�״̬��ʧЧ\n");
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
			KL_VERBOSE("�Ű�������\n");
			return true;
		}
		else if  (beforeState!=state && state == 1) 
		{
			//dataid == 0x2110
			beforeState = state;
			sw = BELL_RING_OFF;
			ioctl(fd, KGESG_IO_SET_RCUBELL, &sw);
			KL_VERBOSE("���尴���ͷ�\n");
			return true;
		}
/*		else if  (beforeState!=state && state == 1 && (tt-recordTick)>3) //3
		{//3//��ʱ3����ж������Դ
			beforeState = state;
			sw = 1;
			ioctl(fd, KGESG_IO_SET_RCUBELL, &sw);
			KL_VERBOSE("���尴���ͷ�\n");
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
					tempData.push_back((INT8U)st);//�������Ƶ���·��
					tempData.push_back((INT8U)state);//��ǰ�İ���·����״ֵ̬
					//���ݼ̵�����ͨ��״̬�����ư����ı���Ĵ򿪻�ر�
					pDI->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);
				}
			}
		}
	}
	return true;
}

//�ܵ�Դ������ϴ���
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//����
				
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(2);//��
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_OFF)//�ܵ�Դ����ֻ�رռ̵���
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(3);//��
				
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(0);//����0����ʾ�ر�
				
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(3);//��
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_OFF)//�ܵ�Դ����ֻ�رռ̵���
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(3);//��
				
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
		/* �ر��ܵ�Դָʾ��*/
		
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
		KL_VERBOSE("1�ܵ�Դָʾ�ƹر�\n");
#endif
//		TotalPowerCompose(0);

	}
	else
	{
		STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState = 1;
		/* ���ܵ�Դָʾ��*/
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
		KL_VERBOSE("�ܵ�Դָʾ�ƴ�\n");
#endif
		TotalPowerCompose(1);
	}
	
//	for (int i=0; i<device.m_MapItemCount; i++)
	{
		/* ҹ���ܵ�Դ�رմ�ҹ��*/
		//��ҹ�䣨18��00��8��00�����ܵ�ԴҪ��ҹ��ָʾ�ƣ�������ܵ�Դ����ҹ��ָʾ��
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
						tempData.push_back((INT8U)st);//�������Ƶ���·��
						tempData.push_back(FUNC_NORMAL);//�������⹦��
						tempData.push_back(i);//�ܵ�Դ��ֵ
						tempData.push_back(0X01);//��ҹ��

						KL_WARN("ҹ���ܵ�Դ�رմ�ҹ��\n");
						STATUS.deviceStatus.m_NightLightState = 1;
						/* ��ҹ��ָʾ��*
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
							KL_VERBOSE("ҹ��ָʾ�ƴ�\n");
						#endif
						//�Եڼ�·ҹ�ƽ��в���
						ret = pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);
						return true;
					}
				}*/

				KL_WARN("ҹ���ܵ�Դ�رմ�ҹ��ָʾ��\n");
				STATUS.deviceStatus.m_NightLightState = 1;
				/* ��ҹ��ָʾ��*/
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
					KL_VERBOSE("ҹ��ָʾ�ƴ�\n");
				#endif
				
		}

		//���ܵ�Դ������
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
				tempData.push_back(0X00);//�ر���������ʾ
				
//				ret = pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);
				STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
				KL_WARN("�ر���������ʾ\n");
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

				sw = CLEAN_LIGHT_OFF;//�ر���������ʾ
				ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
				close(fd);
				STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
				KL_WARN("�ر���������ʾ\n");
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
	//�ſ��γ���Ч
	if (STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0)
	{
		return false;
	}

	if (STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState != 0)
	{
		STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;//�ر������䰴ť
	}
	else
	{
		STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 1;//�����������䰴ť
	}
	
	//���������䰴ť���ſ���ʾ��������ָʾ������
	if ( STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState == 1)
	{
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			KL_WARN("���������䰴ť���ſ���ʾ��������ָʾ����\n");
			CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3104);
			if (pDataItem == NULL)
			{
				return false;
			}
			int ret;
			tempData.clear();
			tempData.push_back(0X01);//����������ʾ
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			
			KL_WARN("���������䰴ť���������ָʾ�ƹر� \n");
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3103);
			if (pDataItem == NULL)
			{
				return false;
			}

			tempData.clear();
			tempData.push_back(0X00);//�ر�������ű���
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;

			KL_WARN("���������䰴ť�����屳�⿪��\n");
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3102);
			if (pDataItem == NULL)
			{
				return false;
			}

			tempData.clear();
			tempData.push_back(0X01);//���屳�⿪��
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
			KL_WARN("���������䰴ť���ſ���ʾ��������ָʾ����\n");
			sw = CLEAN_LIGHT_ON;//����������ʾ
			ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
			
			KL_WARN("���������䰴ť���������ָʾ�ƹر� \n");
			sw = DNT_LIGHT_OFF;//�ر�������ű���
			ioctl(fd, KGESG_IO_SET_RCUDND, &sw );
			STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;


			KL_WARN("���������䰴ť�����屳�⿪��\n");
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
			tempData.push_back(0X00);//�ر���������ʾ
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("�ر���������ʾ\n");
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

			sw = CLEAN_LIGHT_OFF;//�ر���������ʾ
			ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
			close(fd);
			KL_WARN("�ر���������ʾ\n");
			return true;	
#endif			
		}

	}

	return false;
}
//���尴������
bool CDataItemCmd::BellKeyControl(MapItem mapItem)
{
	bstring tempData;

	
	//������ſ�����������˰����壬�������ָʾ����3�롣
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
			tempData.push_back(0X03);//���屳����3��
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("������ſ�����������˰����壬�������ָʾ����3��\n");
			return true;
		}
		else
		{
			//RCU����
			KL_WARN("������ſ�����������˰����壬�������ָʾ����3��\n");
			return true;
		}
	}
	return false;
}

bool CDataItemCmd::DonotDisturbKeyControl(MapItem mapItem)
{
	bstring tempData;

	//�ſ��γ���Ч
	if (STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0)
	{
		return false;
	}

	if (STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState != 0)
	{
		STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 0;//δ��
	}
	else
	{
		STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState = 1;//����
	}
	
	//����������Ű���
	if ( STATUS.deviceStatus.m_RcuStatus.m_DonotDisturButtonState == 1)
	{
		if (STATUS.deviceStatus.m_DoorDisplayType == 1)
		{
			//�����ڰ�������Ű�ť���ر����尴������
			CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3102);
			if (pDataItem == NULL)
			{
				return false;
			}
			int ret;
			tempData.clear();
			tempData.push_back(0X00);//�ر����屳��
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("�����ڰ�������Ű�ť���ر����尴������\n");
			
			//�����ڰ�������Ű�ť���ſ���ʾ�������ָʾ����
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3103);
			if (pDataItem == NULL)
			{
				return false;
			}
			tempData.clear();
			tempData.push_back(0X01);//��������ű���
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("�����ڰ�������Ű�ť���ſ���ʾ�������ָʾ����\n");

			//�����ڰ�������Ű�ť��������ƹص�
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3104);
			if (pDataItem == NULL)
			{
				return false;
			}
			tempData.clear();
			tempData.push_back(0X00);//�ر������䱳��
			
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
			KL_WARN("�����ڰ�������Ű�ť��������ƹص�\n");
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

			//�����ڰ�������Ű�ť���ر����尴������
			sw = BELL_BG_LIGHT_OFF;
			ioctl(fd, KGESG_IO_SET_RCUBELLBG, &sw );
			KL_WARN("�����ڰ�������Ű�ť���ر����尴������\n");

			//�����ڰ�������Ű�ť���ſ���ʾ�������ָʾ����
			sw = DNT_LIGHT_ON;
			ioctl(fd, KGESG_IO_SET_RCUDND, &sw );
			KL_WARN("�����ڰ�������Ű�ť���ſ���ʾ�������ָʾ����\n");

			//�����ڰ�������Ű�ť��������ƹص�
			sw = CLEAN_LIGHT_OFF;
			ioctl(fd, KGESG_IO_SET_RCUCLEAN, &sw );
			STATUS.deviceStatus.m_RcuStatus.m_CleanRoomButtonState = 0;
			KL_WARN("�����ڰ�������Ű�ť��������ƹص�\n");
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
			tempData.push_back(0X00);//�ر�������ű���
			ret = pDataItem->Exec(CONTEXT.Devices.m_Item[mapItem.m_mapTn],tempData);
			KL_WARN("�����ڰ�������Ű�ť���ſ���ʾ�������ָʾ����\n");
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

			//�����ڰ�������Ű�ť���ſ���ʾ�������ָʾ����
			sw = DNT_LIGHT_OFF;
			ioctl(fd, KGESG_IO_SET_RCUDND, &sw );
			close(fd);
			KL_WARN("�����ڰ�������Ű�ť���ſ���ʾ�������ָʾ����\n");
#endif						
		}
	}
	return false;
}


//���ƺ���ʾ
void CDataItemCmd::OpenDoorNoDisplay()
{
	/* ������ʾ����*/
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
				//���ű�����
				 CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3101);
				 bstring wtData;
				 wtData.clear();
				 wtData.push_back(0x01);//��
				 pDataItem->Exec(tempdev,wtData);
		 	}
			else
			{
#ifndef WIN32		
				//���ű�����
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
			tempData.push_back(no);//�ڼ�·
			
			int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			if (ret == 0&&tempData.at(2) == 1)//�̵�����
			{
				KL_WARN("��ͷ���ѿ�\n");
				return true;
			}
		}
	}

	if (count == 0)//���û���䴲ͷ����Ҳ����true
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
			//�����ͷ�Ƶļ̵����ǿ�״̬����Ҫ�ٿ�
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
			tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
			tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//����
			
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

	/* 12·����˶̵�ƽɨ�账��*/
	for (INT8U i=0; i<CONS_IORELAY_INPUTPORT_MAX_COUNT; i++)
	{
		if (bitStat&(0x01<<i))
		{
			INT8U subno = i+1;
			idata.clear();
			idata.push_back(subno);
			idata.push_back(0x01);//state,Ĭ�ϼ̵�����ת
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
	
	/* 12·����˳���ƽɨ�账��*/
	for (INT8U i=0; i<CONS_IORELAY_INPUTPORT_MAX_COUNT; i++)
	{
		if (bitStat&(0x01<<i))
		{
			INT8U subno = i+1;
			idata.clear();
			idata.push_back(subno);
			idata.push_back(0x01);//state,Ĭ�ϼ̵�����ת
			SoftLongKeyProcess(device,subno,pDataItem,idata);
		}
		else
		{
			if (backup_longkey_state[device.m_Tn]&(0x01<<i))
			{
				INT8U subno = i+1;
				idata.clear();
				idata.push_back(subno);
				idata.push_back(0x00);//state,Ĭ�ϼ̵�����ת
				SoftLongKeyProcess(device,subno,pDataItem,idata);
			}
		}
	}
	backup_longkey_state[device.m_Tn] = bitStat;
}

int CDataItemCmd::SoftKeyProcess(Device device,INT8U subno,CDataItem *pDataItem,bstring idata)
{
	/* ����������ܵ�Դ�������ر��ܵ�Դָʾ��*/
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
		KL_VERBOSE("3�ܵ�Դָʾ�ƹر�\n");
		 STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState =0;
	#endif
						
	if (device.m_MapItemCount > 0)
	{
		bstring tempData;
		
		for (int i=0; i<device.m_MapItemCount; i++)
		{
			if (device.m_MapItems[i].m_subno == subno)//�����ļ��Ƿ���ڴ�·��
			{

				//���ܵ�Դ��������£�ҹ��ʱ��������ȵ������Ҵ����𽥵�����20%�������ܵ�Դ������ָʾ�ƣ��ٰ������ִ�����Ӧ�İ������ܣ�
				if ( STATUS.deviceStatus.m_RcuStatus.m_PowerSwitchState == 1
					&& STATUS.deviceStatus.IsNight() == true
				)
				{
					if (IsBedLightOn() == true)//��ͷ���Ѵ�����Ҫ��
					{
						;
					}
					else //�����Ҵ�ͷ��
					{
						OpenBedLight(); 
						return 0;
					}
				}

				/* ���ſ��γ���ֻ�н�����ť��Ч��������ť����Ч*/
				if (STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0 && device.m_MapItems[i].m_funcType != FUNC_URGENT_KEY)
				{
					KL_VERBOSE(" ���ſ��γ���ֻ�н�����ť��Ч��������ť����Ч\n");
					return 0;
				}
				
				/* ����ö˿�������Ϊ��ƽ�������ƣ����˳�����*/
				if (device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_TRIGLE
					||device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_HIGH_LEVEL
					||device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_LOW_LEVEL)
				{
					return 0;
				}
				
				/* ���Ϊ�ܿ��ذ���������������⴦��*/
				if (device.m_MapItems[i].m_funcType == FUNC_POWER_KEY)
				{
					if (PowerSwitchControl(device) == true)
					{
						return 0;
					}
				}

				/* ���Ϊ�������ذ���������������⴦��*/
/*				if (device.m_MapItems[i].m_funcType == FUNC_URGENT_KEY)
				{
					if (UrgentKeyControl(device.m_MapItems[i]) == true)
					{
						return 0;
					}
				}*/

				/* ���Ϊ�����䰴��������������⴦��*/
				if (device.m_MapItems[i].m_funcType == FUNC_KEY_CLEANROOM)
				{
					if (CleanRoomKeyControl(device.m_MapItems[i]) == true)
					{
						return 0;
					}					
				}

				/* ���Ϊϴ�·�����*/
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


				/* ���Ϊ������Ű���������������⴦��*/
				if (device.m_MapItems[i].m_funcType == FUNC_KEY_DONOT_DISTURB)
				{
					if (DonotDisturbKeyControl(device.m_MapItems[i]) == true)
					{
						return 0;
					}	
				}

				/* ���Ϊ���尴��������������⴦��*/
				if (device.m_MapItems[i].m_funcType == FUNC_KEY_BELL)
				{
					if (DonotDisturbKeyControl(device.m_MapItems[i]) == true)
					{
						return 0;
					}	
				}
				/* �����ϵƹ���������*/
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
						tempData.push_back((INT8U)st);//�������Ƶ���·��
						tempData.push_back(device.m_MapItems[i].m_funcType);//�������⹦��
						tempData.append(idata);//��ǰ�İ���·����״ֵ̬
							//�Եڼ�·���в���
						ret = pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);

						//���ݼ̵�����ͨ��״̬�����ư����ı���Ĵ򿪻�ر�
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

//�ر��ѿ��ĳ���
//�ر�ԭ���Ѵ򿪵ĳ���,���³���Ҫ�򿪵ĳ�Ա���Ѵ򿪵ĳ����д���
//����Ҫ�ر�
//INT8U planNo:ԭ������
//INT8U refScenePlanNo:�³�����
void CDataItemCmd::CloseOtherSceneCompose(INT8U planNo,INT8U refScenePlanNo)
{
	SSceneCompose &SceneCompose = STATUS.deviceStatus.m_SceneComposes[planNo-1];

	if (SceneCompose.m_ComposeState == 1)
	{
		for(int i=0; i<SceneCompose.m_MemberNum; i++)
		{
			//�жϸó�����Ա�Ƿ�������ĳ��ͨ������ϵĳ�Ա����һ��
			//���һ�¾Ͳ���Ҫ�رգ�����ر���������Ҫ��
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(0);//����0����ʾ�ر�
				
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(3);//��
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}
		}
		SceneCompose.m_ComposeState = 0;
		STATUS.deviceStatus.m_CurrentOpenedSceneNo = 0;//����ѿ��������豸�ż�¼
	}
}
//������ϴ���
void CDataItemCmd::SceneCompose(INT8U planNo)
{
	SSceneCompose &SceneCompose = STATUS.deviceStatus.m_SceneComposes[planNo-1];

	//���Ѵ򿪵ĳ���������Ҫ���Ƶĳ����Ų����Ѵ򿪵ĳ�����
	if (STATUS.deviceStatus.m_CurrentOpenedSceneNo != 0 && planNo != STATUS.deviceStatus.m_CurrentOpenedSceneNo) 
	{
		//���³������ر�ԭ���Ѵ򿪵ĳ���,���³���Ҫ�򿪵ĳ�Ա���Ѵ򿪵ĳ����д���
		//����Ҫ�ر�
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//����
				
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(2);//��
				
				int ret = pDataItem->Exec(CONTEXT.Devices.m_Item[SceneCompose.m_SceneComposeMember[i].m_DeviceNo],tempData);
			}

			if (SceneCompose.m_SceneComposeMember[i].m_Function == FUNC_RELAY_ON)//�ܵ�Դ����ֻ�رռ̵���
			{
				CDataItem * pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1001);
				if (pDataItem == NULL)
				{
					return;
				}
				
				bstring tempData;
				tempData.clear();
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(2);//��
				
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(0);//����0����ʾ�ر�
				
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
				tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
				tempData.push_back(3);//��
				
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
			if (device.m_MapItems[i].m_subno == subno)//�����ļ��Ƿ���ڴ�·��
			{

				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
					if (device.m_MapItems[i].m_mapNbs.test(st))
					{
						int ret;
						tempData.clear();
						tempData.push_back((INT8U)st);//�������Ƶ���·��
						tempData.push_back(device.m_MapItems[i].m_funcType);//�������⹦��
						tempData.append(idata);//��ǰ�İ���·����״ֵ̬
						//�Եڼ�·���в���
						ret = pDataItem->Exec(CONTEXT.Devices.m_Item[device.m_MapItems[i].m_mapTn],tempData);

						//���ݼ̵�����ͨ��״̬�����ư����ı���Ĵ򿪻�ر�
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

	orData.erase(0,2);//ȥ��dataid

	if (orData.size() == 0)
	{
		return 0;
	}	
	
	INT8U subno=0;

	if (dataid == 0x1000 && orData.size() == 8 )//�̵�����״̬��8���ֽڵ�״̬������
	{
		memcpy(device.m_StatusData,orData.data(),8);
		return 0;
	}

	if (dataid == 0x2110 )//�ſ�
	{
		pDataItem = CONTEXT.m_DataItems.GetDataItem(dataid);
		pDataItem->Exec(device,orData);
		return 0;
	}
	
	if (dataid ==  0x3001&&STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 1)//�յ��������,�����������������
	{
		KL_VERBOSE("device.m_Tn = %d,\n",device.m_Tn);
		KL_VERBOSE("�յ������������ǰ����Ϊ%d,���ú����Ϊ%d\n",m_WindSpeed[device.m_Tn],orData[0]);
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

	if (dataid ==  0x1107)//��������̵������̵�ƽ����
	{
		INT16U low_word = orData.at(0) + orData.at(1)*256;
		INT16U high_word = orData.at(2) + orData.at(3)*256;
		
		IORelayShortKey(device,low_word,orData);
		IORelayLongKey(device,high_word,orData);
		return 0;
	}
	
	//��������
	//device�Ƿ���ӳ���豸Ҫ���ƣ���������������ӳ���豸���������
	if ((dataid == 0x1101||dataid == 0x1104) && (orData.size()>0))//����·��//�̰���
	{
		subno = orData[0];
	}

	if ((dataid == 0x1102||dataid == 0x1105)&& (orData.size()>0))//����·��//�̰���
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
		KL_VERBOSE("�޴�������\n");
		return -1;
	}
	
	//device�Ƿ���ӳ���豸Ҫ���ƣ���������������ӳ���豸���������
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
		mapData.push_back(num);//ӳ�����
		for (int i=0; i<device.m_MapItemCount; i++)
		{
			if (device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_TRIGLE)
			{
				mapData.push_back(device.m_MapItems[i].m_subno);//�����·��
				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
						if (device.m_MapItems[i].m_mapNbs.test(st))
						{
							mapData.push_back((INT8U)st);//ӳ�������·��
							mapData.push_back((INT8U)0X00);//���ϵ�ƽ����
							mapData.push_back((INT8U)0X00);//��Чֵ
						}
				}
			}

			if (device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_HIGH_LEVEL)
			{
				mapData.push_back(device.m_MapItems[i].m_subno);//�����·��
				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
						if (device.m_MapItems[i].m_mapNbs.test(st))
						{
							mapData.push_back((INT8U)st);//ӳ�������·��
							mapData.push_back((INT8U)0X01);//��ƽ����
							mapData.push_back((INT8U)0X01);//�ߵ�ƽ����
						}
				}
			}

			if (device.m_MapItems[i].m_funcType == FUNC_INPUT_PORT_LOW_LEVEL)
			{
				mapData.push_back(device.m_MapItems[i].m_subno);//�����·��
				for(NoBitSet::SizeType st = device.m_MapItems[i].m_mapNbs.FromPos; st != device.m_MapItems[i].m_mapNbs.ToPos; st++)
				{
						if (device.m_MapItems[i].m_mapNbs.test(st))
						{
							mapData.push_back((INT8U)st);//ӳ�������·��
							mapData.push_back((INT8U)0X01);//��ƽ����
							mapData.push_back((INT8U)0X00);//�͵�ƽ����
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
	/* �豸����*
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

		//device�Ƿ���ӳ���豸Ҫ���ƣ���������������ӳ���豸���������
		if (device.m_MapItemCount > 0)
		{
			for (int i=0; i<device.m_MapItemCount; i++)
			{
				if ( device.m_MapItems[i].m_funcType ==FUNC_POWER_KEY)
				{
					odevice = device;//�ܵ�Դ�����豸
					
					CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x0900+device.m_MapItems[i].m_subno);
					if (pDataItem->Read(device,orData) == true)
					{
						if (orData[2] == 0)//�ܵ�Դ��
						{
							return 0;
						}
						else
						{
							return 1;//�ܵ�Դ��
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
	/* �յ���ʱ*/
	if (STATUS.deviceStatus.m_AirConditionType == 0)
	{
		pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1210);
		if (pDataItem == NULL)
		{
			KL_VERBOSE("�޴�������\n");
			return -1;
		}
		pDataItem->Exec(device,orData);
	}

	//���յ��¶�
	pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3003);
	if (pDataItem == NULL)
	{
		KL_VERBOSE("�޴�������\n");
		return -1;
	}

	orData.clear();
	bstring iData;

	if (STATUS.deviceStatus.m_AirConditionType == 1 || STATUS.deviceStatus.m_AirConditionType == 2)//modbusЭ��յ�ģ��
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
			{//��MODBUSЭ������
				if (orData[0] == 0x03&& orData[2]==0x2)//������Ϊ3�����ݳ���Ϊ2
				{
					temperature = orData.at(4);
					STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirTemperature = temperature;
					KL_VERBOSE("�����¶�Ϊ%d\n",temperature);
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
			{//��MODBUSЭ������
				if (orData[0] == 0x03&& orData[2]==0x33)//������Ϊ3
				{
					temperature = orData.at(4);
					STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirTemperature = temperature;
					KL_VERBOSE("�����¶�Ϊ%d\n",temperature);
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
			orData.erase(0,2);//ȥ��dataid
			temperature = orData.at(0);
			STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirTemperature = temperature;
			KL_VERBOSE("�����¶�Ϊ%d\n",temperature);
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

		if (STATUS.deviceStatus.m_AirConditionType == 1 || STATUS.deviceStatus.m_AirConditionType == 2)//modbusЭ��յ�ģ�����
		{
			/* ���ÿյ�����ģʽ*/
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3004);
			if (pDataItem == NULL)
			{
				KL_VERBOSE("�޴�������\n");
				return -1;
			}
			iData.clear();
			iData.push_back(0x00);
			if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 1)//����
			{
				iData.push_back(0x00);
			}
			else if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 0)
			{
				iData.push_back(0x01);//����
			}
			else
			{
				iData.push_back(0x02);//ͨ��
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
				//if (rtData[0] == 0x06&& rtData[2]==0x31)//������Ϊ3�����ݳ���Ϊ2
				{
					KL_VERBOSE("�յ�����ģʽ%d ���óɹ�\n",iData[1]);
				}
			}
		}
		else
		{
			/* ���ÿյ�����ģʽ*/
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3004);
			if (pDataItem == NULL)
			{
				KL_VERBOSE("�޴�������\n");
				return -1;
			}
			orData.clear();
			orData.push_back(STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode);
			pDataItem->Exec(device,orData);
			
			/* ���ÿյ�����ģʽ*/
			pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3005);
			if (pDataItem == NULL)
			{
				KL_VERBOSE("�޴�������\n");
				return -1;
			}
			orData.clear();
			orData.push_back(STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSeason);
			pDataItem->Exec(device,orData);
		}

		/* ���յ������¶�*/
		if (GetAirConditonSettingTemperature(device,setValue)	 == false)
		{
			return -1;
		}
		STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSetTemperture = setValue;
	

		//����ģʽ���������¶ȵ��������¶�1�ȣ��ȷ�����
		if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSeason == SEASON_WINNER)
		{
			if (temperature < (setValue -1))
			{
				/* ���ȵ���*/
				KL_VERBOSE("�򿪵���\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3007);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("�޴�������\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x01);//�򿪷�
				pDataItem->Exec(device,orData);

				/* ��˫���ȷ�*/
				KL_VERBOSE("��˫��\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3008);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("�޴�������\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x02);//�򿪷�
				pDataItem->Exec(device,orData);
			}
			else
			{
				KL_VERBOSE("�رյ���\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3007);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("�޴�������\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x00);//�رշ�
				pDataItem->Exec(device,orData);

				//�ر�˫���ȷ�
				KL_VERBOSE("�ر�˫��\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3008);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("�޴�������\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x00);//�رշ�
				pDataItem->Exec(device,orData);

			}
		}

		//�ļ�ģʽ���������¶ȸ��������¶�1�ȣ��䷧����
		if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirSeason == SEASON_SUMMER)
		{
			KL_VERBOSE("�����¶���%d �ȣ������¶���%d ��\n",temperature,setValue);
			if (temperature > (setValue +1))
			{
				/* �򿪵��ܷ�*/
				KL_VERBOSE("�򿪵��ܷ�\n");
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3007);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("�޴�������\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x01);//�򿪷�
				pDataItem->Exec(device,orData);

				/* ��˫���䷧*/
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3008);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("�޴�������\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x01);//�򿪷�
				pDataItem->Exec(device,orData);
				
			}
			else
			{
				//�رյ��ܷ�
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3007);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("�޴�������\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x00);//�رշ�
				pDataItem->Exec(device,orData);

				//�ر�˫���䷧
				pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3008);
				if (pDataItem == NULL)
				{
					KL_VERBOSE("�޴�������\n");
					return -1;
				}
				orData.clear();
				orData.push_back(0x00);//�رշ�
				pDataItem->Exec(device,orData);
			}

		}

	return 0;
	
}
/* ��ȡ�յ������¶�*/
bool CPollAirConditionCmd::GetAirConditonSettingTemperature(Device device,INT8U &value)
{
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x3009);
	if (pDataItem == NULL)
	{
		KL_VERBOSE("�޴�������\n");
		return false;
	}

	bstring orData;

	if (STATUS.deviceStatus.m_AirConditionType == 1 || STATUS.deviceStatus.m_AirConditionType == 2)//modbusЭ��յ�ģ�����
	{
		bstring iData;
		INT16U dataid;
		INT8U commNo;

		orData.clear();
			
		iData.push_back(0X00);
		iData.push_back(0X01);

		if (STATUS.deviceStatus.m_AirConditionType == 1)
		{
			if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 1)//����
			{
				dataid = 0x0036;
			}
			else if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 0)
			{
				dataid = 0x004d;//����
			}
			else
			{
				dataid = 0x004c;//����
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
			{//��MODBUSЭ������
				if (orData[0] == 0x03&& orData[2]==0x2)//������Ϊ3�����ݳ���Ϊ2
				{
					value = orData.at(4);
					KL_VERBOSE("�յ������¶�Ϊ%d\n",value);
					return true;
				}
			}
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)
		{
			commNo = MODBUS_B_COMMNO;
			if (pDataItem->ModbusRead(device,commNo,MODBUS_READ,dataid,iData,orData) == true)
			{//��MODBUSЭ������
				if (orData[0] == 0x03&& orData[2]==0x36)//������Ϊ3��ͨѶ��ַ0x0036
				{
					value = orData.at(4);
					KL_VERBOSE("�յ������¶�Ϊ%d\n",value);
					return true;
				}
			}
		}
			
		
	}
	else
	{
		//���յ��¶�
		if (pDataItem->Read(device,orData) == true)
		{
			orData.erase(0,2);//ȥ��dataid
			value = orData[0];
			KL_VERBOSE("�յ������¶�Ϊ%d\n",value);
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
		KL_VERBOSE("�޴�������\n");
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
				dataid = 0x004d;//����
			}
			else
			{
				dataid = 0x0036;//����
			}
		}
		else if (STATUS.deviceStatus.m_AirConditionType == 2)//MODBUS_B
		{
			 if (STATUS.deviceStatus.m_AirConditionDevices[device.m_Tn].m_AirWorkMode == 0)
			{
				dataid = 0x004c;//����
			}
			else
			{
				dataid = 0x004d;//����
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
		{//��MODBUSЭ������
		
			KL_VERBOSE("�յ������¶ȳɹ�\n");
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
		KL_VERBOSE("�յ������¶�Ϊ%d\n",tmp);
		pDataItem->Exec(device,Data);
	}
}


int CCheckTimeCmd::OnExc(Device &device,bstring& orData)
{
	CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(0x1210);
	if (pDataItem == NULL)
	{
		KL_VERBOSE("�޴�������\n");
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

/* ���ȵ�*/
/*
	param[in]Device device:�ŴŲ��������ڵ��豸
	param[in]int min:�ŴŲ������豸���Ŵŵ���ӳ������Ƶĵ�������ʱ��
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
			tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_SubNo);//�ڼ�·
			tempData.push_back(0x01);//ͨ
			tempData.push_back(SceneCompose.m_SceneComposeMember[i].m_FuncData[0]);//�����򿪼�����
			
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
				if (CONTEXT.Devices.m_Item[i].m_MapItems[j].m_funcType == FUNC_DOOR_CHECK)//�����Ŵż��
				{

					int subno = CONTEXT.Devices.m_Item[i].m_MapItems[j].m_subno;
					INT16U dataid = 0x0900 + subno;
					device = CONTEXT.Devices.m_Item[i];//�ŴŲ�����ģ��

					CDataItem *pDataItem = CONTEXT.m_DataItems.GetDataItem(dataid);
					if (pDataItem == NULL)
					{
						KL_VERBOSE("�޴�������\n");
						return false;
					}

					bstring Data;

					if (pDataItem->Read(device,Data) == true)
					{
						if (Data[0] == 0 && STATUS.deviceStatus.m_RcuStatus.m_DoorState == 0)//�Ŵż����Ч���ſ����Žڵ��ƽΪ0Ϊ��Ч
						{
							STATUS.deviceStatus.m_RcuStatus.m_DoorState = 1;
							//����ſ�δ���룬���Ž����ȵƳ�����1����
							OpenGalleryLight(device,1);//���ȵ���1����
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
	KL_VERBOSE("�Ŵż��:state=%d,m_DoorState=%d\n",state,STATUS.deviceStatus.m_RcuStatus.m_DoorState);
/*	if (state == 1 && STATUS.deviceStatus.m_RcuStatus.m_DoorState == 0)//�Ŵż����Ч���ſ����Žڵ��ƽΪ0Ϊ��Ч
	{
		STATUS.deviceStatus.m_RcuStatus.m_DoorState = 1;
		KL_WARN("���Ŵ�\n");
		//����ſ�δ���룬���Ž����ȵƳ�����1����
		if (STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0)
		{
			KL_VERBOSE("���ȵ���1����\n");
			OpenGalleryLight(device,1);//���ȵ���1����
		}
		
		return true;
	}*/
	if (state == 1 
	    && STATUS.deviceStatus.m_RcuStatus.m_DoorState == 0
	    &&STATUS.deviceStatus.m_RcuStatus.m_DoorCardState == 0)//�Ŵż����Ч���ſ����Žڵ��ƽΪ0Ϊ��Ч
	{
		STATUS.deviceStatus.m_RcuStatus.m_DoorState = 1;
		KL_WARN("���Ŵ�\n");
		//����ſ�δ���룬���Ž����ȵƳ�����1����
		KL_VERBOSE("���ȵ���1����\n");
		OpenGalleryLight(device,1);//���ȵ���1����
		return true;
	}
	else if (state == 0 && STATUS.deviceStatus.m_RcuStatus.m_DoorState == 1)
	{
		STATUS.deviceStatus.m_RcuStatus.m_DoorState = 0;
		KL_WARN("���Źر�\n");
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


