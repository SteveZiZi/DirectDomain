#include "RtfCtrl.h"

int CRtfCtrl::RtfVolCurSetCtl(SSmartGarageItem &command, INT16U vol,INT16U cur)
{
	CRtfCmdItem * pRtfCmdItem = NULL;
	pRtfCmdItem = CONTEXT.m_RtfCmdItems.GetRtfCmdItem(0x0601);
	if (pRtfCmdItem == NULL)
	{
		return -1;
	}
	
	bstring tempData;
	tempData.clear();
	tempData.push_back((INT8U)(vol>>8&0xff));//��ѹ���ֽ�
	tempData.push_back((INT8U)(vol&0xff));//��ѹ���ֽ�
	tempData.push_back((INT8U)(cur>>8&0xff));//���Ƶ������ֽ�
	tempData.push_back((INT8U)(cur&0xff));//���Ƶ������ֽ�

	int ret = pRtfCmdItem->Exec(CONTEXT.Devices.m_Item[command.m_DeviceNo],tempData);
	return ret;
}

int CRtfCtrl::RtfClVolCurSetCtl(SSmartGarageItem &command, INT16U vol,INT16U cur)
{
	S_CLRelays& clrelays = CONTEXT.ClRelays;
	bstring tempData;
	tempData.clear();

	int ret=0;
	for (DevicesBitSet::SizeType i = clrelays.m_Item[command.m_DeviceNo].m_Dbs.FromPos; i<clrelays.m_Item[command.m_DeviceNo].m_Dbs.ToPos;i++)
	{
		if (clrelays.m_Item[command.m_DeviceNo].m_Dbs.test(i) != true)
		{
			continue;
		}

		if (CONTEXT.Devices.m_Item[i].m_Enable == false
			||CONTEXT.Devices.m_Item[i].m_Type != MD_RTF)
		{
			continue;
		}	
		
		CRtfCmdItem * pRtfCmdItem = NULL;
		pRtfCmdItem = CONTEXT.m_RtfCmdItems.GetRtfCmdItem(0x0601);
		if (pRtfCmdItem == NULL)
		{
			return -1;
		}

		tempData.clear();
		tempData.push_back((INT8U)(vol>>8&0xff));//��ѹ���ֽ�
		tempData.push_back((INT8U)(vol&0xff));//��ѹ���ֽ�
		tempData.push_back((INT8U)(cur>>8&0xff));//���Ƶ������ֽ�
		tempData.push_back((INT8U)(cur&0xff));//���Ƶ������ֽ�
		if (pRtfCmdItem->Exec(CONTEXT.Devices.m_Item[i],tempData) == -1)
			ret = -1;
	}
	return ret;
}

int CRtfCtrl::RtfClOnCtl(SSmartGarageItem &command)
{
	S_CLRelays& clrelays = CONTEXT.ClRelays;
	SSmartGarageItem cmd;
	bstring tempData;
	tempData.clear();

	for (DevicesBitSet::SizeType i = clrelays.m_Item[command.m_DeviceNo].m_Dbs.FromPos; i<clrelays.m_Item[command.m_DeviceNo].m_Dbs.ToPos;i++)
	{
		if (clrelays.m_Item[command.m_DeviceNo].m_Dbs.test(i) != true)//�˴���command.m_DeviceNo�ǻ�·�ţ�Ҳ����clrelay��
		{
			continue;
		}

		if (CONTEXT.Devices.m_Item[i].m_Enable == false)
		{
			continue;
		}

		cmd.m_DeviceNo = CONTEXT.Devices.m_Item[i].m_Tn;
		cmd.m_SubNo = 0;
			
		RtfSwitchSetCtl(cmd, 0, 0);
	}
	return 0;
}

int CRtfCtrl::RtfClOffCtl(SSmartGarageItem &command)
{
	S_CLRelays& clrelays = CONTEXT.ClRelays;
	SSmartGarageItem cmd;
	bstring tempData;
	tempData.clear();

	for (DevicesBitSet::SizeType i = clrelays.m_Item[command.m_DeviceNo].m_Dbs.FromPos; i<clrelays.m_Item[command.m_DeviceNo].m_Dbs.ToPos;i++)
	{
		if (clrelays.m_Item[command.m_DeviceNo].m_Dbs.test(i) != true)
		{
			continue;
		}

		if (CONTEXT.Devices.m_Item[i].m_Enable == false)
		{
			continue;
		}

		cmd.m_DeviceNo = CONTEXT.Devices.m_Item[i].m_Tn;
		cmd.m_SubNo = 0;
			
		RtfSwitchSetCtl(cmd, 1, 0);
	}
	return 0;
}

int CRtfCtrl::RtfSwitchSetCtl(SSmartGarageItem &command, INT8U state, INT8U delayTime)
{
	CRtfCmdItem * pRtfCmdItem = NULL;
	pRtfCmdItem = CONTEXT.m_RtfCmdItems.GetRtfCmdItem(0x0602);
	if (pRtfCmdItem == NULL)
	{
		return -1;
	}
	
	bstring tempData;
	tempData.clear();
	tempData.push_back(state);//����״̬
	tempData.push_back(delayTime);//�������ֽ�
	int ret = pRtfCmdItem->Exec(CONTEXT.Devices.m_Item[command.m_DeviceNo],tempData);
	return ret;
}

bool CRtfCtrl::GetRtfClVol(INT16U clNo,INT16U& clVol)
{
	S_CLRelays& clrelays = CONTEXT.ClRelays;
	SSmartGarageItem cmd;
	bstring tempData;
	tempData.clear();
	INT16U lastvol=0,tmpvol=0;

	if (clNo>CONS_CLRELAY_MAX_COUNT || clNo == 0)
		return false;
	
	for (DevicesBitSet::SizeType i = clrelays.m_Item[clNo].m_Dbs.FromPos; i<clrelays.m_Item[clNo].m_Dbs.ToPos;i++)
	{
		if (clrelays.m_Item[clNo].m_Dbs.test(i) != true)
		{
			continue;
		}

		if (CONTEXT.Devices.m_Item[i].m_Enable == false)
		{
			continue;
		}

		
		tmpvol = (INT16U)((CONTEXT.m_DevicesRegData.m_Items[i].mReg_0501[0]<<8)|(CONTEXT.m_DevicesRegData.m_Items[i].mReg_0501[1]) );
		if (tmpvol >= lastvol)	//��·��ѹȡ��·��������ѹ���ֵ
			lastvol = tmpvol;
	}
	clVol = lastvol;
	return true;
}

