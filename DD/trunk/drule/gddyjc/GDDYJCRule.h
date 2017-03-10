///**/
#ifndef INCLUDED_DYJC_RULE
#define INCLUDED_DYJC_RULE

#ifndef INCLUDED_TASK_DATA_FILE
#include "../../share/TaskDataFile.h"
#endif

#ifndef INCLUDED_REG_ITEM
#include "../../share/RegItem.h"
#endif

#ifndef INCLUDED_REG_FILE
#include "../../share/RegFile.h"
#endif

#ifndef INCLUDED_TASK
#include "../../share/Task.h"
#endif

#ifndef INCLUDED_STATUS
#include "../../share/Status.h"
#endif

#ifndef	INCLUDED_GDDYJC_DATA
#include "GDDYJCData.h"
#endif

#ifndef INCLUDED_FILE_DIR
#include "../../include/FileDir.h"
#endif

#ifndef INCLUDED_FILE_BASE
#include "../../base/FileBase.h"
#endif

#ifndef INCLUDED_STRING
#include "../../base/String.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../../share/SysBase.h"
#endif

#ifndef INCLUDED_DEVICE_RULE
#include "../../share/DeviceRule.h"
#endif

#ifndef INCLUDED_DEVICE_RULE_INFO
#include "../../share/DeviceRuleInfo.h"
#endif

#ifndef INCLUDED_CONNECTION
#include "../../share/Connection.h"
#endif

#define GDDYJC_RULE_NAME "��ѹ����"

#ifndef INCLUDED_CONTEXT
#include "../../share/Context.h"
#endif

#ifndef INCLUDED_GDDYJC_DATA
#include "GDDYJCData.h" 
#endif

#ifndef INCLUDED_GDDYJC_RULE_FUNCTION
#include "GDDYJCRuleFunction.h"
#endif

#ifndef INCLUDED_METER_CFG
#include "../../config/MeterCfg.h"
#endif

#ifndef INCLUDED_ALARM_FILE
#include "../../share/AlarmFile.h"
#endif

#ifndef INCLUDED_SYS_IPC
#include "../../share/SysIpc.h"
#endif

#ifndef INCLUDED_IPC_INFO
#include "../../base/IpcInfo.h"
#endif

#ifndef	INCLUDED_CONTEXT_CFG
#include "../../config/ContextCfg.h"
#endif

#define CONS_LOGON_HEARTBEAT_MSTA 255

#define CONST_HTTX_TO_JILIAN_PACKAGES_BUFFER_SIZE 20

#ifndef INCLUDED_GDDYJC_STRUCT_DATA
#include "GDDYJCRule_struct.h"
#endif

#ifndef INCLUDED_GDDYJC_SET_SETTINGS
#include "GDDYJCRule_set_settings.h"
#endif

#ifndef INCLUDED_GDDYJC_STATUS
#include "GddyjcStatus.h"
#endif



#define CONS_FIX_DATE_LABEL "FixDateLabel"

typedef enum _GDDYJC_RULE_MESSAGE_TYPE
{
	GDDYJC_RULE_MESSAGE_TYPE_REAL_GATHER = 1,//suxufeng:[2010-3-25]ʵʱ�ٲ�
	GDDYJC_RULE_MESSAGE_TYPE_CANCEL_REAL_GATHER = 2,//suxufeng:[2010-3-25]ȡ��ʵʱ�ٲ�
	GDDYJC_RULE_MESSAGE_TYPE_METER_OPEN_CLOSE = 3,//suxufeng:[2010-3-25]ʵʱ�ٲ�
	GDDYJC_RULE_MESSAGE_TYPE_DEBUG_COMMAND = 4,//suxufeng:[2010-3-25]ʵʱ�ٲ�
}GDDYJC_RULE_MESSAGE_TYPE;

