/** @file
 *  @brief QT�������̨��ؽ��̹����ڴ�ͨѶ�ӿ���
 *  @author Steven Wan
 *  @date 2015-08/30
 *  @version 0.1
*/

#ifndef _IPC_SHAREMEM_H_
#define _IPC_SHAREMEM_H_

//#include "context.h"
#include "src/SmartBuilding/dataLevel/dataComm/base/FileBase.h"
#include "src/SmartBuilding/dataLevel/dataComm/include/typedef.h"
#include "ShareMem.h"

#ifdef WIN32
#define MNT_PATH_BASE	"./mnt"
#else
#define MNT_PATH_BASE	"/mnt"
#endif


//#define FILE_IPC_SHAREMEM MNT_PATH_BASE"/ramdisk/dyjc/ipcsharemem.dat"	//DYJCϵͳ���������ڴ湲���ļ�
#define FILE_IPC_SHAREMEM "/mnt/ramdisk/ipcsharemem.dat"	//share memory file

#define CONS_IPC_REAL_CONTROL_COMMAND_MAX_COUNT	20 //ʵʱ������������Ա��
/* �豸�ڴ�����*/
struct SIpcDeviceRegData
{
    INT8U m_DeviceNo;//�豸���
    INT8U m_ClNo;//��·��
    INT8U m_DeviceType;//�豸����
    INT8U m_Reg0501[9];//�Ĵ�������
};

/*
brief: ʵʱ����
*/
struct SIpcRealControlCommand
{
    INT8U m_DeviceNo;
    INT8U m_SubNo;
    INT8U m_Function;
    INT8U m_FuncData[4];
};
/* ʵʱ��������*/
struct SIpcRealControlCommandQueue
{
    bool m_bBusy;
    int m_Num;//������������
    int m_FinishedNum;//�Ѵ������������
    SIpcRealControlCommand m_RealCtlCmd[CONS_IPC_REAL_CONTROL_COMMAND_MAX_COUNT];
    T_emResult m_Result[CONS_IPC_REAL_CONTROL_COMMAND_MAX_COUNT];
};

/*
brief:������������
*/
struct SEth0Config
{
    INT8U m_TermIP[4];
    INT8U m_NetMask[4];
    INT8U m_GateWay[4];
};

/*
brief:������IP����
*/
struct SServerIpConfig
{
    INT8U m_IP[4];
    INT16U m_Port;
};

/*
brief:�̿�����
*/
#define CONS_IPC_NORTASK_MAX_COUNT		12
#define CONS_IPC_TASKPLAN_MAX_COUNT		12
/*
brief:�������
*/
#define CONS_IPC_LUXTASK_MAX_COUNT		2
#define CONS_IPC_LUXTASKPLAN_MAX_COUNT		12

/*
brief:��������
*/
#define CONS_IPC_CONSTANTLUXTASK_MAX_COUNT		2

/* �������Ͷ���*/
enum EM_IPC_PERIODUNIT
{
    EM_IPC_PERIODUNIT_DAY=0,//��ÿ�յ�СʱΪ����
    EM_IPC_PERIODUNIT_WEEK=1,//���ܵ���Ϊ����
    EM_IPC_PERIODUNIT_MONTH= 2//��ÿ�µ���Ϊ����
};

struct SSmartGarageItem
{
    INT8U m_DeviceNo;
    INT8U m_SubNo;
    INT8U m_Function;
    INT8U m_FuncData[4];
};

/* ���񷽰� */
struct SIpcPlan
{
    INT8U m_Enable;//ִ�з���ʹ��λ
    INT8U m_BeginTimeHHMM[2];//��ʼʱ��
    INT8U m_ActionItemsNum;//ִ�������
    SSmartGarageItem m_ActionItems[32];//ִ�����б�
};

/*
brief:�̿��������ݽṹ
*/
struct SIpcNorTask
{
    INT8U m_Enable;//����ʹ��
    INT8U m_TaskNo;//�����
    INT8U m_Priority;//���ȼ�
    time_t  m_StartTime;//��ʼʱ��
    time_t  m_EndTime;//����ʱ��
    INT8U m_PeriodUnit;//ִ������
    INT32U m_PeriodDetailValue;//����ϸ������
    SIpcPlan m_Plans[CONS_IPC_TASKPLAN_MAX_COUNT];//ִ�з���
};

struct SIpcNorTasks
{
    SIpcNorTask m_Tasks[CONS_IPC_NORTASK_MAX_COUNT+1];/* ���񼯺� */
};

