///*SysBitSet.h*/

#ifndef INCLUDED_SYS_BASE
#define INCLUDED_SYS_BASE

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_BIT_SET
#include "../base/BitSet.h"
#endif

#ifndef INCLUDED_KL_DUMP
#include "../base/KLDump.h"
#endif


/* MODBUS_A */
#define MODBUS_A_COMMNO	0x3C	
#define MODBUS_READ	0x03
#define MODBUS_WRITE	0x06
#define MODBUS_A_RD_TEMPERATER 0X0033	//�����¶�
#define MODBUS_A_RD_SETTEMPERATER 0X0036	//�յ������¶�
#define MODBUS_A_RD_WINDSPEED 0x0030 //�յ�����
#define MODBUS_A_WR_SETTEMPERATER 0X0036	//�յ������¶�
#define MODBUS_A_WR_MODE	0X0031 //���ÿյ�ģʽ
#define MODBUS_A_WR_WINDSPEED 0x0030 //�յ�����


/* MODBUS B*/
#define MODBUS_B_COMMNO	0x01
#define MODBUS_B_RD_TEMPERATER 0X0033	//�����¶�
#define MODBUS_B_RD_SETTEMPERATER 0X0036	//�յ������¶�
#define MODBUS_B_RD_WINDSPEED 0X0032	//�յ�����
#define MODBUS_B_WR_SETTEMPERATER 0X0036	//�յ������¶�
#define MODBUS_B_WR_MODE 0X0031 //���ÿյ�ģʽ
#define MODBUS_B_WR_WINDSPEED 0X0032	//�յ�����

enum
{
 XIAOCHENGZB=0,ROUBLE=1,
};


typedef enum  _ACK_EXECEPT_CODE_T
{
	ROUBLE_COMM_EXECEPT                   = 0x11,
	ROUBLE_NOT_IN_NETWORK                 = 0x12,
	ROUBLE_EXIST_BUT_NOT_CONNECT          = 0x13,
	ROUBLE_CONNECTED_BUT_RS485_DISCONNECT = 0x14,
	ROUBLE_CONNECTED_BUT_METER_FAILURE    = 0x16,
} ACK_EXECEPT_CODE;

typedef enum ACK_CODE_T
{ 
	TIME_OUT_FRAME    = 0x01,
	REASSON_FRAME     = 0x03,
	VALID_FRAME       = 0x02,
	AUTO_REPORT_FRAME = 0x05,

} ACK_CODE;

enum Rceive_Status
{
	RECEIVED_FAILED_FRAME = -1,
       NOT_SURPPORT_FRAME = 0x01,
	RECEIVE_MISMATCH_FRAME = 0x04,
	RECEIVE_DENY_FRAME = 0x02,
	RECEIVE_OK_FRAME   = 0x00,
	RECEIVE_INFO_FRAME = 0x05,
	RECEIVE_REPORT_FRAME =  0x03,  
	RECEIVE_NOMETERADDR_FRAME = 0x06,
	RECEIVE_REPEATMADDR_FRAME = 0x07,
	RECEIVE_REQGATHERDATA_FRAME = 0x08,

};


//enum {DATA_BIT_MAX_COUNT=CONS_EVENT_ID_MAX_COUNT>CONS_REG_ID_MAX_COUNT?CONS_EVENT_ID_MAX_COUNT:CONS_REG_ID_MAX_COUNT};

enum {DATA_BIT_MAX_COUNT=CONS_REG_ID_MAX_COUNT};

struct DataBitSet:public FastBitSet<DATA_BIT_MAX_COUNT>
{
};

struct MeterBitSet:public FastBitSet<CONS_METER_MAX_COUNT>
{
};

struct RelayBitSet:public FastBitSet<CONS_RELAY_MAX_COUNT>
{
};
struct RouterBitSet:public FastBitSet<CONS_ROUTER_MAX_COUNT>
{
};

struct TaskBitSet:public FastBitSet<CONS_TASK_MAX_COUNT>
{
	//friend TaskBitSet operator|(const TaskBitSet& tbs1, const TaskBitSet& tbs2);
	//friend TaskBitSet operator^(const TaskBitSet& tbs1, const TaskBitSet& tbs2);
};

struct ClrelaySensorsBitSet:public FastBitSet<16>
{
	//friend TaskBitSet operator|(const TaskBitSet& tbs1, const TaskBitSet& tbs2);
	//friend TaskBitSet operator^(const TaskBitSet& tbs1, const TaskBitSet& tbs2);
};

struct DeviceRuleBitSet:public FastBitSet<CONS_DEVICE_RULE_MAX_COUNT>
{
};

typedef bitset<8> BitSet8;

struct BitSet256:public FastBitSet<256>
{
};

typedef enum _LCD_DEBUG
{
	L_NONE=0x00,
	L_REALGATHER_ONE=0x01,
	L_REALGATHER_ALL=0x02,	
	L_SEARCHMETER=0x03,
	L_REALGATHER_NEWMETER=0x04,
	L_CALLMASTER=0x05,
	L_RENETWORKING=0x06,
}LCD_DEBUG;

