#ifndef __LCD_ITEMS_H__
#define __LCD_ITEMS_H__

#include "LcdDataGwjc.h"

typedef int (*HANDLER_GETDATA)(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
typedef void (*HANDLER_SAVE)(char *i_cStr,unsigned char i_cNum,char *o_cPromptStr);	//������i_cStr-������ִ���i_cNum-�б������
typedef int (*HANDLER_LIST)(unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);

#define LCD_ITEM_END() {"",0,Form_None,0,0,0,NULL}
#define LCD_SETITEM_END() {"",SetNone,0,0,0,0xff,0,0,NULL,NULL}
#define LCD_TURNDISP_END()	{0,"",0,0,NULL}

enum LcdState_Enum
{
	LcdTurnDisp=0x01,
	LcdAlarmDisp=0x02,	//�澯����
	LcdMenuDisp=0x03,
	LcdDataDisp=0x04,
	LcdSetParam=0x05,
	LcdModifyStr=0x06,	//�ִ������ݴ��༭
	LcdListSelect=0x07,	//�б�ѡ��
	LcdSaveParam=0x08,		//�������
	LcdSpecialView=0x09,	//������ʾ
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

//�������ͣ���λΪ���ͱ�ţ���λΪ���ô�����
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
	TimeView=0x09,	//ʱ����ʾ��ʽ������ˢ���Ҳ��˳�ֱ�����˳���Ϊֹ
};

struct LcdSetItem
{
	char ItemDesc[24];
	enum LcdSetType_Enum eEditType;		//�༭����
	unsigned char EditLen;		//�༭����
	unsigned short Remark;		//��ע����Ϊ�༭״̬ʱ��ÿ��λ��ֵ��ʾ��Ӧֵ��λ��Ϊ���ɱ༭
	unsigned char Right;		//����Ȩ��
	unsigned char Row;			//�ڲ˵����������к�(��̬����,δ����ʱΪOxff,����ֵΪ0xffʱ���������裬Ϊ�����к�ʱ��ʾ���п�����)
	unsigned char IsTwoLines;	//ռ����0x5a,ռ1��0
	char cSpaceMark;			//����SetHex��SetVal����ʱΪ������ţ�����SetString����ʱΪ��������
	char *Special;		//��������ָ��
	HANDLER_SAVE ProcSaveParam;	//һ�������Ϊ�������ú���,���༭����ΪExecProcʱΪִ�в�������
	HANDLER_LIST ProcList;		//�б�
};

struct LcdMenuItem
{
	char MenuDesc[24];
	unsigned char Right;//Ȩ�� 1�����û���2�߼�Ȩ�ޣ�3�ͼ�Ȩ�ޣ�Right>10���⴦�������û����õ������ļ�ȷ����
	LcdForm_Enum FormType;	//��������
	struct LcdSetItem *sSetItems;
	struct LcdMenuItem *sNext;
	struct LcdMenuItem *sPred;
	HANDLER_GETDATA ProcGetData;	//ȡ��
};

struct TurnDispItem
{
	unsigned char Num;			//���
	char           szMenuDesc[24];//Raymond 2007.07.31
	unsigned char DispType;		//�������ͣ�1-���ԣ�2-�澯�ԣ����ڵ���3-�̶��澯��(����ֵ��Сȷ�����ȼ���3���ȣ�4��֮���Դ�����)
	unsigned char ValidFlag;	//��Ч��(���б�Ҫ�ɸ�Ϊ���������ļ����ã���Ŀǰ����֧�������ļ����ù���)
	HANDLER_GETDATA ProcGetData;
};


class LcdItemDeal
{
public:
	static LcdMenuItem *gMainMenu;
	static TurnDispItem *gTurnDispItems;

	static void MenuItems_Init();	//�˵����ʼ������ʼ��ÿ��ĸ��˵�ָ��
	static bool IsSuitedItem(char *i_cMenuStr,LcdSetItem *sItems);
};

#endif
