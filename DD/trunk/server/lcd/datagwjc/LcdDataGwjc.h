#ifndef __LCD_DATA_Gwjc_H__
#define __LCD_DATA_Gwjc_H__

#ifndef INCLUDED_STD_INCLUDE
#include "../../../include/StdInclude.h"
#endif

#ifndef __LCD_BASE_H__
#include "../LcdBase.h"
#endif

#ifndef INCLUDED_STRING
#include "../../../base/String.h"
#endif

class LcdDataGwjc
{ 
public:
	inline LcdDataGwjc()
	{
	}
	static void GetMenuHeader(char o_cHeadInfo[][CONS_LCD_MAXCOLS]);
	static void GetMenuHeader();
	static void GetMainInterface();
	static void GetMenuStatus(char *pMenuStatus);
	static void GetData_DeviceNo(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static void GetData_Date(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_WorkingStatus(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_VaildStatus(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_TaskStatus(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_CommStatus(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	/*抄表查询菜单*/
	//static int GetData_YGJ(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	//static int GetData_YGF(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	//static int GetData_YGP(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	//static int GetData_YGG(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	//static int GetData_DayData(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	/*参数查询菜单*/
	static int GetData_TNParam(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_CommParam(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_GatherParam(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_RouterInfo1(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_RouterInfo2(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	/*重点用户信息查询*/
	//static int GetData_VIPPower(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	//static int GetData_VIPYG(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	//static int GetData_VIPWG(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	//static int GetData_VIPCurrent(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	//static int GetData_VIPVol(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	/*统计查询菜单*/
	static int GetData_GatherStatInfo(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_GatherFailInfo(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_CommStat(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_RouterStat(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	/*参数设置菜单*/	
	static int GetData_SetCommParam(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);

	static void SetData_AdminCode(char *i_cStr,unsigned char i_cNum,char *o_cPromptStr);
	static void SetData_CommChannle(char *i_cStr,unsigned char i_cNum,char *o_cPromptStr);
	static void SetData_APN(char *i_cStr,unsigned char i_cNum,char *o_cPromptStr);
	static void SetData_MainIP(char *i_cStr,unsigned char i_cNum,char *o_cPromptStr);
	static void SetData_SpareIP(char *i_cStr,unsigned char i_cNum,char *o_cPromptStr);
	static void SetData_LocaiIP(char *i_cStr,unsigned char i_cNum,char *o_cPromptStr);
	static void SetData_PortNum(char *i_cStr,unsigned char i_cNum,char *o_cPromptStr);
	/*现场调试菜单*/
	static int Debug_RealGather_Single(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int Debug_RealGather_All(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int Debug_SearchMeter(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int Debug_NewAddMeterList(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int Debug_FailSearchMeterList(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int Debug_AutoReport(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int Debug_DispRouter(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int Debug_DispXcProtocalType(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	/*工程调试菜单*/
	static int GetData_FailNodes(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);


	/*读取冻结数据*/
	static int GetData_VIPData(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_DayData(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	static int GetData_MonthData(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);

private:
	static int GetData_FreezeData(INT8U DataType,char *i_cStr,bstring& o_Records);

	//static int GetClass2Data4LcdYG(INT16U i_TN, INT8U i_DataType, char *i_BCDDate4,bstrings &o_YGdatas);
	//
	//static int GetFNandTime(char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	////static int GetData_Class2Vip(INT8U VIP_Type,char *i_cStr,unsigned char i_cNum,char o_cWords[][CONS_LCD_MAXCOLS]);
	//static int GetClass2VipData(INT8U VIP_Type,INT16U i_TN, INT16U i_StartPoint, INT16U i_Cnt, char *i_BCDDate4,bstring &o_data);
	//static int GetOneZbMeterAddr(INT16U &start, bstring &ZbAddr);
	//static INT8U GetAscii(INT8U ch);
public:
	static char gcHeadPrompt[32],gcHeadDefault[32];
	static unsigned char cPredID,cDataMenuID,cPredPredID;	//父菜单行数,数据菜单行数,祖父菜单行数
	static int giSignalIntensity;
};
int GetNewMeterList(int start, int cnt, int &total, bstrings &addrs);


#endif
