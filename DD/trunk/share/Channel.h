///*Channel.hͨ��*/

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
	unsigned char IP[4];	//IP��ַ����192.168.8.22��IP[0]=192,IP[1]=168
	unsigned short PortNo; //����˿�
	unsigned char Type;	//�μ�NET_CONNECT_TYPE 0 TCP_CLIENT 1 TCP_SERVER 2 UDP
	unsigned char PPPNeedLevel;//�μ�PPP_NEED_LEVEL 0����Ҫ 1��Ҫ 2������ 

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
	INT8U PortNo;	//�˿ں�
	INT8U PortType;//�˿����ͣ�0��ʾ���ڣ�1��ʾPPC16C554��
	INT8U CDCheck;//0��ʾ������CD���,��0��ʾ����CD���
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
	INT8U PortNo;	//�˿ں�
	INT8U ModemType;//MODEM_TYPE�ͺ�
	INT8U PowerMethod;//��Դ�������μ�PSTN_POWER_METHOD
	INT8U DialMethod;//���ŷ�ʽ���μ�PSTN_DIAL_METHOD
	struct PortSet PortSet;
	INT16U CheckInterS;//�Լ������룩
	char InitCmd[CONS_MODEM_INIT_CMD_MAX_LEN+1];//MODEM��ʼ������

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
	INT8U Enabled;//�Ƿ���Ч��0��ʾ��Ч����0��ʾ��Ч
	INT8U Type;//���ͣ��μ�CHANNEL_TYPE
	INT8U Priority;//���ȼ������ж��ͨ��ʱ������
	char Name[CONS_CHANNEL_NAME_MAX_LEN+1];//ͨ������
	union ChannelSetting Setting;//ͨ�����ã����ÿ��ͨ�������в�ͬ�ĸ�ʽ
	unsigned char Tag[CONS_CHANNEL_TAG_MAX_LEN];//������Ϣ
	INT32S PackageMaxLen;//���֡����
	INT32U RecvBytes;//���յ����ֽ���
	INT32U SendBytes;//���͵��ֽ���
	INT32U CharReadTimeoutMs;//�ַ���ʱ(��������һ�ַ���ʱ�䳬����ֵ�������ݰ�����һ�飬���д��򽫸ð����������ǲ�������������ȴ���һ�ַ�)
	INT32U RecvTimeoutMs;//���ճ�ʱ(��������һ�ַ���ʱ�䳬����ֵ�����ʾ����ʧ�ܣ����ƻ��������ݰ��˳��ð�����)
	INT32U WriteTimeoutMS;//Ĭ�ϵ�д��ʱ
	INT32U ReadTimeoutMS;//Ĭ�ϵĶ���ʱ
	INT32U SubReadTimeoutMS;//���Ե���������ʱ
	INT16U IdleTimeoutS;//����ʱ�䣨�룩����ͨ��������ʱ��û���յ�����ʱ�����˳�ͨ����ʹ��
	INT16U ConfirmTimeoutS;//ȷ�ϳ�ʱ
	INT16U ConnectTimeoutS;//������ʱ
	INT16U ConnectSubTimeoutS;//��һ�ν��������У�ÿ����һ�γ������ӵĳ�ʱ
	INT16U ConnectInterS;//��������֮��ļ��
	INT16U ServiceTimes;//һ��ͨ����������������������˴��������Ժ󣬽���������ͨ��	
	INT8U LogonEnabled;//�Ƿ������½
	INT8U ReplyEnabled;//�Ƿ�����ش�
	INT8U ReportEnabled;//�Ƿ������ϱ�
	INT8U ConfirmNeeded;//�ϱ�ʱ�Ƿ���Ҫȷ��
	INT8U HeartBeatEnabled;//�Ƿ���������
	INT8U AutoConnectEnabled;//�Ƿ������Զ�����
	INT8U Status;//ͨ��״̬���μ�CHANNEL_STATUS
	INT8U HexDataOutput;//�Ƿ�ʹ��HEXģʽ���������
	INT8U AscDataOutput;//�Ƿ�ʹ��ASCģʽ���������
	char KeyChar;//�ؼ��ַ�������Һ����ʾ��Ϊ' 'ʱ��ʾ����ʾ��Һ����

public:
	inline Channel()
	{
//INT32U Baud;	/*115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600,300*/
//	INT8S DataBit; /*5,6,7,8 */
//	INT8S StopBit; /*1->1λ,2->2λ,3->1.5λ*/
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

	inline bool IsKernelChanged(const Channel& other) const	//���ļ��ĸı䣬ָ�ᵼ��������Ҫ���õĸı�
	{
		return Type!=other.Type || memcmp(&Setting,&other.Setting,sizeof(Setting))!=0;
	}
	//����ʱʹ��
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


