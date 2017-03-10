///*Config.h中包括配置的定义*/

#ifndef INCLUDED_CONFIG
#define INCLUDED_CONFIG

#include "StdInclude.h"

#define UPDATE_DISABLE 0x00
#define UPDATE_AUTOROUTE 0x01
#define UPDATE_DELRELAY 0x02
#define UPDATE_ADDRELAY 0x04
#define UPDATE_MDFRELAY 0x08
#define UPDATE_AJUSTROUTE 0x10

//************当系统已发布后，每次更改请注意更新版本号以及发布日期***********************
#define CONS_MAJOR_VERSION	1						//主版本号
#define CONS_MINOR_VERSION	0						//次版本号
#define CONS_FIX_VERSION	3						//修正版本号
#define CONS_RELEASE_DATE	"2016-4-05"			//发布日期
#define CONS_SOFT_VERSION "DCUS-001-003-610000"		//软件版本号
//***************************************************************************************


//********************************系统配置，根据现场更改*********************************
#if defined(DEV_6100)
	#define CONS_SYSTEM_NAME	"GD_DJS"		//系统名称
	#define CONS_SYSTEM_MODEL	"GD_DJS"		//系统型号
	#define CONS_SYSTEM_MODEL_LOWER	"djs"		//系统型号（小写）
	#define CONS_HTTX_MAX_COUNT		3			//最大的后台通讯进程数目
	#define CONS_METER_MAX_COUNT	32		//最大的电表数目，电表编号从0开始
	#define CONS_METER_COUNT_NO_VIP	4		//除去重点用户表的最大电表数目
	#define CONS_VIP_MAX_COUNT		6			//重点户用户个数
	#define CONS_JILIAN_MAX_COUNT	1			//最大的级联进程数目
	#define CONS_RELAY_MAX_COUNT	1			//一个集中器可支持的最大采集终端数
	#define CONS_CLRELAY_MAX_COUNT	16+1		//最大回路终端数
	#define CONS_ROUTER_MAX_COUNT	1			//255//一个集中器可支持的最大路由数//刘伟01.04改
	#define CONS_ROUTER_ITEM_MAX_COUNT 3		//最大路由时段数
#elif defined(WIN32)
	#define CONS_SYSTEM_NAME	"GD_DJS"	//系统名称
	#define CONS_SYSTEM_MODEL	"GD_DJS"	//系统型号
	#define CONS_SYSTEM_MODEL_LOWER	"djs"	//系统型号（小写）
	#define CONS_HTTX_MAX_COUNT		3		//最大的后台通讯进程数目
	#define CONS_METER_MAX_COUNT	32	//最大的电表数目，电表编号从0开始
	#define CONS_METER_COUNT_NO_VIP	4	//除去重点用户表的最大电表数目
	#define CONS_VIP_MAX_COUNT		1		//重点户用户个数
	#define CONS_JILIAN_MAX_COUNT	1		//最大的级联进程数目
    #define CONS_RELAY_MAX_COUNT	1		//一个集中器可支持的最大采集终端数
	#define CONS_CLRELAY_MAX_COUNT	16+1		//最大回路终端数
    #define CONS_ROUTER_MAX_COUNT	1		//一个集中器可支持的最大路由数
	#define CONS_ROUTER_ITEM_MAX_COUNT 3		//最大路由时段数
#else
	#error "Need apply define"
#endif
#define CONS_GATHERELOG_MAX_COUNT		15	//采集日志的长度
#define CONS_HTTX_CHANNEL_MAX_COUNT		6	//每个后台通讯进程最大的通道数目
#define CONS_TASK_MAX_COUNT		16	//最大的任务数目，任务编号从0开始
#define CONS_ACTION_MAX_COUNT 16	//最大的操作数目
#define CONS_ZBSNF_TIME_SECTION_MAX_COUNT 24 //载波侦测任务可分的最多的时间段数

