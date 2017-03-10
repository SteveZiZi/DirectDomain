///*Meter.h中包括电表（广义上的电表，即测量点）的定义*/
//测量点号，从0至CONS_METER_MAX_COUNT-1

#ifndef INCLUDED_METER
#define INCLUDED_METER

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_STATUS
#include "Status.h"
#endif

#ifndef INCLUDED_PORT_SET
#include "../base/PortSet.h"
#endif

#ifndef INCLUDED_DATA_ID
#include "DataID.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

#ifndef INCLUDED_METER_GATHER_INFO
#include "MeterGatherInfo.h"
#endif

class KLDump;
//日电量采集时间(3)＋日电量数据(4)＋月电量采集时间(3)＋月电量数据(4)+电表采集失败天数(1)+校时失败次数(1)；
#define METER_GATHER_DATALEN 20
#define VIP_GATHER_DATALEN 8

struct Meter
{
	INT16U MeterNo;//电表号，测量点号,MeterNo＝1表示多功能总表	
	INT8U operatecode;//操作类型
	INT8U Enable;//有效性，1表示有效，0无效
	INT8U BusNo;//总线号，0表示未知,从1到CONS_DBCJ_MAX_COUNT
	INT8U RuleID;//规约ID，0表示无效
	INT8U MeterType;//属性，参见METER_TYPE	电表类型，1表示普通用户表，2表示分总表，3总表
	INT8U Prop;//属性，参见METER_PROPERTY	电表属性，1表示普通485表，6表示载波通讯表，7表示简易多功能表
	//INT8U PowerOffFlag;//是否具备停电事件
	//INT8U VolOverLoadFlag;//是否具备电压超限
	INT8U  PortNo;//端口号
	INT8U ConnectType;//属性，参见CONNECT_TYPE	电表通讯类型

	char  Addr[CONS_METER_ADDR_MAX_LEN+1];//485电子表通讯地址
	struct PortSet PortSet;//端口设置

	/*局编号说明：
	局编号第1~5字节为ASC码，第6~9字节为压缩BCD码（每2位数字0~9占用一字节），
	第10十字节自动的写入十六进制码的”00”，以作为压缩表号的标志
	合法表号： 	XXXXX 12345678
     “XXXXX”表示ASC码，即英文字母，数字等均可；“12345678”表示0~9数字	
	*/
	char  BureauNo[CONS_METER_ADDR_MAX_LEN+3];//局编号1是485总表,2是485分表,3是载波
	INT8U CompressFlag;//局编号的压缩标志

	char  User[CONS_METER_USER_MAX_LEN+1];//用户名
	char  Pwd[CONS_METER_PWD_MAX_LEN+1];//密码

	INT8U SetRegGrade;//设置能量寄存器（包括拉合闸）权限等级
	char  SetRegPwd[CONS_METER_PWD_MAX_LEN+1];//设置能量寄存器（包括拉合闸）密码
	INT8U BrakeFlag;//拉闸功能，0表示不带拉闸，1表示带拉闸
	INT32U CT;//电流互感系数，0表示未知
	INT32U PT;//电压互感系数，0表示未知	
	
	INT16S CommRetryTimes;//通讯重试次数，-1表示采用规约默认的值
	
	INT8U PrefixChar;//前导字符
	INT8U PrefixCount;//前导字符个数	

	INT32U Option;//选项，供扩展用


	//电表需要采集的数据量
	DataBitSet RegDBS;//电表能量寄存器

	//各种数据量的最近采集信息
	MeterGatherInfo GatherInfo[DATA_TYPE_METER_MAX_VALUE+1];//能量寄存器、电表事件、电表分时
   	INT32U CommOkCount;//通讯成功个数
	INT8U BrakeChangeFlag;//继电器变位标志,0x00无变位,0x01,拉闸,0x02合闸	
	INT16U RelayNo;//采集终端号
	INT8U SetTimeFailTimes;
	INT8U Phase;

public:
	static bool IsValidNo(INT32U MeterNo)
	{
		return MeterNo<CONS_METER_MAX_COUNT;
	}
	inline bool IsValid() const 
	{
		return (Enable>0 &&
			ConnectType!=METER_CONNECTTYPE_UNKNOWN &&
			Prop!=METER_PROPERTY_UNKNOWN &&
			Prop!=METER_PROPERTY_JXB &&
			RuleID!=METER_RULE_UNKNOWN);
	}
	inline bool IsForBus(INT8U cBusNo) const
	{
		return BusNo==cBusNo;
	}
	inline INT8U GetCommMaxTimes() const
	{
		return CommRetryTimes<0?3:(CommRetryTimes+1);
	}
	inline bool CanRealRead()
	{
		return true;
	}

 INT8U GetMeterRuleID() const ;
#if 0
	inline INT8U GetMeterRuleID() const 
	{		
		
	
		   if (RuleID==METER_RULE_GB645)
			{
				return METER_RULE_GB645;
			}
			else if(RuleID==METER_RULE_GD645)
			{		
				return METER_RULE_GD645;			
			}
			else
		      return RuleID;
	}
#endif

	bool IsNeedTouChuan()
	{
		if (BusNo==DBCJ_3_BUSRULE_GENERAL)
		{
			if (RuleID!=METER_RULE_GB645 && RuleID!=METER_RULE_GB2007 &&RuleID!=METER_RULE_GD645)
			{
				return true;
			}
		}
		return false;
	}

