#ifndef INCLUDED_DAEMON_SERVER
#define INCLUDED_DAEMON_SERVER

#ifndef INCLUDED_DEFINE
#include "../../include/Define.h"
#endif

#ifndef INCLUDED_FILE_DIR
#include "../../include/FileDir.h"
#endif

#ifndef INCLUDED_KL_BASE
#include "../../base/KLBase.h"
#endif

#ifndef INCLUDED_CONTEXT
#include "../../share/Context.h"
#endif

#ifndef INCLUDED_STATUS
#include "../../share/Status.h"
#endif

#ifndef INCLUDED_CONTEXT_CFG
#include "../../config/ContextCfg.h"
#endif

#ifndef INCLUDED_STATUS_CFG
#include "../../config/StatusCfg.h"
#endif

#ifndef INCLUDED_SERVER
#include "../../share/Server.h"
#endif

#ifndef INCLUDED_DIR_SCAN
#include "../../base/DirScan.h"
#endif

#ifndef INCLUDED_CONTEXT
#include "../../share/Context.h"
#endif

#define SERVER_RETRY_TIMES	3
struct ServerInfo;
class Connection;

class DaemonServer: public Server
{
    public:
        DaemonServer(const char* pathFile);
        ~DaemonServer();
        int OnTickServer();
        int OnSigAlrm();
        int OnSigTerm();
        int OnSigUser1(SigTag tag, PID_T srcpid);
        static int ClearAllChild();
        //int OnSigUser2(SigTag tag,PID_T srcpid);
    protected:
        int BeforeService();
        void OnHeartBeat(PID_T pid);
        int CheckStatus();
        int ScanServers();
        int KillServers();
        int SuspendServers(bool bResume = false);
        int StartServer(ExeInfo& exeInfo, ServerInfo& serverInfo);
        int KillServer(ExeInfo& exeInfo, ServerInfo& serverInfo);
        int WaitServer(PID_T pid = -1);

        int OnStatusSaveSig(int iForce, PID_T srcpid)
        {
            return StatusCfg::SaveStatus((iForce > 0) ? true : false);
        }
        int WatchDog();
		//suxufeng:[2010-5-2]�澯���ػ�����
		int WatchAlarmLed();
		////suxufeng:[2010-5-2]���е��ػ�����
		//int WatchRunLed();
        bool CheckUpdateKey(const char *ipUpdateKeyFileName = CONS_AUTO_UPDATE_KEY_FILE_NAME);
        //void OnUpdateKeyFind(const char *ipUpdateKeyFileName = CONS_AUTO_UPDATE_KEY_FILE_NAME);
        //void CheckUpdate(const char *ipUpdateKeyFileName = CONS_AUTO_UPDATE_KEY_FILE_NAME);
        //int BatteryGather(int action,int times);

        static bool ClearAllChildProc(int iiDepth, bool ibIsDir, const char* ipDir, const char* ipName, void* ipOther);
        static bool ChildServerProc(int type, int iiDepth, bool ibIsDir, const char* ipDir, const char* ipName, void* ipOther);
		//suxufeng:[2010-6-17]������ͨ��״̬���������е��һ���춼û��ͨ�ųɹ�������λ��������
		void CheckMeterCommStatus();

        int ExecStopScript();
    private:
        DirScan m_DirScan;
        INT8U m_PIDCount;//����PID>0���ӽ��̵���Ŀ
        bool m_bStopWatchDog;
        bool m_bEnableAlrmWatchDog;
        time_t& m_TickSecond;
};

#endif


