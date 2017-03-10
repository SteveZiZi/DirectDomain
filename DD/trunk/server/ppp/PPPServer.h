#ifndef INCLUDED_PPP_SERVER
#define INCLUDED_PPP_SERVER

#ifndef INCLUDED_SERVER
#include "../../share/Server.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../../base/SysBase.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../../include/Config.h"
#endif

#ifndef INCLUDED_MODEM_CONNECTION
#include "../../conn/ModemConnection.h"
#endif

#define CONS_FILE_PPP_TRAFFIC_STAT_LOG "/mnt/data/log/ppp"		//�ı��ļ���ͳ��ÿ��װ�õ�����
#define CONS_FILE_PPP_TRAFFIC_STAT_DAT "/mnt/data/log/ppp.dat"		//�������ļ������浱ǰװ������
#define CONS_PPP_RECV_INTER_SECOND 30								//PPP���հ��������ʱ����������ʱ����ΪPPP�Ѿ�����

//TODO:���Ƕ��Ż���ģʽ�µĿ���ʱ���жϣ���Ҫ��SetLastRecvTime()�����Ļ��������һ���жϿ���ʱ��ĺ����������л������Ż��ѷ�ʽ��

struct SysConfig;
struct Channels;
class ModemConnection;

class PPPServer:public Server
{
public:
	PPPServer();
	~PPPServer();
	int OnTickServer();
	static int ResetModem();
	static int PowerOffModem();
	static int PowerOnModem();
	static long GetSimIP(void){return GetNetDevIP(PPP_DEV_NAME);}
	static long GetNetDevIP(char *pDev);
	inline static int SendOnLineRequest()
	{
		return SendLineRequest(IPC_INFO_TYPE_PPP_ONLINE_REQUEST);
	}
	inline static int SendOffLineRequest()
	{
		return SendLineRequest(IPC_INFO_TYPE_PPP_OFFLINE_REQUEST);
	}
	inline static int SendReOnLineRequest()
	{
		return SendLineRequest(IPC_INFO_TYPE_PPP_REONLINE_REQUEST);
	}
protected:
	int CheckModem();
	int CheckNotify();
	int DetectSignal(ModemConnection& conn,int &signal);
	int OnLine();
	int OffLine(bool bForce=false);
	int WaitPPP();

	bool NeedOnLine();
	bool NeedOffLine();
	
	void RefreshStatus();

	int GetDevInfo(INT32U* ipRecvPackages,INT32U* ipSendPackages);
	void SetLastRecvTime();

	int CheckWirelessType();
	void ClearRecvSendStat();

	bool IsOnLine();
	static int SendLineRequest(IPC_INFO_TYP infoType);
	void SetModemConnection();
	bool HasTraffics();
	/*
	�Ƿ���ͨ�ţ������ܹ����յ�GPRS�����ϵ����ݣ�����Ϊ��ʱGPRS���ߡ�
	�ھ�������ǰ���������������Ƿ��б�Ҫ���ߣ�û�б�Ҫ�򲻱ؼ��ڵ��ߡ�
	�����ڿ���ʹ��PPP���ӽ���SOCKET�������޷���½�ɹ��������
	�Լ��޷�����SOCKET���ӣ���������ʹ��PPP����FTP�ļ����������£�����FTP�����նˡ�
	�������30�롣Ҳ����˵����30����֮�ڣ����PPP���������ݣ�����ΪPPP���ߡ�
	*/
	int OnPPPOffLine(Context &context);		//����ʧ�ܺ���ã�ͳ�Ʋ���ʧ�ܴ���
	int OnPPPOnLine(Context &context);		//���ųɹ�����ã���λ����ʧ�ܴ���������
	bool CanOnLine(Context &context);		//����ǰ���ã��ж��Ƿ��������
	void OnTraffics();						//������������ã���ppp0���ӽ�������ͳ��
	void LoadTraffics();					//��������ͳ������
	void ResetTraffics();					//����ͳ���������㣬ÿ��1��0�����㣬�����û�����ʱ����
	void SaveTraffics();					//��������ͳ������
	void FprintTraffics();					//���������ݱ���Ϊ�ı��ļ�
	//int RemoveFiles(char* FName);			//����ǰ���ļ�ɾ��
	bool CheckIdleness();					//�жϿ���ʱ�䣬���ڶ��Ż���ģʽ�µĿ��м�飬��������ʱ��PPP�Զ�����
private:
	PPPServerInfo& m_serverInfo;
	SysConfig& m_sysConfig;

	PPPSetting m_curSetting;
	PID_T m_curPPP;
	int m_curModemType;
	int m_nPPPFailedCount;//PPP��������ʧ�ܵĴ���
	int m_nModemFailedCount;//Modem����ʧ�ܵĴ���
	INT8U m_curNetType;
	INT32U m_lastRecvPackages;		//�ϴν��յİ����������˴β���
	INT32U m_lastSendPackages;		//�ϴη��͵İ����������˴β���
	INT32U m_RecvBytes;				//���յ��ֽ�����
	INT32U m_SendBytes;				//���͵��ֽ�����
	INT32U m_RecvBytesOnce;			//���յ��ֽ�����
	INT32U m_SendBytesOnce;			//���͵��ֽ�����
	INT32U m_LastRecvBytes;			//�ϴν��յİ����������˴β���
	INT32U m_LastSendBytes;			//�ϴη��͵İ����������˴β���
	INT32U m_RecvStat;				//���յ��ֽ�����
	INT32U m_SendStat;				//���յ��ֽ�����
	INT32U m_TrafficStat;			//���ͺͽ��յ��ֽ�����
	ModemConnection* m_pModemConnection;
	INT8U   m_PppdKillTimes; //pppd ��ppp_gprs_off.srt ɱ���Ĵ���
};

#endif