typedef enum _DATA_TYPE
{
	DATA_TYPE_VIP=0,//�ص��û�����
	DATA_TYPE_DAY=1,//���������
	DATA_TYPE_MONTH=2,//������
	DATA_TYPE_EVENT=3,//����¼�
	DATA_TYPE_REG=4,//ʵʱ����
	DATA_TYPE_ACTION=5,//ʱ���쳣
	DATA_TYPE_STATUS=6,//��ȡ���״̬����Уʱ�������
	DATA_TYPE_POWEROFF=7,//���ͣ���¼������ڵ��ͣ���¼��������ԣ�ʹ֮���ܺ������¼�����һ��EVENT�¼������뵥����һ���¼���
	//DATA_TYPE_ZBSNIFF=8,//�ز������������
	DATA_TYPE_INVALID=0xFF,
	DATA_TYPE_METER_MAX_VALUE=8,//�����ص��������͵����ֵΪ8
}DATA_TYPE;
KLDump& operator<< (KLDump& dump, DATA_TYPE v);
INT8U MAKE_DATA_TYPE_VAL(const char* key);
int MAKE_DATA_TYPE_KEY(INT8U val,char* oKey);
int MAKE_DATA_TYPE_CN_KEY(INT8U val,char* oKey);
inline bool IsRegDataType(INT8U typeVal)
{
	return (typeVal!=DATA_TYPE_ACTION);
}
inline bool IsActionDataType(INT8U typeVal)
{
	return (typeVal==DATA_TYPE_ACTION);
}
;

typedef enum _SERVER_MODE
{
	SERVER_MODE_ACTIVED=0,
	SERVER_MODE_INACTIVED=1,
	SERVER_MODE_SUSPENDED=2
}SERVER_MODE;
KLDump& operator<< (KLDump& dump, SERVER_MODE v);

typedef enum _SERVER_STATUS
{
	SERVER_STATUS_STOPPED,//δ����
	SERVER_STATUS_RUNNING,//��������
	SERVER_STATUS_DEADED,//����
	SERVER_STATUS_STOPPING//����ֹͣ
}SERVER_STATUS;

KLDump& operator<< (KLDump& dump, SERVER_STATUS v);

typedef enum _SERVER_TYPE
{
	SERVER_TYPE_UNKNOWN=0,
	SERVER_TYPE_DAEMON=1,
	//SERVER_TYPE_WORK=2,//suxufeng:[2010-3-31]ȥ��work����
	SERVER_TYPE_HTTX=3,
	SERVER_TYPE_DBCJ=4,
	SERVER_TYPE_PPP=5,
	SERVER_TYPE_JILIAN=6,
	SERVER_TYPE_LCD=7,
	SERVER_TYPE_ROUTER=8,
	SERVER_TYPE_SCANF=9,
}SERVER_TYPE;

KLDump& operator<< (KLDump& dump, SERVER_TYPE v);

typedef enum _METER_PROPERTY
{
	METER_PROPERTY_UNKNOWN=0,
	METER_PROPERTY_DXB=1,//������ӱ�
	METER_PROPERTY_JXB=2,//��е��
	METER_PROPERTY_JYDGN=3,//���׶๦�ܱ�
	METER_PROPERTY_WG=7,//���׶๦�ܱ��޹�
	METER_PROPERTY_DGN=8,//�๦�������
	METER_PROPERTY_DGNZ=9,//�๦���ܱ�
}METER_PROPERTY;
KLDump& operator<< (KLDump& dump, METER_PROPERTY v);
INT8U MAKE_METER_PROPERTY_VAL(const char* key);
int MAKE_METER_PROPERTY_KEY(INT8U val,char* oKey);


typedef enum _METER_CONNECTTYPE
{
	METER_CONNECTTYPE_UNKNOWN=0,
	METER_CONNECTTYPE_485=1,//485���ӱ�
	METER_CONNECTTYPE_ZB=2,//�ز���
    METER_CONNECTTYPE_RADIO=3,
}METER_CONNECTTYPE;
KLDump& operator<< (KLDump& dump, METER_CONNECTTYPE v);
INT8U MAKE_METER_CONNECTTYPE_VAL(const char* key);
int MAKE_METER_CONNECTTYPE_KEY(INT8U val,char* oKey);



typedef enum _METER_METERTYPE
{
	METER_METERTYPE_USERMETER=0,//�û���
	METER_METERTYPE_MAINMETER=1,//�ܱ�
	METER_METERTYPE_UNKNOWN=3,
}METER_METERTYPE;
KLDump& operator<< (KLDump& dump, METER_METERTYPE v);
INT8U MAKE_METER_METERTYPE_VAL(const char* key);
int MAKE_METER_METERTYPE_KEY(INT8U val,char* oKey);


