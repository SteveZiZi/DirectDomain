
#ifndef INCLUDED_SERVER
#define INCLUDED_SERVER

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_DEFINE
#include "../include/Define.h"
#endif

#ifndef INCLUDED_SERVER_INFO
#include "ServerInfo.h"
#endif

#ifndef INCLUDED_SERVER_INFO
#include "ServerInfo.h"
#endif


class Server
{
public:
	typedef	enum _SigTag
	{
		HeartBeatSigTag=0,
		MemResetTag=1,//�ڴ渴λ
		DataResetTag=2,//���ݸ�λ
		ParamResetTag=3,//������λ
		ParamChangedTag=4,//�����ı�
		TimeChangedTag=5,//ʱ��ı�
		MainUpdateTag=6,//����������
		SysUpdateTag=7,//ϵͳ����
		RefreshUpdateTag=8,//ˢ�����ļ�����
		DataFileRebuildTag=9,//�����ļ��ؽ�
		UpdateResumeTag=10,//������ָ��ָ�
	}SigTag;

	inline Server(ServerInfo& serverInfo,time_t& tickSecond):m_ServerInfo(serverInfo),m_TickSecond(tickSecond){};

	virtual ~Server(){};

	//ͨ��SIGUSR1�źŸ���sigTag�����źŷ���
	static int User1Sig(PID_T pid,int sigTag);

	//ͨ��SIGUSR2�źŸ���sigTag�����źŷ���
	static int User2Sig(PID_T pid,int sigTag);
	
	static int RemoveMeterSig(PID_T pid, int meterno);
	static int TaskFinishedSig(PID_T pid, int task_type);
	static int StatusSaveSig(PID_T pid, int iForce);

	//static int CheckProgress(const char* pName,PID_T pid=-1);
	static int GetSigAlrmInterUS()
	{
		return m_pCurServer?m_pCurServer->GetAlrmInterUS():0;
	}
	#ifndef WIN32
	static void SigHandler(int signo, siginfo_t *siginfp, struct sigcontext * scp);
	#endif
	inline static void SigTerm(PID_T pid)
	{
		#ifndef WIN32
		if (pid>0)
		{
			kill(pid,SIGTERM);
		}
		#endif
	}

	virtual int Init();

	virtual int BeforeService();

	virtual int Service();


	virtual bool IsStartable()
	{
		return m_ServerInfo.IsStartable();
	}

	static inline Server* GetCurServer()
	{
		return m_pCurServer;
	}
	static Server* SetCurServer(Server* pCurServer);
	static INT16U GetCurServerID();
	static time_t GetCurTickSecond();

	//static void ServerPrint(const char *pszFmt, ...);

	inline ServerInfo& GetServerInfo()
	{
		return m_ServerInfo;
	}
	inline char* GetName()
	{
		return m_ServerInfo.Name;
	}
	inline bool IsAlone() const
	{
		return (m_ServerInfo.PID!=getpid());
	}
	inline bool HeartBeatable() const
	{
		return (m_ServerInfo.HBInterS>0 && IsAlone()==false);
	}
	inline INT32U GetAlrmInterUS() const
	{
		return m_ServerInfo.AlrmInterUS;
	}
	inline time_t GetTickSecond() const
	{
#ifndef WIN32
		return m_TickSecond;
#else
		return time(NULL);
#endif
	}
	inline INT32U GetHBInterS()
	{
		return m_ServerInfo.HBInterS;
	}
public:
	void HeartBeat();
	int OnSig(int signo,SigTag tag,PID_T srcpid);
	virtual int OnSigAlrm();
	virtual int OnSigTerm();
	virtual int OnSigUser1(SigTag tag,PID_T srcpid);
	virtual int OnSigUser2(SigTag tag,PID_T srcpid);
	virtual int OnRemoveMeter(int meterno,PID_T srcpid);
	virtual int OnTaskFinished(int task_type,PID_T srcpid);
	virtual int OnStatusSaveSig(int iForce,PID_T srcpid);
	virtual int OnTickServer()=0;
private:
	static void InstallSig();
protected:
	ServerInfo& m_ServerInfo;
	static Server* m_pCurServer;
	time_t& m_TickSecond;
};

#endif

