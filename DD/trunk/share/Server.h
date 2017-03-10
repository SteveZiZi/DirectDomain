
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
		MemResetTag=1,//内存复位
		DataResetTag=2,//数据复位
		ParamResetTag=3,//参数复位
		ParamChangedTag=4,//参数改变
		TimeChangedTag=5,//时间改变
		MainUpdateTag=6,//主程序升级
		SysUpdateTag=7,//系统升级
		RefreshUpdateTag=8,//刷新性文件升级
		DataFileRebuildTag=9,//数据文件重建
		UpdateResumeTag=10,//升级后恢复恢复
	}SigTag;

	inline Server(ServerInfo& serverInfo,time_t& tickSecond):m_ServerInfo(serverInfo),m_TickSecond(tickSecond){};

	virtual ~Server(){};

	//通过SIGUSR1信号附加sigTag进行信号发送
	static int User1Sig(PID_T pid,int sigTag);

	//通过SIGUSR2信号附加sigTag进行信号发送
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

