///*SysConfig.h通用的设置*/

#ifndef INCLUDED_SYS_CONFIG
#define INCLUDED_SYS_CONFIG

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

//SysConfig中的设置将在出厂时设置，即使装置参数复位也不会掉失
struct SysConfig
{
	INT32U DeviceNo;			//装置编号
	INT16U HardwareVerion;		//硬件版本 //880A
	char m_softVerion[19+1];		//软件版本号 8809
	char ModuleSoftversion[19+1];

	//2006-6-8刘志强+ 
	//具体解释参看<<广东电网公司低压电力用户集中抄表系统集中器上行规约(试行)2006-5-1实施版本>> 集中器参数设置部分设定
	char RTUA[4+1];				//地市区县8016 
								//集中器地址 编号8017
	INT32U m_RTUA;				//RTUA 集中器逻辑地址   A0 A1 B1 B2	  //2006-6-8刘志强+
	INT16U m_MSTA;				//RTUA 主站地址地址   就是一个整数	  //2006-6-8刘志强+
	//INT8U  m_BusType;               //总线通讯类型
	char m_ChipAddr[6];
	INT8U ChipChannel;
	INT8U m_RouterMode;			//路由芯片工作模式
	//char IP_ADDR[8];			//IP地址   8010
	//INT8U m_ChannelType;		//通道类型	 8010 

	//char IP_ADDR_bk1[8];		//备用IP地址1		
	//INT8U m_ChannelType_bk1;	//备用通道类型1	

	//char IP_ADDR_bk2[8];		//备用IP地址2	
	//INT8U m_ChannelType_bk2;	//备用通道类型2
	int LCDPassword;

	char m_PhoneNum[16+1];			//短信中心号码8013
	char m_Default_GateWay[16+6+1];	//默认网关地址或者代理服务器地址和端口8014


	char m_psw_Read[6+1];		//普通密码,只读权限8020  
	char m_psw_lowSet[6+1];		//低级设置权限 8021 "111111" 
	char m_Admin_psw[6+1];		//管理员密码 缺省为集中器地址的BCD码表示 8022	"CMNET"
	

	INT16U m_Customs;			//普通用户表总数 8812
	INT16U m_MutilFunMeters;	//简易多功能表总数 8813
	INT8U  m_VIP;				//重点户数 8814
	INT8U m_TimeLock;			//定时任务 8815	时
	INT8U m_Space;				//执行间隔 8816 00H:30分钟 01H:60分钟 02H:120分钟 03H:240分钟 04H:360分钟 05H:720分钟 
	INT8U m_Relay;				//中继方式 8817 00:自动中继 其他:固定中继
	char m_Alarm_gate[2+1];		//线损报警阀值 8818 %

	INT8U m_ReadTime;			//日冻结数据抄收时间 8819 时
	INT8U m_Reportable;			//VIP数据上报时间 FF为不住动上报 默认为FF 上传间隔为24小时 0-23为主动上报时间 881A
	INT8U m_Alarm_Shield[4];		//报警屏蔽字881B，D7=1允许线损超阀值报警，D6=1允许失压报警，D5=1允许失流报警，D4=1允许电流反向报警，D3=1允许电表编程报警，D2=1允许抄表失败报警，D1=1允许校时失败报警，D0=1允许继电器变位事件报警
	char m_Lose_ToDay[3+1];		//本日线损率 881C
	char m_SideDJS[16+1];		//从集中器地址 881D
	char m_SlaveDistribut[4+1];	//从配变终端地址8822
	//刘志强注 此处,为使内存中保存的形式为HEX形式,配置文件中只好用等价的ascii形式了.如HEX的21被保存成了33.
	INT8U AlarmCheckStartTime;	//2006-8-7，苏煦烽扩展881F 线损超值报警、抄表失败报警和校时失败报警的启动时间，默认在每天夜晚23点开始检查
	INT16U m_Month_trail_read;	//月末数据抄收开始时间 8820 DD:1-28 HH:1-23
	INT16U m_ReportTime;		//月末数据上传时间 8821 DD:1-28 HH:0-23
	//INT16U m_BroadcastSetTime;	//
	INT8U m_BroadcastSetTime[3];	//广播校时开始时间，3个字节分别存储周时分
	INT8U m_BroadcastInterval;	//广播校时补校间隔，以分钟为单位
	INT16U MaxMeterCount;		//支持的最大电表数8811
	INT8U WirelessModemType;	//MODEM型号,参见MODEM_TYPE
	//2006-9-10刘志强+ 	
	INT32U BeatReply;//心跳间隔 分钟
	INT32U BEATIMES;//心跳次数  次

	char RelaysVersion[4];		// laosu[2010-11-30] 集中器下面所挂的采集器的版本号