INT8U MAKE_BUS_VAL(const char *ikey);
int MAKE_BUS_KEY(INT8U val,char* oKey);



typedef enum _METER_XWTYPE//�ز�������λ
{		
	METER_XW_UNKNOWN=0,//δ֪,
	METER_XW_A=1,//A��
	METER_XW_B=2,//B��
	METER_XW_C=3,//C��
	METER_XW_X=4,//ר��
}METER_XWTYPE;
KLDump& operator<< (KLDump& dump, METER_XWTYPE v);
INT8U MAKE_ZB_XWTYPE_VAL(const char* key);
int MAKE_ZB_XWTYPE_KEY(INT8U val,char* oKey);

//CGH 2010-04-10 modified
typedef enum _RELAY_TYPE//�ɼ��ն�����
{		
	RELAY_TYPE_EASTSOFT = 0, //�����ز�
	RELAY_TYPE_XIAOCHENG =1,//5200�������ն� 
	RELAY_TYPE_RUBONET = 2, //6100S��������߹����ն�
	RELAY_TYPE_DX=3,//����
	RELAY_TYPE_ROUBLE,//5300�������ն�  
	RELAY_TYPE_LIHE, //�����ز�
	RELAY_TYPE_RISSCOM,//5100����˹���ն� 
	RELAY_TYPE_MAX
}RELAY_TYPE;

typedef enum _DN_PROTOCOL_TYPE//�ɼ��ն�����
{		
	DN_PROTOCOL_TYPE_CAN_HDM = 0,
	DN_PROTOCOL_TYPE_RS485_A = 1,//����Դ
	DN_PROTOCOL_TYPE_RS485_B = 2,//�����
	DN_PROTOCOL_TYPE_MAX
}DN_PROTOCOL_TYPE;

typedef enum _METER_SETTIME_METHOD
{
	METER_SETTIME_METHOD_DEFAULT=0,//Ĭ�Ϸ�ʽ�����е���Լ���ɵ���Լȥ���������Ķ�ʱ��ʽ
	METER_SETTIME_METHOD_NONE=1,//����ʱ�����е���Լ��ĳЩ��������Ҫ�ر��ֹ��ʱ
	METER_SETTIME_METHOD_COMMON=2,//��ͨ��ʽ�������Լû�д˷�ʽ��ABB����������������
	METER_SETTIME_METHOD_BROADCAST=3,//�㲥��ʱ������645����Լ
	METER_SETTIME_METHOD_ONESTEP=4,//������ʱ��������645��Լ�������Լ
	METER_SETTIME_METHOD_TWOSTEP=5,//˫����ʱ������645��Լ�������Լ
}METER_SETTIME_METHOD;

KLDump& operator<< (KLDump& dump, METER_SETTIME_METHOD v);
INT8U MAKE_METER_SETTIME_METHOD_VAL(const char* key);
int MAKE_METER_SETTIME_METHOD_KEY(INT8U val,char* oKey);

typedef enum _METER_RULE
{
	METER_RULE_UNKNOWN=0,

	//METER_RULE_GB645_BASICBATCH=10,
	//METER_RULE_GB645_PARTBATCH=18,
	//METER_RULE_GB645_ONEITEM=19,
	METER_RULE_GB645=1,//��׼����645��Լ
	METER_RULE_MK3=2,//����MK3��
	METER_RULE_MK6=3,
	METER_RULE_LGB=4,//������B
	METER_RULE_LGD=5,//������D
	METER_RULE_ABBAIN=6,//ABB�Ħ���
	METER_RULE_ABBAIR=7,//ABB��Բ���
	METER_RULE_WS=8,//��ʤ
	METER_RULE_GB645_WEISHENG3=9,//��ʤ����
	METER_RULE_GB645_HAONINGDA=10,//���������
	METER_RULE_GB645_HUALONG=11,//���껪¡
	METER_RULE_GB645_KELU=12,//�����½
	METER_RULE_GD645=13,//���645��Լ	
	//2010-3-22�����ע�������������Ͱ���˹�ر���ص㣬�����������ʱ��
	METER_RULE_ELSTER=14,//����˹�ر�
	METER_RULE_GDZB=15,//����ز���Լ
	METER_RULE_GB2007 =16,//gb2007rule

	//METER_RULE_TOUCHUAN=18,//�����ز�͸����Լ
	
	METER_RULE_ALL=0xFF
}METER_RULE;

KLDump& operator<< (KLDump& dump, METER_RULE v);
INT8U MAKE_METER_RULE_VAL(const char* key);
int MAKE_METER_RULE_KEY(INT8U val,char* oKey);

typedef enum _DEVICE_RULE
{
	DEVICE_RULE_UNKNOWN=0,
	DEVICE_RULE_GDDYJC=1,
	DEVICE_RULE_P8090=2,
}DEVICE_RULE;

