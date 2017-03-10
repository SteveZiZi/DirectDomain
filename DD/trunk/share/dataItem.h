#ifndef _DATAITEM_H_
#define _DATAITEM_H_

#include "sysbase.h"
#include "meter.h"


#define CONS_DATAITEM_COUNT_MAX		64

#define DNT_LIGHT_ON	1
#define DNT_LIGHT_OFF	0
#define CLEAN_LIGHT_ON  1
#define CLEAN_LIGHT_OFF  0
#define BELL_BG_LIGHT_ON 1
#define BELL_BG_LIGHT_OFF 0

#define BELL_RING_ON 1
#define BELL_RING_OFF 0

#define POWER_INDECATION_ON	0
#define POWER_INDECATION_OFF	1

#define NIGHTLIGHT_INDECATION_ON	0
#define NIGHTLIGHT_INDECATION_OFF	1

#define INSET_CARD_VALID_LEVEL	0
#define INSET_CARD_INVALID_LEVEL	1


typedef enum
{
	FUNC_NORMAL=0,
	FUNC_LIGHT_UP=1,//����
	FUNC_LIGHT_DOWN=2,//����
	FUNC_LIGHT_AUTO=3,
	FUNC_LIGHT_AUTO_UP=4,//�򿪼̵�������
	FUNC_LIGHT_AUTO_DOWN=5,//�������ص��̵���
	FUNC_POWER_KEY=6,
	FUNC_KEYBACKLIGHT=7,
	FUNC_NIGHT_LIGHT=8,
	FUNC_RELAY_ON=9,//�̵�����
	FUNC_RELAY_OFF=10,//�̵�����
	FUNC_WINDOW_OPEN=11,//�����������
	FUNC_WINDOW_CLOSE=12,//�����ص����
	FUNC_SINGLE_BRAKE=13,//�յ����ܷ�
	FUNC_AIRCDI_DOUBLE_COLD_BRAKE=14,//�յ�˫���䷧
	FUNC_AIRCDI_DOUBLE_HOT_BRAKE=15,//�յ�˫���ȷ�
	FUNC_AIRCDI_WIND_SPEED=16,//�յ�����ٶȿ���
	FUNC_AIRCDI_DIGTIAL_BACKLIGHT=17,//�յ�����ܱ������
	FUNC_URGENT_KEY=18,//������ť
	FUNC_DOOR_CHECK=19,//�����Ŵż��
	FUNC_GALLERY_LIGHT=20,//�ȵ�
	FUNC_KEY_CLEANROOM=21,//������
	FUNC_KEY_DONOT_DISTURB=22,//�������
	FUNC_KEY_BELL=23,//����
	FUNC_INPUT_PORT_TRIGLE=24,//����˵�ƽ���ϴ���
	FUNC_INPUT_PORT_HIGH_LEVEL=25,//����˸ߵ�ƽ����
	FUNC_INPUT_PORT_LOW_LEVEL=26,//����˵͵�ƽ����
	FUNC_LIGHT_LEVEL=27,//����������
	FUNC_KEY_WASH_CLOSE=28,//ϴ�·�
	FUNC_SCENE_COMPOSE1=29,//���1�ƹ�����
	FUNC_SCENE_COMPOSE2=30,//���2�ƹ�����
	FUNC_SCENE_COMPOSE3=31,//���3�ƹ�����
	FUNC_SCENE_COMPOSE4=32,//���4�ƹ�����
	FUNC_SCENE_COMPOSE5=33,//���5�ƹ�����
	FUNC_SCENE_COMPOSE6=34,//���6�ƹ�����
	FUNC_SCENE_COMPOSE7=35,//���7�ƹ�����
	FUNC_LEFT_BED_LIGHT= 36,//��ͷ��
	FUNC_RIGHT_BED_LIGHT= 37,//�Ҵ�ͷ��
	FUNC_AIRCONDITION_SET_TEMPER = 38,//�յ��¶�����
	FUNC_AIRCONDITION_WINDSPEED = 39,//�յ�����
	FUNC_RTF_VOL_SET = 40,//��������ѹ����
	FUNC_RTF_ON = 41,//��������
	FUNC_RTF_OFF = 42,//��������
	FUNC_CL_VOL_SET = 43,//��·��ѹ����
	FUNC_CL_ON = 44,//��·������������
	FUNC_CL_OFF = 45,//��·������������
	/* �����ڴ�ͨѶ����*/
	FUNC_TELNO_SET = 50,//����Ա��������
	FUNC_NET_SET = 51,//��������
	
	FUNC_GET_TASKSINFO = 52,//��ȡ�̿�������Ϣ 
	FUNC_SET_TASKSINFO = 53,//���ó̿�������Ϣ 
	FUNC_GET_LUXTASKSINFO = 54,//��ȡ���������Ϣ 
	FUNC_SET_LUXTASKSINFO = 55,//���ù��������Ϣ 
	FUNC_GET_CONSLUXTASKSINFO = 56,//��ȡ����������Ϣ 
	FUNC_SET_CONSLUXTASKSINFO = 57,//���ú���������Ϣ 

	FUNC_SET_MANUAL_VALIDTIME = 58,//�ֶ�����ʱЧʱ������ 
	FUNC_SET_CL_LEAKDTINFO = 59,//���û�·©������Ϣ
	FUNC_CLEAR_IMPALARM_RECORDS = 60,//�����Ҫ�澯��־
	FUNC_GET_SERVER_IP_PORT=61,//��ȡ������ip�Ͷ˿�
	FUNC_SET_SERVER_IP_PORT=62,//���÷�����ip�Ͷ˿�
	FUNC_MAX
}T_emFuncType;

