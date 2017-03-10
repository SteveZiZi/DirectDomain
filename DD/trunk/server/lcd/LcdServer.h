/**
*@file		文件名称(LcdServer.h)
*@brief		液晶进程的调度流程
*
*@version	Ver 1.0
*
*@date		2010-03-10
*@author	胡中明
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
#define CONS_SEARCH_TIMEOUT	1800000 //半小时
#define CONS_REPORT_TIMEOUT	60000 //1分钟
#define CONS_NETWORKING_TIMEOUT	3600000 //半小时

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
	int GetMainMenu();	//取主菜单
	int GetMenuDesc();	//有下级菜单则取下级菜单，无下级菜单返回0,需要输入密码的，必须输入密码，成功后，30分钟有效，失败返回-1；
	int GetPredMenu();	//有上级菜单则取上级菜单，无上级菜单返回0
	int GetData(int i_iKey);		//取数
	int CheckingKey();	//按键检测，返回键值，键值为0表示5秒内都无按键
	void DisplayRefresh(enum LcdState_Enum i_State,int i_iKey);//根据当前所处状态刷新显示
	enum LcdState_Enum CheckState(enum LcdState_Enum i_State,int i_iKey);//根据按键值确定当前状态及操作

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

	int ParsePackage(bstring &i_Package);//正确返回相应的功能码，错误返回-1；
	int Send_GatherMsg_DBCJ(INT16U m_TN=0xFFFF);
	int GetInputDate(char *o_buf,int num);

	void State_TurnDisp(int ikey=0);//按步骤调用LcdYnfk中的具体函数(并可根据配置文件动态变化)
	void State_SetParam();
	void State_ListSelect();


	void ShowStringSelect();
	enum LcdState_Enum ModifyStr_Chr(struct LcdSetItem *i_sSetItem,int i_iKey);
	enum LcdState_Enum ModifyStr_Num(struct LcdSetItem *i_sSetItem,int i_iKey);
	void InitSetString(char *io_cStr,struct LcdSetItem *i_sSetItem);//初始化设置字串为符合设置的长度
	void FillMissStr(char *io_cCutStr,char *io_cResult,char i_cSpaceMark,unsigned char i_cSite);

	bool IsCanDisp(unsigned char i_cMenuRight);
	bool IsCanSet(unsigned char i_cSetRight);//判断该行是否可编辑
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
	enum LcdState_Enum Last_lcdstate;//上次的状态
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
