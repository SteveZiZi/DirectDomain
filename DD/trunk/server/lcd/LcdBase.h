#ifndef __LCD_BASE_H__
#define __LCD_BASE_H__

#include <string.h>
#include<stdio.h>
#include "lcdBottom/lcdBottom.h"


#define CONS_LCD_MAXROW 9 //屏幕显示的行数
#define CONS_LCD_MAXCOLS 52   //屏幕每行的字数
#define CONS_LCD_FOOT 145   
#define CONS_HEAD_NUM 10






#define CONS_HZK_MODE_MAXLEN	500
#define CONS_FILEPATH_RAMLEN	100

#define KEY_UP      1
#define KEY_DOWN    2
#define KEY_LEFT    3
#define KEY_RIGHT   4
#define KEY_ESC     5
#define KEY_ENTER	6
#define KEY_NONE	-1
#define WAIT_INPUT_TIME 10000

#define KEY_LOCK		0xAA	//ljt	锁 按键

#define	COLOR_WHITE  0
#define COLOR_BLACK  1

#define X_CENTER		80
#define Y_CENTER		80

#define LETTER_CHINESE 1
#define LETTER_ENGLISH 2

enum Frame_enum
{
	Frame_None=0x01,
	Frame_Rectangle=0x02,	
	Frame_Circle=0x03,
	
}; 

struct sCursorAttribute_Struct
{
	unsigned char cCursorPos;
	unsigned char cCursorLen;
	unsigned char cCursorMove;
	unsigned char cCursorEnd;
};
////==lcdBottom.cpp===================
//class LcdBottom
//{
//public:
//	static int mmap_init(void);
//	static int mmap_exit(void);
//	static int lcd_on(void);
//	static int lcd_init(void);
//	static void lcd_LightOpen(void);
//	static void lcd_LightClose(void);
//	static int lcd_off(void);
//	static int lcd_update(char *buf);
//
//	static int btfd;
//private:
//	static void lcd_delay(int n);
//	static int lcd_read(int addr);
//	static int lcd_write(int addr, int data);
//
//	static int paddr;
//	static int vaddr;
//	static char dbuf[8][192];
//};

////==================================
class LcdBase
{
public:
	inline ~LcdBase()
	{
	}
	static void Lcd_Button_init();
	static void Prepare_Hzk();
	static void ClearScr();
	static void ClearContext();
	static void DrawFile(char strT[CONS_FILEPATH_RAMLEN]);
	static int Out_Str(int x,int y,char *p);
	static void ReverseLine(int nLine);
	static void RenewLine(int nLine);
	static void ReverseBit(int iLine,int icol);
	static void DrawScr_Full();
	static void lcd_SetContrast(int contrast);
	static void lcd_SetGrayness(int grayness);
	static void CloseLcdLight();
	static void OpenLcdLight();
	static int Read_Key();
	static void MessageBoxUp(char *p);
	static void MessageBoxUp(char *p1,char *p2);
	static void Set_CursorAttribute(struct sCursorAttribute_Struct *sCurAttr,unsigned char cCurPos,unsigned char cCurLen,unsigned char cCurMove,unsigned char cCurEnd);
	static void DrawDialog_Modify(char *sTitle,char *sContent,struct sCursorAttribute_Struct sCurAttr);
	static void ModifySglVal_OnCursor(struct sCursorAttribute_Struct sCurAttr,int i_iKey,char *io_cDlgStr,char *o_cGetStr);
	static void ModifySglHex_OnCursor(struct sCursorAttribute_Struct sCurAttr,int i_iKey,char *io_cDlgStr,char *o_cGetStr);
	static void DrawDialog_Confirm(char *cTitle,int i_Flag=-1);
	static void DrawDialog_VirtualKeyBoard(int i_Flag=-1);	
	static void ClearHead();
	static int GetChar32(char * input,char * mold32, enum Frame_enum type,int df=0);
	static void DrawStr(int x,int y,char * p,int type);
	static void MemLine(int startx,int starty,int endx,int endy,char color);
	static int DrawDialog(char *cTitle);
	static void DrawPrompt(char *p);
	static void DrawMeterTN(char *i_MeterTN,struct sCursorAttribute_Struct sCurAttr);
	static int DrawGather();
	static void Draw_Button(int i_x,int i_y,char *icStr);
	static void Draw_Box(int i_x,int i_y,char *icStr,bool i_Flag=true);

private:
	static int Code_Match(char code[2]);
	static void Draw_Chinese(int x,int y,char mold[32]);
	static void Draw_English(int x,int y,char mold[32]);
	static int Get_Wei(char c);
	static void MemPoint(int x,int y,char color);
	static void MemRectangle(int x1,int y1,int x2,int y2,char color);
	static void Read_Mold(int handle,int qu,int wei,char mold[32]);
	static void Read_Mold(FILE* fp,int qu,int wei,char mold[32]);
	static void Head();
};

#endif