#define CONS_HTTX_CONFIG_PLAN_COUNT	16	//最大的后台通讯配置方案数目
#define CONS_DEVICE_RULE_OPTION_MAX_LEN		32	//装置规约选项的最大字节数

#define CONS_REG_MAX_OBJ_COUNT CONS_METER_MAX_COUNT
#define CONS_REG_ID_BLOCK_COUNT 40	//能量寄存器每块的大小，对能量寄存器文件进行初始化时将根据此值每一批一批进行
#define CONS_REG_ID_MAX_COUNT	120	//最大的电表能量寄存器ID数
#define CONS_DEVICE_RULE_MAX_COUNT	4	//最大的装置规约数
#define CONS_METER_RULE_MAX_COUNT	16	//最大的电表规约数
#define CONS_DEFAULT_NET_IP	"192.168.8.222"
#define CONS_CHANNEL_CHECK_INTERS	5	//通道更改检查间隔
#define CONS_DEFAULT_PSTN_MODEM_CHECK_INTERS	60	//默认的PSTN拨号MODEM自检间隔
#define CONS_IEC102_IOA_MAP_BLOCK_MAX_COUNT	32	//对于IEC102规约最多同时支持32个不同的映射配置

//********************************其它配置，一般不更改***************************************
#define CONS_DAEMON_KEY "daemon"
#define CONS_DBCJ_KEY "dbcj"
#define CONS_HTTX_KEY "httx"
#define CONS_LCD_KEY  "lcd"
#define CONS_PPP_KEY "ppp"
#define CONS_JILIAN_KEY "jilian"	//2006-6-10，由苏煦烽添加级联通信服务进程
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
#define NET_DEV_NAME "eth1" //低成本集中器使用的是USB转网口

#define CONS_UPDATE_PACKAGE_BACKUP_DAYS 7			//升级包的备份时长，单位：天
#define CONS_MAIN_UPDATE_BACKUP_PACKAGE	"/mnt/old_main.tar.gz"		//ini升级备份文件
#define CONS_SYS_UPDATE_BACKUP_PACKAGE "/mnt/old_sys.tar.gz"		//系统升级备份文件
#define CONS_MAIN_UPDATE_PACKAGE	".tar.gz"		//主升级包
#define CONS_SYS_UPDATE_PACKAGE		"sys.tar.gz"	//系统升级包
#define CONS_EXEC_UPDATE_PACKAGE	".exec"			//命令文件包

#define CONS_DEFAULT_KEY "[Default]"

#define CONS_REG_EXPIRE_CHECK_OFFSET_MINUTES	60	//能量寄存器失效检查时间偏移分钟
#define CONS_DEFAULT_METER_PREFIX_CHAR	0xFE		//默认的电表前导字符

#define CONS_DBCJ_HEART_BEAT_INTER_S 150			//电表通讯进程心跳间隔
#define CONS_HTTX_HEART_BEAT_INTER_S 60				//后台通讯进程心跳间隔
//#define CONS_WORK_HEART_BEAT_INTER_S 120			//工作进程心跳间隔
#define CONS_PPP_HEART_BEAT_INTER_S 60				//PPP进程心跳间隔
#define CONS_JILIAN_HEART_BEAT_INTER_S 120			//级联进程心跳间隔
#define CONS_ROUTER_HEART_BEAT_INTER_S 120			//自动路由进程心跳间隔

#define CONS_METER_ADDR_MAX_LEN 12			//电表地址最大字符数
#define CONS_METER_ADDR_LEN 6				//电表地址长度 
#define CONS_METER_USER_MAX_LEN 12			//电表用户名最大字符数
#define CONS_METER_PWD_MAX_LEN 12			//电表密码最大字符数
#define CONS_METER_SETTING_KEY_MAX_LEN 32	//电表设置关键字最大字符数
#define CONS_CONFIG_PLAN_KEY_MAX_LEN 16		//配置方案关键字最大字符数
#define CONS_CONFIG_PLAN_NAME_MAX_LEN 32	//配置方案名称最大字符数

