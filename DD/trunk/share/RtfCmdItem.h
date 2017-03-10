#ifndef _RtfCmdItem_H_
#define _RtfCmdItem_H_

#include "sysbase.h"
#include "meter.h"


#define CONS_CMDITEM_COUNT_MAX		10

#define CAN_CMD_SET_PARAM		0x01
#define CAN_CMD_READ_STATE	0x02




class CRtfCmdItem
{
public:
	CRtfCmdItem(INT16U dataId)
	{
		m_dataId = dataId;
		
	}
	//~CRtfCmdItem(){}
	INT16U GetRtfCmdItemID(void){return m_dataId;}
	void SetRtfCmdItemID(INT16U dataid){m_dataId = dataid;}
	virtual int Exec(Device &device,bstring &Data){return 0;};
	virtual bool Read(Device device,INT8U ctl,bstring &rtData);
	virtual bool Write(Device device,INT8U ctl,bstring wtData);
	virtual bool Write(Device device,INT8U ctl,bstring wtData,bstring &rtData);
	virtual bool Write(INT8U srcAddr,INT8U destAddr,INT8U cmd, INT8U *pDatabuf,INT8U len,bstring &rtData);
	bool ModbusRead(Device device,INT8U commNo,INT8U ctl,INT16U dataid,bstring iData,bstring &rtData);
private:
	int MakePackage(Device device,INT8U ctl,const INT8U* iData,const INT8U iDataLen,bstring& oPackage);
	int MakeModbusPackage(INT8U commNo,INT8U ctl,INT16U dataid, const INT8U* iData,const INT8U iDataLen,bstring& oPackage);
private:
	INT16U m_dataId;
};

/***************************************************/
/**       class CAlarms declare                    */
/**            									   */	
/***************************************************/
//**添加告警步骤
//**1. 新增一个告警类
//**2. 在CAlarms。init实例化该告警类*/


class CRtfCmdItems
{
public:
	CRtfCmdItems()
	{
	}
	/*
		brief: initialize all alarm types
		param [in] void
		return void
		author:WanQingsong
		date: 2012-04-13
	*/  
    void Init(void);
    /*
		brief: get alarm type according to alarm name
		param [in] string alarmName: alarm name
		return CAlarm*  
		author:WanQingsong
		date: 2012-04-09
	*/ 
	CRtfCmdItem* GetRtfCmdItem(INT16U dataId);
	
	/*
		brief: get alarms array
		param [in] void
		return CAlarm*  
		author:WanQingsong
		date: 2012-04-09
	*/ 

	CRtfCmdItem** GetRtfCmdItems(void){return (&m_pRtfCmdItems[0]);}
	
	INT8U GetRtfCmdItemNum(void){return m_count;}
private:
	/*
		brief: add a alram type to Alarm types muster
		param [in] CAlarm alarm
		return bool 
		author:WanQingsong
		date: 2012-04-13
	*/  
	bool AddRtfCmdItem(CRtfCmdItem *pItem);

private:
	CRtfCmdItem *m_pRtfCmdItems[CONS_CMDITEM_COUNT_MAX];
	INT8U m_count;	
};


/***************************************************/
/**       class CAlarm type define                 */
/**            									   */	
/***************************************************/

/* brief:读整流器电流、电压、告警量等信息类
*/
class CRtfCmdItem_0501:public CRtfCmdItem
{
public:
	CRtfCmdItem_0501():CRtfCmdItem((INT16U)0x0501)
	{
	}
	int Exec(Device &device,bstring &Data);
};

/* brief:设置整流器电压
*/
class CRtfCmdItem_0601:public CRtfCmdItem
{
public:
	CRtfCmdItem_0601():CRtfCmdItem((INT16U)0x0601)
	{
	}
	int Exec(Device &device,bstring &Data);
};

/* brief:设置整流器开关机
*/
class CRtfCmdItem_0602:public CRtfCmdItem
{
public:
	CRtfCmdItem_0602():CRtfCmdItem((INT16U)0x0602)
	{
	}
	int Exec(Device &device,bstring &Data);
};


#endif



