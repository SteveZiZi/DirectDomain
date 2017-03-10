#ifndef INCLUDED_ALARM_FILE
#define INCLUDED_ALARM_FILE

#ifndef INCLUDED_CONTEXT
#include "Context.h"
#endif

#ifdef WIN32
#define PATH_ALARM_BASE	"./mnt"
#else
#define PATH_ALARM_BASE	"/mnt"
#endif

#define CONST_ALARM_FILE_NAME PATH_ALARM_BASE"/data/alarm/alarm.dat"
#define CONST_ALARM_FILE_LENGTH 120			//�ļ���¼��������10�ı�����ʼ��
#define CONST_ALARM_RECORD_ITEM_LENGTH 32		//�����¼���༴��
#define CONST_ALARM_RECORD_OFFSET 0xffff		//Ĭ�ϵ�io_Offset��ֵ

#define CONST_VOLTAGE_LOST_ALARM_NO 0x0171		//ʧѹ�¼�����
#define CONST_CURRENT_LOST_ALARM_NO 0x0172		//ʧ���¼��澯
#define CONST_CURRENT_RESERVE_ALARM_NO 0x0173	//���������¼��澯
#define CONST_PROGRAM_ALARM_NO 0x0174			//����¼��澯
#define CONST_LINE_WASTE_ALARM_NO 0x0175		//����ֵ/��ֵ�澯
#define CONST_RELAY_CHANGE_ALARM_NO 0x0176		//�̵�����λ�¼��澯
#define CONST_GATHER_FAIL_ALARM_NO 0x0177		//����ʧ�ܸ澯
#define CONST_SETTIME_FAIL_ALARM_NO 0x0178		//Уʱʧ�ܸ澯
#define CONST_TIMEALARM_ALARM_NO	0x0179		//ʱ���쳣�澯
#define CONST_UPDATE_EVENT_ALARM_NO 0x017A		//�����¼��澯
//#define CONST_CHANGE_METER_ALARM_NO 0x017B		//�����¼��澯
#define CONST_RELAY_FAILROUTER_ALARM_NO 0x017C	//�ɼ��ն���Ѱ·��ʧ�ܸ澯
#define CONST_POWEROFF_EVENT_ALARM_NO 0x017D	//ͣ���¼��澯
#define CONST_BRAKE_FAIL_ALARM_NO	0x017E		//�������բʧ��

/* Monitor �澯����*/
#define ALARM_RTF_CONFIGURE_FILE_CLEAN		0x0200
#define ALARM_RTF_CONFIGURE_FILE_DOWNLOAD	0x0201
#define ALARM_RTF_VOL_SET	0x0202
#define ALARM_CL_VOL_SET	0x0203
#define ALARM_RTF_ONOFF	0x0208
#define ALARM_CL_CTRL		0x020A

#define ALARM_RELAY_CTRL	0x020D
#define ALARM_COMM_FAIL	0x020F
#define ALARM_TASK_CL_VOL_SET		0X0215
#define ALARM_TASK_CL_ONOFF_SET		0X0216
#define ALARM_CPR_ONLINE_STATE	0x020F

/* �������*/
#define LUXTASK_CL_VOL_SET		0X0301
#define LUXTASK_END		0X0302
#define LUXTASK_CL_ONOFF_SET		0X0303

//2006-8-14����������
//2007-8-30������������˴��������ɹ�����д������������WorkServer˽�С�
//2007-9-4�������Զ���¼�����������·�װ��֧�ֶ�ȡ����һ��δ�ϱ����ݣ����������ϱ���Ҳ֧�ֶ�ȡ���з������������ݣ�������վ�����澯����
struct AlarmRecordInfo
{
	INT8U AlarmData[1];
public:
	int Init(FILE* fp,INT16U iLen,INT8U iItemLen)
	{
		//INT32U Len=iLen*iItemLen;
		AlarmRecordReset(fp,iLen,iItemLen);
		return 0;
	}
	int AlarmRecordReset(FILE* fp,INT16U iLen,INT8U iItemLen);
};

class AlarmFile
{
public:
	static int GetAlarmRecord(bstring &iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//ȡ��һ����Ҫ�����ϱ��ļ�¼�����������ϱ�

	static int GetAlarmRecord(INT8U *iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//ȡ��һ����Ҫ�����ϱ��ļ�¼�����������ϱ�

	static int GetAlarmRecords(bstrings &iRecords,INT8U irCount = 0,time_t tBeginTime = 0,time_t tEndTime = 0,INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//ȡ�������ļ������з���Ҫ������ݼ�¼��������վ���ո澯����

	static int GetAlarmRecords(bstrings &iRecords,INT8U irCount = 0,char *tBeginTimeBCD7 = NULL,char *tEndTimeBCD7 = NULL,INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U iFileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//ȡ�������ļ������з���Ҫ������ݼ�¼��������վ���ո澯����

	static int SetAlarmRecordReaded(INT16U io_Offset,INT8U iItemLen  = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U FileLen = CONST_ALARM_FILE_LENGTH,char *FileName = CONST_ALARM_FILE_NAME);//��һ���Ѿ��ϱ��ļ�¼�ı�־λ��Ϊ1

	static int SetAlarmRecordReaded(FILE* fp,INT16U io_Offset,INT8U iItemLen  = CONST_ALARM_RECORD_ITEM_LENGTH,
		INT16U FileLen = CONST_ALARM_FILE_LENGTH);//��һ���Ѿ��ϱ��ļ�¼�ı�־λ��Ϊ1


	static int AddAlarmRecord(const INT8U* iRecordContent,INT8U iTrueLen,INT16U iAlarmNo,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
		INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ALARM_FILE_LENGTH,
		char *FileName = CONST_ALARM_FILE_NAME);//��ӱ�����¼�ļ���һ��ֻ�����һ����¼

	static int AddAlarmRecord(const INT8U* iRecordContent,INT8U iTrueLen,INT16U iAlarmNo,const char *bcdtime,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
		INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ALARM_FILE_LENGTH,
		char *FileName = CONST_ALARM_FILE_NAME);//��ӱ�����¼�ļ���һ��ֻ�����һ����¼

	//static int ConvertMeterNoToBCD(INT16U iMeterNo,INT8U *iBcdMeterNo);//������Ŵ�HEX��ת����BCD��

	static int GetTrueLen(INT8U *iAlarmNo,INT8U &i_TrueLen);//�ڶ�ȡ�ļ�ʱ��ȡ������¼��ʵ�ʳ���

	//static int AddAlarmRecord(bstring &iRecordContent,INT8U iTrueLen,INT16U iAlarmNo,INT16U iMeterNo = CONS_UNVALID_METER_NUMBER,
	//	INT8U iItemLen = CONST_ALARM_RECORD_ITEM_LENGTH,INT16U FileLen = CONST_ALARM_FILE_LENGTH,
	//	char *FileName = CONST_ALARM_FILE_NAME);//��ӱ�����¼�ļ�
};

#endif
