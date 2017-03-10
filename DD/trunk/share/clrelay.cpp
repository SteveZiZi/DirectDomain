#include "clrelay.h"
#include "../include/FileDir.h"
#include "../config/confuse/confuse.h"
#include "../base/KLTrace.h"
#include "../base/KLDump.h"
#include "../base/FileBase.h"
#include "context.h"
#include "RtfCtrl.h"
#include "ImportmentAlarmFile.h"
#include "ipcStatus.h"
#include "mcp3208.h"

void S_CLRelay::Init(INT16U CLRelayNo)
{
	memset(this,0,sizeof(*this));
	m_CLRelayNo = CLRelayNo;
}

void S_CLRelay::GetAddr_6(INT8U *ipDest)
{
	char buf[2];
	char addr[24];
	char *const ipSrc=this->m_Addr;
	memset(addr,CONS_DEFAULT_METER_ADDR,sizeof(addr));
	size_t len=strlen(ipSrc);
	if (len<=12)
	{
		if (len % 2==0)
		{
			strcpy(addr+12-len,ipSrc);
		}
		else
		{
			sprintf(addr+12-(len+1),"0%s",ipSrc);
		}
	}
	else
	{
		ASSERT(false);
		strncpy(addr,ipSrc,12);
	}
	for (int i=0;i<6;i++)
	{

		buf[0]=addr[i*2];
		buf[1]=addr[i*2+1];
		ipDest[5-i]=String_Asc2TouChar(buf);
	}
}

void S_CLRelay::SaveAddr(unsigned char *BCD,INT8U len,bool reverse)
{
	if (reverse)
	{
		reverse_str((char*)BCD,len);
	}
	memset(m_Addr,'0',sizeof(m_Addr));
	ASSERT(len<=6);
	for (int i=0; i<len; ++i)
	{
		String_uCharToAsc2(BCD[i],m_Addr+2*i);
	}
}

bool S_CLRelay::operator==(S_CLRelay& CLRelay)
{
	return (strcmp(m_Addr,CLRelay.m_Addr)==0);
}


bool S_CLRelay::operator=(S_CLRelay& CLRelay)
{
	m_Enable=CLRelay.m_Enable;
	m_CLRelayNo=CLRelay.m_CLRelayNo;

	strcpy(m_Addr,CLRelay.m_Addr);
	m_ZBXW=CLRelay.m_ZBXW;

	CLRelay.m_Dbs.Stat();
;
	for(MeterBitSet::SizeType i=CLRelay.m_Dbs.FromPos;i<CLRelay.m_Dbs.ToPos;i++)
	{
		if(CLRelay.m_Dbs.test(i))
			m_Dbs.set(i);
	}
	m_Dbs.Stat();
	return true;
}