KLDump& operator<< (KLDump& dump, DEVICE_RULE v);
INT8U MAKE_DEVICE_RULE_VAL(const char* key);
int MAKE_DEVICE_RULE_KEY(INT8U val,char* oKey);

typedef enum _DEVICE_RULE_SUB
{
	DEVICE_RULE_SUB_NONE=0,
	DEVICE_RULE_SUB_CHONGQING=1,//����
	DEVICE_RULE_SUB_BEIJING=2,//����
	DEVICE_RULE_SUB_GUANGXI=3,//����
	DEVICE_RULE_SUB_GUANGDONG=4,//�㶫

}DEVICE_RULE_SUB;

KLDump& operator<< (KLDump& dump, _DEVICE_RULE_SUB v);
INT16U MAKE_DEVICE_RULE_SUB_VAL(const char* key);
int MAKE_DEVICE_RULE_SUB_KEY(INT16U val,char* oKey);

typedef enum _GATHER_STATUS
{
	GATHER_STATUS_UNKNOWN=0,
	GATHER_STATUS_FREE=1,//����
	GATHER_STATUS_IN_NORMAL_GATHER=2,//���������ɼ���
	GATHER_STATUS_IN_IDLE_GATHER=3,//����ִ�п��вɼ�����
	GATHER_STATUS_IN_ZBSNIFF_GATHER=4,//����ִ���ز��������
	GATHER_STATUS_ROUTER=5,//��������
	GATHER_STATUS_IN_REALCALL_GATHER = 6//ʵ�вɼ�
}GATHER_STATUS;

KLDump& operator<< (KLDump& dump, GATHER_STATUS v);

//�ɼ����Ϊ0ʱ��ʾû�вɼ�������������¹�����������
//��0λ ��1��ʾ�ɼ��ɹ�
//��1λ ��1��ʾ���ʹ���
//....

typedef enum _GATHER_RESULT
{
	
	GATHER_RESULT_NOT_GATHERED=0x00,//û�вɼ�
	GATHER_RESULT_SUCCEED=0x01,//�ɼ��ɹ�
	GATHER_RESULT_SEND_ERROR=0x02,//���ʹ���
	GATHER_RESULT_RECV_TIMEOUT=0x3,//���ճ�ʱ
	GATHER_RESULT_REFUSED=0x04,//�ܾ�
	GATHER_RESULT_DATAID_NOT_EXIST=0x05, // ���֧�ָ�������
	GATHER_RESULT_NOT_SUPPORT=0x08,//��֧�ֵĲɼ�
	GATHER_RESULT_COMMSUCCEED=0x09,//�����ͨѶ�ɹ�,����������ʧ��
	GATHER_RESULT_NOT_ANSWER=0x10,//����Ӧ
	GATHER_RESULT_ERROR_ANSWER=0x20,//��Ӧ������
	GATHER_RESULT_FINISHED_ERRDATA=0x21,//�ɼ���ɣ����ݲ�����
	GATHER_RESULT_BOARDCAST=0X30,//�㲥���񷵻ؽ��
	GATHER_RESULT_ERROR_DATA=0x40,//���ݲ�����ȷ����
	GATHER_RESULT_MUST_EXIT=0x50,//��������
	GATHER_RESULT_ROUTER_EXCEPTION = 0x60,
	GATHER_RESULT_PARTIAL_SUCCEED=0x70,
	GATHER_RESULT_OTHER=0x80,//����
}GATHER_RESULT;

KLDump& operator<< (KLDump& dump, GATHER_RESULT v);

typedef enum _CHANNEL_TYPE
{
	CHANNEL_TYPE_UNKNOWN=0,
	CHANNEL_TYPE_GPRS_CDMA=2,
	CHANNEL_TYPE_PSTN=3,
	CHANNEL_TYPE_NET=4,
	CHANNEL_TYPE_COM=6,
	CHANNEL_TYPE_CSD=7,
	CHANNEL_TYPE_Radio=8,
	CHANNEL_TYPE_MESSAGE=9,

}CHANNEL_TYPE;

KLDump& operator<< (KLDump& dump, CHANNEL_TYPE v);
INT8U MAKE_CHANNEL_TYPE_VAL(const char* key);
int MAKE_CHANNEL_TYPE_KEY(INT8U val,char* oKey);


typedef enum _CHANNEL_STATUS
{
	CHANNEL_STATUS_FREE=0,
	CHANNEL_STATUS_CONNECTING=1,
	CHANNEL_STATUS_CONNECTED=2,
	CHANNEL_STATUS_DISCONNECTING=3
}CHANNEL_STATUS;

typedef enum 
{
	ROUTER_LEARN=1,
	ROUTER_READMETER=2,
	ROUTER_MAX
}T_emRouterWorkMode;

KLDump& operator<< (KLDump& dump, CHANNEL_STATUS v);

