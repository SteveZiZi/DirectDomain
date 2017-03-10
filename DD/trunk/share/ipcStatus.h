/** @file
 *  @brief QT�������̨��ؽ��̹���״̬����ͨѶ�ӿ���
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

#define CONS_DIM_ADJUST_LEVEL_MAX	20 //Dim�ĵ��ڼ���

/* 
brief:ϵͳDIM�������ݽṹ
*/
struct SDimTable
{
	INT16U m_TabVaule[CONS_DIM_ADJUST_LEVEL_MAX][2];//��һ���ǰٷֱ�ֵ0~100,�ڶ����ǵ�ѹֵ��BCD��ʽ
};

/* �豸��������*/
struct SIpcDeviceCfgData
{
	INT8U m_LuxRate;//Ȩ��ֵ 0~100��Ӧ0%~100%
};


struct SIpcStatus
{
public:
	struct HeaderChecker Checker;
	double m_Reg00010000;//�����ۼ���
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


