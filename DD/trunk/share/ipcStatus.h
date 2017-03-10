/** @file
 *  @brief QT进程与后台监控进程共享状态数据通讯接口类
 *  @author Steven Wan
 *  @date 2015-09/26
 *  @version 0.1
*/
 
#ifndef _IPC_STATUS_H_
#define _IPC_STATUS_H_

#include "context.h"
#include "../base/FileBase.h"
#include "../base/ShareMem.h"

#ifdef WIN32
#define MNT_PATH_BASE	"./mnt"
#else
#define MNT_PATH_BASE	"/mnt"
#endif


#define FILE_IPC_STATUS "/mnt/data/ipcstatus.dat"	//share memery file

#define CONS_DIM_ADJUST_LEVEL_MAX	20 //Dim的调节级数

/* 
brief:系统DIM表存放数据结构
*/
struct SDimTable
{
	INT16U m_TabVaule[CONS_DIM_ADJUST_LEVEL_MAX][2];//第一列是百分比值0~100,第二列是电压值，BCD格式
};

/* 设备配置数据*/
struct SIpcDeviceCfgData
{
	INT8U m_LuxRate;//权重值 0~100对应0%~100%
};


struct SIpcStatus
{
public:
	struct HeaderChecker Checker;
	double m_Reg00010000;//电能累计量
	SDimTable m_Dimtable[CONS_CLRELAY_MAX_COUNT];
	SIpcDeviceCfgData m_DeviceCfgData[CONS_DEVICE_MAX_COUNT];
	INT8U  m_ClleakState[CONS_CLRELAY_MAX_COUNT];
public:
	SIpcStatus();
	static SIpcStatus& Instance(void);
	bool Update(void){return m_IpcStatus.Save(this,sizeof(*this));}

private:
	static bool InitFile(FILE* fp, void* ipOther);
	static bool CheckFile(FILE* fp, void* ipOther);
	int Save(bool ibForce);
	
private:
	static ShareMem m_IpcStatus;	
};

#define IPCSTATUS SIpcStatus::Instance()
#define DECLARE_IPCSTATUS(ipcsharemem) SIpcStatus& ipcstatus=IPCSTATUS

#endif


