#ifndef HTTX_JILIAN
#define HTTX_JILIAN

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef _cfg_h_
#include "../config/confuse/confuse.h"
#endif

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

#ifndef INCLUDED_REG_ITEM
#include "RegItem.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

//#ifndef SEMAPHORE_H
//#include "SemObject.h"
//#endif

typedef vector <unsigned int> int32KLIDs;
struct Meter;

struct SEND_BUFFER
{
public:
	void SetFinishFlag(int task_type, int id);
	void CleanFlag(int task_type);
	bool IsFinished(int task_type);
private:
	char task_finished[9];
//�����Ӳɼ��ն�ʱ��֪ͨ·�ɽ���
public:
	int newRelays;

//============�������ڼ���============================================
	public:
	SEND_BUFFER();
	~SEND_BUFFER();
	bool lock;

		bool IsMasterChannelOnline;//��ͨ���Ƿ����ߣ����ڼ����ж��Ƿ�Ӧ�������ϱ�
		//�����2008-3-14�޸�
		bool IsMasterChannelIdle;//��ͨ���Ƿ���У����������жϴ�ֵΪ"true"ʱ���Ż�ԴӼ�����������ѯ

	public:
		//int DbcjToWork;//���ɼ����̱������յ�������֮��Ӧ����Ϣ֪ͨ����������̽����������
	//=========�������ڼ���=================================================

	//===========����ר��������բ===========================================
	//bstring sendto_dbcj;  //�͵��ͨѶ����ͨѶ�ĵط�
private:
    unsigned char realReadCommand[SHARE_MEM_LENGTH];	
	int oc_length;
public:
	int HttxWrite_toDbcj_openclose(bstring req);
	int Read_openclose(bstring& rep,int &TN,int instanceID=1000);
	unsigned short openClosebackdata;//��������
	int aim;//1 to dbcj/ 2 to httx	3Ϊ���� 4ΪDBCJ���Ƴ�ʱ ÿ�ζ�д��Ҫ��Ϊ����״̬��
	int timeout;//������Ƴ�ʱʱ��.
	time_t whentodo;//�����ҷ���DBCJ��������ʱ��ʱ�䡣
	//======================================================================

	//===========��������ʵʱ�в�===========================================
private:
	  unsigned char sendto_dbcj[SHARE_MEM_LENGTH];
	  INT32U real_length;
public:
	  int HttxWrite_toDbcj_realRead(bstring req);
		/*���ɼ����̵��øú�����ȡ��վ�·���ʵʱ�в�����*/
	  int Read_realRead(bstring& rep,int &TN);

	  int Read_RealReadcomand(bstring & rep);
	  void HttxReadRealTimeDataTimeOut();
	  void HttxReadRealTimeDataOk();	   	  
	  DataBitSet idbs;//�·�����ʵʱ�ٲ�����ݱ�ʶ��Ӧ������
	  unsigned char ReadBackchar[10];
	  
	  RegItem ReadBackRegItems[CONS_REG_ID_MAX_COUNT];
	  int Reg_num;
	  