typedef enum _PARSE_RESULT
{
	PARSE_RESULT_INVALID_NO_HEADER_PACKAGE=-1,//������Ч֡(һ��֡ͷ������0x68��Ҳû��)
	PARSE_RESULT_INVALID_WITH_HEADER_PACKAGE=-2,//��һ��0x68����֡ͷ��������鷢���д�֡ͷ�����Ǳ�������Ч֡)
	PARSE_RESULT_ERROR_PACKAGE=-3,//����֡�����������Ч�ȡ�
	PARSE_RESULT_OK_PACKAGE=0,//��Ч֡������Ԥ�ڵ�֡
	PARSE_RESULT_PARTICAL_PACKAGE=1,//����ȫ֡
	PARSE_RESULT_NOT_EXPECTED_PACKAGE=2,//��Ч֡��������Ԥ�ڵ�֡
	PARSE_RESULT_RELAY_PACKAGE=3,//�м�֡
	PARSE_RESULT_OK_EXPECTED_PACKAGE,//��Ч֡�����һ��к���֡
	PARSE_RESULT_TIMEOUT,
	PARSE_RESULT_OK_TIMEOUT,
	PARSE_RESULT_COMMFAILURE,
	PARSE_RESULT_DATAID_NOT_EXIST
}PARSE_RESULT;

typedef enum _IPC_INFO_TYP
{
	IPC_INFO_TYPE_UNKNOWN=0,
	IPC_INFO_TYPE_CHANNEL_CHANGED=1,//ͨ������
	IPC_INFO_TYPE_WIRELESS_SIGNAL_PUBLISH=2,//�����ź�ǿ�ȷ���
	IPC_INFO_TYPE_PPP_ONLINE_REQUEST=3,//����PPP���������δ�����Ļ���
	IPC_INFO_TYPE_PPP_OFFLINE_REQUEST=4,//����PPP����������ѽ����Ļ���
	IPC_INFO_TYPE_PPP_REONLINE_REQUEST=5,//����PPP�������ٽ���
	IPC_INFO_TYPE_PPP_CHECK_REQUEST=6,	//����PPP���
	IPC_INFO_TYPE_JILIAN_DOWN=7,//����ͨ�����б���
	IPC_INFO_TYPE_JILIAN_UP=8,//����ͨ�����ͱ���
	IPC_INFO_TYPE_LCD_CMD_REQUEST=16,//Һ�����̷��͸���������
	IPC_INFO_TYPE_LCD_CMD_REPLY=17,//�������̷��ظ�Һ������
}IPC_INFO_TYP;

typedef enum _DATA_GET_METHOD
{
	DATA_GET_METHOD_SNAP=0,//����
	DATA_GET_METHOD_DIFF=1,//��ֵ

	DATA_GET_METHOD_MAX_VAL=DATA_GET_METHOD_DIFF,
}DATA_GET_METHOD;
KLDump& operator<< (KLDump& dump, DATA_GET_METHOD v);
INT8U MAKE_DATA_GET_METHOD_VAL(const char* key);
int MAKE_DATA_GET_METHOD_KEY(INT8U val,char* oKey);

typedef enum _DATA_FIX_METHOD
{
	DATA_FIX_METHOD_CURHD=0,//���Ե�ǰʱ���ж����ʵ����ݽ��в���
	DATA_FIX_METHOD_NONE=1,//�����в���
	DATA_FIX_METHOD_ALL=2,//ǿ�Ʋ���
	DATA_FIX_METHOD_CUSTOM=3,//�Զ���
}DATA_FIX_METHOD;
KLDump& operator<< (KLDump& dump, DATA_FIX_METHOD v);
INT8U MAKE_DATA_FIX_METHOD_VAL(const char* key);
int MAKE_DATA_FIX_METHOD_KEY(INT8U val,char* oKey);


typedef enum _DATA_PERIOD
{
	DATA_PERIOD_CUR=0,//��ǰʱ��
	DATA_PERIOD_PRE=1,//��ʱ��
	DATA_PREIOD_PREPRE=2,//����ʱ��
}DATA_PERIOD;
KLDump& operator<< (KLDump& dump, DATA_PERIOD v);
INT8U MAKE_DATA_PERIOD_VAL(const char* key);
int MAKE_DATA_PERIOD_KEY(INT8U val,char* oKey);

typedef enum _RECORD_DATA_FLAG
{
	RECORD_DATA_FLAG_NORMAL=0,
	RECORD_DATA_FLAG_GUESSED=1,
}RECORD_DATA_FLAG;

typedef enum _XC_PROTOCOL
{
	XC_PROTOCOL_N6_N12=1,
	XC_PROTOCOL_N12=2
}T_emXC_PROTOCOL;

