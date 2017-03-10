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
	INT16U RelayNo;//�ɼ��ն˺�
	INT8U Enable;//��Ч�ԣ�1��ʾ��Ч��0��Ч
	INT16U Priority;//���ȼ�
	INT16U CountRelays;//����ͳ�Ƹ��ն���Ϊ�м̵����������ж����ȼ�
	char  Addr[CONS_METER_ADDR_MAX_LEN+1];//ͨѶ��ַ
	INT8U  Type;//�ɼ��ն��ͺ�
//	INT8U relayLevel;//100301 cgh ���Ӳɼ������м̼���
	MeterBitSet MBS;//���ӻ������е��
	INT8U ZBXW;//�ز���λ��0��ʾ��ȷ����1��ʾA�࣬2��ʾB�࣬3��ʾC��,4��ʾר��
	INT16U RouterNo;

	//RelayBitSet SucceedRBS;//��˲ɼ��ն�ͨѶ�ɹ��Ĳɼ��ն˵ļ���
	RelayBitSet FailRBS;//��˲ɼ��ն�ͨѶ���ɹ��Ĳɼ��ն˵ļ���

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
	//�������ж������ն˲�����Enable��Type��Addr��ZBXW��ԭ�����Ƿ����
	bool operator==(Relay& relay);
	bool operator=(Relay& relay);
};


struct Relays
{
	Relay Item[CONS_RELAY_MAX_COUNT];
	RelayBitSet RBS;//�ɼ��ն˵ļ���
	RelayBitSet ValidRBS;//��Ч�Ĳɼ��ն�
	Relay GetRelayInfo(const INT16U RelayNo);//��ȡ�ɼ��ն���Ϣ��ʹ���ն�TN��
	Relay GetRelayInfo(const char *RelayAddr);//ʹ���ն˵�ַ��ȡ�ն���Ϣ
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
	�������	�ɼ��ն˺�
	���ز���	û������
	*/
	int DelRelay(int whichRelay,bool force=true);
	//int DelRelay(INT8U whichRelay);
	/*�����Ƿ��Ѿ��������ն�
		����õ�ַ�Ĳɼ��ն��Ѿ�����,�򷵻� RelayNo��
		���������,�򷵻� CONS_RELAY_MAX_COUNT+1
	*/
	int lsHavedRelay(const char *temp);
	int HadRelay(Relay& relay);
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