typedef enum
{
	RELAY_REVERSE=1,
	RELAY_ON=2,
	RELAY_OFF=3
}T_emRelayState;

class CDataItem
{
public:
	CDataItem(INT16U dataId)
	{
		m_dataId = dataId;
		
	}
	//~CDataItem(){}
	INT16U GetDataItemID(void){return m_dataId;}
	void SetDataItemID(INT16U dataid){m_dataId = dataid;}
	virtual int Exec(Device &device,bstring &Data){return 0;};
	virtual bool Read(Device device,bstring &rtData);
	virtual bool Read(INT8U busNo,Device device,bstring &rtData);
	virtual bool ModbusRead(Device device,INT8U commNo,INT8U ctl,INT16U dataid,bstring iData,bstring &rtData);
	virtual bool Write(Device device,INT16U dataid,bstring wtData);
	virtual bool Write(Device device,INT16U dataid,bstring wtData,bstring &rtData);
	virtual bool ModbusWrite(Device device,INT8U commNo,INT8U ctl,INT16U dataid,bstring iData,bstring &rtData);
private:
	int MakePackage(Device device,INT8U ctl,INT16U dataid, const INT8U* iData,const INT8U iDataLen,bstring& oPackage);
	int MakeModbusPackage(INT8U commNo,INT8U ctl,INT16U dataid, const INT8U* iData,const INT8U iDataLen,bstring& oPackage);
private:
	INT16U m_dataId;
};

/***************************************************/
/**       class CAlarms declare                    */
/**            									   */	
/***************************************************/
//**��Ӹ澯����
//**1. ����һ���澯��
//**2. ��CAlarms��initʵ�����ø澯��*/


class CDataItems
{
public:
	CDataItems()
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
	CDataItem* GetDataItem(INT16U dataId);
	
	/*
		brief: get alarms array
		param [in] void
		return CAlarm*  
		author:WanQingsong
		date: 2012-04-09
	*/ 

	CDataItem** GetDataItems(void){return (&m_pDataItems[0]);}
	
	INT8U GetDataItemNum(void){return m_count;}
private:
	/*
		brief: add a alram type to Alarm types muster
		param [in] CAlarm alarm
		return bool 
		author:WanQingsong
		date: 2012-04-13
	*/  
	bool AddDataItem(CDataItem *pItem);

private:
	CDataItem *m_pDataItems[CONS_DATAITEM_COUNT_MAX];
	INT8U m_count;	
};


/***************************************************/
/**       class CAlarm type define                 */
/**            									   */	
/***************************************************/


class CDataItem_0110:public CDataItem
{
public:
	CDataItem_0110():CDataItem((INT16U)0x0110)
	{
	}
};
class CDataItem_1000:public CDataItem
{
public:
	CDataItem_1000():CDataItem((INT16U)0x1000)
	{
	}
	int Exec(Device &device,bstring &Data){return 0;}
};
class CDataItem_1001:public CDataItem
{
public:
	CDataItem_1001():CDataItem((INT16U)0x1001)
	{
	}
	int Exec(Device &device,bstring &Data);
};