//���ݡ�EAC5000B��������ű��е��¼���Ž��и�ֵ
typedef enum _NOTIFY_TYPE
{
	NOTIFY_TYPE_UNKNOWN=0xFFFFFFFF,


	//װ���¼�

	NOTIFY_TYPE_DEVICE_SHUTDOWN=0x00000000,//ϵͳ�ػ�
	NOTIFY_TYPE_DEVICE_START=0x00000001,//ϵͳ����
	NOTIFY_TYPE_DEVICE_SUSPEND=0x00000002,//ϵͳ����

	NOTIFY_TYPE_DEVICE_TIME_CHANGED=0x00000010,//ʱ��ı�
	NOTIFY_TYPE_DEVICE_VERSION_CHANGED=0x00000011,//�汾�ı�
	NOTIFY_TYPE_DEVICE_PARAM_CHANGED=0x00000012,//�����ı�

	NOTIFY_TYPE_DEVICE_HARDWARE_RESET=0x00000020,//Ӳ����λ
	NOTIFY_TYPE_DEVICE_SOFTWARE_RESET=0x00000021,//�����λ
	NOTIFY_TYPE_DEVICE_MEM_RESET=0x00000022,//�ڴ渴λ
	NOTIFY_TYPE_DEVICE_DATA_RESET=0x00000023,//���ݸ�λ
	NOTIFY_TYPE_DEVICE_PARAM_RESET=0x00000024,//������λ

	NOTIFY_TYPE_DEVICE_DATA_LOST=0x00000030,//���ݵ�ʧ

	NOTIFY_TYPE_DEVICE_POWEROFF_HAPPEN=0x00004000,//�ϵ緢����������
	NOTIFY_TYPE_DEVICE_POWEROFF_RESUME=0x00008000,//�ϵ�ָ���������


	//����¼�
	NOTIFY_TYPE_METER_TIME_CHANGED=0x10000000,//ʱ�Ӹı�
	NOTIFY_TYPE_METER_PROGRAM=0x10000001,//���

	NOTIFY_TYPE_METER_COMMTROUBLE_HAPPEN=0x10004000,//ͨѶ���Ϸ���
	NOTIFY_TYPE_METER_COMMTROUBLE_RESUME=0x10008000,//ͨѶ���ϻָ�

	NOTIFY_TYPE_METER_TIMEUNNORMAL_HAPPEN=0x10004001,//ʱ���쳣����
	NOTIFY_TYPE_METER_TIMEUNNORMAL_RESUME=0x10008001,//ʱ���쳣�ָ�

	NOTIFY_TYPE_METER_ZGLYX_LOWER_HAPPEN=0x1000400D,//�ܹ������ص�����ֵ����
	NOTIFY_TYPE_METER_ZGLYX_LOWER_RESUME=0x1000800D,//�ܹ������ص�����ֵ�ָ�

	NOTIFY_TYPE_METER_BATTERYLOWVOL_HAPPEN=0x10004002,//���Ƿѹ����
	NOTIFY_TYPE_METER_BATTERYLOWVOL_RESUME=0x10008002,//���Ƿѹ�ָ�

	NOTIFY_TYPE_METER_OVERVOL_HAPPEN=0x10004800,//�����ѹ����
	NOTIFY_TYPE_METER_OVERVOL_RESUME=0x10008800,//�����ѹ�ָ�
	NOTIFY_TYPE_METER_AOVERVOL_HAPPEN=0x10005800,//A���ѹ����
	NOTIFY_TYPE_METER_AOVERVOL_RESUME=0x10009800,//A���ѹ�ָ�
	NOTIFY_TYPE_METER_BOVERVOL_HAPPEN=0x10006800,//B���ѹ����
	NOTIFY_TYPE_METER_BOVERVOL_RESUME=0x1000A800,//B���ѹ�ָ�
	NOTIFY_TYPE_METER_COVERVOL_HAPPEN=0x10007800,//C���ѹ����
	NOTIFY_TYPE_METER_COVERVOL_RESUME=0x1000B800,//C���ѹ�ָ�

	NOTIFY_TYPE_METER_OVERCUR_HAPPEN=0x10004801,//�����������(�ݲ�֧��)
	NOTIFY_TYPE_METER_OVERCUR_RESUME=0x10008801,//��������ָ�(�ݲ�֧��)
	NOTIFY_TYPE_METER_AOVERCUR_HAPPEN=0x10005801,//A���������(�ݲ�֧��)
	NOTIFY_TYPE_METER_AOVERCUR_RESUME=0x10009801,//A������ָ�(�ݲ�֧��)
	NOTIFY_TYPE_METER_BOVERCUR_HAPPEN=0x10006801,//B���������(�ݲ�֧��)
	NOTIFY_TYPE_METER_BOVERCUR_RESUME=0x1000A801,//B������ָ�(�ݲ�֧��)
	NOTIFY_TYPE_METER_COVERCUR_HAPPEN=0x10007801,//C���������(�ݲ�֧��)
	NOTIFY_TYPE_METER_COVERCUR_RESUME=0x1000B801,//C������ָ�(�ݲ�֧��)

	NOTIFY_TYPE_METER_CUTVOL_HAPPEN=0x10004803,//������෢��
	NOTIFY_TYPE_METER_CUTVOL_RESUME=0x10008803,//�������ָ�
	NOTIFY_TYPE_METER_ACUTVOL_HAPPEN=0x10005803,//A����෢��
	NOTIFY_TYPE_METER_ACUTVOL_RESUME=0x10009803,//A�����ָ�
	NOTIFY_TYPE_METER_BCUTVOL_HAPPEN=0x10006803,//B����෢��
	NOTIFY_TYPE_METER_BCUTVOL_RESUME=0x1000A803,//B�����ָ�
	NOTIFY_TYPE_METER_CCUTVOL_HAPPEN=0x10007803,//C����෢��
	NOTIFY_TYPE_METER_CCUTVOL_RESUME=0x1000B803,//C�����ָ�


}NOTIFY_TYPE;


