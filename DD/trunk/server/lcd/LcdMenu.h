/**
*@file		文件名称(LcdMenu.h)
*@brief		液晶界面的界面显示
*
*@version	Ver 1.0
*
*@date		2010-03-10
*@author	胡中明
*/
#ifndef __LCD_MENU_H__
#define __LCD_MENU_H__

#include "LcdBase.h"
#include "DataGwjc/LcdItemsGwjc.h"

#ifndef INCLUDED_KL_TRACE
#include "../../base/KLTrace.h"
#endif

typedef enum LcdDispType_Enum
{
	DispReverse=1,		//反显
	DispNoReverse=2,	//无反显
	DispReverseBit=3	//按位反显
};

typedef enum LcdHZKType_Enum
{
	HZKStand=1,
	HZKSelf=2,
};

typedef enum _AlignmentType  //对齐标识
{
	AT_Left=1,
	AT_Center=2,
}AlignmentType;

class LcdMenu
{
public:
	char mwords[CONS_METER_MAX_COUNT][CONS_LCD_MAXCOLS];
	char c_foot[CONS_LCD_MAXCOLS];
	
	int mFirstRow,mCurRow,mCurCol,mTitleCount,mPage,mCurPage;//页数是从1开始
	char mPrompt[20],mDefaultInfo[20];	//mDefaultInfo-GPRS信号，mPrompt－简单信息
	char mHeadInfo[CONS_HEAD_NUM][CONS_LCD_MAXCOLS];	
	enum LcdDispType_Enum mDispType;	//显示类型（反显一行/非反显/反显一个字)
	enum LcdHZKType_Enum mHZKDispType;	//汉字显示类型
	LcdMenuItem *sMenuItems;
	AlignmentType AligFlag;
	int iFlag;
	bool f_NextPage;
private:
	INT16U mLines;
public:
	inline LcdMenu()
	{
		mLines=mFirstRow=mCurRow=mTitleCount=0;
		mPage=mCurPage=1;
	}
	inline ~LcdMenu()
	{
	}
	inline void MenuReset()
	{
		mLines=mFirstRow=mCurRow=mTitleCount=0;
		mPage=mCurPage=1;
	}
	inline void MenuInit(unsigned char i_cLines,char *i_cStr[])
	{
		mLines=i_cLines;
		for(int i=0;i<mLines;i++)
		{
			memset(mwords[i],0x0,CONS_LCD_MAXCOLS);
			strcpy(mwords[i],i_cStr[i]);
		}
			mFirstRow=mCurRow=mTitleCount=0;
			mPage=mCurPage=1;
			iFlag=-1;
			f_NextPage=false;
	}
	inline void SetMenuParam(unsigned char i_cLines,unsigned char i_TitleCount=CONS_LCD_MAXROW)
	{        
		mLines=i_cLines;
		if (i_TitleCount!=CONS_LCD_MAXROW)
		{
			mTitleCount=i_TitleCount;
		}
		int p_records=CONS_LCD_MAXROW-mTitleCount;
		mPage=(mLines-mTitleCount)/ p_records;
		int d_left=(mLines-mTitleCount) % p_records;
		if (d_left>0)
		{
			mPage++;
		}	
	}
	inline INT16U GetMenu_Line()
	{
			return mLines;
	}
	void MenuDraw(bool DrawScr_Full=true, int gstate = 0);
	inline void OnPressKey(int key)
	{
		MenuMoveKey(key);
	}
	void RefleshMenuInfo(char *i_cPrompt,enum LcdDispType_Enum i_DispType,enum LcdHZKType_Enum i_HZKType,AlignmentType i_AligType=AT_Left);
	void RefleshMenuInfo(char *i_cPrompt,char *i_cDefaultStr,enum LcdDispType_Enum i_DispType,enum LcdHZKType_Enum i_HZKType,AlignmentType i_AligType=AT_Left);
private:
	void MenuMoveKey(int key);
};

#endif