	  int HttxRead_RealRead(RegItems &regitems);
	  time_t realTimeNow;//���ݸ������ʱ��;
	  //real_Aim��ÿ�ε������ڴ��д����Ҫ�ޣ��������Ŵ���
	  //1 to Dbcj;2 to httx��3:����״̬,����վȡ��ʵʱ�в����
	  //4:HTTX���ܶ�д��DBCJֻд�����������Ѿ���ĳ��DBCJ����ȡ�ߣ�
	  //5��DBCj�����жϳ�ʱ��
	  //6��Dbcj����ʵʱ�ٲ�ʧ��
	  int real_Aim;
	  int instanceID;
	  MilliSecondTimeout recvTs;
	//======================================================================
public:	
	void Init();	
	
//======================================================================
/*���ڹ㲥Уʱ*/
public:
    INT8U CheckTimeFlag;//1 to dbcj 3 no command
//======================================================================
public:
	/*0��ʾδУ��3��ʾ��У*/
	INT8U SetTimeFlag;
	//���ڴ��ʵʱ����״̬,��ʽΪ(��ű�ʾ�ֽ���RouterState��λ��)
	/*
	0�������Ƿ���ɡ�0��ɣ�1δ���
	1��������������λ��1��ʾA�࣬2��ʾB�࣬3��ʾC�࣬4��ʾר��
	2���ն˸���
	3-5���м�1��ַ
	6-8���м�2��ַ
	9-11���м�3��ַ
	12-14���м�4��ַ
	15-17���м�5��ַ
	*/
	INT8U RouterState[32];
	//��������澯
	bool LostAlarmFlag;
//=====================================================================================


/*���ڱ��������ϱ�2006-11-6*/
//0û�б������������N��ʾ�ж��ٸ澯
unsigned char Alarm_report_flag;

//����PPP���̸��ߺ�̨ͨ�Ž��̿��Խ���������
bool PPPHasOnline;
unsigned char Lock_GPRSConnectFailedTimes;
unsigned char Lock_GPRSConnectFailedMaxTimes;
unsigned char GPRSConnectFailedTimes;
unsigned char GPRSConnectFailedMaxTimes;
unsigned char Get_GPRSConnectFailedTimes();
unsigned char Get_GPRSConnectFailedMaxTimes();
unsigned char Set_GPRSConnectFailedTimes();
unsigned char Set_GPRSConnectFailedMaxTimes();
unsigned char Reset_GPRSConnectFailedTimes();
unsigned char Reset_GPRSConnectFailedMaxTimes();
};
/***************************************************************************************/
#define MAX_DEBUG_TASK 10
#define DEFAULT_DELAY_TIME 30
//struct DEBUG_COMMAND
//{
//	INT16U 
//};
struct DEBUG_BUFFER
{
public:
	DEBUG_BUFFER();
	void Init();
	/*bool OnDebug(INT8U selc) const;
	bool OffDebug(INT8U selc) const;*/
	int SetCommand(INT16U DataID, const bstring& data,INT16U _instanceID,const INT8U hData[6]=NULL);
	int GetCommand(INT16U &DataID, bstring& data,INT8U selc);
	int SetRplData(INT16U DataID, const bstring& data,INT8U ansType);
	int GetRplData(INT16U &DataID, bstring& data,INT8U &ansType,INT16U _instanceID);
	bool IsDebugState();
	bool IsTimeOut();
	bool IsTaskCanceled(INT16U DataID);
	INT8U GetState(INT16U taskNo);
	bool GetHead(INT8U hData[6]);
	bool IsMe(int _instanceID)
	{
		return _instanceID==instanceID;
	}
	INT8U GetTaskStatus(INT16U DataID);
	void SetTerminalResetFlag(const bstring &data);
	void GetTerminalResetFlag(bstring &data);
	void SystemCommand(const bstring &data);
	void ExecCommand(const bstring &data);
	void ClearAll();
private:
	bool  InDebugFlag;//�Ƿ���Ҫ�������̬��
	INT8U head[6];
	INT8U HasTask();
	INT8U HasRunningTask();
	INT8U HasReply();
	INT8U FindTaskNo(INT16U DataID);
	bool Set4152Data(const bstring& data);
	INT8U TerminalResetFlag[7];
public:
	//��Ӧ������ŵ�cmd��ֵ���壺0������1������ûִ�У�2����ȡ����3����������Ӧ��4�����쳣��Ӧ��5��������ִ�У�����Ϊ��Чֵ��
	//���ڽ����˳�����̬�����������3��ʾ�յ���λ���Ľ������̬�����4��ʾ�յ���λ�����˳�����̬�����1����ʾ�Ѿ��������̬��0��ʾ�Ѿ����ڵ���̬��
	INT8U cmd[16];
	INT16U delayTime;//�����ʱʱ�����û�е�������Զ���������̬
	time_t dbgTime;//���Կ�ʼʱ��
	INT16U instanceID;
	union
	{
		INT8U dbg_InOutDebug[2];
		INT8U dbg_StaticRouter[20];
		INT8U dbg_LinkState[25];
		INT8U dbg_SendPackage[25];
		INT8U dbg_StartRouter[3];
		INT8U dbg_StopRouter[1];
		INT8U dbg_AdjustRouter[771];
		INT8U dbg_SetZBSniffer[149];
		INT8U dbg_BatchRealTimeRead[511];
		INT8U dbg_CallTest[25];
		INT8U dbg_GetNetStatus[32];
		INT8U dbg_GetNetFailedNodes[25];
		
	}FromHttx;
	union
	{
		INT8U rpl_StaticRoutre[1];
		INT8U rpl_LinkState[1];
		INT8U rpl_SendPackage[1];
		INT8U rpl_StartRouter[766];
		INT8U rpl_StopRouter[1];
		INT8U rpl_AdjustRouter[766];
		INT8U rpl_SetZBSniffer[1];
		INT8U rpl_BatchRealTimeRead[151];
		INT8U rpl_CallTest[1];
		INT8U rpl_GetNetStatus[9];
		INT8U rpl_GetNetFailedNodes[542];
		
	}ToHttx;


};

#endif