//��Դ
typedef enum _POWER_SOURCE
{
	POWER_SOURCE_NORMAL=0,//������������
	POWER_SOURCE_BATTERY=1,//��ع���

	POWER_SOURCE_MAX_VAL=POWER_SOURCE_BATTERY
}POWER_SOURCE;

KLDump& operator<< (KLDump& dump, POWER_SOURCE v);
int MAKE_POWER_SOURCE_KEY(INT8U val,char* oKey);

//PPP������
typedef enum _PPP_NEED_LEVEL
{
	PPP_NEED_LEVEL_NONE=0,//����ҪPPP
	PPP_NEED_LEVEL_NEED=1,//��ҪPPP
	PPP_NEED_LEVEL_FORBID=2,//������PPP
}PPP_NEED_LEVEL;

KLDump& operator<< (KLDump& dump, PPP_NEED_LEVEL v);
int MAKE_PPP_NEED_LEVEL_KEY(INT8U val,char* oKey);
INT8U MAKE_PPP_NEED_LEVEL_VAL(const char* key);

typedef enum _NET_CONNECT_TYPE
{
	NET_CONNECT_TYPE_TCP_CLIENT=0,
	NET_CONNECT_TYPE_TCP_SERVER=1,
	NET_CONNECT_TYPE_UDP=2,
}NET_CONNECT_TYPE;

INT8U MAKE_NET_CONNECT_TYPE_VAL(const char* key);
int MAKE_NET_CONNECT_TYPE_KEY(INT8U val,char* oKey);
KLDump& operator<< (KLDump& dump, NET_CONNECT_TYPE v);

typedef enum _NET_TYPE
{
	NET_TYPE_LAN=0,
	NET_TYPE_GPRS=1,
	NET_TYPE_CDMA=2,
}NET_TYPE;


typedef enum
{
	GATHER_PORT_GATHER = 0,
	GATHER_PORT_JILIAN = 1,	
	GATHER_PORT_BE_GATHERED = 2
}GATHER_PORT_TYPE;

INT8U MAKE_NET_TYPE_VAL(const char* key);
int MAKE_NET_TYPE_KEY(INT8U val,char* oKey);
KLDump& operator<< (KLDump& dump, NET_TYPE v);

NET_TYPE GetWirelessType(int wirelessModemType);

//PSTN���ŷ�ʽ
typedef enum _PSTN_DIAL_METHOD
{
	PSTN_DIAL_METHOD_NORMAL=0,//�������ŷ�ʽ
	PSTN_DIAL_METHOD_TWICE=1,//���β��ŷ�ʽ
}PSTN_DIAL_METHOD;

INT8U MAKE_PSTN_DIAL_METHOD_VAL(const char* key);
int MAKE_PSTN_DIAL_METHOD_KEY(INT8U val,char* oKey);
KLDump& operator<< (KLDump& dump, PSTN_DIAL_METHOD v);

//PSTN��Դ��ʽ
typedef enum _PSTN_POWER_METHOD
{
	PSTN_POWER_METHOD_ALWAYS_ON=0,//���ϵ緽ʽ
	PSTN_POWER_METHOD_RING1_ON=1,//һ�������ϵ緽ʽ
	PSTN_POWER_METHOD_RING2_ON=2,//���������ϵ緽ʽ
	PSTN_POWER_METHOD_RING3_ON=3,//���������ϵ緽ʽ
}PSTN_POWER_METHOD;

INT8U MAKE_PSTN_POWER_METHOD_VAL(const char* key);
int MAKE_PSTN_POWER_METHOD_KEY(INT8U val,char* oKey);
KLDump& operator<< (KLDump& dump, PSTN_POWER_METHOD v);

typedef enum _ACTION_TYPE
{
	ACTION_TYPE_NONE=0,//ʲôҲ����
	ACTION_TYPE_METERTIMEALARM=1,//���ʱ���쳣����
	ACTION_TYPE_DEVICETIMEALARM=2,//ֻ�жϼ������Ƿ�ʱ���쳣

}ACTION_TYPE;