	INT8U GPRSDialMaxTimes;				//最大重拨次数，用于GPRS
	INT8U GPRSReConnectMaxTimes;		//最大重连次数，用于GPRS
	INT8U GPRSAutoConnectStartDate;		//GPRS自动重连起始日期
	INT8U GPRSAutoConnectStartHour;		//GPRS自动重连起始时间
	INT8U GPRSAutoConnectEndDate;		//GPRS自动重连结束日期
	INT8U GPRSAutoConnectEndHour;		//GPRS自动重连结束时间
	INT8U GPRSAutoDialInterMinute;		//GPRS自动重拨间隔，单位：分钟
	INT8U GPRSAutoConnectInterMinute;	//GPRS自动重连的频率，单位：分钟
	//INT32U GPRSMonthGeneralTraffics;	//GPRS当月总流量
	// 1:广东日冻结数据突变处理方法:当天冻结的日与outvalue做比价，大于则认为数据过大。outvalue = (float)(上一天*(1.0+TOLERANCE)+2*上五天数据的相隔天的平均值差    //   值);          
    // 2：深圳区日冻结数据突变处理方式: （今天数据—昨天数据）>5（昨天数据—前天数据）并且单日用电量超过1000度时，选择过滤,用上一天冻结数据替换
	INT8U m_Check_Daydata_Type;	
	INT8U m_XC_Protocol_Type;	// 1:N6+N12, 2:N12
	//2006-8-22苏煦烽+
	INT8U mBusType;

	/** 端口映射表
	 */
	INT8U m_PortMap[SYSTEM_SERIAL_PORT_MAX]; ///< 端口映射表

	inline const char* GetUserCode() const
	{
		return UserCode;
	}
	inline const char* GetPlanKey() const
	{
		return PlanKey;
	}
	const char* SetUserCode(const char* userCode);
	const char* SetPlanKey(const char* planKey);
	//2006-6-8刘志强+ 
	const char* Setm_PhoneNum(const char* m_phonenum);
	const char* bcd_to_asc_PhoneNum(const char* bcd8PhoneNum,int r=1);//1表示传入正序数组0反序
	const char* asc_to_bcd_PhoneNum(char *tep,int r=1);//1表示传出正序数组0反序
	const char* Setm_Default_GateWay(const char* m_default_gateway);
	const char* Setm_psw_Read(const char* m_psw_read);
	const char* Setm_psw_lowSet(const char* m_psw_lowset);
	const char* Setm_Admin_psw(const char* m_admin_psw);
	const char* Setm_softVerion(const char* m_softVerion);
	const char* Setm_Alarm_gate(const char* m_alarm_gate);
	const char* Setm_Lose_ToDay(const char* m_lose_today);
	const char* Setm_ReadTime(const char *m_readtime);
	//const char* Setm_Alarm_Door(const char* m_alarm_door);
	const char* Setm_Reportable(const char* m_reportable);
	const char* Setm_Alarm_Shield(const char* m_alarm_shield);
	const char* Setm_ReportTime(const char *m_readtime);
	const char* Setm_BroadcastSetTime(const char *m_readtime);
	const char* SetalarmCheckStartTime(const char *alarmCheckStartTime);

    const char* SetChipAddr(const char* rouble_addr);


	    const char*  Setm_ModuleSoftversion(const char* m_softverion);
		void    Getm_ModuleSoftversion(char* m_softverion);
	//const char* Set
	//2006-6-8刘志强+ 
private:
	char UserCode[CONS_USER_CODE_MAX_LEN+1];		//系统用户，每个地区有一个编号
	char PlanKey[CONS_CONFIG_PLAN_KEY_MAX_LEN+1];	//系统配置方案
public:
	//int daylock,viplock,monthlock;
	//2006-6-8刘志强+
	const char* SetRTUA(const char* rtua);
	INT32U GetRTUA(void);
	void SetMSTA(INT16U RT);
	INT16U GetMSTA(void);
    //2006-6-8刘志强+ 	
	void Get_psw_Read(unsigned char *psw);
	void set_psw_read3(unsigned char *psw);
	//void set_psw_read7(char *psw);
	void Get_psw_lowSet(unsigned char *psw);
	void set_psw_lowSet3(unsigned char *psw);
	//void set_psw_read7(char *psw);
	void Get_Admin_psw(unsigned char *psw);
	void set_Admin_psw3(unsigned char *psw);

    void GetDeviceAddr(char addr[6]);

	void SetConnChannel(INT8U channelNO)
	{
		   ChipChannel = channelNO;
	}
	void GetConnChannel(INT8U &ChannelNo)
	{
		 ChannelNo = ChipChannel;
	}
	void SetRouterWorkMode(INT8U routerWorkMode)
	{
		m_RouterMode = routerWorkMode;
	}
	void GetRouterWorkMode(INT8U &routerWorkMode)
	{
		routerWorkMode = m_RouterMode;
	}
	void SaveBusAddr(INT8U *addr)
	{
		memcpy(m_ChipAddr,(char *)addr,sizeof(m_ChipAddr));
	}

	//void set_Admin_psw7(char *psw);
	inline SysConfig()
	{
		Init();
	}
	inline void Init()
	{
		memset(this,0,sizeof(*this));
	}

	inline bool HasChipAddr() const
	{
		char validaddr[6];
		memset(validaddr,0x00,6);
		return strncmp(m_ChipAddr,validaddr,6)==0?false:true;
	}
	KLDump& Dump(KLDump& dump) const;
};

#endif