#define CONS_FILE_NAME_MAX_LEN	64	//文件名最大字符数
#define CONS_PATH_FILE_MAX_LEN	255	//路径文件最大字符数
#define CONS_SERVER_NAME_MAX_LEN	8	//服务进程名最大字符数
#define CONS_RULE_NAME_MAX_LEN	24	//规约名最大字符数
#define CONS_CHANNEL_NAME_MAX_LEN	16	//通道名最大字符数
#define CONS_CHANNEL_SETTING_MAX_LEN	16	//通道设置最大字符数
#define CONS_CHANNEL_TAG_MAX_LEN	16	//通道附加信息最大字符数
#define CONS_ACTION_KEY_MAX_LEN	32	//操作关键字最大字符数
#define CONS_ACTION_NAME_MAX_LEN	32	//操作名称最大字符数
#define CONS_USER_CODE_MAX_LEN	64	//用户代码最大字符数，用于SysConfig中
#define CONS_MODEM_INIT_CMD_MAX_LEN 128	//MODEM初始化命令最大字符数
#define CONS_PPP_APN_MAX_LEN 32	//APN最大字符数
#define CONS_PPP_TELENO_MAX_LEN 32	//PPP拨号时电话号码最大字符数
#define CONS_PPP_USER_MAX_LEN 32	//PPP拨号时用户最大字符数
#define CONS_PPP_PWD_MAX_LEN 32	//PPP拨号时密码最大字符数

#define CONS_PACK_SEND_DEFAULT_TIME_OUT_SECOND	15	//默认的数据包发送超时值（秒）

//suxufeng:[2010-3-25]新系统的内存比较大，可以分配较多的空间给消息缓冲
#define CONS_MESSAGE_BUFFER_LEN	1024*128 //消息机制中读消息时的缓冲字节数


#define CONS_SERVER_MAIN_INFO_MAX_LEN	64	//用于在液晶上显示进程主要信息的缓冲区大小
#define CONS_DEAULT_CHAR_READ_TIMEOUT_MS	50
#define CONS_DEAULT_RECV_TIMEOUT_MS	5000	
#define CONS_DEAULT_WRITE_TIMEOUT_MS	3000
#define CONS_DEAULT_READ_TIMEOUT_MS	3000
#define CONS_DEAULT_SUB_READ_TIMEOUT_MS	50
#define CONS_REG_DEFAULT_CHAR 0xFF
#define CONS_TASK_FILE_DEFAULT_CHAR 0xFF
#define CONS_TASK_FILE_DEFAULT_VAL_FORMAT	0x84	//任务默认的数值型数据保存的格式
#define CONS_KEY_MAX_LEN 32	//关键字最大字符数
#define CONS_ACTION_PARAM_MAX_LEN 255 //操作参数最大字节数
#define CONS_DEFAULT_TRACE_LEVEL TRACE_LEVEL_VERBOSE	//默认的跟踪等级

#define CONS_SUSPEND_MAX_WAIT_SECOND 30 //等待子进程全部挂起最大的秒数
#define CONS_SYNC_MAX_WAIT_SECOND	90	//等待同步完成最大的秒数
#define CONS_RESET_COMMAND_DELAY_SECOND 3 //复位命令延时秒数(超过此秒数后才进行复位命令)


#define CONS_HARDWARE_DOG_DEAD_SECOND 15  //当没有喂狗时间超过此秒数时硬件狗将启动
#define CONS_SOFTWARE_RESET_DELAY_SECOND	3 //软件重启的延时秒数


#define CONS_SUSPEND_DELAY_SECOND	CONS_RESET_COMMAND_DELAY_SECOND	//超过此秒数后对系统进行挂起
#define CONS_SYNC_DELAY_SECOND	(CONS_SUSPEND_DELAY_SECOND+CONS_SUSPEND_MAX_WAIT_SECOND) //超过此秒数后对系统进行同步
#define CONS_HARDWARE_RESET_DELAY_SECOND (CONS_SYNC_DELAY_SECOND+CONS_SYNC_MAX_WAIT_SECOND)	//超过此秒数后对系统进行复位

