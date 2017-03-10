/**
*@file		�ļ�����(LcdServer.h)
*@brief		Һ�����̵ĵ�������
*
*@version	Ver 1.0
*
*@date		2010-03-10
*@author	������
*/

#ifndef INCLUDED_LCD_SERVER
#define INCLUDED_LCD_SERVER

#ifndef INCLUDED_SERVER
#include "../../share/Server.h"
#endif

#include "LcdBase.h"
#include "LcdMenu.h"
#include "DataGwjc/LcdItemsGwjc.h"
#include "DataGwjc/LcdDataGwjc.h"
#include "../../share/SysBase.h"

#define CONS_REBACK_WAITTIME	15000
#define CONS_HEADCHAGE_WAITTIME	5000
#define CONS_TURNDISP_WAITTIME	300000
#define CONS_SELECHAR_ROW	4
#define CONS_SELECHAR_COL	25
#define CONS_NOKEY_TIMEOUT	600000
#define CONS_GATHER_TIMEOUT	60000
#define CONS_SEARCH_TIMEOUT	1800000 //��Сʱ
#define CONS_REPORT_TIMEOUT	60000 //1����
#define CONS_NETWORKING_TIMEOUT	3600000 //��Сʱ

#define CONS_ESC_CNT	5
#define CONS_PROJECT_TIMEOUT	600000 



#define CONS_LCD_HIGH_PASSWORD	"123456"		
#define CONS_LCD_SUPPER_PASSWORD	"999999"

typedef LcdDataGwjc DATACLS;

class LcdServer:public Server
{
public:
	LcdServer();
	int OnTickServer();
	int BeforeService();
private:
	int GetMainMenu();	//ȡ���˵�
	int GetMenuDesc();	//���¼��˵���ȡ�¼��˵������¼��˵�����0,��Ҫ��������ģ������������룬�ɹ���30������Ч��ʧ�ܷ���-1��
	int GetPredMenu();	//���ϼ��˵���ȡ�ϼ��˵������ϼ��˵�����0
	int GetData(int i_iKey);		//ȡ��
	int CheckingKey();	//������⣬���ؼ�ֵ����ֵΪ0��ʾ5���ڶ��ް���
	void DisplayRefresh(enum LcdState_Enum i_State,int i_iKey);//���ݵ�ǰ����״̬ˢ����ʾ
	enum LcdState_Enum CheckState(enum LcdState_Enum i_State,int i_iKey);//���ݰ���ֵȷ����ǰ״̬������

	enum LcdState_Enum OnPress_TurnDisp(int i_iKey);
	enum LcdState_Enum OnPress_Menu(int i_iKey);
	enum LcdState_Enum OnPress_DataMenu(int i_iKey);
	enum LcdState_Enum OnPress_SetState(struct LcdSetItem *i_sSetItem,int i_iKey);
	enum LcdState_Enum OnPress_ModifyStr(struct LcdSetItem *i_sSetItem,int i_iKey);
	enum LcdState_Enum OnPress_List(struct LcdSetItem *i_sSetItem,int i_iKey);
	enum LcdState_Enum OnPress_RealGather_One();
	enum LcdState_Enum OnPress_RealGather_All();
	int  OnPress_RealGather_All(LcdMenu &i_Menu,bool &GatheringFlag,int i_iKey);
	enum LcdState_Enum OnPress_SearchMeter();
	enum LcdState_Enum OnPress_NewMeterList();	
	int OnPress_NewMeterList(LcdMenu &i_Menu,bool &GatheringFlag,int i_iKey);
	enum LcdState_Enum OnPress_Report();
	enum LcdState_Enum OnPress_QueryData();
	enum LcdState_Enum OnPress_QueryVIPData();
	enum LcdState_Enum OnPress_ChangePassword();
	enum LcdState_Enum OnPress_Reset();
	enum LcdState_Enum OnPress_ClearUserDir();
	enum LcdState_Enum OnPress_DeleteHttxCfg();
	enum LcdState_Enum OnPress_ChangeRelay();
	enum LcdState_Enum OnPress_AutoNetworking();
	enum LcdState_Enum OnPress_SelectRouter();
	enum LcdState_Enum OnPress_SelectXCProtocol();
	int  GetinputNum(char *i_sTitle,char * i_initchar,int i_charLen,char *o_charbuf);
	int CheckPassWord();


	int CheckMessage();

	int ParsePackage(bstring &i_Package);//��ȷ������Ӧ�Ĺ����룬���󷵻�-1��
	int Send_GatherMsg_DBCJ(INT16U m_TN=0xFFFF);
	int GetInputDate(char *o_buf,int num);

	void State_TurnDisp(int ikey=0);//���������LcdYnfk�еľ��庯��(���ɸ��������ļ���̬�仯)
	void State_SetParam();
	void State_ListSelect();


	void ShowStringSelect();
	enum LcdState_Enum ModifyStr_Chr(struct LcdSetItem *i_sSetItem,int i_iKey);
	enum LcdState_Enum ModifyStr_Num(struct LcdSetItem *i_sSetItem,int i_iKey);
	void InitSetString(char *io_cStr,struct LcdSetItem *i_sSetItem);//��ʼ�������ִ�Ϊ�������õĳ���
	void FillMissStr(char *io_cCutStr,char *io_cResult,char i_cSpaceMark,unsigned char i_cSite);

	bool IsCanDisp(unsigned char i_cMenuRight);
	bool IsCanSet(unsigned char i_cSetRight);//�жϸ����Ƿ�ɱ༭
	int GetPassword(char *o_cHighPassw,char *o_cSuperPassw);
private:
	LcdMenu gMenu,gDataMenu,gTurnDispMenu,gSpecialMenu,gQueryMenu;
		
	char PromptStr[40],cDlgTitle[40],cDlgStr[40],cStr_get[40];
	struct LcdSetItem sSetItem;
	char cSeleChar[CONS_SELECHAR_ROW][CONS_SELECHAR_COL];
	struct sCursorAttribute_Struct sCursorAttr;
	unsigned char gcSelectLines,mcStrSetOK;
	int isTime; 
	bool bHardwareProtect;
	strings strs_results;
	enum LcdState_Enum Last_lcdstate;//�ϴε�״̬
	MeterBitSet ValidMBS;
	MeterBitSet FinishedMBS;
	INT16U initMeterTN;
	enum LcdState_Enum gState;

	int ESC_cnt;
	MilliSecondTimeout ts_project_timeout;
	bool InputpswFlag;

	char d_argv[20];
	int  d_argc;

};




#endif
