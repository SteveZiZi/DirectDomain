/**********************************���Ӽ�������***************************************
˵����
1����Ҫ���ĸ����ܣ�һ��͸�����ܣ�����������ѯ���ܣ���������ն��ն���������գ��ġ���������ַ
2�����Ӽ����������ӣ�һ��485���ö�ռ���ӡ�
3�����Ӽ��������������������������뱻����Ϊ����������
4�����̣�
	��1��͸�����̣�
		A������ʱ�䣨Ĭ��5���ӣ�����ʱΪ5�룩��ѯ�����ڴ棬�鿴�Ƿ�����Ҫת�������
		B��ͨ�����ĸ�485��·��COM7���������ӣ��ȴ��Ӽ����������ݣ�
		C�����յ��Ӽ��������ݣ����͸���̨ͨѶ���̣�д�����ڴ棩��
	��2���������ܣ�
		A��ÿ�����������ѯ�ĸ��Ӽ������������ϱ������򼴿�֪ͨ��̨ͨѶ���̽����ϱ���
		B��ÿ���ϱ�ֻ��һ֡���ϱ���¼���������ݱ������յ�ȷ�ϻ��ߴ������֮����ܼ������ͣ�
		C��ÿִ��һ�μ�����ѯ֮ǰӦ�ȼ�鹲���ڴ����ݣ�������Ҫת��������������ת����
	��3�������ܣ�
		A������Ĺ��̵��в������κ���վ���
		B���Ӽ������ɱ�Ĺ����У���������ֻ��������������Ӧ��
	��4�����������Ӽ������Ĺ���
		A�������Ĺ��̵��в������κ���վ���
		B�������������β��ɹ����˳����ٶԴӼ���������������
		C����������������ɾ��update.tgz�ļ���
		D������ģ��Ĺ��ܵ�Э��ͨ�ŷ�ʽ�ͼ������ܡ�͸�����ܵ�Э��ͨ�ŷ�ʽ��ͬ��������ͬ���Ĵ�����
5��ע�����
	A�����Ӽ������̲����ڴӼ����������У�
	B�����Ӽ���ֻ��͸������ѯ��������������Լ��
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

#define CONS_SLAVE_MACHINE_COUNT 4		//4̨���ն�

#define CONST_JILIAN_DEFAULT_TIME_OUT_MS 5000	//��λ�Ǻ���
#define CONST_JILIAN_PACKAGES_BUFFER_SIZE 50	//��������ֻ����CONST_JILIAN_PACKAGES_BUFFER_SIZE��������ն�Ӧ�����ƺ�����������Ӧ��ͬʱ�����ն˷�����֡���ϵ�����

class Connection;
class JilianServer;

class JilianServer:public Server
{
public:
	JilianServer(int InstanceServerID);
	~JilianServer();
	int OnTickServer();//���̺���
protected:
	int GatherSlaveTermianl(Channel &channel,Connection *irConnection);//�ɼ����Լ���Ե�����ն�

	bool IsNeedGather();//�ж��Ƿ���Ҫ���вɱ�
	void GetTerminalAddr(const int iSepNum,INT8U *oAddr);//����һ����ţ���ȡ�����Ӽ������ĵ�ַ
	void GetFrameHead(bstring &irDataBuff,INT8U *irRTUA,INT8U iCtrlNo);//��ȡ��Լͷ
	void GetFrameFeed(bstring &irDataBuff);//��ȡ��Լβ
	int OnReceivePackage(bstring &irRecvData,Channel &channel,Connection *irConnection);
	bool CheckAddr(bstring &irData);
	int OnReceiveData(bstring &irRecvDatas,Channel &channel,Connection *irConnection);//������������յ������ݽ��м�飬ֻҪ�ҵ�����0x68��һ��0x16����������0x68��λ������ȷ�ģ������Ͻ��а���飬Ȼ���������
	inline bool IsValidRTUA(INT8U *irRTUA)
	{
		return (!(irRTUA[0] == 0xff && irRTUA[1] == 0xff && irRTUA[2] == 0xff && irRTUA[3] == 0xff));
	}

	int CommandTransffer(Channel &channel,Connection *irConnection,Context &context);//͸������
	int Jilian(Channel &channel,Connection *irConnection,Context &context,int MachineName);//��������
	int SendTransCtrl(Channel &channel,Connection *irConnection,bstring &irRequestData);//�����������
	bool RecvCmd(Channel &channel,Connection *irConnection,int MachineName);//�����Ӽ�������������
	bool CheckPackage(bstring &m_CmdBuff);//����麯�������ڼ��Ӽ��������ϱ�����
	bool ReceiveData(Connection *irConnection,Channel &channel,INT32U iiTimeoutMS,INT32U TimeoutMS,bstring &m_DataBuff);//��������
	bool RecvData(Connection *irConnection,Channel &channel,INT32U iiTimeoutMS,INT32U TimeoutMS,bstring &m_DataBuff);//�������ݣ����ҽ��г�ʱ����֡���
	bool ParsePkg(bstring &Package);
	bool DecompressData(bstring& irCompressedData,bstring& irDecompressedData);
	PARSE_RESULT ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int ParseCompressOrEncryptPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int ParsePackageBase(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterParsedBegin,bstring::iterator& iterParsedEnd);
	int SendToHttxServer(bstring &irRecvData);//���յ��Ķ������͸���̨ͨ�Ž���

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