#define CONS_DAEMON_ALRM_INTER_US 1000000
//#define CONS_WORK_ALRM_INTER_US 0
#define CONS_DBCJ_ALRM_INTER_US 2000000
#define CONS_HTTX_ALRM_INTER_US 0
#define CONS_PPP_ALRM_INTER_US 0
#define CONS_JILIAN_ALRM_INTER_US 1000000
#define CONS_ROUTER_ALRM_INTER_US 1000000

#define CONS_PPP_DEFAULT_PORT_NO 8	//PPP拨号采用的默认串口号
#define CONS_PPP_DEFAULT_BAUD 19200	//PPP拨号采用的默认波特率
#define CONS_PPP_DEFAULT_CHECK_INTER_M 720	//PPP默认的检测间隔（分钟）
#define CONS_PPP_DEFAULT_CHECK_FAIL_TIMES 2	//PPP默认的连续检测失败断线阀值
#define CONS_PPP_DEFAULT_MODEM_INIT_CMD "AT+CGDCONT=1,\"IP\",\"CMNET\"" //PPP的MODEM初始化命令

#define CONS_PPP_FAILED_TIMES_FOR_MODEM_RESET 3		//连续超过此次数PPP拨号失败，将对MODEM进行复位
#define CONS_MODEM_FAILED_TIMES_FOR_RESET 3		//连续超过此次数MODEM测试失败，将对MODEM进行复位
#define CONS_MODEM_UNNORMAL_CHECK_INTERS 5	//当Modem有问题时，每隔此值秒后进行检测
#define CONS_BATTERY_IO_BUS_NO	4	//电池IO相关联的总线号
#define CONS_FREEZE_MAX_DELAY_MINUTES 30	//冻结时最多可以延迟冻结的分钟数（注：任务也有可能有冻结延迟设置，此时两者都使用来决定是否延迟冻结）


#define CONS_DEFAULT_EVENT_METER_TIME_MAX_DIFF_M 10	//默认的判断电表时钟异常的阀值（分钟）
#define CONS_DEFAULT_EVENT_METER_ZGLYX_MIN_VAL 0.8f	//默认的总功率因数阀值，当功率因素小于此值时，将产生功率因数低于阀值报警

#define CONS_DEFAULT_SET_METER_TIME_MAX_DIFF_M 10	//默认的电表对时超差，当电表时间与装置时间差别超过此值时，将不对时
#define CONS_DEFAULT_SET_DEVICE_TIME_MAX_DIFF_M 1440	//默认的电表对时超差，当电表时间与装置时间差别超过此值时，将不对时

#define CONS_FLOAT_PART_DIFF 0.00001

//2006-9-19，苏煦烽添加报警记录长度，供整个程序使用
#define CONS_DEFALT_ALARM_FILE_LENGTH	256
#define CONS_FAIL_ALARM_FILE_LENGTH		32
#define CONS_GATHER_FAIL_ALARM_DAYS		3 //当抄表失败了这个天数，就要上报
#define CONS_SET_TIME_FAIL_ALARM_TIMES  3//当连续校时失败次数达到三次，就要上报
#define CONS_GATHER_FAIL_ALARM_TIMES	10000 //当一天失败了这个数，今天就不会再采了
#define CONS_GATHER_FAIL_ALARM_HORURS	72 //上次抄表到目前有72小时产生抄表失败告警

#define SHARE_MEM_LENGTH 256		//单位 字节	//刘志强 共享内存大小 2006-7-25号讨论，不再使用bstring存储结构
									//改用数组，数组大小使用固定大小。所有刘志强定义的共享内存大小统一使用这个大小
#define LOG_ON_TIME_OUT		30 //单位是秒 登陆	
#define REAL_READ_TIME_OUT  4000 //单位毫秒 实时招测	

