#ifndef INCLUDED_RELAY
#define INCLUDED_RELAY

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
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

class KLDump;

struct Relay
{
	INT16U RelayNo;//采集终端号
	INT8U Enable;//有效性，1表示有效，0无效
	INT16U Priority;//优先级
	INT16U CountRelays;//用于统计该终端作为中继的数量，以判断优先级
	char  Addr[CONS_METER_ADDR_MAX_LEN+1];//通讯地址
	INT8U  Type;//采集终端型号
//	INT8U relayLevel;//100301 cgh 增加采集器的中继级数
	MeterBitSet MBS;//该子机下所有电表
	INT8U ZBXW;//载波相位，0表示不确定，1表示A相，2表示B相，3表示C相,4表示专线
	INT16U RouterNo;

	//RelayBitSet SucceedRBS;//与此采集终端通讯成功的采集终端的集合
	RelayBitSet FailRBS;//与此采集终端通讯不成功的采集终端的集合

public:
	inline void GetAddr_3_Fix0(unsigned char *ipDest)
	{
		return ::GetAddr_3_Fix0(Addr, ipDest);
	}
	void GetAddr_6_Fix0(INT8U *ipDest);
	void GetAddr_6(INT8U *ipDest);
	void SaveAddr(unsigned char *BCD,INT8U len,bool reverse=false);
	bool IsNeedTouChuan();
	static bool IsValidNo(INT16U RelayNo)
	{
		return (RelayNo>0);
	}
	//inline bool IsValid() const 
	//{
	//	return ((Enable>0)&&(RelayNo>0)&&(MBS.count()>0));
	//}
	inline bool IsValid() const 
	{
		return ((Enable>0)&&(RelayNo>0)&&(RelayNo<CONS_RELAY_MAX_COUNT));
	}
	void Init(INT16U relayNo);	
	//＝＝：判断两个终端参数的Enable、Type、Addr、ZBXW与原参数是否相等
	bool operator==(Relay& relay);
	bool operator=(Relay& relay);
};


struct Relays
{
	Relay Item[CONS_RELAY_MAX_COUNT];
	RelayBitSet RBS;//采集终端的集合
	RelayBitSet ValidRBS;//有效的采集终端
	Relay GetRelayInfo(const INT16U RelayNo);//获取采集终端信息，使用终端TN号
	Relay GetRelayInfo(const char *RelayAddr);//使用终端地址获取终端信息
public:
	void Init();
	void OnChanged();
	int GetAnyRelayType();
	int GetRelayBusNo();
	/*
	Return a relay no which is not used.If return 0,all relay no is used.
	*/
	INT16U GetRelayNo(int start=1);
	/*
	传入参数	采集终端号
	返回参数	没有意义
	*/
	int DelRelay(int whichRelay,bool force=true);
	//int DelRelay(INT8U whichRelay);
	/*查找是否已经包含该终端
		如果该地址的采集终端已经存在,则返回 RelayNo号
		如果不存在,则返回 CONS_RELAY_MAX_COUNT+1
	*/
	int lsHavedRelay(const char *temp);
	int HadRelay(Relay& relay);
	/*
	添加一个采集终端
	参数:TN  与该终端绑定的电表TN号
		 m_Addr 传入新终端地址
		 m_Type 传入终端类型
		 m_ZBXW 载波相位
		 m_RouterNo 中继序号
	 返回:
		添加的位置就是RelayNo
		如果返回 CONS_RELAY_MAX_COUNT+1 表示添加失败
	*/
	int AddRelay(INT16U TN, const char  *m_Addr,INT8U m_Type=0x01,INT8U m_ZBXW=0x00,INT16U m_RouterNo=0x0000);
	int AddRelay();
	int GetRelayNum();
	INT16U FindRelayNo(INT8U addr[3]);
	INT16U FindRelayNo(const INT8U RelayAddr6[6]);
	Relay &FindRelay(const INT8U RelayAddr6[6]);
private:
	int Load(const char* cfgFile);
};


#endif

