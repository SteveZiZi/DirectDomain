#ifndef INCLUDED_GPRS_INFO
#define INCLUDED_GPRS_INFO

#ifndef INCLUDED_FILE_BASE
#include "../base/FileBase.h"
#endif

struct GprsInfo
{
public:
	bool EnableFlowCtrl;				//是否启用流量限制功能
	INT16U LogOnInterS;					//登陆延时时间
	INT8U LogOnRetryTimes;				//登陆重试次数
	INT16U SocketConnectInterS;			//SOCKET重连间隔，非抄表日
	INT16U DialUpInterS;				//拨号重试间隔
	bool EnableMaxFlowLimit;			//是否启用最大流量限制
	INT32U GenericTrafficStatOfMonth;	//当月流量统计值
	INT8U DialFailTimes;				//成功则置零，不成功则累加，最大数值从SysConfig读取。
	INT8U ReConnectTimes;				//同上
	time_t LastConnectFailedTime;		//上次连接失败时间
	time_t LastConnectTime;				//上次连接时间

	unsigned char UplinkStatus;			// 上行通道的状态
	unsigned char SimCardIP[4];			// SIM卡IP
	unsigned char SignalValue;			// GPRS/CDMA信号值

	void Init();

	inline void SetMainServeMode() // 运行模式为主模式
	{
		UplinkStatus |= 0x01;
	}
	inline void SetSlaveServeMode() // 运行模式为从模式
	{
		UplinkStatus &= 0xFE;
	}
	inline bool IsMainMachine()
	{
		return ((UplinkStatus & 0x01) == 0x01);
	}
	inline void SetLogonStatus() // 已经登录上后台
	{
		UplinkStatus |= 0x02;
		if (!IsMainMachine())
		{
			SetJilianOnlineStatus();
		}
	}
	inline void SetLogoutStatus() // 没登录上后台
	{
		UplinkStatus &= 0xFD;
		if (!IsMainMachine())
		{
			SetJilianOnlineStatus();
		}
	}
	inline bool HasLogon()
	{
		return ((UplinkStatus & 0x02) == 0x02);
	}
	inline void SetJilianOnlineStatus() // 设置级联口状态为在线
	{
		UplinkStatus |= 0x04;
	}
	inline void SetJilianOfflineStatus() // 设置级联口状态为不在线
	{
		UplinkStatus &= 0xFB;
	}
	inline bool IsJilianOnline()
	{
		return ((UplinkStatus & 0x04) == 0x04);
	}
	inline void SetModemOnlineStatus() // 设置MODEM在线状态
	{
		UplinkStatus |= 0x08;
	}
	inline void SetModemOfflineStatus() // 设置MODEM离线状态
	{
		UplinkStatus &= 0xF7;
	}
	inline bool IsModemOnline()
	{
		return ((UplinkStatus & 0x08) == 0x08);
	}
};

#endif
