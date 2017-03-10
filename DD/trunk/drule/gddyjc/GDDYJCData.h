#ifndef INCLUDED_GDDYJC_DATA
#define INCLUDED_GDDYJC_DATA

#ifndef INCLUDED_STD_INCLUDE
#include "../../include/StdInclude.h"
#endif

#ifndef DATA_CONVERT
#include "data_convert.h"
#endif

#ifndef INCLUDED_CONNECTION
#include "../../share/Connection.h"
#endif

#ifndef INCLUDED_CHANNEL
#include "../../share/Channel.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../../share/SysBase.h"
#endif

#ifndef INCLUDED_GDDYJC_RULE_FUNCTION
#include "GDDYJCRuleFunction.h"
#endif

#ifndef INCLUDED_KL_BASE
#include "../../base/KLBase.h"
#endif

#ifndef INCLUDED_KL_TRACE
#include "../../base/KLTrace.h"
#endif

#ifndef INCLUDED_FILE_BASE
#include "../../base/FileBase.h"
#endif

#ifndef INCLUDED_MODEM
#include "../../base/Modem.h"
#endif

#ifndef INCLUDED_IPC_INFO
#include "../../base/IpcInfo.h"
#endif

#ifndef INCLUDED_CONTEXT
#include "../../share/Context.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../../share/SysBase.h"
#endif

#ifndef INCLUDED_STATUS
#include "../../share/Status.h"
#endif

#ifndef INCLUDED_SYS_IPC
#include "../../share/SysIpc.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../../include/Config.h"
#endif

#ifndef INCLUDED_SYS_CONFIG_CFG
#include "../../config/SysConfigCfg.h"
#endif

#ifndef INCLUDED_METER_CFG
#include "../../config/MeterCfg.h"
#endif

#ifndef INCLUDED_RELAY_CFG
#include "../../config/RelayCfg.h"
#endif

#include "../../config/ClRelayCfg.h"

#ifndef INCLUDED_TASK_CFG
#include "../../config/TaskCfg.h"
#endif

#ifndef INCLUDED_SERVER_CFG
#include "../../config/ServerCfg.h"
#endif

#ifndef INCLUDED_CONTEXT_CFG
#include "../../config/ContextCfg.h"
#endif

#ifndef PBZDCFG
#include "../../config/JilianSlavesCfg.h"
#endif



#ifndef FIXDATACFG
#include "../../config/FixDataCfg.h"
#endif

#define CONS_MAX_FILE_NAME_LEN 164
#define ENABLED 0x01
#define UPDATEFLAG 0x02
#define VALID_PORT_NO_MIN 3
#define VALID_PORT_NO_MAX 5



class GDDYJCData
{
public:
	GDDYJCData()
	{
		InitialPortFunc();
	}
	inline ~GDDYJCData()
	{
	}

	int set_NET(char *i_cStr,int wHttx,int wChannel,INT8U ppp=1);
	int set_GPRS_CDMA(char *i_cStr,int wHttx,int wChannel);
	int set_PSTN(char *i_cStr,int wHttx,int wChannel);
	int set_COM(char *i_cStr,int wHttx,int wChannel);
	int set_CSD(char *i_cStr,int wHttx,int wChannel);
	int set_RADIO(char *i_cStr,int wHttx,int wChannel);//����
	int set_WAKEUP(char *i_cStr,int wHttx,int wChannel);//���Ż���
	int read_COM(bstring& temp,int wHttx,int wChannel);//����������
	int read_NET(bstring& temp,int wHttx,int wChannel);//����������
	int read_PSTN(bstring& temp,int wHttx,int wChannel);//��PSTN����
	int read_GPRS(bstring& temp,int wHttx,int wChannel);//��GPRS����
	int delChannel(char* i_cStr,int wHttx,int wChannel);//ɾ��ͨ�� 1Ϊ�ɹ� 0Ϊʧ��
	void AddUnvalidData(bstring &irData, size_t irLength)
	{
		irData.append(irLength, (INT8U)0xff);
	}

	INT8U Save_8010(char *i_cStr,INT8U limit=0x00);//������վ��ַ 2006-6-8 ��־ǿ+
	INT8U Read_8010(bstring& temp,INT8U limit=0x00);

