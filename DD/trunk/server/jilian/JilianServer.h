/**********************************主从级联进程***************************************
说明：
1、主要有四个功能：一、透传功能；二、级联轮询功能；三、配变终端日冻结电量抄收；四、集中器地址
2、主从级联所需连接：一个485永久独占连接。
3、主从级联进程启动条件：集中器必须被配置为主集中器。
4、流程：
	（1）透传流程：
		A、空闲时间（默认5分钟，测试时为5秒）轮询共享内存，查看是否有需要转发的命令；
		B、通过第四个485线路（COM7）建立连接，等待从集中器的数据；
		C、接收到从集中器数据，发送给后台通讯进程（写共享内存）。
	（2）级联功能：
		A、每五分钟依次轮询四个从集中器，如有上报需求则即刻通知后台通讯进程进行上报；
		B、每次上报只报一帧的上报记录，其余数据必须在收到确认或者传输控制之后才能继续发送；
		C、每执行一次级联轮询之前应先检查共享内存内容，如有需要转发的命令则立即转发。
	（3）抄表功能：
		A、抄表的过程当中不处理任何主站命令；
		B、从集中器采表的过程中，主集中器只作监听，不作回应。
	（4）主动升级从集中器的功能
		A、升级的过程当中不处理任何主站命令；
		B、升级重试三次不成功就退出不再对从集中器进行升级；
		C、主集中器升级后不删除update.tgz文件。
		D、升级模块的功能的协议通信方式和级联功能、透传功能的协议通信方式不同，不可用同样的处理方法
5、注意事项：
	A、主从级联进程不可在从集中器中运行；
	B、主从级联只做透传和轮询工作，不解析规约。
**************************************************************************************/

#ifndef INCLUDED_JILIAN_SERVER
#define INCLUDED_JILIAN_SERVER

#ifndef INCLUDED_SERVER
#include "../../share/Server.h"
#endif

#ifndef INCLUDED_CONTEXT
#include "../../share/Context.h"
#endif

#ifndef INCLUDED_CHANNEL
#include "../../share/Channel.h"
#endif

#ifndef INCLUDED_DEFINE
#include "../../include/Define.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../../share/SysBase.h"
#endif

#ifndef INCLUDED_TASK
#include "../../share/Task.h"
#endif

#ifndef INCLUDED_TASK_DATA_FILE
#include "../../share/TaskDataFile.h"
#endif

#ifndef INCLUDED_SYS_IPC
#include "../../share/SysIpc.h"
#endif

#ifndef INCLUDED_IPC_INFO
#include "../../base/IpcInfo.h"
#endif

#ifndef _CCEMAN_H_
#include "../../base/CCEMan.h"
#endif

#define CONS_SLAVE_MACHINE_COUNT 4		//4台从终端

#define CONST_JILIAN_DEFAULT_TIME_OUT_MS 5000	//单位是毫秒
#define CONST_JILIAN_PACKAGES_BUFFER_SIZE 50	//主集中器只缓存CONST_JILIAN_PACKAGES_BUFFER_SIZE条命令，从终端应当控制好流量，而不应该同时向主终端发送两帧以上的数据

class Connection;
class JilianServer;

class JilianServer:public Server
{
public:
	JilianServer(int InstanceServerID);
	~JilianServer();
	int OnTickServer();//进程核心
protected:
	int GatherSlaveTermianl(Channel &channel,Connection *irConnection);//采集与自己配对的配变终端

	bool IsNeedGather();//判断是否需要进行采表
	void GetTerminalAddr(const int iSepNum,INT8U *oAddr);//传入一个序号，获取级联从集中器的地址
	void GetFrameHead(bstring &irDataBuff,INT8U *irRTUA,INT8U iCtrlNo);//获取规约头
	void GetFrameFeed(bstring &irDataBuff);//获取规约尾
	int OnReceivePackage(bstring &irRecvData,Channel &channel,Connection *irConnection);
	bool CheckAddr(bstring &irData);
	int OnReceiveData(bstring &irRecvDatas,Channel &channel,Connection *irConnection);//检包函数，将收到的数据进行检查，只要找到两个0x68，一个0x16，并且两个0x68的位置是正确的，则马上进行包检查，然后存起来。
	inline bool IsValidRTUA(INT8U *irRTUA)
	{
		return (!(irRTUA[0] == 0xff && irRTUA[1] == 0xff && irRTUA[2] == 0xff && irRTUA[3] == 0xff));
	}

	int CommandTransffer(Channel &channel,Connection *irConnection,Context &context);//透传功能
	int Jilian(Channel &channel,Connection *irConnection,Context &context,int MachineName);//级联功能
	int SendTransCtrl(Channel &channel,Connection *irConnection,bstring &irRequestData);//传输控制命令
	bool RecvCmd(Channel &channel,Connection *irConnection,int MachineName);//监听从集中器返回命令
	bool CheckPackage(bstring &m_CmdBuff);//包检查函数，用于检查从集中器的上报需求
	bool ReceiveData(Connection *irConnection,Channel &channel,INT32U iiTimeoutMS,INT32U TimeoutMS,bstring &m_DataBuff);//接收数据
	bool RecvData(Connection *irConnection,Channel &channel,INT32U iiTimeoutMS,INT32U TimeoutMS,bstring &m_DataBuff);//接收数据，并且进行超时检查和帧检查
	bool ParsePkg(bstring &Package);
	bool DecompressData(bstring& irCompressedData,bstring& irDecompressedData);
	PARSE_RESULT ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int ParseCompressOrEncryptPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int ParsePackageBase(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int SendToHttxServer(bstring &irRecvData);//将收到的东西发送给后台通信进程

private:
	int SlaveMachineTN;
	bstring m_sParsedBuff;
//#ifndef WIN32
//	CCEMan m_CCEMan;
//#endif

	JilianServerInfo &m_JilianServerInfo;
	Connection *m_pCurConnection;
};

#endif