class CDataItem_09XX:public CDataItem
{
public:
	CDataItem_09XX():CDataItem((INT16U)0x0900)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};
class CDataItem_1004:public CDataItem
{
public:
	CDataItem_1004():CDataItem((INT16U)0x1004)
	{
	}
	int Exec(Device &device,bstring &Data);
};
//����һ��
class CDataItem_1030:public CDataItem
{
public:
	CDataItem_1030():CDataItem((INT16U)0x1030)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

//����һ��
class CDataItem_1031:public CDataItem
{
public:
	CDataItem_1031():CDataItem((INT16U)0x1031)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_1032:public CDataItem
{
public:
	CDataItem_1032():CDataItem((INT16U)0x1032)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_1035:public CDataItem
{
public:
	CDataItem_1035():CDataItem((INT16U)0x1035)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};


class CDataItem_1101:public CDataItem
{
public:
	CDataItem_1101():CDataItem((INT16U)0x1101)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_1102:public CDataItem
{
public:
	CDataItem_1102():CDataItem((INT16U)0x1102)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_1105:public CDataItem
{
public:
	CDataItem_1105():CDataItem((INT16U)0x1104)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_1106:public CDataItem
{
public:
	CDataItem_1106():CDataItem((INT16U)0x1105)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_1107:public CDataItem
{
public:
	CDataItem_1107():CDataItem((INT16U)0x1107)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_1133:public CDataItem
{
public:
	CDataItem_1133():CDataItem((INT16U)0x1133)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_1144:public CDataItem
{
public:
	CDataItem_1144():CDataItem((INT16U)0x1144)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};


class CDataItem_1210:public CDataItem
{
public:
	CDataItem_1210():CDataItem((INT16U)0x1210)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_2010:public CDataItem
{
public:
	CDataItem_2010():CDataItem((INT16U)0x2010)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};
class CDataItem_2011:public CDataItem
{
public:
	CDataItem_2011():CDataItem((INT16U)0x2011)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_2110:public CDataItem
{
public:
	CDataItem_2110():CDataItem((INT16U)0x2110)
	{
	}
	int Exec(Device &device,bstring &Data);
private:
	void DoorcardCompose(INT8U state);
};

class CDataItem_3001:public CDataItem
{
public:
	CDataItem_3001():CDataItem((INT16U)0x3001)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_3002:public CDataItem
{
public:
	CDataItem_3002():CDataItem((INT16U)0x3002)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_3003:public CDataItem
{
public:
	CDataItem_3003():CDataItem((INT16U)0x3003)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_3004:public CDataItem
{
public:
	CDataItem_3004():CDataItem((INT16U)0x3004)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_3005:public CDataItem
{
public:
	CDataItem_3005():CDataItem((INT16U)0x3005)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_3007:public CDataItem
{
public:
	CDataItem_3007():CDataItem((INT16U)0x3007)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_3008:public CDataItem
{
public:
	CDataItem_3008():CDataItem((INT16U)0x3008)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_3009:public CDataItem
{
public:
	CDataItem_3009():CDataItem((INT16U)0x3009)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_3010:public CDataItem
{
public:
	CDataItem_3010():CDataItem((INT16U)0x3010)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};
class CDataItem_3011:public CDataItem
{
public:
	CDataItem_3011():CDataItem((INT16U)0x3011)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};
class CDataItem_3101:public CDataItem
{
public:
	CDataItem_3101():CDataItem((INT16U)0x3101)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};
class CDataItem_3102:public CDataItem
{
public:
	CDataItem_3102():CDataItem((INT16U)0x3102)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};
class CDataItem_3103:public CDataItem
{
public:
	CDataItem_3103():CDataItem((INT16U)0x3103)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};
class CDataItem_3104:public CDataItem
{
public:
	CDataItem_3104():CDataItem((INT16U)0x3104)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_3105:public CDataItem
{
public:
	CDataItem_3105():CDataItem((INT16U)0x3105)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_4001:public CDataItem
{
public:
	CDataItem_4001():CDataItem((INT16U)0x4001)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};

class CDataItem_C343:public CDataItem
{
public:
	CDataItem_C343():CDataItem((INT16U)0xC343)
	{
	}
	int Exec(Device &device,bstring &Data);
		
};
#endif