KLDump& operator<< (KLDump& dump, ACTION_TYPE v);
int MAKE_ACTION_TYPE_KEY(INT8U val,char* oKey);
INT8U MAKE_ACTION_TYPE_VAL(const char* key);
bool IsValidActionType(INT8U actionType);


typedef enum _COMMAND_TYPE
{
	COMMAND_UNKNOWN=0,//δ֪
	COMMAND_TYPE_RELAY_GATHER=1,//�м̲ɼ�
	COMMAND_TYPE_BROADCAST_GATHER=2,//�㲥�ɼ�
	COMMAND_TYPE_INSTANT_GATHER=3,//ʵʱ�ɼ�
}COMMAND_TYPE;

KLDump& operator<< (KLDump& dump, COMMAND_TYPE v);
int MAKE_COMMAND_TYPE_KEY(INT8U val,char* oKey);
INT8U MAKE_COMMAND_TYPE_VAL(const char* key);

//ѹ����ʽ
typedef enum _COMPRESS_METHOD
{
	COMPRESS_METHOD_NONE=0,//��ѹ��
}COMPRESS_METHOD;

INT8U MAKE_COMPRESS_METHOD_VAL(const char* key);
int MAKE_COMPRESS_METHOD_KEY(INT8U val,char* oKey);
KLDump& operator<< (KLDump& dump, COMPRESS_METHOD v);

//���ܷ�ʽ
typedef enum _ENCRYPT_METHOD
{
	ENCRYPT_METHOD_NONE=0,//�޼���
}ENCRYPT_METHOD;

typedef enum
{
	STATE_RESET=1,
	STATE_PAUSE,
	STATE_RESUME
}T_emRouterState;

//�������״̬�ֶ���
typedef struct _METER_RUN_STATUS_T
{
	INT8U  Curr_lxx:1;
	INT8U  Vol_lxx:1;	
	INT8U  Curr_bph:1;
	INT8U  Vol_bph:1;
	INT8U  clock:1;
	INT8U  Power_loss:1;
	INT8U  Demand_cx;
	INT8U  relay_stat:1;


	INT8U  A_sy:1;
	INT8U  A_qy:1;
	INT8U  A_gy:1;
	INT8U  A_sl:1;
	INT8U  A_gl:1;
	INT8U  A_gz:1;
	INT8U  A_clfx:1;
	INT8U  A_dx:1;

	INT8U  B_sy:1;
	INT8U  B_qy:1;
	INT8U  B_gy:1;
	INT8U  B_sl:1;
	INT8U  B_gl:1;
	INT8U  B_gz:1;
	INT8U  B_clfx:1;
	INT8U  B_dx:1;

	INT8U  C_sy:1;
	INT8U  C_qy:1;
	INT8U  C_gy:1;
	INT8U  C_sl:1;
	INT8U  C_gl:1;
	INT8U  C_gz:1;
	INT8U  C_clfx:1;
	INT8U  C_dx:1;

}METER_STAT;

INT8U MAKE_ENCRYPT_METHOD_VAL(const char* key);
int MAKE_ENCRYPT_METHOD_KEY(INT8U val,char* oKey);
KLDump& operator<< (KLDump& dump, ENCRYPT_METHOD v);


int GetExistPathFile(const char* path,const char* fileName,char* opPathFile);
int GetDynamicCfgPathFile(const char* path,const char* baseName,const char* postfix,char* opFileName,char* opPathFile);

//�Ա��ʱ��ʽ��Ϣ
struct MeterSetTimeMethodInfo
{
	INT32U SuppportMask;//֧�ֵĶ�ʱ��ʽ
	INT8U DefaultMethod;//Ĭ�ϵĶ�ʱ��ʽ

	INT8U GetTrueMethod(INT8U settingMethod);
	
};


class INT16UIDS:public vector<INT16U>
{
public:
	KLDump& Dump(KLDump& dump) const;
};

class INT32UIDS:public vector<INT32U>
{
public:
	KLDump& Dump(KLDump& dump) const;
};

typedef enum
{
	SEASON_SUMMER=1,
	SEASON_WINNER=2
}T_emSeason;

struct S_IpConfig
{
	unsigned char m_TermIP[4];
	unsigned char m_NetMask[4];
	unsigned char m_GateWay[4];
	unsigned char m_MAC[32];
	unsigned char m_IP_eth1[4];
	unsigned char m_NetMask_eth1[4];
	unsigned char m_GateWay_eth1[4];
	unsigned char m_MAC_eth1[32];
	
	unsigned char m_ProxyIP[4];
	unsigned char m_ProxyType;
	unsigned char m_ProxyLink;
	unsigned short m_ProxyPort;
	unsigned char m_UserName[24];
	unsigned char m_Password[24];

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct SSmartGarageItem
{
	INT8U m_DeviceNo;
	INT8U m_SubNo;
	INT8U m_Function;
	INT8U m_FuncData[4];
};

struct SSmartGarages
{
	SSmartGarageItem m_smartGarageItem;
};

#endif


