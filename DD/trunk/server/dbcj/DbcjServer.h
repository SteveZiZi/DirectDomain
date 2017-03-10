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

	//�ڲɼ������У�ÿ���һ���ɼ������Ӧ���ô˹��̣��������false��˵��Ӧ�������˳��ɼ�������ɼ���
	bool OnGatherTick();
	static DbcjServer *GetCurServer()
	{
		return (DbcjServer *)Server::GetCurServer();
	}

protected:
	int OnTickServer();
	int OnSigAlrm();
private:
	//�����Զ�·��ģ��
	int RouterServer();
	int SetFirstRouter();// laosu[2010-12-30] ��һ��·��
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