bool S_CLRelay::IsLeak(void)
{
	if (m_ClLeakInfo.m_Enable == false)
	{
		KL_VERBOSE("m_ClLeakInfo.m_Enable == false\n");
		return false;
	}

	INT8U adChannel = 0;	
	int rddtVol=0;

	int ret;
#ifndef WIN32
	adChannel = m_ClLeakInfo.m_ChannelNo;
	if (adChannel >= 1 && adChannel <= 8)
	{
		ret = mcp3208_read(&rddtVol, adChannel-1);
	}
	else
	{
		KL_VERBOSE("adChannel 值为%d,超出1~8的有效范围\n",adChannel);
		return false;
	}
	KL_VERBOSE("clrelayno = %d,adChannel=%d,rddtVol=%x\n",m_CLRelayNo,adChannel,rddtVol);
#endif	
	if (m_ClLeakInfo.m_DtVol < rddtVol)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void S_CLRelays::Init()
{
	for(size_t i=0;i<sizeof(m_Item)/sizeof(m_Item[0]);i++)
	{
		m_Item[i].Init((INT16U)i);
	}
}
void S_CLRelays::OnChanged()
{
/*
	RBS.Stat();
	ValidRBS=RBS;
	DECLRelayARE_CONTEXT(oContext);
	Meters &i_Meters=oContext.Meters;
	for(RelayBitSet::SizeType i=RBS.FromPos;i<RBS.ToPos;i++)
	{
		if ((RBS.test(i)==false)||(Item[i].IsValid()==false))
		{
			ValidRBS.reset(i);
		}
		else
		{
			MeterBitSet imbs(Item[i].MBS);
			for(MeterBitSet::SizeType j=imbs.FromPos;j<imbs.ToPos;j++)
			{
				Meter &meter=i_Meters.Item[j];
				if((imbs.test(j))&&(meter.Enable==1))
				{
					meter.RelayNo=(INT16U)i;
				}
			}
		}
	}
	ValidRBS.Stat();*/
}


S_CLRelay S_CLRelays::GetCLRelayInfo(const INT16U CLRelayNo)
{
	return m_Item[CLRelayNo];
}

S_CLRelay S_CLRelays::GetCLRelayInfo(const char *CLRelayAddr)
{
	S_CLRelay CLRelay;
	for (RelayBitSet::SizeType i = CLRelayBS.FromPos;i != CLRelayBS.ToPos;i++)
	{
		if (CLRelayBS.test(i))
		{
			if (memcmp(CLRelayAddr,m_Item[i].m_Addr,CONS_METER_ADDR_MAX_LEN+1) == 0)
			{
				CLRelay = m_Item[i];
			}
		}
	}
	return CLRelay;
}

int S_CLRelays::DelCLRelay(int whichRelay,bool force)
{
/*	if (whichRelay<1 || whichRelay>CONS_RELAY_MAX_COUNT)
	{
		return -1;
	}
	DevicesBitSet& dbs =m_Item[whichRelay].m_Dbs;
	//删除一个终端，其下的电表也被删除。刘伟08.02.26

	if(!force)
		if(dbs.count()!=(DevicesBitSet::SizeType)0)
			return 0;

	for (INT16U i=0; i<CONS_METER_MAX_COUNT; ++i)
	{
		if (dbs.test(i))
		{
			CONTEXT.Meters.DelMeter(i);
		}
	}

	INT16U rn= Item[whichRelay].RouterNo;
	RBS.reset(whichRelay);
	Item[whichRelay].Init(whichRelay);
	OnChanged();
	return (INT32U)rn;*/
	return 0;
}
	
//int Relays::DelRelay(INT8U whichRelay)
//{
//	int wh=(int)whichRelay;
//	return DelRelay(wh);
//}

int S_CLRelays::lsHavedCLRelay(const char *temp)
{
	int result=CONS_CLRELAY_MAX_COUNT+1;
	for(int i=1;i<CONS_CLRELAY_MAX_COUNT;i++)
	{
		if(m_Item[i].m_Enable==0)
			continue;
		if(strcmp(temp,m_Item[i].m_Addr)==0)
		{
			return i;
		}
	}
	return result;
}

int S_CLRelays::HadCLRelay(S_CLRelay& clrelay)
{
	for(int i=1;i<CONS_CLRELAY_MAX_COUNT;i++)
	{
		if(m_Item[i].m_Enable == 0)
			continue;
		if(m_Item[i] == clrelay)
		{
			return i;
		}
	}
	return CONS_CLRELAY_MAX_COUNT+1;
}


INT16U S_CLRelays::FindCLRelayNo(const INT8U CLRelayAddr6[6])
{
	for (INT16U i=0; i<CONS_CLRELAY_MAX_COUNT; i++)
	{
		if (CLRelayBS.test(i))
		{
			INT8U ipDest[6];
			S_CLRelay& relay=m_Item[i];
			GetAddr_6_Fix0(relay.m_Addr,ipDest);
			if (strncmp((char *)CLRelayAddr6,(char *)ipDest,6) == 0)
			{
				return i;
			}			
		}
	}

	return 0;
}


S_CLRelay &S_CLRelays::FindCLRelay(const INT8U CLRelayAddr6[6])
{
		int relayno = FindCLRelayNo(CLRelayAddr6);

		if (relayno != 0)
		{
				return CONTEXT.ClRelays.m_Item[relayno];
		}
		else
			return CONTEXT.ClRelays.m_Item[0]; //没有找到则返回默认0
}

INT16U S_CLRelays::GetCLRelayNo(int start)
{
	int i=start; 
	if(i==0)
		i=1;
	while(i<CONS_CLRELAY_MAX_COUNT)
	{
		if (m_Item[i].m_Enable==0x00)
		{
			return i;
		}
		i++;
	}
	return 0;
}
int S_CLRelays::AddCLRelay(INT16U TN,const char  *m_Addr,INT8U m_ZBXW)
{
	int result=CONS_RELAY_MAX_COUNT+1;

	int rt=lsHavedCLRelay(m_Addr);
	if(rt==result)
	{
		for(int i=1;i<CONS_RELAY_MAX_COUNT;i++)
		{
			if(m_Item[i].m_Enable==1)
				continue;
			else
			{
				S_CLRelay& clrelay=m_Item[i];
				clrelay.m_Enable=1;
				strncpy(clrelay.m_Addr,m_Addr,CONS_METER_ADDR_MAX_LEN+1);
				clrelay.m_ZBXW=m_ZBXW;
				clrelay.m_Dbs.set(TN);
				CLRelayBS.set(i);
				clrelay.m_Dbs.Stat();
				CLRelayBS.Stat();
				return i;
			}
		}
	}
	else
	{
		S_CLRelay& clrelay=m_Item[rt];
		clrelay.m_Enable=1;
		clrelay.m_ZBXW=m_ZBXW;
		clrelay.m_Dbs.set(TN);
		CLRelayBS.set(rt);
		clrelay.m_Dbs.Stat();
		CLRelayBS.Stat();
		return rt;
	}
	return result;
}

int S_CLRelays::AddCLRelay()
{
	for(int i=1;i<CONS_CLRELAY_MAX_COUNT;i++)
	{
		if(m_Item[i].m_Enable==1)
			continue;
		else
		{
			return i;
		}
	}
	return CONS_CLRELAY_MAX_COUNT+1;
}

bool S_CLRelays::IsExist(INT16U relayno)
{
	for(int i=1;i<CONS_CLRELAY_MAX_COUNT+1;i++)
	{
		if(m_Item[i].m_Enable==1&&m_Item[i].m_CLRelayNo == relayno)
			return true;
	}
	return false;
}

int S_CLRelays::GetCLRelayNum()
{
	int count=0;
	for(int i=1;i<CONS_CLRELAY_MAX_COUNT;i++)
	{
		if(m_Item[i].m_Enable==1)
			count+=1;
	}
	return count;
}

void S_CLRelays::LeakDetect(void)
{
	for (CLRelayBitSet::SizeType i=CLRelayLeakDtBS.FromPos; i<CLRelayLeakDtBS.ToPos; i++)
	{
		if (CLRelayLeakDtBS.test(i) == false)
		{
			continue;
		}

		S_CLRelay &Clrekay = m_Item[i];
		bool leac_state = Clrekay.IsLeak(); 
		if (leac_state && (STATUS.deviceStatus.m_CLStatus.GetClLeakState(Clrekay.m_CLRelayNo) == 0))
		{
			STATUS.deviceStatus.m_CLStatus.SetClLeakState(Clrekay.m_CLRelayNo,1);
			IPCSTATUS.m_ClleakState[Clrekay.m_CLRelayNo] = 1;
			/* 产生漏电告警*/
			{
				KL_VERBOSE("回路%d 漏电超限产生\n",Clrekay.m_CLRelayNo);
				char bcdtime7[7];
				INT8U AlarmData[3];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 2;//告警内容长度
				AlarmData[1] = Clrekay.m_CLRelayNo;
				AlarmData[2] = 1;//漏电超限产生
				KL_VERBOSE("--------  IMPALARM_CL_LEAK_CREATE-----------\n");
				ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CL_LEAK_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}

			/* 产生漏电切电动作 */
			if (Clrekay.m_ClLeakInfo.m_CutEnable == true)//切电使能打开
			{
				time_t timenow,tt;
				time(&timenow);
				tt = timenow + Clrekay.m_ClLeakInfo.m_DelayMs/1000;
				STATUS.deviceStatus.m_CLStatus.SetClLeakActionTime(Clrekay.m_CLRelayNo,tt);
			}
		}

		if (leac_state ==false && STATUS.deviceStatus.m_CLStatus.GetClLeakState(Clrekay.m_CLRelayNo) == 1)
		{
			STATUS.deviceStatus.m_CLStatus.SetClLeakState(Clrekay.m_CLRelayNo,0);			
			IPCSTATUS.m_ClleakState[Clrekay.m_CLRelayNo] = 0;
			/* 恢复漏电告警*/
			{
				KL_VERBOSE("回路%d 漏电超限恢复\n",Clrekay.m_CLRelayNo);
				char bcdtime7[7];
				INT8U AlarmData[3];
				Get_CurBCDTime7(bcdtime7);
				AlarmData[0] = 2;//告警内容长度
				AlarmData[1] = Clrekay.m_CLRelayNo;
				AlarmData[2] = 0;//漏电超限产生
				KL_VERBOSE("--------  IMPALARM_CL_LEAK_CREATE-----------\n");
				ImpAlarmFile::AddAlarmRecord(AlarmData,sizeof(AlarmData),IMPALARM_CL_LEAK_STATE,bcdtime7,CONS_UNVALID_METER_NUMBER);
			}
		}
	}

	CRtfCtrl rtfctl;
	time_t timenow;
	
	for (CLRelayBitSet::SizeType i=CLRelayLeakDtBS.FromPos; i<CLRelayLeakDtBS.ToPos; i++)
	{
		if (CLRelayLeakDtBS.test(i) == false)
		{
			continue;
		}
		time(&timenow);
		S_CLRelay &Clrekay  = m_Item[i];
		if (STATUS.deviceStatus.m_CLStatus.GetClLeakState(Clrekay.m_CLRelayNo) == 1 && Clrekay.m_ClLeakInfo.m_CutEnable == true)
		{
			if (STATUS.deviceStatus.m_CLStatus.GetClLeakActionTime(Clrekay.m_CLRelayNo) != 0
				&& timenow >= STATUS.deviceStatus.m_CLStatus.GetClLeakActionTime(Clrekay.m_CLRelayNo))
			{
				SSmartGarageItem command;
				command.m_DeviceNo = Clrekay.m_CLRelayNo;
				KL_VERBOSE("回路%d执行切电动作\n",Clrekay.m_CLRelayNo);
				rtfctl.RtfClOffCtl(command);
				STATUS.deviceStatus.m_CLStatus.SetClLeakActionTime(Clrekay.m_CLRelayNo,0);//动作已执行，清除执行时间
			}
		}
	}
	
}

