#ifndef _CLRelayRelay_H
#define _CLRelayRelay_H


/**************************************************/
/*                    CLRelay 回路							    */	
/*												    */		
/**************************************************/

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

#include "meter.h"

class KLDump;

struct CLRelayBitSet:public FastBitSet<CONS_CLRELAY_MAX_COUNT>
{
};

struct SClLeakInfo
{
	INT8U m_Enable;
	INT8U m_ChannelNo;//漏电检测通道序号1~8
	INT16U m_DelayMs;
	INT16S m_DtVol;
	INT8U m_CutEnable;
public:
	void Init(void){memset(this,0x00,sizeof(*this));}
};

struct S_CLRelay
{
	INT16U m_CLRelayNo;//采集终端号
	INT8U m_Enable;//有效性，1表示有效，0无效
	char  m_Addr[CONS_METER_ADDR_MAX_LEN+1];//通讯地址
	DevicesBitSet m_Dbs;
	INT8U m_ZBXW;//载波相位，0表示不确定，1表示A相，2表示B相，3表示C相,4表示专线
	SClLeakInfo m_ClLeakInfo;
public:
	void GetAddr_6_Fix0(INT8U *ipDest);
	void GetAddr_6(INT8U *ipDest);
	void SaveAddr(unsigned char *BCD,INT8U len,bool reverse=false);

	static bool IsValidNo(INT16U RelayNo)
	{
		return (RelayNo>0);
	}

	inline bool IsValid() const 
	{
		return ((m_Enable>0)&&(m_CLRelayNo>0)&&(m_CLRelayNo<CONS_CLRELAY_MAX_COUNT));
	}
	void Init(INT16U relayNo);	
	//＝＝：判断两个终端参数的Enable、Type、Addr、ZBXW与原参数是否相等
	bool operator==(S_CLRelay& CLRelay);
	bool operator=(S_CLRelay& CLRelay);
	bool IsLeak(void);
};


struct S_CLRelays
{
	S_CLRelay m_Item[CONS_CLRELAY_MAX_COUNT+1];
	CLRelayBitSet CLRelayBS;//回路集合
	CLRelayBitSet CLRelayLeakDtBS;//漏电检测回路集合
	S_CLRelay GetCLRelayInfo(const INT16U CLRelayNo);//获取采集终端信息，使用终端TN号
	S_CLRelay GetCLRelayInfo(const char *CLRelayAddr);//使用终端地址获取终端信息
public:
	void Init();
	void OnChanged();
	/*
	Return a relay no which is not used.If return 0,all relay no is used.
	*/
	INT16U GetCLRelayNo(int start=1);
	/*
	传入参数	采集终端号
	返回参数	没有意义
	*/
	int DelCLRelay(int whichRelay,bool force=true);
	//int DelRelay(INT8U whichRelay);
	/*查找是否已经包含该终端
		如果该地址的采集终端已经存在,则返回 RelayNo号
		如果不存在,则返回 CONS_RELAY_MAX_COUNT+1
	*/
	int lsHavedCLRelay(const char *temp);
	int HadCLRelay(S_CLRelay& CLRelay);
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
	int AddCLRelay(INT16U TN,const char  *m_Addr,INT8U m_ZBXW);
	int AddCLRelay();
	int GetCLRelayNum();
	INT16U FindCLRelayNo(const INT8U CLRelayAddr6[6]);
	S_CLRelay &FindCLRelay(const INT8U CLRelayAddr6[6]);
	bool IsExist(INT16U relayno);
	void LeakDetect(void);
private:
	int Load(const char* cfgFile);
};

#endif

