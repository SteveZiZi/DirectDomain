///*Config.h�а������õĶ���*/

#ifndef INCLUDED_CONFIG
#define INCLUDED_CONFIG

#include "StdInclude.h"

#define UPDATE_DISABLE 0x00
#define UPDATE_AUTOROUTE 0x01
#define UPDATE_DELRELAY 0x02
#define UPDATE_ADDRELAY 0x04
#define UPDATE_MDFRELAY 0x08
#define UPDATE_AJUSTROUTE 0x10

//************��ϵͳ�ѷ�����ÿ�θ�����ע����°汾���Լ���������***********************
#define CONS_MAJOR_VERSION	1						//���汾��
#define CONS_MINOR_VERSION	0						//�ΰ汾��
#define CONS_FIX_VERSION	3						//�����汾��
#define CONS_RELEASE_DATE	"2016-4-05"			//��������
#define CONS_SOFT_VERSION "DCUS-001-003-610000"		//����汾��
//***************************************************************************************


//********************************ϵͳ���ã������ֳ�����*********************************
#if defined(DEV_6100)
	#define CONS_SYSTEM_NAME	"GD_DJS"		//ϵͳ����
	#define CONS_SYSTEM_MODEL	"GD_DJS"		//ϵͳ�ͺ�
	#define CONS_SYSTEM_MODEL_LOWER	"djs"		//ϵͳ�ͺţ�Сд��
	#define CONS_HTTX_MAX_COUNT		3			//���ĺ�̨ͨѶ������Ŀ
	#define CONS_METER_MAX_COUNT	32		//���ĵ����Ŀ������Ŵ�0��ʼ
	#define CONS_METER_COUNT_NO_VIP	4		//��ȥ�ص��û�����������Ŀ
	#define CONS_VIP_MAX_COUNT		6			//�ص㻧�û�����
	#define CONS_JILIAN_MAX_COUNT	1			//���ļ���������Ŀ
	#define CONS_RELAY_MAX_COUNT	1			//һ����������֧�ֵ����ɼ��ն���
	#define CONS_CLRELAY_MAX_COUNT	16+1		//����·�ն���
	#define CONS_ROUTER_MAX_COUNT	1			//255//һ����������֧�ֵ����·����//��ΰ01.04��
	#define CONS_ROUTER_ITEM_MAX_COUNT 3		//���·��ʱ����
#elif defined(WIN32)
	#define CONS_SYSTEM_NAME	"GD_DJS"	//ϵͳ����
	#define CONS_SYSTEM_MODEL	"GD_DJS"	//ϵͳ�ͺ�
	#define CONS_SYSTEM_MODEL_LOWER	"djs"	//ϵͳ�ͺţ�Сд��
	#define CONS_HTTX_MAX_COUNT		3		//���ĺ�̨ͨѶ������Ŀ
	#define CONS_METER_MAX_COUNT	32	//���ĵ����Ŀ������Ŵ�0��ʼ
	#define CONS_METER_COUNT_NO_VIP	4	//��ȥ�ص��û�����������Ŀ
	#define CONS_VIP_MAX_COUNT		1		//�ص㻧�û�����
	#define CONS_JILIAN_MAX_COUNT	1		//���ļ���������Ŀ
    #define CONS_RELAY_MAX_COUNT	1		//һ����������֧�ֵ����ɼ��ն���
	#define CONS_CLRELAY_MAX_COUNT	16+1		//����·�ն���
    #define CONS_ROUTER_MAX_COUNT	1		//һ����������֧�ֵ����·����
	#define CONS_ROUTER_ITEM_MAX_COUNT 3		//���·��ʱ����
#else
	#error "Need apply define"