	INT8U Save_8011(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8011(bstring& temp,INT8U limit=0x00);

	INT8U Save_8012(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8012(bstring& temp,INT8U limit=0x00);

	INT8U Save_8013(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8013(bstring& temp,INT8U limit=0x00);

	INT8U Save_8014(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8014(bstring& temp,INT8U limit=0x00);

	INT8U Save_8015(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8015(bstring& temp,INT8U limit=0x00);

	INT8U Save_8016(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8016(bstring& temp,INT8U limit=0x00);

	INT8U Save_8017(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8017(bstring& temp,INT8U limit=0x00);

	INT8U Save_8018(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8018(bstring& temp,INT8U limit=0x00);

	INT8U Save_8019(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8019(bstring& temp,INT8U limit=0x00);

	INT8U Save_801A(char *i_cStr,INT8U limit=0x00);
	INT8U Read_801A(bstring& temp,INT8U limit=0x00);

	INT8U Save_801C(char *i_cStr,INT8U limit=0x00);
	INT8U Read_801C(bstring& temp,INT8U limit=0x00);

	INT8U Save_801E(char *i_cStr,INT8U limit=0x00);
	INT8U Read_801E(bstring& temp,INT8U limit=0x00);

	INT8U Save_801F(char *i_cStr,INT8U limit=0x00);
	INT8U Read_801F(bstring& temp,INT8U limit=0x00);

	INT8U Save_8020(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8020(bstring& temp,INT8U limit=0x00);

	INT8U Save_8021(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8021(bstring& temp,INT8U limit=0x00);

	INT8U Save_8022(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8022(bstring& temp,INT8U limit=0x00);

	INT8U Save_8030(bstring req,char ct,INT8U limit=0x00);
	INT8U Save_8030(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8030(bstring& temp,INT8U limit=0x00);

	INT8U Save_8031(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8031(bstring& temp,INT8U limit=0x00);

	INT8U Save_8032(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8032(bstring& temp,INT8U limit=0x00);

	//suxufeng:[2010-3-24]
	//��������Ϊ��������ͨ�Ų������ã��������ü������ɱ�����ȵ�ͨ�ſڡ�
	//suxufeng:[2010-3-24]��ȡ���ں�
	PortSet &GetSerialPortSet(int iiPortNo);//suxufeng:[2010-3-24]Ĭ��Ϊ7�ſڣ�������

	//suxufeng:[2010-3-24]������
	INT8U Save_87X0(int iiPortNo,char *i_cStr,INT8U limit=0x00);
	INT8U Read_87X0(int iiPortNo,bstring& temp,INT8U limit=0x00);

	//suxufeng:[2010-3-24]У����
	INT8U Save_87X1(int iiPortNo,char *i_cStr,INT8U limit=0x00);
	INT8U Read_87X1(int iiPortNo,bstring& temp,INT8U limit=0x00);

	//suxufeng:[2010-3-24]����λ
	INT8U Save_87X2(int iiPortNo,char *i_cStr,INT8U limit=0x00);
	INT8U Read_87X2(int iiPortNo,bstring& temp,INT8U limit=0x00);

	//suxufeng:[2010-3-24]ֹͣλ
	INT8U Save_87X3(int iiPortNo,char *i_cStr,INT8U limit=0x00);
	INT8U Read_87X3(int iiPortNo,bstring& temp,INT8U limit=0x00);

	//suxufeng:[2010-3-24]����
	INT8U Save_87X4(int iiPortNo,char *i_cStr,INT8U limit=0x00);
	INT8U Read_87X4(int iiPortNo,bstring& temp,INT8U limit=0x00);

	//suxufeng:[2010-3-24]�����������
	INT8U Save_87XF(int iiPortNo,char *i_cStr,INT8U limit=0x00);
	INT8U Read_87XF(int iiPortNo,bstring& temp,INT8U limit=0x00);


	INT8U Save_803E(char *i_cStr,INT8U limit=0x00);

	INT8U Save_8809(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8809(bstring& temp,INT8U limit=0x00);

	INT8U Save_880A(char *i_cStr,INT8U limit=0x00);
	INT8U Read_880A(bstring& temp,INT8U limit=0x00);

	INT8U Save_8810(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8810(bstring& temp,INT8U limit=0x00);

	INT8U Save_8811(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8811(bstring& temp,INT8U limit=0x00);

	INT8U Save_8812(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8812(bstring& temp,INT8U limit=0x00);

	INT8U Save_8813(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8813(bstring& temp,INT8U limit=0x00);

	INT8U Save_8814(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8814(bstring& temp,INT8U limit=0x00);

	INT8U Save_8815(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8815(bstring& temp,INT8U limit=0x00);

	INT8U Save_8816(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8816(bstring& temp,INT8U limit=0x00);
	void SaveTask(INT8U m_Space,INT8U m_TimeLock);

	INT8U Save_8817(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8817(bstring& temp,INT8U limit=0x00);

	INT8U Save_8818(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8818(bstring& temp,INT8U limit=0x00);

	INT8U Save_8819(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8819(bstring& temp,INT8U limit=0x00);

	INT8U Save_881A(char *i_cStr,INT8U limit=0x00);
	INT8U Read_881A(bstring& temp,INT8U limit=0x00);

	INT8U Save_881B(char *i_cStr,INT8U limit=0x00);
	INT8U Read_881B(bstring& temp,INT8U limit=0x00);

	INT8U Save_881C(char *i_cStr,INT8U limit=0x00);
	INT8U Read_881C(bstring& temp,INT8U limit=0x00);

	INT8U Save_881D(char *i_cStr,INT8U limit=0x00);
	INT8U Read_881D(bstring& temp,INT8U limit=0x00);

	// void Save_881E(char *i_cStr);
	// INT8U Read_881E(bstring& temp);

	INT8U Save_881F(char *i_cStr,INT8U limit=0x00);
	INT8U Read_881F(bstring& temp,INT8U limit=0x00);

	//INT8U Save_882F(char *i_cStr,INT8U limit=0x00);
	//INT8U Read_882F(bstring& temp,INT8U limit=0x00);

	INT8U Save_8820(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8820(bstring& temp,INT8U limit=0x00);

	INT8U Save_8821(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8821(bstring& temp,INT8U limit=0x00);

	INT8U Save_8822(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8822(bstring& temp,INT8U limit=0x00);

	INT8U Save_8823(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8823(bstring& temp,INT8U limit=0x00);

	//��ȡ������δ���������ն���Ϣ����ΰ01.07��
	INT8U Read_882A(bstring& temp,INT8U limit=0);
	//��ȡ������·����Ϣ����ΰ01.07��
	INT8U Read_882B(bstring& temp,INT8U limit=0);

	//���ü���������ģ������
	INT8U Save_88A1(const INT8U &busType,INT8U limit=0);

	//INT8U Save_8901(char *i_cStr,int meterNo,INT8U limit=0x00);
	// INT8U Read_8901(bstring& temp);

	//INT8U Save_8902(char *i_cStr,int meterNo,INT8U limit=0x00);
	// INT8U Read_8902(bstring& temp);


	INT8U Save_8904(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8904(bstring& temp);

	INT8U Save_8944(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8944(bstring& temp);

	INT8U Save_8945(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8945(bstring& temp);

	INT8U Save_8946(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8946(bstring& temp);

	INT8U Save_8947(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8947(bstring& temp);

	INT8U Save_8948(char *i_cStr,INT8U limit=0x00);
	INT8U Read_8948(bstring &temp,INT32U code,INT8U limit=0x00);
	//int read_8903(bstring& temp,int tn,INT8U limit=0x00);
public://��������
	INT8U Read_9010(int tn,bstring& temp,INT8U limit=0);
public: //RCU���
	INT8U Save_0101(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0101(bstring& temp,INT8U limit=0x00);
	INT8U Save_0102(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0102(bstring& temp,INT8U limit=0x00);
	INT8U Save_0103(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0103(bstring& temp,INT8U limit=0x00);
	INT8U Save_0104(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0104(bstring& temp,INT8U limit=0x00);
	INT8U Save_0105(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0105(bstring& temp,INT8U limit=0x00);
	INT8U Read_0106(bstring& temp,INT8U limit=0x00);
	INT8U Read_0107(bstring& temp,INT8U limit=0x00);
	INT8U Read_0108(bstring& temp,INT8U limit=0x00);
	INT8U Save_0108(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0109(bstring& temp,INT8U limit=0x00);
	INT8U Save_0109(char *i_cStr,INT8U limit=0x00);
	INT8U Read_010a(bstring& temp,INT8U limit=0x00);
	INT8U Save_010a(char *i_cStr,INT8U limit=0x00);
	INT8U Read_010d(bstring& temp,INT8U limit=0x00);
	INT8U Save_010d(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0111(bstring& temp,INT8U limit=0x00);
	INT8U Save_0111(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0112(bstring& temp,INT8U limit=0x00);
	INT8U Save_0112(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0113(bstring& temp,INT8U limit=0x00);
	INT8U Save_0113(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0140(bstring& temp,INT8U limit=0x00);
	INT8U Save_0140(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0142(bstring& temp,INT8U limit=0x00);
	INT8U Save_0142(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0143(bstring& temp,INT8U limit=0x00);
	INT8U Save_0143(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0144(bstring& temp,INT8U limit=0x00);
	INT8U Save_0144(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0145(bstring& temp,INT8U limit=0x00);
	INT8U Save_0145(char *i_cStr,INT8U limit=0x00);
	INT8U Read_0146(bstring& temp,INT8U limit=0x00);
	INT8U Save_0146(char *i_cStr,INT8U limit=0x00);

	INT8U Save_0201(char *i_cStr,INT8U limit=0x00);

	INT8U Read_0202(bstring& temp,INT8U limit=0x00);
	INT8U Read_0203(bstring& temp,INT8U limit=0x00);
	INT8U Read_0210(bstring& temp,INT8U limit=0x00);
	INT8U Read_0211(bstring& temp,INT8U limit=0x00);
	INT8U Read_0212(bstring& temp,INT8U limit=0x00);
	INT8U Read_0213(bstring& temp,INT8U limit=0x00);
	INT8U Read_0214(bstring& temp,INT8U limit=0x00);
	INT8U Read_0215(bstring& temp,INT8U limit=0x00);
	INT8U Read_0216(bstring& temp,INT8U limit=0x00);
	INT8U Read_0217(bstring& temp,INT8U limit=0x00);
	INT8U Read_0218(bstring& temp,INT8U limit=0x00);
	INT8U Read_0219(bstring& temp,INT8U limit=0x00);
	INT8U Read_021a(bstring& temp,INT8U limit=0x00);
	INT8U Read_021b(bstring& temp,INT8U limit=0x00);
	INT8U Read_021c(bstring& temp,INT8U limit=0x00);
	INT8U Read_021d(bstring& temp,INT8U limit=0x00);
	INT8U Read_021e(bstring& temp,INT8U limit=0x00);
	INT8U Read_021f(bstring& temp,INT8U limit=0x00);
	
	INT8U Save_0211(char *i_cStr,INT8U limit=0x00);
	INT8U Save_0212(char *i_cStr,INT8U limit=0x00);
	INT8U Save_0213(char *i_cStr,INT8U limit=0x00);
	INT8U Save_0214(char *i_cStr,INT8U limit=0x00);
	INT8U Save_0215(char *i_cStr,INT8U limit=0x00);
	INT8U Save_0216(char *i_cStr,INT8U limit=0x00);
	INT8U Save_0217(char *i_cStr,INT8U limit=0x00);
	INT8U Save_0218(char *i_cStr,INT8U limit=0x00);
	INT8U Save_0219(char *i_cStr,INT8U limit=0x00);
	INT8U Save_021a(char *i_cStr,INT8U limit=0x00);
	INT8U Save_021b(char *i_cStr,INT8U limit=0x00);
	INT8U Save_021c(char *i_cStr,INT8U limit=0x00);
	INT8U Save_021d(char *i_cStr,INT8U limit=0x00);
	INT8U Save_021e(char *i_cStr,INT8U limit=0x00);
	INT8U Save_021f(char *i_cStr,INT8U limit=0x00);
public://���������
	INT8U Read_0401(int tn,bstring& temp,INT8U limit=0x00);
	INT8U Read_040F(bstring& temp,INT8U limit=0x00);
	INT8U Save_0501(char *i_cStr,int& index,INT8U limit);

	INT8U Read_0511(bstring& temp,INT8U limit=0x00);
	INT8U Read_0512(bstring& temp,INT8U limit=0x00);
	INT8U Read_0513(bstring& temp,INT8U limit=0x00);
	INT8U Read_0514(bstring& temp,INT8U limit=0x00);
	INT8U Read_0515(bstring& temp,INT8U limit=0x00);
	INT8U Read_0516(bstring& temp,INT8U limit=0x00);
	INT8U Read_0517(bstring& temp,INT8U limit=0x00);
	INT8U Read_0518(bstring& temp,INT8U limit=0x00);
	INT8U Read_0519(bstring& temp,INT8U limit=0x00);
	INT8U Read_051a(bstring& temp,INT8U limit=0x00);
	INT8U Read_051b(bstring& temp,INT8U limit=0x00);
	INT8U Read_051c(bstring& temp,INT8U limit=0x00);

	INT8U Read_051F(bstring& temp,INT8U limit=0x00);
	INT8U Save_052F(char *i_cStr,int& index,INT8U limit);
	INT8U Save_053F(char *i_cStr,int& index,INT8U limit);
	INT8U Save_054F(char *i_cStr,int& index,INT8U limit);
	bool GetTaskData(INT8U taskno,bstring &odata);	

public://������������	
	INT8U Read_897F(bstring& temp,INT8U limit=0x00);
private:
	INT8U ReadDeviceStatus(bstring& temp,INT8U tn);
	INT8U WriteDeviceStatus(char *i_cStr,INT8U tn);

private:	
	void FillRcuStatus(bstring& temp);
	bool FillRelay8Status(bstring& temp, INT8U *dataPtr);
	bool FillRelay2Status(bstring& temp, INT8U *dataPtr);
	bool FillLight4Status(bstring& temp, INT8U *dataPtr);
	bool FillLED12V3Status(bstring& temp, INT8U *dataPtr);
	bool FillAirconditionStatus(bstring& temp, INT8U *dataPtr);
private:
	int save_8904(char *i_cStr,INT8U limit=0x00);
	int read_8904(bstring& temp,INT32U code,INT8U limit=0x00);
	int save_8944(char *i_cStr,INT8U limit=0x00);
	int read_8944(bstring& temp,INT32U code,INT8U limit=0x00);
	int save_8945(char *i_cStr,INT8U limit=0x00);
	int read_8945(bstring& temp,INT32U code,INT8U limit=0x00);
	int save_8946(char *i_cStr,INT8U limit=0x00);
	int read_8946(bstring& temp,INT32U code,INT8U limit=0x00);
	int save_8947(char *i_cStr,INT8U limit=0x00);
	int read_8947(bstring& temp,INT32U code,INT8U limit=0x00);

	//int save_894F(char *i_cStr,INT8U limit=0x00);
	//int read_894F(bstring& temp,INT32U code,INT8U limit=0x00);
public:
	//INT8U Save_8940(char *i_cStr,int meterNo,INT8U limit=0x00);
	// INT8U Read_8940(bstring& temp);

	INT8U Save_8941(char *i_cStr,int meterNo,INT8U limit=0x00);
	// INT8U Read_8941(bstring& temp);

	INT8U Save_8943(char *i_cStr,int meterNo,INT8U limit=0x00);
	// INT8U Read_8943(bstring& temp);

	INT8U Save_C117(char *i_cStr,INT8U limit=0x00);
	INT8U Read_C117(bstring& temp,INT8U limit=0x00);

	INT8U Save_894D(char *i_cStr,INT8U limit=0x00);//����ɾ���޸Ĳɼ��ն�
	INT8U Save_895D(char *i_cStr,INT8U limit=0x00,int *pos=0);//����ɾ���޸Ĳɼ��ն�
	INT8U Read_894D(bstring &temp,INT32U code,INT8U limit=0x00);//�ж��ɼ��ն�����

	INT8U Save_894E(char *i_cStr,INT8U limit=0x00);//����ɾ���޸ı��
	INT8U Save_895E(char *i_cStr,INT8U limit=0x00);
	//INT8U SetMeter(char *i_cStr,INT8U limit=0x00,int pl=0);
	INT8U Read_894E(bstring &temp,INT32U code,INT8U limit=0x00);//�ж��������
	/*��������д�������������
	��������setmeter
	�����б� 	Meter& meter������..�������
	Relay& relay���������ն˻�����
	Router& router������.·�ɻ�����
	Char* SetParam�������������ַ���
	INT8U operate������..��/ɾ/�ı�ʶ��
	˵�����ú�������SetParamָ���ĵ���������meter��relay��router������ߡ�

	*/
//	INT8U SetMeterCache(Meter& meter,Relay& relay,Router& router,INT16U* routerno,char* SetParam,INT8U operate);
	INT8U SetDeviceCache(Device& device,char* SetParam,INT8U operate);
	INT8U SetDevicesCache(char* SetParam,int *pos);

	INT8U Save_895F(char *i_cStr,INT8U limit,int *pos);
	INT8U Save_896F(char *i_cStr,INT8U limit,int *pos);	
	INT8U AddDevicesParam(char* SetParam,int *pos);
	INT8U DelDevicesParam(char* SetParam,int *pos);
	
	INT8U Save894D(char *i_cStr,INT8U limit);
	/*	��������WriteToMem
	�����б�	Meter& meter������..�������
	Relay& relay���������ն˻�����
	Router& router������.·�ɻ�����
	*/	
	//INT8U WriteToMem(Meter meter[],Relay relay[],Router router[],INT16U routerno[][6],int num,INT8U operate,bool check=false);
//	INT8U WriteToMem(Meter meter[],Relay relay[],Router router[],INT16U routerno[][6],int num,INT8U operate,bool check=false);
//	INT8U WriteToMem2(Meter meter[],Relay relay[],Router router[],INT16U routerno[][6],Relays& relays,Routers& routers,int num,INT8U operate,bool check=false);
//	bool lsValidParam(Meter meter[],Relay relay[],Router router[],INT16U routerno[][6],int num,INT8U operate);
	Meter lsHaveMeter(Meter meter[],INT16U TN,int num);
	Relay lsHaveRelay(Relay relay[],INT16U TN,int num);
//	Router lsHaveRouter(Router router[],INT16U TN,int num);
	bool delRelayMeter(Relay& relay,Meter meter);
	bool copyToVip(Meter& meter,Meter meters[],Relays& relays,Routers& routers);
	bool copyToUserMeter(Meter& meter,Meter meters[],Relays& relays,Routers& routers);
	int lsVIPMeter(Meter& meter,int tn,Meter meters[]);
	int CheckRelayRouter(Meter meter[],int meternum,Relays& relays,Routers& routers);

	INT8U Save_894F(char *i_cStr,INT8U limit=0x00);//�����޸�ɾ���ɼ��ն�
	INT8U Read_894F(bstring &temp,INT32U code,INT8U limit=0x00);//

	INT8U Save_8950(INT8U *i_cStr,INT8U limit=0x00);//����ɾ���޸�·�ɱ�
	bool Check8950(INT8U *i_cStr);
	INT8U Read_8950(bstring &temp,INT32U code,INT8U limit=0x00);//�ж�·������

	INT8U Save_8951(char *i_cStr,INT8U limit=0x00);//�޸�·�ɷ�ʽ
	INT8U Read_8951(bstring &temp,INT32U code,INT8U limit=0x00);//

	INT8U Save_8952(char *i_cStr,INT8U limit=0x00);//�޸ĸ��ٵ��������ļ�
	INT8U Read_8952(bstring &temp,INT32U code,INT8U limit=0x00);//�ж��޸ĸ��ٵ��������ļ�
	INT8U Save_8940(char *i_cStr,int meterNo,INT8U limit=0x00);

	//���º����������2007-7-25��ӣ����ڴ��������չ�ļ������������˺��������չ�Ĳ������ڴ˺�����Ӵ�����
	INT8U Read_8A01(bstring &temp,INT8U limit=0x00);
	INT8U Save_8A01(INT8U *i_cStr,INT8U limit=0x00);

	INT8U Save_8A03(INT8U *i_cStr,INT8U limit=0x00);
	INT8U Read_8A03(bstring &temp,INT8U limit=0x00);

	INT8U Save_8A10(INT8U *i_cStr,INT8U limit=0x00);
	INT8U Read_8A10(bstring &temp,INT8U limit=0x00);

	INT8U Save_8A11(INT8U *i_cStr,INT8U limit=0x00);
	INT8U Read_8A11(bstring &temp,INT8U limit=0x00);

	INT8U Save_8A2C(INT8U *i_cStr,INT8U limit=0x00);
	INT8U Read_8A2C(bstring &temp,INT8U limit=0x00);

	
	INT8U Read_8A2D(bstring &temp,INT8U limit=0x00);
	

	int datalen(unsigned char ed,unsigned char bg);

	int GetRegData(unsigned char ed,unsigned char bg,bstring& temp,unsigned char data[8],bool right=true);
	int GetRelayNo(RelayBitSet& RBS);
	int GetRouterNo(RouterBitSet& ROBS);

	void closejilian();
	void openjilian();
	INT8U compressChars(char c1,char c2);
	INT8U delmeter(int tn, const char *i_cStr);
	bool checkFF(char* temp,int num);
	INT8U Save_8A2D(unsigned char *c_Src,INT8U limit=0);

private:
	void InitialPortFunc(void);
	void SetPort(INT8U PortNo,INT32U baund,INT8S DataBit,INT8S StopBit,INT8S Parity,INT8U Function);	
private:
	INT8U m_RingTimes;//���������Ӧ�����Ժ����
//#ifdef WIN32
//	void test();
//#endif
};
bool get_set(bstring& rep,int tn,unsigned char cha0,unsigned char cha1,INT32U packageLen,GDDYJCData& gde,INT8U limit=0x00);

bool getRegData(bstring &rep,unsigned char ed,unsigned char bg,RegItems &regitems,INT32U packageLen,GDDYJCData& gde,INT8U limit=0x00);
#endif
