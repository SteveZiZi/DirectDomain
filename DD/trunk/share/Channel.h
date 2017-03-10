///*Channel.h通道*/

#ifndef INCLUDED_CHANNEL
#define INCLUDED_CHANNEL

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_PORT_SET
#include "../base/PortSet.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

struct NetSetting
{
	unsigned char IP[4];	//IP地址，如192.168.8.22则IP[0]=192,IP[1]=168
	unsigned short PortNo; //网络端口
	unsigned char Type;	//参见NET_CONNECT_TYPE 0 TCP_CLIENT 1 TCP_SERVER 2 UDP
	unsigned char PPPNeedLevel;//参见PPP_NEED_LEVEL 0不需要 1需要 2不允许 

public:
	void Init();
	void GetIP(string& oStr) const
	{
		char ipStr[32];
		sprintf(ipStr,"%d.%d.%d.%d",IP[0],IP[1],IP[2],IP[3]);
		oStr.append(ipStr);
	}
	void ToString(string& oStr) const;
	int Parse(const char* setting);
	bool IsValid() const
	{
		if (Type!=::NET_CONNECT_TYPE_TCP_SERVER)
		{
			unsigned int sum=IP[0]+IP[1]+IP[2]+IP[3];
			return sum!=0 && sum!=0xff*4;
		}
		return true;
	}
};

struct ComSetting
{
	INT8U PortNo;	//端口号
	INT8U PortType;//端口类型，0表示串口，1表示PPC16C554口
	INT8U CDCheck;//0表示不启用CD检测,非0表示启用CD检测
	struct PortSet PortSet;
public:
	void Init();
	void ToString(string& oStr) const;
	int Parse(const char* setting);
	inline bool IsValid() const
	{
		return PortNo>0;
	}

};

struct PstnSetting
{
	INT8U PortNo;	//端口号
	INT8U ModemType;//MODEM_TYPE型号
	INT8U PowerMethod;//电源方案，参见PSTN_POWER_METHOD
	INT8U DialMethod;//拨号方式，参见PSTN_DIAL_METHOD
	struct PortSet PortSet;
	INT16U CheckInterS;//自检间隔（秒）
	char InitCmd[CONS_MODEM_INIT_CMD_MAX_LEN+1];//MODEM初始化命令

public:
	void Init();
	void ToString(string& oStr) const;
	int Parse(const char* setting);
	inline bool IsValid() const
	{
		return PortNo>0;
	}
};

union ChannelSetting
{
	struct ComSetting Com;
	struct NetSetting Net;
	struct PstnSetting Pstn;
};

class KLDump;


struct Channel
{
public:
	INT8U Enabled;//是否有效，0表示无效，非0表示有效
	INT8U Type;//类型，参见CHANNEL_TYPE
	INT8U Priority;//优先级，具有多个通道时才有用
	char Name[CONS_CHANNEL_NAME_MAX_LEN+1];//通道名称
	union ChannelSetting Setting;//通道设置，针对每种通道类型有不同的格式
	unsigned char Tag[CONS_CHANNEL_TAG_MAX_LEN];//附加信息
	INT32S PackageMaxLen;//最大帧长度
	INT32U RecvBytes;//接收到的字节数
	INT32U SendBytes;//发送的字节数
	INT32U CharReadTimeoutMs;//字符超时(若接收下一字符的时间超过此值，则将数据包解释一遍，若有错则将该包丢掉，若是不完整包则继续等待下一字符)
	INT32U RecvTimeoutMs;//接收超时(若接收下一字符的时间超过此值，则表示接收失败，则破坏接收数据包退出该包接收)
	INT32U WriteTimeoutMS;//默认的写超时
	INT32U ReadTimeoutMS;//默认的读超时
	INT32U SubReadTimeoutMS;//尝试的连续读超时
	INT16U IdleTimeoutS;//空闲时间（秒），当通道超过此时间没有收到数据时，将退出通道的使用
	INT16U ConfirmTimeoutS;//确认超时
	INT16U ConnectTimeoutS;//建链超时
	INT16U ConnectSubTimeoutS;//在一次建链操作中，每进行一次尝试连接的超时
	INT16U ConnectInterS;//两次连接之间的间隔
	INT16U ServiceTimes;//一个通道连续服务的最大次数，当此次数超过以后，将尝试其它通道	
	INT8U LogonEnabled;//是否允许登陆
	INT8U ReplyEnabled;//是否允许回答
	INT8U ReportEnabled;//是否允许上报
	INT8U ConfirmNeeded;//上报时是否需要确认
	INT8U HeartBeatEnabled;//是否允许心跳
	INT8U AutoConnectEnabled;//是否允许自动连接
	INT8U Status;//通道状态，参见CHANNEL_STATUS
	INT8U HexDataOutput;//是否使用HEX模式输出数据流
	INT8U AscDataOutput;//是否使用ASC模式输出数据流
	char KeyChar;//关键字符，用于液晶显示，为' '时表示不显示在液晶上

public:
	inline Channel()
	{
//INT32U Baud;	/*115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600,300*/
//	INT8S DataBit; /*5,6,7,8 */
//	INT8S StopBit; /*1->1位,2->2位,3->1.5位*/
//	INT8S Parity; /*0->no parity;1->odd parity;2->even parity; */
		memset(this,0,sizeof(*this));
		//Type=CHANNEL_TYPE_COM;
		//union ChannelSetting set;
		//set.Com.PortNo=2;
		//set.Com.PortSet.Baud=9600;
		//set.Com.PortSet.DataBit=8;
		//set.Com.PortSet.StopBit=1;
		//set.Com.PortSet.Parity=0;
		//Setting=set;
	}
	inline Channel(const Channel& channelSrc)
	{
		memcpy(this,&channelSrc,sizeof(*this));
	}
	inline void operator=(const Channel& channelSrc)
	{
		memcpy(this,&channelSrc,sizeof(*this));
	}
	
	inline bool IsValid()
	{
		return  (Enabled!=0) && ((Type==CHANNEL_TYPE_COM && Setting.Com.IsValid()==true) || (Type==CHANNEL_TYPE_NET && Setting.Net.IsValid()==true) || (Type==CHANNEL_TYPE_PSTN && Setting.Pstn.IsValid()==true) );
	}

	inline bool IsKeyCharValid() const
	{
		return KeyChar!='\0';
	}

	inline bool NeedCDCheck() const
	{
		return ((Type==CHANNEL_TYPE_COM && Setting.Com.CDCheck!=0) || Type==CHANNEL_TYPE_PSTN);
	}

	inline bool IsKernelChanged(const Channel& other) const	//核心级的改变，指会导致连接需要重置的改变
	{
		return Type!=other.Type || memcmp(&Setting,&other.Setting,sizeof(Setting))!=0;
	}
	//排序时使用
	static bool IsGreatPriority(const Channel& l,const Channel& r)
	{
		return l.Priority>r.Priority;
	}
	void MakeDefaultDate(CHANNEL_TYPE type);
	INT8U GetPPPNeedLevel() const;
	void SettingToString(string& oStr) const;
	int SettingParse(INT8U channelType,const char* setting);
	KLDump& Dump(KLDump& dump) const;
};


#endif


