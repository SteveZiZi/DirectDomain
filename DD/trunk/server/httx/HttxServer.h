#ifndef INCLUDED_HTTX_SERVER
#define INCLUDED_HTTX_SERVER

#ifndef INCLUDED_SERVER
#include "../../share/Server.h"
#endif

#ifndef INCLUDED_CONNECTION
#include "../../share/Connection.h"
#endif

#ifndef INCLUDED_IPC_INFO
#include "../../base/IpcInfo.h"
#endif

#ifndef INCLUDED_SYS_IPC
#include "../../share/SysIpc.h"
#endif

#ifndef INCLUDED_CONTEXT
#include "../../share/Context.h"
#endif

#ifndef INCLUDED_DEFINE
#include "../../include/Define.h"
#endif

#ifndef INCLUDED_DEVICE_RULE
#include "../../share/DeviceRule.h"
#endif

#ifndef INCLUDED_DEVICE_RULE_FACTORY
#include "../../drule/DeviceRuleFactory.h"
#endif

#ifndef INCLUDED_CONNECTION_FACTORY
#include "../../conn/ConnectionFactory.h"
#endif

#ifndef INCLUDED_SERVER_CFG
#include "../../config/ServerCfg.h"
#endif

class HttxServer:public Server,ConnectionIntercept
{
public:
	HttxServer(int InstanceServerID);
	int OnTickServer();
	void SetCurConnection(Connection* pConnection);

protected:
	bool PreConnect();
	void PostConnect();
	bool PreDisconnect();
	void PostDisconnect();

	int ChannelServer(Channel& channel,Connection& connection,const HttxPlan& plan);
private:
	void CheckDebug();
	void CheckReport();
	int OnTaskFinished(int task_type,PID_T srcpid);//收到电表采集进程的任务完成信息之后，就置相应的主动上报任务为true，以便在规约中主动上报和补报所有的数据
	int times;
	HttxServerInfo& m_HttxServerInfo;
	Connection* m_pCurConnection;
	const HttxPlan* m_pCurPlan;
	//bool m_isMaster;
	INT16U CurrentID;
};

#endif
