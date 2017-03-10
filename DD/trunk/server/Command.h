#ifndef INCLUDED_COMMAND
#define INCLUDED_COMMAND

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

//命令对象，例如httx1,dbcj1,task等
typedef struct _CMD_OBJ
{
	char* Name;//名称，例如httx,dbcj,task
	INT16U MinID;//最小的实例ID
	INT16U MaxID;//最大的实例ID，如果最小实例ID与最大实例ID相等，则说明只可能有一个实例
}CMD_OBJ;

struct _CMD;
typedef int (*CMD_HANDLER_PROC)(struct _CMD* pCmd,void* pOther);
typedef int (*CMD_OBJ_HANDLER_PROC)(struct _CMD* pCmd,CMD_OBJ* pCmdObj,INT16U InstanceID,const char* pPropName,const char* pPropVal,void* pOther);

typedef struct _CMD
{
	char* Key;
	char* Format;
	CMD_OBJ* Objects;
	CMD_HANDLER_PROC BeforeCmdHandler;
	CMD_OBJ_HANDLER_PROC CmdObjHandler;
	CMD_HANDLER_PROC AfterCmdHandler;
	void* Arg;
	char* Desc;
}CMD;


#define BEGIN_CMD_OBJ_MAP(x) static CMD_OBJ x[] = {
#define END_CMD_OBJ_MAP()   {NULL,0,0}};
#define CMD_OBJ_ENTRY(Name,MinID,MaxID){Name,MinID,MaxID},

#define BEGIN_CMD_MAP(x) static CMD x[] = {
#define END_CMD_MAP()   {NULL,NULL,NULL,NULL,NULL,NULL,NULL}};
#define CMD_ENTRY(Key,Format,Objects,BeforeCmdHandler,CmdObjHandler,AfterCmdHandler,HandlerArg,Desc){Key,Format,Objects,BeforeCmdHandler,CmdObjHandler,AfterCmdHandler,HandlerArg,Desc},


#define PROP_FLAG "."
#define PROP_DELIMITER "="
#define PROP_LINE "-"
#define PROP_ULINE "_"
#define PROP_LBRACKET "["
#define PROP_RBRACKET "]"
#define PROP_CURVE "~"
#define PROP_COMMA ","

struct ServerInfo;
class KLDump;

class Command
{
public:
	Command(int argc,char* argv[]);
	void Help();

	int Exec();
	inline int GetArgCount()
	{
		return m_argc;
	}
	inline char** GetArg()
	{
		return m_argv;
	}

	static int OnStart(CMD* pCmd,CMD_OBJ* cmdObj,INT16U id,const char *propName,const char* propVal,void* pOther);
	static int OnExec(CMD* pCmd,CMD_OBJ* cmdObj,INT16U id,const char *propName,const char* propVal,void* pOther);
	static int OnStop(CMD* pCmd,CMD_OBJ* cmdObj,INT16U id,const char *propName,const char* propVal,void* pOther);
	static int OnSet(CMD* pCmd,CMD_OBJ* cmdObj,INT16U id,const char *propName,const char* propVal,void* pOther);
	static int OnSave(CMD* pCmd,CMD_OBJ* cmdObj,INT16U id,const char *propName,const char* propVal,void* pOther);
	static int OnList(CMD* pCmd,CMD_OBJ* cmdObj,INT16U id,const char *propName,const char* propVal,void* pOther);
	static int OnAction(CMD* pCmd,CMD_OBJ* cmdObj,INT16U id,const char *propName,const char* propVal,void* pOther);
    static int OnBuild(CMD* pCmd,CMD_OBJ* cmdObj,INT16U id,const char *propName,const char* propVal,void* pOther);

	static int StartDaemon(CMD* pCmd,void* pOther);
private:
	CMD_OBJ* GetCmdObj(char* pArg);
	int CheckKey();
	int HandleCommand(CMD_OBJ* cmdObj,INT32S minID,INT32S maxID,const char* propName,const char* propVal);
	int InitContext();
	static void ParseArgForCmdObj(CMD_OBJ* pCmdObj,char* pArg,INT32S& orMinID,INT32S& orMaxID,string& orPropName,string& orPropVal);
	static void ParseStatusParam(CMD_OBJ* pCmdObj,char* param,INT32S& orMinID,INT32S& orMaxID,string& orPropName,string& orPropVal);
	static int OnSetEnergy(INT8U id,const char *propName,const char* propVal);
	static int OnSetMeter(KLDump& dump,INT8U id,const char *propName,const char* propVal);
	static int OnSetChannel(KLDump& dump,INT8U id,const char *propName,const char* propVal);
	static int OnSetTrace(KLDump& dump,ServerInfo& serverInfo,const char *propName,const char* propVal);
private:
	int m_argc;
	char** m_argv;
	CMD* m_pCmd;
	int m_iKeyIndex;
	int m_iKeyCount;
	static bool m_IsDaemonRunning;
};
#endif