#endif
#define CONS_GATHERELOG_MAX_COUNT		15	//�ɼ���־�ĳ���
#define CONS_HTTX_CHANNEL_MAX_COUNT		6	//ÿ����̨ͨѶ��������ͨ����Ŀ
#define CONS_TASK_MAX_COUNT		16	//����������Ŀ�������Ŵ�0��ʼ
#define CONS_ACTION_MAX_COUNT 16	//���Ĳ�����Ŀ
#define CONS_ZBSNF_TIME_SECTION_MAX_COUNT 24 //�ز��������ɷֵ�����ʱ�����

#define CONS_HTTX_CONFIG_PLAN_COUNT	16	//���ĺ�̨ͨѶ���÷�����Ŀ
#define CONS_DEVICE_RULE_OPTION_MAX_LEN		32	//װ�ù�Լѡ�������ֽ���

#define CONS_REG_MAX_OBJ_COUNT CONS_METER_MAX_COUNT
#define CONS_REG_ID_BLOCK_COUNT 40	//�����Ĵ���ÿ��Ĵ�С���������Ĵ����ļ����г�ʼ��ʱ�����ݴ�ֵÿһ��һ������
#define CONS_REG_ID_MAX_COUNT	120	//���ĵ�������Ĵ���ID��
#define CONS_DEVICE_RULE_MAX_COUNT	4	//����װ�ù�Լ��
#define CONS_METER_RULE_MAX_COUNT	16	//���ĵ���Լ��
#define CONS_DEFAULT_NET_IP	"192.168.8.222"
#define CONS_CHANNEL_CHECK_INTERS	5	//ͨ�����ļ����
#define CONS_DEFAULT_PSTN_MODEM_CHECK_INTERS	60	//Ĭ�ϵ�PSTN����MODEM�Լ���
#define CONS_IEC102_IOA_MAP_BLOCK_MAX_COUNT	32	//����IEC102��Լ���ͬʱ֧��32����ͬ��ӳ������

//********************************�������ã�һ�㲻����***************************************
#define CONS_DAEMON_KEY "daemon"
#define CONS_DBCJ_KEY "dbcj"
#define CONS_HTTX_KEY "httx"
#define CONS_LCD_KEY  "lcd"
#define CONS_PPP_KEY "ppp"
#define CONS_JILIAN_KEY "jilian"	//2006-6-10�����������Ӽ���ͨ�ŷ������
#define CONS_SCANF_KEY "scanf"

#define CONS_START_COMMAND "-start"
#define CONS_EXEC_COMMAND "-exec"
#define CONS_STOP_COMMAND "-stop"
#define CONS_SET_COMMAND "-set"
#define CONS_LIST_COMMAND "-list"
#define CONS_ACTION_COMMAND "-action"
#define CONS_SAVE_COMMAND "-save"
#define	CONS_DEBUG_COMMAND "-debug"

#define CONS_REBOOT_COMMAND "reboot"
#define CONS_SYNC_COMMAND "sync"

#define PPP_DEV_NAME "ppp0"
//#define NET_DEV_NAME "eth0"
#define NET_DEV_NAME "eth1" //�ͳɱ�������ʹ�õ���USBת����

#define CONS_UPDATE_PACKAGE_BACKUP_DAYS 7			//�������ı���ʱ������λ����
#define CONS_MAIN_UPDATE_BACKUP_PACKAGE	"/mnt/old_main.tar.gz"		//ini���������ļ�
#define CONS_SYS_UPDATE_BACKUP_PACKAGE "/mnt/old_sys.tar.gz"		//ϵͳ���������ļ�
#define CONS_MAIN_UPDATE_PACKAGE	".tar.gz"		//��������
#define CONS_SYS_UPDATE_PACKAGE		"sys.tar.gz"	//ϵͳ������
#define CONS_EXEC_UPDATE_PACKAGE	".exec"			//�����ļ���

#define CONS_DEFAULT_KEY "[Default]"

#define CONS_REG_EXPIRE_CHECK_OFFSET_MINUTES	60	//�����Ĵ���ʧЧ���ʱ��ƫ�Ʒ���
#define CONS_DEFAULT_METER_PREFIX_CHAR	0xFE		//Ĭ�ϵĵ��ǰ���ַ�

#define CONS_DBCJ_HEART_BEAT_INTER_S 150			//���ͨѶ�����������
#define CONS_HTTX_HEART_BEAT_INTER_S 60				//��̨ͨѶ�����������
//#define CONS_WORK_HEART_BEAT_INTER_S 120			//���������������
#define CONS_PPP_HEART_BEAT_INTER_S 60				//PPP�����������
#define CONS_JILIAN_HEART_BEAT_INTER_S 120			//���������������
#define CONS_ROUTER_HEART_BEAT_INTER_S 120			//�Զ�·�ɽ����������

#define CONS_METER_ADDR_MAX_LEN 12			//����ַ����ַ���
#define CONS_METER_ADDR_LEN 6				//����ַ���� 
#define CONS_METER_USER_MAX_LEN 12			//����û�������ַ���
#define CONS_METER_PWD_MAX_LEN 12			//�����������ַ���
#define CONS_METER_SETTING_KEY_MAX_LEN 32	//������ùؼ�������ַ���
#define CONS_CONFIG_PLAN_KEY_MAX_LEN 16		//���÷����ؼ�������ַ���
#define CONS_CONFIG_PLAN_NAME_MAX_LEN 32	//���÷�����������ַ���

#define CONS_FILE_NAME_MAX_LEN	64	//�ļ�������ַ���
#define CONS_PATH_FILE_MAX_LEN	255	//·���ļ�����ַ���
#define CONS_SERVER_NAME_MAX_LEN	8	//�������������ַ���
#define CONS_RULE_NAME_MAX_LEN	24	//��Լ������ַ���
#define CONS_CHANNEL_NAME_MAX_LEN	16	//ͨ��������ַ���
#define CONS_CHANNEL_SETTING_MAX_LEN	16	//ͨ����������ַ���
#define CONS_CHANNEL_TAG_MAX_LEN	16	//ͨ��������Ϣ����ַ���
#define CONS_ACTION_KEY_MAX_LEN	32	//�����ؼ�������ַ���
#define CONS_ACTION_NAME_MAX_LEN	32	//������������ַ���
#define CONS_USER_CODE_MAX_LEN	64	//�û���������ַ���������SysConfig��
#define CONS_MODEM_INIT_CMD_MAX_LEN 128	//MODEM��ʼ����������ַ���
#define CONS_PPP_APN_MAX_LEN 32	//APN����ַ���
#define CONS_PPP_TELENO_MAX_LEN 32	//PPP����ʱ�绰��������ַ���
#define CONS_PPP_USER_MAX_LEN 32	//PPP����ʱ�û�����ַ���
#define CONS_PPP_PWD_MAX_LEN 32	//PPP����ʱ��������ַ���

#define CONS_PACK_SEND_DEFAULT_TIME_OUT_SECOND	15	//Ĭ�ϵ����ݰ����ͳ�ʱֵ���룩

//suxufeng:[2010-3-25]��ϵͳ���ڴ�Ƚϴ󣬿��Է���϶�Ŀռ����Ϣ����
#define CONS_MESSAGE_BUFFER_LEN	1024*128 //��Ϣ�����ж���Ϣʱ�Ļ����ֽ���


#define CONS_SERVER_MAIN_INFO_MAX_LEN	64	//������Һ������ʾ������Ҫ��Ϣ�Ļ�������С
#define CONS_DEAULT_CHAR_READ_TIMEOUT_MS	50
#define CONS_DEAULT_RECV_TIMEOUT_MS	5000	
#define CONS_DEAULT_WRITE_TIMEOUT_MS	3000
#define CONS_DEAULT_READ_TIMEOUT_MS	3000
#define CONS_DEAULT_SUB_READ_TIMEOUT_MS	50
#define CONS_REG_DEFAULT_CHAR 0xFF
#define CONS_TASK_FILE_DEFAULT_CHAR 0xFF
#define CONS_TASK_FILE_DEFAULT_VAL_FORMAT	0x84	//����Ĭ�ϵ���ֵ�����ݱ���ĸ�ʽ
#define CONS_KEY_MAX_LEN 32	//�ؼ�������ַ���
#define CONS_ACTION_PARAM_MAX_LEN 255 //������������ֽ���
#define CONS_DEFAULT_TRACE_LEVEL TRACE_LEVEL_VERBOSE	//Ĭ�ϵĸ��ٵȼ�

#define CONS_SUSPEND_MAX_WAIT_SECOND 30 //�ȴ��ӽ���ȫ��������������
#define CONS_SYNC_MAX_WAIT_SECOND	90	//�ȴ�ͬ�������������
#define CONS_RESET_COMMAND_DELAY_SECOND 3 //��λ������ʱ����(������������Ž��и�λ����)


#define CONS_HARDWARE_DOG_DEAD_SECOND 15  //��û��ι��ʱ�䳬��������ʱӲ����������
#define CONS_SOFTWARE_RESET_DELAY_SECOND	3 //�����������ʱ����


#define CONS_SUSPEND_DELAY_SECOND	CONS_RESET_COMMAND_DELAY_SECOND	//�������������ϵͳ���й���
#define CONS_SYNC_DELAY_SECOND	(CONS_SUSPEND_DELAY_SECOND+CONS_SUSPEND_MAX_WAIT_SECOND) //�������������ϵͳ����ͬ��
#define CONS_HARDWARE_RESET_DELAY_SECOND (CONS_SYNC_DELAY_SECOND+CONS_SYNC_MAX_WAIT_SECOND)	//�������������ϵͳ���и�λ

#define CONS_DAEMON_ALRM_INTER_US 1000000
//#define CONS_WORK_ALRM_INTER_US 0
#define CONS_DBCJ_ALRM_INTER_US 2000000
#define CONS_HTTX_ALRM_INTER_US 0
#define CONS_PPP_ALRM_INTER_US 0
#define CONS_JILIAN_ALRM_INTER_US 1000000
#define CONS_ROUTER_ALRM_INTER_US 1000000

#define CONS_PPP_DEFAULT_PORT_NO 8	//PPP���Ų��õ�Ĭ�ϴ��ں�
#define CONS_PPP_DEFAULT_BAUD 19200	//PPP���Ų��õ�Ĭ�ϲ�����
#define CONS_PPP_DEFAULT_CHECK_INTER_M 720	//PPPĬ�ϵļ���������ӣ�
#define CONS_PPP_DEFAULT_CHECK_FAIL_TIMES 2	//PPPĬ�ϵ��������ʧ�ܶ��߷�ֵ
#define CONS_PPP_DEFAULT_MODEM_INIT_CMD "AT+CGDCONT=1,\"IP\",\"CMNET\"" //PPP��MODEM��ʼ������

#define CONS_PPP_FAILED_TIMES_FOR_MODEM_RESET 3		//���������˴���PPP����ʧ�ܣ�����MODEM���и�λ
#define CONS_MODEM_FAILED_TIMES_FOR_RESET 3		//���������˴���MODEM����ʧ�ܣ�����MODEM���и�λ
#define CONS_MODEM_UNNORMAL_CHECK_INTERS 5	//��Modem������ʱ��ÿ����ֵ�����м��
#define CONS_BATTERY_IO_BUS_NO	4	//���IO����������ߺ�
#define CONS_FREEZE_MAX_DELAY_MINUTES 30	//����ʱ�������ӳٶ���ķ�������ע������Ҳ�п����ж����ӳ����ã���ʱ���߶�ʹ���������Ƿ��ӳٶ��ᣩ


