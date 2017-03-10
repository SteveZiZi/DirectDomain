#ifndef _CLRelayRelay_H
#define _CLRelayRelay_H


/**************************************************/
/*                    CLRelay ��·							    */	
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
	INT8U m_ChannelNo;//©����ͨ�����1~8
	INT16U m_DelayMs;
	INT16S m_DtVol;
	INT8U m_CutEnable;
public:
	void Init(void){memset(this,0x00,sizeof(*this));}
};

struct S_CLRelay
{
	INT16U m_CLRelayNo;//�ɼ��ն˺�
	INT8U m_Enable;//��Ч�ԣ�1��ʾ��Ч��0��Ч
	char  m_Addr[CONS_METER_ADDR_MAX_LEN+1];//ͨѶ��ַ
	DevicesBitSet m_Dbs;
	INT8U m_ZBXW;//�ز���λ��0��ʾ��ȷ����1��ʾA�࣬2��ʾB�࣬3��ʾC��,4��ʾר��
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
	//�������ж������ն˲�����Enable��Type��Addr��ZBXW��ԭ�����Ƿ����
	bool operator==(S_CLRelay& CLRelay);
	bool operator=(S_CLRelay& CLRelay);
	bool IsLeak(void);
};


struct S_CLRelays
{
	S_CLRelay m_Item[CONS_CLRELAY_MAX_COUNT+1];
	CLRelayBitSet CLRelayBS;//��·����
	CLRelayBitSet CLRelayLeakDtBS;//©�����·����
	S_CLRelay GetCLRelayInfo(const INT16U CLRelayNo);//��ȡ�ɼ��ն���Ϣ��ʹ���ն�TN��
	S_CLRelay GetCLRelayInfo(const char *CLRelayAddr);//ʹ���ն˵�ַ��ȡ�ն���Ϣ
public:
	void Init();
	void OnChanged();
	/*
	Return a relay no which is not used.If return 0,all relay no is used.
	*/
	INT16U GetCLRelayNo(int start=1);
	/*
	�������	�ɼ��ն˺�
	���ز���	û������
	*/
	int DelCLRelay(int whichRelay,bool force=true);
	//int DelRelay(INT8U whichRelay);
	/*�����Ƿ��Ѿ��������ն�
		����õ�ַ�Ĳɼ��ն��Ѿ�����,�򷵻� RelayNo��
		���������,�򷵻� CONS_RELAY_MAX_COUNT+1
	*/
	int lsHavedCLRelay(const char *temp);
	int HadCLRelay(S_CLRelay& CLRelay);
	/*
	���һ���ɼ��ն�
	����:TN  ����ն˰󶨵ĵ��TN��
		 m_Addr �������ն˵�ַ
		 m_Type �����ն�����
		 m_ZBXW �ز���λ
		 m_RouterNo �м����
	 ����:
		��ӵ�λ�þ���RelayNo
		������� CONS_RELAY_MAX_COUNT+1 ��ʾ���ʧ��
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

