#ifndef INCLUDED_TASKDATASAVES_H
#define INCLUDED_TASKDATASAVES_H

#include "../../share/SysBase.h"
#include "../../include/Config.h"
#include "../../base/String.h"


struct Task;
struct Meter;
class RegItems;


struct VIPDATA
{
private:
	//6个重点用户，每个重点用户保存16个数据项，一个数据项18个字节，共6*16*18
	INT8U VIPData[1728];
private:

		
	
public:
	inline VIPDATA()
	{
		memset(VIPData,0xFF,sizeof(VIPData));
	}


	inline int GetMeterVIPData(INT16U MeterNo,INT8U Intervalhours,INT8U *data)
	{
		if((MeterNo<1201)||(MeterNo>1206))
		{
			return -1;
		}
		if(Intervalhours>48)
		{
			return -1;
		}
		int meterpos=MeterNo-1201;
		memcpy(data,VIPData+meterpos*288+(Intervalhours-1)*6,6);
		return 0;

	}

	inline int SetMeterVIPData(INT16U MeterNo,INT8U fromhours,INT8U counthours,INT8U *data)
	{
		if((MeterNo<1201)||(MeterNo>1206))
		{
			return -1;
		}
		if((fromhours>48)||((fromhours+counthours)>48))
		{
			return -1;
		}
		int meterpos=MeterNo-1201;
		memcpy(VIPData+meterpos*288+(fromhours-1)*6,data,6*counthours);
		return 0;

	}

};


struct FlexRegItem
{
	unsigned int RegID;//能量寄存器ID
	unsigned char Time[7];//YYYYMMDDHHNNSS,BCD码
	unsigned short ObjID;//对象ID
	bstring Data;
	INT8U index; //CGH 2010-05-19 增加下标 解决一个regitem可能包含多个数据项的问题
};

struct FlexRegItems:public vector<FlexRegItem>
{
};

class TaskDataSaves
{
 public:
	static int WriteRegDataToFile_Day(MeterBitSet &iMBS,Task&task);
	static bool SaveDataFile_Day(INT16U MeterNo);
	static bool SaveDataFile_Month(INT16U MeterNo);
	static int WriteRegDataToFile_Month(MeterBitSet &iMBS,Task&task);
	static int WriteRegDataToFile_VIP(MeterBitSet &iMBS,Task&task);
	static int WriteRegDataToFile_Event(Task &task,Meter& meter,RegItems &m_RegItems);
	static int WriteRegDataToFile(MeterBitSet &iMBS,Task&task); 
	static void GetAbnormityInfo(INT16U MeterNo,int Abnormityid,bstring &data);
	static void UpdateMeterRecordTime(const MeterBitSet &mbs,Task &task,time_t t_RecordTime=0);
	static void SaveToVipData(INT16U MeterNo,INT8U fromhours,INT8U counthours,INT8U *data);
	static void ReadVipData(INT16U MeterNo,INT8U Intervalhours,INT8U *data);
private:
	static int FormatRegItems(RegItems &m_RegItems,FlexRegItems &flexRegItems);
private:
	 static VIPDATA VipData;
	 static FlexRegItems flexRegItems;
};

#endif