class GDDYJCRule:public DeviceRule
{

private:
	//��֡��ŵķ�ʽ from=1��ʾ����վ�µ�֡��ȡMSE&SEQ,0��ʾ�ӱ���ȡMSE&SEQ
	unsigned char AddRepHead(bstring& rep,bstring& irRequestData,INT8U ctl,unsigned char& frame_count,int _1_from_request_0_from_config=1);
	bool AddRepHead(bstring& rep,bstring& irRequestData,INT8U ctl,int _1_from_request_0_from_config=1);
	bool AddRepFeed(bstring& rep,INT16U len=0);
	unsigned char AddRepFeedWithMCount(bstring& rep,bool need_count=false,INT8U m_count=0,INT16U len=0);
	unsigned char MakeRepLastFrame(bstrings & bs);
	bool IsMainDJS();
	bool IsJilianHttx();
public:
	GDDYJCRule();
	~GDDYJCRule(void);
	int GetRuleBaseInfo(DeviceRuleInfo& oRuleInfo);
	//void OnReceivedMessage(IpcInfo &iiMessage);
	//int StatPackages(bstring &irRecvDatas,bstrings &orRecvPackages);
	PARSE_RESULT ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int ParsePackageBase(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
    int ParseCompressOrEncryptPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int GetReplyPackage(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int AdjustLinkAddr(bstring& irRequestData,int addr=1);
	int GetReportPackage(Connection& irConnection,bstrings& orSendDatas,bstring& orConfirmData,int& irMaxSendTime,INT32U iiPackageMaxLen,bool bForceCheck);
	void OnPackageConfirmed(Connection& irConnection,DeviceRuleDataBuff& irSendInfo,bstring& irConfirmedData);//�ܵ�ȷ�ϰ���
	void Make_Package(bstring& reply,bstring& reqst,INT8U ctrlCode);
	int CheckRtuaAddr(const unsigned char *addr);

//Э�鴦����
public:
	//д�������
	int SetDJSsetings(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen,INT8U m_iCtrl=0x88);
	//suxufeng:[2010-3-24]���ò�������Ĺ��ò��֣���SetDJSsettings��SetRealTimeSetting���á�
	int SetSettings(Connection &irConnection, int &i, INT16U &DataLen, bstring &irRequestData, bstring::iterator &operatorNow, bstring::iterator &operatorEnd, bstring &orAns);
	//���Ż���
	int WakeUp(bstring& irRequestData);
	//����ǰ����
	int RealRead(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//�����������ճ��ۺ�����
	int ReadDailyData(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//�ط��ճ�����
	int reReplyDalyData(Connection& irConnection,bstring& irRequestData);
	 //�����������ص㻧��������
	int ReadVIPData(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//�ط��ص㻧
	int reReplyVIP(Connection& irConnection,bstring& irRequestData);
	//����բ����
	int MeterOpenClose(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);   
	//����û�м�ʱת����DBCJ���̵ķֺ�բ����
	int DillMeterOpenClose(Connection& irConnection);
	//�������쳣����,����ȷ��.
	int DJSAlarmConfirm(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//�������
	bool HeartBeating(Connection& m_pconnection);
	//���͸���������
	int SendToJilian(Connection& irConnection,bstring& irRequestData);
	int SendToJilian(bstring &irRequestData,bstrings &orRecvPackage);
	void BroadCastToJilian(bstring &irRequestData);
	bstring ChangeBroadCastPkg2DstPkg(bstring &irRequest, const unsigned char *addr);
	//void OnPackageSended(Connection &irConnection,SendInfo &irSendInfo,bstring &irSendedData);
	//void OnPackageReceived(Connection &irConnection,SendInfo &irSendInfo,bstring &irReceivedData);
	//ȷ���Ƿ�Ϊȷ�Ϸ���֡
	bool IsConfirmCtrl(Connection& irConnection,bstring req);
	inline unsigned char GetMSTA(unsigned char ibByte1,unsigned char ibByte2)
	{
		return (ibByte1 & 0x3f);
	}
	inline unsigned char GetFSEQ(unsigned char ibByte1,unsigned char ibByte2)
	{
		return ((ibByte2<<2) &0x7c)+((ibByte1>>6) & 0x03);
	}
	//�����ӵ�ʱ��,�����ϱ�һ����½֡.�����������ϱ���ʱ�������
	int logon(Connection& m_pconnection);
	//�����ϱ�������Ϣ   2006-6-20��־ǿ+
	int ReportAlarm(Connection& irConnection,bstrings& orSendDatas,bstring& orConfirmData,int& irMaxSendTime,INT32U iiPackageMaxLen,unsigned char &frame_count);
    //����ϱ���ʱ
	void OnTickRule(Connection& m_pconnection);
	//�����������ϱ�
	int JilianReports(Connection& m_pconnection);

	// ѹ������
	bool DecompressData(bstring& irCompressedData,bstring& irDecompressedData);
	// ��������ǰ�Ĵ�������
	bool OnSendData(bstrings& orReplyDatas);

	//���������ϱ����ۺ����ݺ��ص㻧����
	int AutoReport_MonthData(Connection& irConnection);
	int AutoReport_DayData(Connection& irConnection);
	int AutoReport_VipData(Connection& irConnection);
	bool Check_Time(INT8U ts);

	//����������
public:
	int Master_DJS_request(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//�����������
	int ReplyJilianControl(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//��������ն�����
	int ReplyFromTerminal(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//only for test
	//�������յ������
	int Read_Meter_set(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);

	void OnSystemUpdated();

//#ifdef WIN32
//	int test();
//#endif
private:
	int ChangeMeterAndTerminal(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int ChangeMeter(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int ChangeTerminal(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
//�����������ļ����������֡������Ӧ��
	int GetUpdateReply(Connection &irConnection,bstring &irRequestData,bstrings &orReplyDatas,INT32U iiPackageMaxLen);
	int FileTransffer(Connection &irConnection,bstring &irRequestData,bstrings &orReplyDatas,INT32U iiPackageMaxLen);

private:
	//ʵʱ�в�
	int ReadrealtimeData(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int CancelReadRealTimeData(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	int DillReadRealTimeData(Connection& irConnection);
	//ʵʱд�������
	int SetRealTimeSetting(Connection& irConnection,bstring& irRequestData,bstrings& orReplyDatas,INT32U iiPackageMaxLen);
	//ȡ��ǰʱ���BCD���ʾ����
	void Get_CurBCDTime5(char* o_cpBcdTime);
	// �������յ�һ���������ģ����ҵ�ַƥ��ɹ�֮��Ĵ���ʽ��
	inline void SetMasterHttxOnlineStatus(const bool status = true)
	{
		CONTEXT.MasterHttxOnline = status;
	}
	bool bcddatedifff(char when[7],char now[7],long howlong);
	INT8U GetReplyCtrlCode(INT8U ctrl,int _1_right_0_wrong=1);
	void Abnormity_Frame(Connection& irConnection,bstring& irRequestData,INT8U Ctr_Code,INT8U Abnormity_Code,int _1_from_request_0_from_config=1);
	void Abnormity_Frame(Connection& irConnection,bstring& irRequestData,INT8U Ctr_Code,const bstring& Abnormity_Code,int _1_from_request_0_from_config=1);
	void Abnormity_Fram(Connection& irConnection,bstring& irRequestData,INT8U Ctr_Code,INT16U Abnormity_Code,int _1_from_request_0_from_config=1);
	bool IsNeedGather();//�ж��Ƿ���Ҫ���вɱ�
	int GetFtsAnswer(bstring &StrDown,bstring &StrFts,bstring &StrUp,bool result);
	int GetFtsData(bstring &strIn,bstring &strOut);//��ȡ�ļ������Լ���ݣ�������㶫ʡ��Լ�İ�ͷ��β��Ȼ��ֱ�Ӷ����ļ������Լȥ���
	int OnTerminalReset();
	unsigned char GetLastDayOfMonth(const char *iBcdtime);

	INT8U OnFixDataCommand(const bstring &irRequestData);
protected:
	unsigned char oldVersion[8];
	unsigned char newVersion[8];
private:
	struct sRTUA_Struct m_RTUA;
	int iDataLen1,iDataLen2;
	bstring m_bHeartBeat;
	byte bMSTA;
	byte bFSEQ;
	byte bISEQ;
	INT8U m_iCtrl;
	INT16U Msta;
	byte seq;
	int curMonth;//����ĩ����ʱ�ĵ�ǰλ��
	int curDay;//����ĩ����ʱ�ĵ�ǰλ��
	INT8U curFrame;//��ǰ֡�ǵڼ�֡
	int m_Tn;
private:
	unsigned char m_ctrlfiled,m_cLastCtrlField,m_cTypeID,m_cVSQ,m_cCOT;
	//m_Meter m_meter[4];
	unsigned char My_Ip[4];
	GDDYJCData m_WriteSettings;
	bool NeedGatherSlaveTerminal;

	
	//REPORTDATA Alarm_repData;//�����ϱ�֡ר�÷������ϱ�
	REPORTDATA VIPData;//�ص㻧�ϱ�
	REPORTDATA DalyData;//�ճ��ۺ�����
	INT8U m_Frame_FSEQ;//�����ϱ�ʱ��֡���
	bstring m_sParsedBuffer;

	bstrings bssMeterOpenClose;//�ֺ�բ�����洢��
	bool m_bZipEnable;

	bool m_bFixDateLabel;//suxufeng:[2010-6-25]�Ƿ��������ʱ��
	long m_lOutLineStartTick; //����ʱ���ۼ�
public:
	HEARTBEATING m_HeartBeat;
};

#endif
