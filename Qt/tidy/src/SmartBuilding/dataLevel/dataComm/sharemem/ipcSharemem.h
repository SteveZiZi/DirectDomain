/** @file
 *  @brief QT进程与后台监控进程共享内存通讯接口类
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


//#define FILE_IPC_SHAREMEM MNT_PATH_BASE"/ramdisk/dyjc/ipcsharemem.dat"	//DYJC系统的上下文内存共享文件
#define FILE_IPC_SHAREMEM "/mnt/ramdisk/ipcsharemem.dat"	//share memory file

#define CONS_IPC_REAL_CONTROL_COMMAND_MAX_COUNT	20 //实时控制命令最大成员数
/* 设备内存数据*/
struct SIpcDeviceRegData
{
    INT8U m_DeviceNo;//设备序号
    INT8U m_ClNo;//回路号
    INT8U m_DeviceType;//设备类型
    INT8U m_Reg0501[9];//寄存器数据
};

/*
brief: 实时命令
*/
struct SIpcRealControlCommand
{
    INT8U m_DeviceNo;
    INT8U m_SubNo;
    INT8U m_Function;
    INT8U m_FuncData[4];
};
/* 实时命令处理队列*/
struct SIpcRealControlCommandQueue
{
    bool m_bBusy;
    int m_Num;//待处理命令数
    int m_FinishedNum;//已处理完的命令数
    SIpcRealControlCommand m_RealCtlCmd[CONS_IPC_REAL_CONTROL_COMMAND_MAX_COUNT];
    T_emResult m_Result[CONS_IPC_REAL_CONTROL_COMMAND_MAX_COUNT];
};

/*
brief:本地网口配置
*/
struct SEth0Config
{
    INT8U m_TermIP[4];
    INT8U m_NetMask[4];
    INT8U m_GateWay[4];
};

/*
brief:服务器IP配置
*/
struct SServerIpConfig
{
    INT8U m_IP[4];
    INT16U m_Port;
};

/*
brief:程控任务
*/
#define CONS_IPC_NORTASK_MAX_COUNT		12
#define CONS_IPC_TASKPLAN_MAX_COUNT		12
/*
brief:光控任务
*/
#define CONS_IPC_LUXTASK_MAX_COUNT		2
#define CONS_IPC_LUXTASKPLAN_MAX_COUNT		12

/*
brief:恒照任务
*/
#define CONS_IPC_CONSTANTLUXTASK_MAX_COUNT		2

/* 周期类型定义*/
enum EM_IPC_PERIODUNIT
{
    EM_IPC_PERIODUNIT_DAY=0,//以每日的小时为周期
    EM_IPC_PERIODUNIT_WEEK=1,//以周的天为周期
    EM_IPC_PERIODUNIT_MONTH= 2//以每月的天为周期
};

struct SSmartGarageItem
{
    INT8U m_DeviceNo;
    INT8U m_SubNo;
    INT8U m_Function;
    INT8U m_FuncData[4];
};

/* 任务方案 */
struct SIpcPlan
{
    INT8U m_Enable;//执行方案使能位
    INT8U m_BeginTimeHHMM[2];//起始时间
    INT8U m_ActionItemsNum;//执行项个数
    SSmartGarageItem m_ActionItems[32];//执行项列表
};

/*
brief:程控任务数据结构
*/
struct SIpcNorTask
{
    INT8U m_Enable;//任务使能
    INT8U m_TaskNo;//任务号
    INT8U m_Priority;//优先级
    time_t  m_StartTime;//起始时间
    time_t  m_EndTime;//结束时间
    INT8U m_PeriodUnit;//执行周期
    INT32U m_PeriodDetailValue;//周期细节描述
    SIpcPlan m_Plans[CONS_IPC_TASKPLAN_MAX_COUNT];//执行方案
};

struct SIpcNorTasks
{
    SIpcNorTask m_Tasks[CONS_IPC_NORTASK_MAX_COUNT+1];/* 任务集合 */
};

