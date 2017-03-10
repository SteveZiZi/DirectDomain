#ifndef INCLUDED_ImpAlarm_FILE
#define INCLUDED_ImpAlarm_FILE

#ifndef INCLUDED_CONTEXT
#include "Context.h"
#endif

#ifdef WIN32
#define PATH_ImpAlarm_BASE	"./mnt"
#else
#define PATH_ImpAlarm_BASE	"/mnt"
#endif

#define CONST_ImpAlarm_FILE_NAME PATH_ImpAlarm_BASE"/data/alarm/impalarm.dat"
#define CONST_ImpAlarm_FILE_LENGTH 120			//�ļ���¼��������10�ı�����ʼ��
#define CONST_ImpAlarm_RECORD_ITEM_LENGTH 32		//�����¼���༴��
#define CONST_ImpAlarm_RECORD_OFFSET 0xffff		//Ĭ�ϵ�io_Offset��ֵ

/* Monitor �澯����*/
#define IMPALARM_CPR_ONLINE_STATE	0x020F
#define IMPALARM_CL_LEAK_STATE	0x0101//©�糬��״̬
//2006-8-14����������
//2007-8-30������������˴��������ɹ�����д������������WorkServer˽�С�
//2007-9-4�������Զ���¼�����������·�װ��֧�ֶ�ȡ����һ��δ�ϱ����ݣ����������ϱ���Ҳ֧�ֶ�ȡ���з������������ݣ�������վ�����澯����
struct ImpAlarmRecordInfo
{
	INT8U ImpAlarmData[1];
public:
	int Init(FILE* fp,INT16U iLen,INT8U iItemLen)
	{
		//INT32U Len=iLen*iItemLen;
		ImpAlarmRecordReset(fp,iLen,iItemLen);
		return 0;
	}
	int ImpAlarmRecordReset(FILE* fp,INT16U iLen,INT8U iItemLen);
};

class ImpAlarmFile
{
public:
	static int GetAlarmRecord(bstring &iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//ȡ��һ����Ҫ�����ϱ��ļ�¼�����������ϱ�

	static int GetAlarmRecord(INT8U *iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//ȡ��һ����Ҫ�����ϱ��ļ�¼�����������ϱ�

	static int GetAlarmRecords(bstrings &iRecords,INT8U irCount = 0,time_t tBeginTime = 0,time_t tEndTime = 0,INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//ȡ�������ļ������з���Ҫ������ݼ�¼��������վ���ո澯����

	static int GetAlarmRecords(bstrings &iRecords,INT8U irCount = 0,char *tBeginTimeBCD7 = NULL,char *tEndTimeBCD7 = NULL,INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//ȡ�������ļ������з���Ҫ������ݼ�¼��������վ���ո澯����

	static int SetAlarmRecordReaded(INT16U io_Offset,INT8U iItemLen  = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH,char *FileName = CONST_ImpAlarm_FILE_NAME);//��һ���Ѿ��ϱ��ļ�¼�ı�־λ��Ϊ1

	static int SetAlarmRecordReaded(FILE* fp,INT16U io_Offset,INT8U iItemLen  = CONST_ImpAlarm_RECORD_ITEM_LENGTH,
		INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH);//��һ���Ѿ��ϱ��ļ�¼�ı�־λ��Ϊ1


	static int AddAlarmRecord(const INT8U* iRecordContent,INT8U iTrueLen,INT16U iImpAlarmNo,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
		INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH,
		char *FileName = CONST_ImpAlarm_FILE_NAME);//��ӱ�����¼�ļ���һ��ֻ�����һ����¼

	static int AddAlarmRecord(const INT8U* iRecordContent,INT8U iTrueLen,INT16U iImpAlarmNo,const char *bcdtime,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
		INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH,
		char *FileName = CONST_ImpAlarm_FILE_NAME);//��ӱ�����¼�ļ���һ��ֻ�����һ����¼

	//static int ConvertMeterNoToBCD(INT16U iMeterNo,INT8U *iBcdMeterNo);//������Ŵ�HEX��ת����BCD��

	static int GetTrueLen(INT8U *iImpAlarmNo,INT8U &i_TrueLen);//�ڶ�ȡ�ļ�ʱ��ȡ������¼��ʵ�ʳ���

	//static int AddImpAlarmRecord(bstring &iRecordContent,INT8U iTrueLen,INT16U iImpAlarmNo,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
	//	INT8U iItemLen = CONST_ImpAlarm_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ImpAlarm_FILE_LENGTH,
	//	char *FileName = CONST_ImpAlarm_FILE_NAME);//��ӱ�����¼�ļ�
};

#endif
