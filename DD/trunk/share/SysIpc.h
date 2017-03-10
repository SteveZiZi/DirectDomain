//消息管道，用于进程间发送消息
#ifndef INCLUDED_SYS_IPC
#define INCLUDED_SYS_IPC

#ifndef INCLUDED_CONTEXT
#include "Context.h"
#endif

#ifndef INCLUDED_SERVER
#include "Server.h"
#endif

#ifndef INCLUDED_MESSAGE
#include "../base/Message.h"
#endif

class IpcInfo;

class SysIpc
{
public:
	int SendToAll(IpcInfo& info,bool bData,bool exceptSelf=true);
	inline static int SendToDaemon(IpcInfo& info,bool bData)
	{
		return SendToServer(info,CONTEXT.ServerInfos.Daemon.BaseInfo,bData);
	}
	inline static int SendToPPP(IpcInfo& info,bool bData)
	{
		return SendToServer(info,CONTEXT.ServerInfos.PPP.BaseInfo,bData);
	}

	//suxufeng:[2010-3-31]去除work进程
	//inline static int SendToWork(IpcInfo& info,bool bData)
	//{
	//	return SendToServer(info,CONTEXT.ServerInfos.Work.BaseInfo,bData);
	//}

	//inline static int SendToUpdate(IpcInfo& info,bool bData)
	//{
	//	return SendToServer(info,CONTEXT.ServerInfos.Update.BaseInfo,bData);
	//}

	inline static int SendToDbcj(IpcInfo& info,bool bData)
	{
		return SendToServer(info,CONTEXT.ServerInfos.Dbcj.BaseInfo,bData);
	}

	static int SendToHttx(INT8U instanceID,IpcInfo& info,bool bData);
	
	static int SendToJilian(INT8U instanceID,IpcInfo& info,bool bData);

	inline static bool HasNotify()
	{
		return s_NotifyMessage.HasMsg(Server::GetCurServerID());
	}
	inline static bool HasData()
	{
		return s_DataMessage.HasMsg(Server::GetCurServerID());
	}
	inline static int ReceiveNotify(IpcInfo& oInfo)
	{
		return ReceiveIpcInfo(s_NotifyMessage,Server::GetCurServerID(),oInfo);
	}
	inline static int ReceiveData(IpcInfo& oInfo)
	{
		return ReceiveIpcInfo(s_DataMessage,Server::GetCurServerID(),oInfo);
	}
	inline static void ClearIpc()
	{
		s_NotifyMessage.Destroy();
		s_DataMessage.Destroy();
	}
	static bool LcdtoDbcj(IPC_INFO_TYP type, bstring &strData);
	static bool LcdtoHttx1(IPC_INFO_TYP type, bstring &strData);

private:
	static int SendToServer(IpcInfo& info,ServerInfo& serverInfo,bool bData);
	static int SendIpcInfo(Message& message,IpcInfo& info,INT16U destServerID,INT16U srcServerID);
	static int ReceiveIpcInfo(Message& message,long lMsgType,IpcInfo& oInfo);
private:
	static Message s_NotifyMessage;
	static Message s_DataMessage;
};
	
#endif