#define CONS_DEFAULT_EVENT_METER_TIME_MAX_DIFF_M 10	//Ĭ�ϵ��жϵ��ʱ���쳣�ķ�ֵ�����ӣ�
#define CONS_DEFAULT_EVENT_METER_ZGLYX_MIN_VAL 0.8f	//Ĭ�ϵ��ܹ���������ֵ������������С�ڴ�ֵʱ�������������������ڷ�ֵ����

#define CONS_DEFAULT_SET_METER_TIME_MAX_DIFF_M 10	//Ĭ�ϵĵ���ʱ��������ʱ����װ��ʱ���𳬹���ֵʱ��������ʱ
#define CONS_DEFAULT_SET_DEVICE_TIME_MAX_DIFF_M 1440	//Ĭ�ϵĵ���ʱ��������ʱ����װ��ʱ���𳬹���ֵʱ��������ʱ

#define CONS_FLOAT_PART_DIFF 0.00001

//2006-9-19���������ӱ�����¼���ȣ�����������ʹ��
#define CONS_DEFALT_ALARM_FILE_LENGTH	256
#define CONS_FAIL_ALARM_FILE_LENGTH		32
#define CONS_GATHER_FAIL_ALARM_DAYS		3 //������ʧ���������������Ҫ�ϱ�
#define CONS_SET_TIME_FAIL_ALARM_TIMES  3//������Уʱʧ�ܴ����ﵽ���Σ���Ҫ�ϱ�
#define CONS_GATHER_FAIL_ALARM_TIMES	10000 //��һ��ʧ���������������Ͳ����ٲ���
#define CONS_GATHER_FAIL_ALARM_HORURS	72 //�ϴγ���Ŀǰ��72Сʱ��������ʧ�ܸ澯

#define SHARE_MEM_LENGTH 256		//��λ �ֽ�	//��־ǿ �����ڴ��С 2006-7-25�����ۣ�����ʹ��bstring�洢�ṹ
									//�������飬�����Сʹ�ù̶���С��������־ǿ����Ĺ����ڴ��Сͳһʹ�������С
#define LOG_ON_TIME_OUT		30 //��λ���� ��½	
#define REAL_READ_TIME_OUT  4000 //��λ���� ʵʱ�в�	

//#define DBCJ_3_BUSRULE_GENERAL 1//ͨ����ţ��ز�ͨ��Ϊ1    //2006-10-12 ��־ǿ
//#define DBCJ_BUSNO_485 2//ͨ����ţ�485ͨ��Ϊ2   //2006-10-12 ��־ǿ

#define METER_CONNECTION_485 1//ͨѶ��ʽ��485    
#define METER_CONNECTION_ZB 3//ͨѶ��ʽ���ز�


/**************************���нṹ����*****************/
/** @brief �������� */
#define SYSTEM_SERIAL_PORT_MAX		32

enum  _METER_COMM_T
{
	DBCJ_1_BUSNO_485 = 1,
	DBCJ_2_BUSNO_485 =2 ,
	DBCJ_3_BUSRULE_GENERAL=3,
};


#define BUS_COUNT 3

enum _BUS_COMM_TYPE_T
{
	BUS_485 = 0,
	BUS_XIAOCHENGZB = 1,
	BUS_ROUBLE = 2,
	BUS_RISECOMM = 3,
	BUS_DINGXIN = 4,
	BUS_LIHE = 5,
	BUS_DLT698 = 6,
	BUS_RUBONET = 7,
	BUS_EASTSOFT = 8,
	BUS_DX = 9,
	BUS_UNKNOWN=0xFF,
};
/***************************************/
//��˹�����ڽ��еĺ�ʱ�Ĳ���
enum TRANS_T
{
	TRANS_INITIALIZE = 0x80,
	TRANS_SELFADAPT = 0x40,
	TRANS_SEEKLOSTNODE=0x20,
	TRANS_UNICAST     =0x10,
	TRANS_MUNUALCONFIGNET=0x08,
	TRANS_FLOODCLR      = 0x04,
	TRANS_PLCTEST = 0x02,
	TRANS_ENDADAPT = 0x01,
};