	void Init(INT16U meterNo);	
	void InitGatherInfo();
	void OnChanged();
	KLDump& BaseDump(KLDump& dump) const;
	KLDump& Dump(KLDump& dump) const;
	bool GetAddr_Bcd(unsigned char *addr,int which_0_to_4=0);
	int OnCommand(KLDump& dump,const char* cmdKey,const char* objName,INT8U id,const char *propName,const char* propVal);
	bool SetPassWord(char temp[CONS_METER_ADDR_MAX_LEN+1]);
	char* GetPassword(char temp[CONS_METER_ADDR_MAX_LEN+1]);
	bool lsValidBureauNo();
	void GetAddr_6(unsigned char *ipDest);

	//＝＝：判断两个电表参数的TN号和通讯地址参数是否相等
	bool operator==(Meter& meter);
	//Meter* operator=(Meter& meter);
//private:
//	bool IsGBMeter();
};

//struct MeterCount
//{
//public:
//	/*这两行变量请不要随意改动*/
//	int vipmc,usemc,zb;//现有重点户表数，用户表数，总表数
//	int jydgn;//现有简易多功能表数
//	void init();
//};
//

struct Meters
{
	MeterBitSet MBS;
	MeterBitSet ValidMBS;//有效的电表
	MeterBitSet UserMBS;//用户表
	MeterBitSet VIPMBS;//重点用户
	MeterBitSet JYDGNMBS;//简易多功能

	Meter Item[CONS_METER_MAX_COUNT];
	time_t LastFsDataTime[CONS_METER_MAX_COUNT];//最后一次分时数据时间

	time_t ChangeTime;//更改时间
	bool  IsCheckTime;//是否已经校时,如果已校时,为true

public:

	void Init();
	int HasZBMeter() const;
	void OnChanged();
	KLDump& BaseDump(KLDump& dump) const;
	KLDump& Dump(KLDump& dump) const;

	int OnCommand(KLDump& dump,const char* cmdKey,const char* objName,INT8U id,const char *propName,const char* propVal);
	bool GetTnChar(bstring temp,unsigned char *tn);
	bool GetTnInt(bstring temp,unsigned int *tn);
	INT16U GetUserMeterTN(char * VIPAddr);
	void altmeter(INT32U code,int how);
	void DelMeter(INT16U tn);
	//void countmeters(struct MeterCount &mc);
private:
	int Load(const char* cfgFile);
};

/***************************************     DEVICE    ****************************************/
#define NAME_SET_device "device.ini"	



typedef enum
{
	MD_RCU=0,
	MD_AIRCONDITION=1,
	MD_RELAY_8=2,
	MD_LIGHT_4=3,
	MD_RELAY_2=4,
	MD_LEDV12_3=5,
	MD_DOORDISPLAY=6,
	MD_RFID_CARD=7,
	MD_KEYBOARD_20=8,
	MD_RTF =9,
	MD_LUX_SENSOR=10,
	MD_TYPE_MAX,
}T_emDeviceType;



struct NoBitSet:public FastBitSet<CONS_MAP_DEVICE_SUBNO_MAX_COUNT>
{
};

struct DevicesBitSet:public FastBitSet<CONS_DEVICE_MAX_COUNT>
{
};


struct MapItem
{
	INT8U m_bValid;
	INT8U m_subno;
	INT8U m_funcType;//见特殊功能定义表//bit0~5:功能码,bit7:1,IORELAY一一对应输入输出映射端,0:无特殊要求
	INT8U m_mapTn;
	NoBitSet m_mapNbs;
};

struct DeviceFlags
{
	INT8U m_bDeviceParamRefresh;
};

struct Device
{
	INT8U m_Enable;//有效性，1表示有效，0无效
	INT8U m_Tn;//电表号，测量点号,MeterNo＝1表示多功能总表	
	INT8U m_Kind;//功能种类
	INT8U m_Type;//设备类型字
	char  m_Addr[CONS_METER_ADDR_MAX_LEN+1];//485电子表通讯地址
	struct PortSet m_PortSet;//端口设置
	INT8U m_MapItemCount;
	MapItem m_MapItems[CONS_MAP_DEVICE_SUBNO_MAX_COUNT];
	INT8U m_StatusData[8];
	INT8U m_ClRelayNo;//回路号
	INT8U m_CommBreakTimes;//通讯中断次数
	INT32U m_LuxValue;//光感度
	INT32U m_LuxRate;//光感倍率
	time_t m_ResumeOnlineRefreshTime;//整流器恢复上线恢复任务设置电压时间
	
public:
	static bool IsValidNo(INT8U MeasureNo)
	{
		return MeasureNo<CONS_DEVICE_MAX_COUNT;
	}
	inline bool IsValid() const 
	{
		if (m_Enable>0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Init(INT8U MeasureNo)
	{
		memset(this,0x00,sizeof(*this));
	}
//	void OnChanged();
	bool GetAddr_Bcd(unsigned char *addr,int which_0_to_4=0);
	void GetAddr_6(unsigned char *ipDest);
};

struct Devices
{
	DevicesBitSet m_DBS;
	DevicesBitSet m_RtfBS;
	DevicesBitSet m_LuxSensorBS;
	DevicesBitSet m_RelayBS;//继电器
	Device m_Item[CONS_DEVICE_MAX_COUNT];
public:
	void Init();
};



#endif

