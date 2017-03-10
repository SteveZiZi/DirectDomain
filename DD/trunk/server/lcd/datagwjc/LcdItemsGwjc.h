#ifndef __LCD_ITEMS_H__
#define __LCD_ITEMS_H__

#include "LcdDataGwjc.h"

typedef int (*HANDLER_GETDATA)(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
typedef void (*HANDLER_SAVE)(char *i_cStr,unsigned char i_cNum,char *o_cPromptStr);	//参数：i_cStr-保存的字串，i_cNum-列表的行数
typedef int (*HANDLER_LIST)(unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);

#define LCD_ITEM_END() {"",0,Form_None,0,0,0,NULL}
#define LCD_SETITEM_END() {"",SetNone,0,0,0,0xff,0,0,NULL,NULL}
#define LCD_TURNDISP_END()	{0,"",0,0,NULL}

enum LcdState_Enum
{
	LcdTurnDisp=0x01,
	LcdAlarmDisp=0x02,	//告警轮显
	LcdMenuDisp=0x03,
	LcdDataDisp=0x04,
	LcdSetParam=0x05,
	LcdModifyStr=0x06,	//字串或数据串编辑
	LcdListSelect=0x07,	//列表选择
	LcdSaveParam=0x08,		//保存参数
	LcdSpecialView=0x09,	//特殊显示
};

typedef enum _LcdForm_Enum
{
	Form_None=0x00,
	Form_Menu=0x01,
	Form_QueryData=0x02,	
	Form_QueryVIP=0x03,
	Form_RealMeter_One=0x04,
	Form_RealMeter_All=0x05,
	Form_SearchMeter=0x07,
	Form_NewMeterList=0x08,
	Form_Report=0x09,	
	Form_InputPSW=0x0A,
	Form_ChangeRelay=0x0B,
	Form_AutoNetworking=0x0C,
	Form_Reset=0x0D,
	Form_ChangePSW=0x0E,
	Form_SlectRouter=0x0F,
	Form_DispRouter=0x10,
	Form_ClearUserDir=0x11,
	Form_XiaoChengProtocol=0x12,
	Form_Del_Httx=0x13,
}LcdForm_Enum;

//设置类型，高位为类型编号，低位为设置串长度
typedef enum LcdSetType_Enum	
{
	SetNone=0x0,
	SetString=0x01,
	SetHex=0x02,
	SetVal=0x03,
	SetList=0x04,
	SetSpecial=0x05,
	SetBool=0x06,
	ExecProc=0x07,
	SpecialView=0x08,
	SetIP=0x09,
	TimeView=0x09,	//时间显示方式，不断刷屏且不退出直到按退出键为止
};

struct LcdSetItem
{
	char ItemDesc[24];
	enum LcdSetType_Enum eEditType;		//编辑类型
	unsigned char EditLen;		//编辑长度
	unsigned short Remark;		//备注，当为编辑状态时，每四位的值表示对应值的位置为不可编辑
	unsigned char Right;		//设置权限
	unsigned char Row;			//在菜单中所处的行号(动态设置,未设置时为Oxff,即当值为0xff时表明不可设，为具体行号时表示该行可设置)
	unsigned char IsTwoLines;	//占两行0x5a,占1行0
	char cSpaceMark;			//当是SetHex、SetVal类型时为间隔符号，当是SetString类型时为结束符号
	char *Special;		//特殊描述指针
	HANDLER_SAVE ProcSaveParam;	//一般情况下为保存设置函数,当编辑类型为ExecProc时为执行操作函数
	HANDLER_LIST ProcList;		//列表
};

struct LcdMenuItem
{
	char MenuDesc[24];
	unsigned char Right;//权限 1超级用户，2高级权限，3低级权限，Right>10特殊处理（根据用户设置的配置文件确定）
	LcdForm_Enum FormType;	//窗体类型
	struct LcdSetItem *sSetItems;
	struct LcdMenuItem *sNext;
	struct LcdMenuItem *sPred;
	HANDLER_GETDATA ProcGetData;	//取数
};

struct TurnDispItem
{
	unsigned char Num;			//编号
	char           szMenuDesc[24];//Raymond 2007.07.31
	unsigned char DispType;		//轮显类型，1-轮显，2-告警显，大于等于3-固定告警显(按数值大小确定优先级，3最先，4次之，以此类推)
	unsigned char ValidFlag;	//有效性(若有必要可改为根据配置文件配置，但目前还不支持配置文件配置功能)
	HANDLER_GETDATA ProcGetData;
};


class LcdItemDeal
{
public:
	static LcdMenuItem *gMainMenu;
	static TurnDispItem *gTurnDispItems;

	static void MenuItems_Init();	//菜单项初始化，初始化每项的父菜单指针
	static bool IsSuitedItem(char *i_cMenuStr,LcdSetItem *sItems);
};

#endif