//�����Ĵ���״̬
enum TRANS_STATUS_T
{
	STATUS_BUSY = 0x01,
	STATUS_FINISHED = 0x00,
	STATUS_UNKNOWN = 0x03,
};

#define CONS_LCD_ALRM_INTER_US			0	
#define CONS_LCD_HEART_BEAT_INTER_S		30		// Һ�������������


/*********************************/

#define CONS_ROUTER_RELAY_MAX_COUNT  5 //���ɼ��ն˸�����1-5���ն˱�ʾ0-4��·��

#define CONS_RROUTER_RELAY_MAX_COUNT  9

#define CONS_MUST_EXIT_FLAG  255 //�����˳���������ֵ
#define CONS_MUST_ADAPTE_CHIP 254

#define CONS_DEFAULT_METER_ADDR  0x00	//����ַĬ�����ֵ

//#define CONS_SERVER_RESTART_TIMES 10	//�κ�һ������������������CONS_SERVER_RESTART_TIMES�������ÿ��Ź�����λ������

//********************************�������ã�һ�㲻����***************************************

//******************���з���Ƕ��ʽ�������С����ģ�����Ӧ�¼���װ�õ�����*******************
#define CONS_GPRS_RECONNECT_MAX_TIMES 15				//GPRS��������

#define CONS_UNVALID_METER_NUMBER 1207	//��Ч����

#define CONS_GDDYJC_DATA_LENGTH 9				//���ݳ��ȱ�־�����й�Լ֡�е�λ�ã����ڼ����������
//#define IPV6_OR_IPV4 6                           //��ǰʹ�õ��Ǽ�λ��IP��ַ
//#define WATCHDOGREBOOT    30//2006-11-16    ppp�������Դ�������������װ������

/*************************************���Լ�Ŀ�����****************************************/
#define MODE_READ   0x00
#define MODE_WRITE  0x01
#define MODE_LOGON 0x03
#define MODE_LOGOUT 0x04
#define MODE_PROAGRAM 0x05
#define MODE_READ_OUT  0x06
#define MODE_CHECK_PWD 0x07

/*******************************************************************************************/
//*****************************��������*************************************
#define DEBUG_INOUT          0x5354		//1����ʼ��������
#define DEBUG_STATICROUTER   0x5254		//2����̬·�ɵ���
#define DEBUG_CHECKRELAYCOMM 0x5254
#define DEBUG_LINKSTATE      0x4C4B		//3�������·����
#define DEBUG_GETNETSTATUS      0x5252	//������ģ������״̬
#define DEBUG_GETNETFAILEDNODES      0x5352   // ������ģ������ʧ�ܽڵ�

#define DEBUG_SENDPACKAGE    0x5054		//4����������
#define DEBUG_SNDMETERPACKGE 0x5054

#define DEBUG_CALLTEST	0x5055	// ��������
#define DEBUG_STARTROUTER    0x424E		//5������·��
#define DEBUG_CHECKLOSTRELAYS 0x424E

#define DEBUG_STOPROUTER      0x434E
#define DEBUG_ADJUSTROUTER   0x4152		//6������·��
#define DEBUG_SETZBSNIFFER   0x6053		//7�������ز��������
#define DEBUG_CONTROL_CODE	 0x31		//�������������
#define DEBUG_NOMAL_REPLY_CODE 0xB1		//��������������Ӧ������
#define DEBUG_ABNOMAL_REPLY_CODE 0xF1	//���������쳣��Ӧ������



enum ProtolcolEnum
{
	Protolcol_A=1,
	Protolcol_B=2,
	Protolcol_C=3,
	Protolcol_D=4,
	Protolcol_E=5,
	Protolcol_Unknown=-1,
};

enum _Operation_Code
{
	 OP_ADD = 1,
	 OP_DEL = 2,
	 OP_MOD = 3,
	 OP_UNKNOW=-1,
};
#endif