//#define DBCJ_3_BUSRULE_GENERAL 1//通道编号：载波通道为1    //2006-10-12 刘志强
//#define DBCJ_BUSNO_485 2//通道编号：485通道为2   //2006-10-12 刘志强

#define METER_CONNECTION_485 1//通讯方式：485    
#define METER_CONNECTION_ZB 3//通讯方式：载波


/**************************下行结构设置*****************/
/** @brief 串口总数 */
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
//瑞斯康正在进行的耗时的操作
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

//操作的处理状态
enum TRANS_STATUS_T
{
	STATUS_BUSY = 0x01,
	STATUS_FINISHED = 0x00,
	STATUS_UNKNOWN = 0x03,
};

#define CONS_LCD_ALRM_INTER_US			0	
#define CONS_LCD_HEART_BEAT_INTER_S		30		// 液晶进程心跳间隔


/*********************************/

#define CONS_ROUTER_RELAY_MAX_COUNT  5 //最大采集终端个数，1-5个终端表示0-4级路由

#define CONS_RROUTER_RELAY_MAX_COUNT  9

#define CONS_MUST_EXIT_FLAG  255 //紧急退出函数返回值
#define CONS_MUST_ADAPTE_CHIP 254

#define CONS_DEFAULT_METER_ADDR  0x00	//电表地址默认填充值

//#define CONS_SERVER_RESTART_TIMES 10	//任何一个进程重启次数超过CONS_SERVER_RESTART_TIMES次数后，让看门狗来复位集中器

//********************************其它配置，一般不更改***************************************

//******************由研发三嵌入式软件开发小组更改，以适应新集抄装置的需求*******************
#define CONS_GPRS_RECONNECT_MAX_TIMES 15				//GPRS重连次数

#define CONS_UNVALID_METER_NUMBER 1207	//无效电表号

#define CONS_GDDYJC_DATA_LENGTH 9				//数据长度标志在上行规约帧中的位置，用于级联组包机制
//#define IPV6_OR_IPV4 6                           //当前使用的是几位的IP地址
//#define WATCHDOGREBOOT    30//2006-11-16    ppp拨号重试次数。超过限制装置重启

/*************************************表规约的控制码****************************************/
#define MODE_READ   0x00
#define MODE_WRITE  0x01
#define MODE_LOGON 0x03
#define MODE_LOGOUT 0x04
#define MODE_PROAGRAM 0x05
#define MODE_READ_OUT  0x06
#define MODE_CHECK_PWD 0x07

/*******************************************************************************************/
//*****************************调试命令*************************************
#define DEBUG_INOUT          0x5354		//1、开始结束调试
#define DEBUG_STATICROUTER   0x5254		//2、静态路由调试
#define DEBUG_CHECKRELAYCOMM 0x5254
#define DEBUG_LINKSTATE      0x4C4B		//3、检查线路连接
#define DEBUG_GETNETSTATUS      0x5252	//读下行模块网络状态
#define DEBUG_GETNETFAILEDNODES      0x5352   // 读下行模块网络失败节点

#define DEBUG_SENDPACKAGE    0x5054		//4、发包测试
#define DEBUG_SNDMETERPACKGE 0x5054

#define DEBUG_CALLTEST	0x5055	// 点名测试
#define DEBUG_STARTROUTER    0x424E		//5、启动路由
#define DEBUG_CHECKLOSTRELAYS 0x424E

#define DEBUG_STOPROUTER      0x434E
#define DEBUG_ADJUSTROUTER   0x4152		//6、调整路由
#define DEBUG_SETZBSNIFFER   0x6053		//7、设置载波侦测任务
#define DEBUG_CONTROL_CODE	 0x31		//调试命令控制码
#define DEBUG_NOMAL_REPLY_CODE 0xB1		//调试命令正常回应控制码
#define DEBUG_ABNOMAL_REPLY_CODE 0xF1	//调试命令异常回应控制码



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
