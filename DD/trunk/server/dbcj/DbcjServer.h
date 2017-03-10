#ifndef INCLUDED_DBCJ_SERVER
#define INCLUDED_DBCJ_SERVER

#include "../../share/Server.h"
#include "../../share/Meter.h"
#include "../../share/Context.h"
#include "../../share/AlarmFile.h"
#include "../../share/TaskDataFile.h"

class DbcjServer;
struct Tasks;

class CLuxGather
{
public:
	void Gather(Devices &devices);
	void Init(void){m_LuxSensorsBS.reset();}
private:
	DevicesBitSet m_LuxSensorsBS;
};

class DbcjServer:public Server
{
public:
	DbcjServer();
	~DbcjServer();

	//在采集过程中，每完成一个采集动作项，应调用此过程，如果返回false，说明应该立即退出采集，否则可继续
	bool OnGatherTick();
	static DbcjServer *GetCurServer()
	{
		return (DbcjServer *)Server::GetCurServer();
	}

protected:
	int OnTickServer();
	int OnSigAlrm();
private:
	//处理自动路由模块
	int RouterServer();
	int SetFirstRouter();// laosu[2010-12-30] 第一次路由
	void CheckPara();
	void OnParaChanged();
	void OnTasksParaChanged();
	bool BeforeGather(Meter& meter);
	void TestComm(void);
private:
	DbcjServerInfo& m_DbcjServerInfo;
	CLuxGather m_LuxGather;

};



class DbcjBusRuleIntercept:public BusRuleIntercept
{
public:
	DbcjBusRuleIntercept(DbcjServer* pServer):m_pServer(pServer)
	{
		ASSERT(m_pServer);
	}
	bool OnGatherTick()
	{
		if (m_pServer != NULL)
		{
			return (m_pServer->OnGatherTick());
		}
		else
		{
			KL_VERBOSE( "DbcjBusRuleIntercept object is NULL\n");
		}
		return false;
	}
private:
	DbcjServer* m_pServer;
	
};



#endif