/*
brief:����������ݽṹ
*/
/* ������񷽰� */
struct SIpcLuxPlan
{
    INT8U m_Enable;//ִ�з���ʹ��λ
    //INT8U m_BeginTimeHHMM[2];//��ʼʱ��
    INT16U m_KeepMins;//�ȴ�ִ�е�ʱ��
    INT8U m_ActionItemsNum;//ִ�������
    SSmartGarageItem m_ActionItems[32];//ִ�����б�
};
struct SIpcLuxTask
{
    INT8U m_Enable;
    INT8U m_TaskNo;
    INT8U m_Priority;
    time_t  m_StartTime;
    time_t  m_EndTime;
    INT8U m_PeriodUnit;//ִ������
    INT32U m_PeriodDetailValue;
    INT8U m_RunState;//Run;Stop;Pause
    INT32U m_StartLuxlimit;//������ֵ
    INT32U m_EndLuxlimit;//������ֵ
    INT32U m_LuxSensorsBS;//������ش�������Чλ��bit1��Ӧsensor1~bit31��Ӧsensor31��Ŀǰֻ����bit1~bit16��Ӧ1~16��sensor
    SIpcLuxPlan m_Plans[CONS_IPC_LUXTASKPLAN_MAX_COUNT];
};
struct SIpcLuxTasks
{
    SIpcLuxTask m_LuxTasks[CONS_IPC_LUXTASK_MAX_COUNT+1];/* ������񼯺� */
};
/*
brief:�����������ݽṹ
*/
struct SIpcConstantLuxTask
{
    INT8U m_Enable;
    INT8U m_TaskNo;
    INT8U m_Priority;
    time_t  m_StartTime;
    time_t  m_EndTime;
    INT8U m_PeriodUnit;//ִ������
    INT32U m_PeriodDetailValue;
    INT8U m_RunState;//Run;Stop;Pause
    INT32U m_ConstantLux;//�㶨ֵ
    INT32U m_LuxSensorsBS;//������ش�������Чλ��bit0��Ӧsensor1~bit31��Ӧsensor32��Ŀǰֻ����bit0~bit15��Ӧ1~16��sensor
    INT8U m_ClrelayNo;//��·��
    INT8U m_TimePeriodInDay[CONS_TIME_PERIOD_IN_DAY_MAX_COUNT][4];//BCD��ʽHHMM,��ʱ��1Ϊ[0][0][0][1]~[0][2][0][3] ʱ��2Ϊ[1][0][1][1]~[1][2][1][3]
};
struct SIpcConstantLuxTasks
{
    SIpcConstantLuxTask m_ConstantLuxTasks[CONS_IPC_CONSTANTLUXTASK_MAX_COUNT+1];/* �������񼯺� */
};

/*
brief:©����������Ϣ
*/
struct SIpcClLeakInfo
{
    INT8U m_Enable;
    INT8U m_ChannelNo;//©����ͨ�����1~8
    INT16U m_DelayMs;
    INT16S m_DtVol;
    INT8U m_CutEnable;
};

/*
brief:�����ڴ�
*/
struct SIpcShareMem
{
public:
    struct HeaderChecker Checker;

    char m_cRTUA[4];	//�ն��߼���ַ

    INT8U m_TeleNo[6];//����Ա����

    INT8U m_ClNum;//��·����

    struct SIpcDeviceRegData m_IpcDevicesRegData[CONS_DEVICE_MAX_COUNT];//�Ĵ�������

    SIpcRealControlCommandQueue m_IpcRealControlCommandQueue;//����ʵʱ�����������

    SEth0Config m_Eth0Config;//��������

    SIpcNorTasks m_IpcNorTasks;//�����������

    SIpcLuxTasks m_IpcLuxTasks;//����������

    SIpcConstantLuxTasks m_IpcConstantLuxTasks;//�����������

    SIpcClLeakInfo m_IpcClLeakInfo[CONS_CLRELAY_MAX_COUNT];//��·©��������
    
    SServerIpConfig m_ServerIpConfig;
    
    time_t m_QtRunTimeStamp; // QtӦ�ó���ʱ���

public:
    SIpcShareMem();
    static SIpcShareMem& Instance(void);
    bool Update(void){return m_IpcShareMem.Save(this,sizeof(*this));}
    void InitDevicesRegData(){memset(&m_IpcDevicesRegData[0],0x00,sizeof(m_IpcDevicesRegData));}
    void InitDevicesRegData(INT16U Tn){memset(&m_IpcDevicesRegData[Tn],0x00,sizeof(m_IpcDevicesRegData[Tn]));}

private:
    static bool InitFile(FILE* fp, void* ipOther);
    static bool CheckFile(FILE* fp, void* ipOther);
    int Save(bool ibForce);

private:
    static ShareMem m_IpcShareMem;
};

#define IPCSHAREMEM SIpcShareMem::Instance()
#define DECLARE_IPCSHAREMEM(ipcsharemem) SIpcShareMem& ipcsharemem=IPCSHAREMEM

#endif