/*
brief:光控任务数据结构
*/
/* 光控任务方案 */
struct SIpcLuxPlan
{
    INT8U m_Enable;//执行方案使能位
    //INT8U m_BeginTimeHHMM[2];//起始时间
    INT16U m_KeepMins;//等待执行的时间
    INT8U m_ActionItemsNum;//执行项个数
    SSmartGarageItem m_ActionItems[32];//执行项列表
};
struct SIpcLuxTask
{
    INT8U m_Enable;
    INT8U m_TaskNo;
    INT8U m_Priority;
    time_t  m_StartTime;
    time_t  m_EndTime;
    INT8U m_PeriodUnit;//执行周期
    INT32U m_PeriodDetailValue;
    INT8U m_RunState;//Run;Stop;Pause
    INT32U m_StartLuxlimit;//启动阈值
    INT32U m_EndLuxlimit;//结束阈值
    INT32U m_LuxSensorsBS;//方案相关传感器有效位，bit1对应sensor1~bit31对应sensor31，目前只用了bit1~bit16对应1~16号sensor
    SIpcLuxPlan m_Plans[CONS_IPC_LUXTASKPLAN_MAX_COUNT];
};
struct SIpcLuxTasks
{
    SIpcLuxTask m_LuxTasks[CONS_IPC_LUXTASK_MAX_COUNT+1];/* 光控任务集合 */
};
/*
brief:恒照任务数据结构
*/
struct SIpcConstantLuxTask
{
    INT8U m_Enable;
    INT8U m_TaskNo;
    INT8U m_Priority;
    time_t  m_StartTime;
    time_t  m_EndTime;
    INT8U m_PeriodUnit;//执行周期
    INT32U m_PeriodDetailValue;
    INT8U m_RunState;//Run;Stop;Pause
    INT32U m_ConstantLux;//恒定值
    INT32U m_LuxSensorsBS;//方案相关传感器有效位，bit0对应sensor1~bit31对应sensor32，目前只用了bit0~bit15对应1~16号sensor
    INT8U m_ClrelayNo;//回路号
    INT8U m_TimePeriodInDay[CONS_TIME_PERIOD_IN_DAY_MAX_COUNT][4];//BCD格式HHMM,如时段1为[0][0][0][1]~[0][2][0][3] 时段2为[1][0][1][1]~[1][2][1][3]
};
struct SIpcConstantLuxTasks
{
    SIpcConstantLuxTask m_ConstantLuxTasks[CONS_IPC_CONSTANTLUXTASK_MAX_COUNT+1];/* 恒照任务集合 */
};

/*
brief:漏电检测配置信息
*/
struct SIpcClLeakInfo
{
    INT8U m_Enable;
    INT8U m_ChannelNo;//漏电检测通道序号1~8
    INT16U m_DelayMs;
    INT16S m_DtVol;
    INT8U m_CutEnable;
};

/*
brief:共享内存
*/
struct SIpcShareMem
{
public:
    struct HeaderChecker Checker;

    char m_cRTUA[4];	//终端逻辑地址

    INT8U m_TeleNo[6];//管理员号码

    INT8U m_ClNum;//回路总数

    struct SIpcDeviceRegData m_IpcDevicesRegData[CONS_DEVICE_MAX_COUNT];//寄存器数据

    SIpcRealControlCommandQueue m_IpcRealControlCommandQueue;//界面实时控制命令队列

    SEth0Config m_Eth0Config;//网络配置

    SIpcNorTasks m_IpcNorTasks;//程序任务控制

    SIpcLuxTasks m_IpcLuxTasks;//光控任务控制

    SIpcConstantLuxTasks m_IpcConstantLuxTasks;//恒照任务控制

    SIpcClLeakInfo m_IpcClLeakInfo[CONS_CLRELAY_MAX_COUNT];//回路漏电检测配置
    
    SServerIpConfig m_ServerIpConfig;
    
    time_t m_QtRunTimeStamp; // Qt应用程序时间戳

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

