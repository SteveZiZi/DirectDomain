#ifndef __KLB_BASE_H
#define __KLB_BASE_H

#include <time.h>
#include "String.h"

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/file.h>
#else
#include <windows.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/Define.h"

inline unsigned char LowChar(unsigned short val)
{
	return (unsigned char)(val & 0xFF);
}
inline unsigned char HighChar(unsigned short val)
{
	return (unsigned char)(val>>8);
}
inline unsigned short MakeShort(unsigned char icHigh,unsigned char icLow)
{
	return (icHigh<<8) | icLow;
}
//#ifndef WIN32
//#define __PowerPC_CPU	//Use Sinovee PowerPC sc860tb
//#else
//#define __X86_CPU
//#endif

union UDataType{
      unsigned long l;
      unsigned char c[4];
};

union IDataType{
      unsigned short i;
      unsigned char c[2];
};

/*
获取[beg,end)范围内随机整数
*/
int RandRange(int beg=0,int end=RAND_MAX);
///*
//获取一个随机小写字符
//*/
//char RandChar_L();
///*
//获取一个随机大写字符
//*/
//char RandChar_U();
///*
//获取一个随机字符，包括大小写
//*/
//char RandChar();
///*
//获取一个随机小写字符串
//*/
//char* RandStr_L(char* dest,int len);
///*
//获取一个随机大写字符串
//*/
//char* RandStr_U(char* dest,int len);
///*
//获取一个随机字符串，包括大小写
//*/
//char* RandStr(char* dest,int len);
//========================= Data Type Convert ===================================//
//Function:Get some bit value from a-byte data
//In:
//	i_cData:a-byte data
//	i_iPos:position of a -byte data
//Out:
//	no
//Return:
//	0 or 1
//char Get_BitValue(unsigned char i_cData,int i_iPos);

//Function:Reverse some bit value in one-byte data
//In:
//	i_cData:one-byte data
//	i_iPos:position of one-byte data
//Out:
//	no
//Return:
//	the reversed data
//unsigned char Reverse_BitValue(unsigned char i_cData,unsigned char i_iPos);

//Function:Clear some bit value with zero in one-byte data
//In:
//	i_cData:one-byte data
//	i_iPos:position of one-byte data
//Out:
//	no
//Return:
//	the cleared data
//unsigned char Clear_BitValue(unsigned char i_cData,unsigned char i_iPos);

//Function:Set some bit value with '1' in one-byte data
//In:
//	i_cData:one-byte data
//	i_iPos:position of one-byte data(0~7)
//Out:
//	no
//Return:
//	the set data
//unsigned char Set_BitValue(unsigned char cByte,unsigned char iPos);

//Function:Caculate CRC according CCITT16
//In:
//	i_iCrcData:source CRC data
//	iData:the data added by i_CrcData
//Out:
//	no
//Return:
//	new CRC data
unsigned short Add_CRC(unsigned short i_iCrcData,unsigned char iData);

//Function:char-data converts to unsigned-long-data(4 bytes)
//In:
//	i_cll:the char-data lowest byte
//	i_cl:the char-data lower byte
//	i_ch:the char-data higher byte
//	i_chh:the char-data highest byte
//out:
//	no
//Return:
//	the converted usigned-long data
//unsigned long char_ulong(char i_cll,char i_cl,char i_ch,char i_chh);

//Function:char-data converts to unsigned-short-data(2 bytes)
//In:
//	i_cl:the char-data low byte
//	i_ch:the char-data high byte
//out:
//	no
//Return:
//	the converted usigned-short data
//unsigned int char_ushort(char i_cl,char i_ch);

//Function:char-data converts to BCD-data
//In:
//	i_cData:the char-data to be converted (00~99)
//out:
//	no
//Return:
//	the converted BCD data (0x00~0x99)
char cbcd(char i_cHexData);

//Function:BCD-data converts to char-data
//In:
//	i_cBcdData:the char-data to be converted (0x00~0x99)
//out:
//	no
//Return:
//	the converted char data (00~99)
char bcdc(char i_cBcdData);

INT16U B2D(INT16U bcd);
INT16U D2B(INT16U DEC);

char* bcd_to_hex(const char* src,int len,char* dest,bool reverse=false);
void reverse_str(char* str,int len);
//Function:Compare two BCD datas according its length
//In:
//	i_cpBcdData1:pointer of the BCD-data1 buffer
//	i_cpBcdData2:pointer the BCD-data2 buffer
//	c_iLen:bytes of comparing data
//out:
//	no
//Return:
//	-1:i_cpBcdData1<i_cpBcdData2
//	0:i_cpBcdData1=i_cpBcdData2
//	1:i_cpBcdData1>i_cpBcdData2
//int Comp_BCD(unsigned char * i_cpBcdData1,unsigned char * i_cpBcdData2,unsigned char i_cLen);

//Function:double-data converts 84BCD-data
//In:
//	i_dData:the double data to be converted
//Out:
//	o_cpBcdData:pointer of the converting 84BCD-data buffer
//Return:
//	no
//Note:if i_data is "-",i_data becomes "+" firstly
//void Double_to_84BCD(unsigned char *o_cpBcdData,double i_dData);

//Function:84BCD-data converts double-data
//In:
//	i_cpBcdData:pointer of the BCD84-data buffer to be converted
//Out:
//	no
//Return:
//	the converting double-data
double  BCD84_to_Double(char *i_cpBcdData);

//Function:BCD-data1 subtract BCD-data2
//In:
//	i_cpBcdData1:pointer of the BCD-data1 buffer
//	i_cpBcdData2:pointer the BCD-data2 buffer
//out:
//	o_cpResult:the subtract result
//Return:
//	-1:i_cpBcdData1<i_cpBcdData2,and o_cpResult is filled with 0x0
//	0:i_cpBcdData1>=i_cpBcdData2,and o_cpResult save the result
//int Sub_84BCD(char *o_cpResult,char *i_cpBcdData1,char *i_cpBcdData2);

//Function:Some BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source BCD data buffer
//	i_cType: type of some BCD data (1~9)
//		1:62-->84	2:24-->84	3:22-->84	4:30-->84	5:13-->84
//		6:40-->84	7:60-->84	8:80-->84	9:20-->84
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	-2:not BCD-data type (i_cType<1 or >9)
//	0:success

//int MeterDataToBCD84(unsigned char *i_cpSrcBcd,unsigned char i_CType,unsigned char *o_cpDstBcd);

//Function:84BCD data convert to other type of BCD meter data  
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//	i_cType: type of some BCD data (1~9)
//		1:84-->62	2:84-->24	3:84-->22	4:84-->30	5:84-->13
//		6:84-->40	7:84-->60	8:84-->80	9:84-->20
//Out:
//	o_cpDstBcd:pointer of destination BCD data buffer
//Return:
//	-2:not BCD-data type (i_cType<1 or >9)
//	0:success
//int BCD84ToMeterData(unsigned char *i_cpSrcBcd,unsigned char i_CType,unsigned char *o_cpDstBcd);


//Function:62BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 62BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD62ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 62BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 62BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD62(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:24BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 24BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD24ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 24BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 24BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD24(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:22BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 22BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD22ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 22BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 22BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD22(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:42BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 42BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD42ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 42BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 42BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD42(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:30BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 30BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD30ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 30BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 30BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD30(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:13BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 13BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD13ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 13BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 13BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD13(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:40BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 40BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD40ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 40BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 40BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD40(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:60BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 60BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD60ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 60BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 60BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD60(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:80BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 80BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD80ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 80BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 80BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD80(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:20BCD data of meter converts to 84BCD data
//In:
//	i_cpSrcBcd:pointer of source 20BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 84BCD data buffer
//Return:
//	0:success
//int BCD20ToBCD84(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);

//Function:84BCD data of meter converts to 20BCD data
//In:
//	i_cpSrcBcd:pointer of source 84BCD data buffer
//Out:
//	o_cpDstBcd:pointer of destination 20BCD data buffer
//Return:
//	0:success
//int BCD84ToBCD20(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int BCD84ToBCD31(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int BCD84ToBCD44(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int BCD84ToBCD64(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);


//////////////////////////////////////////
//int BCD62ToBCD186(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int BCD186ToBCD62(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int BCD24ToBCD186(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int BCD186ToBCD24(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int BCD22ToBCD186(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD186ToBCD22(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD30ToBCD186(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD186ToBCD30(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD13ToBCD186(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD186ToBCD13(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD40ToBCD186(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD186ToBCD40(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD60ToBCD186(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD186ToBCD60(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD80ToBCD186(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD186ToBCD80(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD20ToBCD186(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD186ToBCD20(unsigned char *i_cpSrcBcd,unsigned char *i_cpDstBcd);
//int BCD186ToBCD91(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//
//int BCD186ToBCD31(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int BCD186ToBCD44(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int BCD186ToBCD64(unsigned char *i_cpSrcBcd,unsigned char *o_cpDstBcd);
//int	DoubleToBCD186(unsigned char *o_cpBcdData,double i_dData);
//double  BCD186ToDouble(char *i_cpBcdData);
//int MeterDataToBCD186(unsigned char *i_cpSrcBcd,unsigned char i_CType,unsigned char *o_cpDstBcd);
//int BCD186ToMeterData(unsigned char *i_cpSrcBcd,unsigned char i_CType,unsigned char *o_cpDstBcd);


//关于BCD之间的转化,和BCD与Double之间转换,请大家多使用下面函数

//输入源串地址,整数位数,小数位数,目的串整数位数,小数位数,得到想要格式目的串.返回值0,正常,-1,出错,
int BCDExchange(unsigned char* i_cSrcBCD,unsigned char i_cZhenSrc,unsigned char i_cXiaoSrc,unsigned char* o_cDstBCD,unsigned char i_cZhenDst,unsigned char i_cXiaoDst);
//任何BCD串转换成double,返回值 所的double值,-1出错;
double BCDToDouble(const unsigned char* i_cBCD,unsigned char i_cZhenShuBit,unsigned char i_cXiaoShuBit);
//double转换成任何BCD串,返回值0正常,-1出错;
int DoubleToBCD(double i_dValue,unsigned char i_cZhenShuBit,unsigned char i_cXiaoShuBit,unsigned char* o_cDstBCD);



//======================= String Type Convert ====================================//
//Function:1-byte ASC char converts to 1-byte unsigned char number
//In:
//	i_cAscNumber:the 1-byte ASC char to be converted ('0'~'9','A'~'F')
//Out:
//	no
//Return:
//	the converting 1-byte unsigned char number(0x0~0x0F)
//note:
//	'8' --> 0x08	'D' --> 0x0D
unsigned char Char_No1Hex(char i_cAscChar);

//Function:2-byte ASC string converts to 1-byte unsigned char number
//In:
//	i_cpStr:pointer of the 2-byte ASC number string("00"~"FF")
//Out:
//	no
//Return:
//	the converting 1-byte unsigned char number
//note:
//	"28" --> 0x28	"7D" --> 0x7D	"AB" --> 0xAB
unsigned char String_Asc2TouChar(char *i_cpStr);

unsigned char HexToChar(char* i_cpStr,unsigned char icDefault);


//Function:4-byte ASC string converts to 2-byte unsigned short number
//In:
//	i_cpStr:pointer of the 4-byte ASC number string("0000"~"FFFF")
//Out:
//	no
//Return:
//	the 2-byte converting unsigned short number
//note:if ASC sring length >4,the get front 4 bytes
//	"0128" --> 0x128	"87D" --> 0x87D		"1ABF30" --> 0x1ABF
unsigned short String_Asc4TouShort(const char *i_cpStr);

//Function:8-byte ASC string converts to 4-byte unsigned long number
//In:
//	i_cpStr:pointer of the 8-byte ASC number string("00000000"~"FFFFFFFF")
//Out:
//	no
//Return:
//	the 4-byte converting unsigned long number
//note:if ASC sring length >8,the get front 8 bytes
//	"00000072" --> 0x72 "789" --> 0x789 "12AC7850" --> 0x12AC7850 "12AC789950" --> 0x12AC7899
unsigned long String_Asc8TouLong(char *i_cpStr);

//Function:Get the low4-bit of 1-byte char number
//In:
//	i_cNumber:the 1-byte char number
//Out:
//	no
//Return:
//	the low4-bit value
//note:
//	0x72 --> '2'	0xEB --> 'B'
char Get_Low4ofChar(unsigned char i_cNumber);

//Function:the 1-byte unsigned char number converts to 2-byte ASC string
//In:
//	i_cNumber:the 1-byte char number to be converted
//Out:
//	o_cpStr:the 2-byte converting string
//Return:
//	no
//note:
//	0x72 --> "72"	0xEB --> "EB"
void String_uCharToAsc2(unsigned char i_cNumber,char *o_cpStr);

//Function:the 2-byte usigned short int number converts to 4-byte ASC string
//In:
//	i_iNumber:the 2-byte unsigned short int number to be converted
//Out:
//	o_cpStr:the 4-byte converting string
//Return:
//	no
//note:
//	0x72 --> "0072"	 0x10EB --> "10EB"
void String_uShortToAsc4(unsigned short i_iNumber,char *o_cpStr);

//Function:the 4-byte unsigned long int number converts to 8-byte ASC string
//In:
//	i_iNumber:the 4-byte unsigned long int number to be converted
//Out:
//	o_cpStr:the 8-byte converting string
//Return:
//	no
//note:
//	0x72 --> "00000072"	 0x10EB --> "000010EB"	0x12AC7850 -->"12AC7850"
void String_uLongToAsc8(unsigned long i_lNumber,char *o_cpStr);

//Funciton:two ASC chars convert to 1-byte Decimal number
//In:
//	i_cAscH: the ASC char('0'~'9')
//	i_cAscL: the ASC char('0'~'9')
//Out:
//	no
//Return:
//	the 1-byte converting decimal number:success
//	-2:failure,the char is out range
//note:
//	i_cAsc1='1',i_cAsc2='8' ==>18
unsigned char No2Char_Dec1(char i_cAscH,char i_cAscL);

//============================== String Deal ===================================//
//Function:Separate a string to two parts according to the special char
//In:
//	i_cpLineStr: pointer of the source string to be cut
//	i_cCutChar:the specail char(separator)
//Out:
//	o_cpLeftStr:pointer of the left string
//	o_cpRightStr:pointer of the right string
//Return:
//	-2:failuer,the source string does't include the special char
//	0:success
int String_CutLine(char *o_cpLeftStr,char *o_cpRightStr,char *i_cpLineStr,char i_cCutChar);

//============================== Time Deal ===================================//
//Function:conver 'time_t' time form to 14-byte ASC time form
//In:
//	i_tTime:the 'time_t' time to be converted
//Out:
//	o_cpAsc:pointer of the 14-byte converting ASC time
//Return:
//	no
void Time_tTimeToAscTime14(time_t i_tTime,char  *o_cpAsc);

//Function:Convert 14-byte ASC time form to 'tm'-struct time form
//In:
//	i_cpAsc:pointer of the 14-byte ASC time
//Out:
//	o_tmpTime:pointer of 'tm'-struct time
//Return:
//	no
void Time_Asc14TimeTotmTime(char *i_cpAsc,struct tm *o_tmpTime);

//Function:Convert 'time_t' time form to 7-byte BCD time form
//In:
//	i_tTime:'time_t' time to be converted
//Out:
//	o_cpBcdTime:pointer of 7-byte converting BCD time
//Return:
//	no
void Time_tTimeToBCDTime7(time_t i_tTime,char * o_cpBcdTime);

//Function:Convert 7-byte BCD time form to 'time_t' time form
//In:
//	i_cpBcdTime:pointer of 7-byte BCD time to converted
//Out:
//	o_tTime:'time_t' converting time
//Return:
//	no
void Time_BCDTime7TotTime(const char * i_cpBcdTime,time_t * o_tTime);
time_t Time_BCDTime7TotTime(const char * i_cpBcdTime);

void Time_BCDTime7ToTm(char * i_cpBcdTime,struct tm * o_tm);


//Function:Get current time of 7-byte BCD form
//In:
//	no
//Out:
//	o_cpBcdTime:pointer of the 7-byte converting BCD time
//Return:
//	no
void Get_CurBCDTime7(char* o_cpBcdTime);
//void GetNowTime4(char nowTime[4]);

void Get_BCDTime7(time_t i_tTime,char* o_cpBcdTime);

char* Time2BCDStr_Big(time_t time, char *bcdStr7B);

//获取周
unsigned char GetWeek(char* i_cpBcdTime);

//Function:Get current 'tm'-struct time
//In:
//	no
//Out:
//	o_tmpTime:pointer of the converting 'tm'-struct time
//Return:
//	no
void Get_CurtmTime(struct tm *o_tmpTime);

//Function:Copy 7-byte source time to destination time
//In:
//	i_cpSrcTime:pointer of 7-byte source time
//Out:
//	o_cpSrcTime:pointer of 7-byte destination time
//Return:
//	no
//void CopyTime7(char * i_cpSrcTime,char * o_cpDstTime);

//Function:Copy source time to destination time according to given length
//In:
//	i_cpSrcTime:pointer of source time
//Out:
//	o_cpSrcTime:pointer of destination time
//Return:
//	no
//void CopyTime(char * i_cpSrcTime,char * o_cpDstTime,int i_iLen);

//Function:Compare two times according to given length
//In:
//	i_cpTime1:pointer of time1
//	i_cpTime2:pointer of time2
//	i_iLen:bytes to be compared
//Out:
//	no
//Return:
//	-1:time1 < time2
//	0:time1 = time2
//	1:time1 > time2
int CompTime(unsigned char *i_cpTime1,unsigned char *i_cpTime2,int i_iLen);

//Function:get numbers of radix between two 5-byte times(year/month/day/hour/minute)
//In:
//	i_cpTime1:pointer of time1
//	i_cpTime2:pointer of time2
//	i_iRadix:radix
//Out:
//	no
//Return:
//	0:radix=0,or time1>=time2
//	>0:numbers of radix
//long int Time_BCDTime5ToDots(char *i_cpTime1,char *i_cpTime2,unsigned int i_iRadix);

//Function:get numbers of radix between two 7-byte times(century/year/month/day/hour/minute/seconds)
//In:
//	i_cpTime1:pointer of time1
//	i_cpTime2:pointer of time2
//	i_iRadix:radix
//Out:
//	no
//Return:
//	0:radix=0,or time1>=time2
//	>0:numbers of radix
//long int Time_BCDTime7ToDots(char *i_cpTime1,char *i_cpTime2,unsigned int i_Radix);

//void TimeA_to_Hex5Time(char *i_cTimeA,char *o_5HexTime);
//void BCD6Time_to_TimeB(char *i_cBcd6Time,char *o_cTimeB);


//Function:Add numbers of radix on base of 7-byte BCD time
//In:
//	i_cpSrcBcdTime:pointer of 7-byte source BCD time
//	i_iRadix:radix
//	i_iNum:numbers of radix
//Out:
//	o_cpDstBcdTime:pointer of 7-byte destination BCD time
//Return:
//	no
//void Add_nRadixOnBCDTime7(unsigned char *i_cpSrcBcdTime,unsigned int i_iRadix,int i_iNum,unsigned char *o_cpDstBcdTime);

//Function:Add numbers of radix on base of 5-byte BCD time
//In:
//	i_cpSrcBcdTime:pointer of 5-byte source BCD time
//	i_iRadix:radix
//	i_iNum:numbers of radix
//Out:
//	o_cpDstBcdTime:pointer of 5-byte destination BCD time
//Return:
//	no
//void Add_nRadixOnBCDTime5(unsigned char *i_cpSrcBcdTime,unsigned int i_iRadix,int i_iNum,unsigned char *o_cpDstBcdTime);

bool IsBcd7Time(const unsigned char* bcd7Data);

//Function:Set Operation System time
//In:
//	i_cpBcdTime6:pointer of 6-byte BCD time
//Out:
//	no
//Return:
//	0
bool isVaildBCDData(const unsigned char * data,int dataLen);

int Set_SysTime(char *i_cpBcdTime6);

int Set_SYS_HWClock7(char *i_cpBcdTime7);
int Set_SYS_HWClock(char *i_cpBcdTime6);

int ExchangeBCDTime(unsigned char TimeFormatSrc,unsigned char* DataSrc,unsigned char TimeFormatDst,unsigned char* DataDst);

unsigned int BcdToDec(int n,unsigned char *c);
unsigned long HexToDec(const unsigned char *hex,int length);
int ParseRangeNum(const char* pSetting,unsigned int& oMin,unsigned int& oMax);


template<typename T>
T SerializeVal(T val)
{
	IDataType aa;
	aa.i=0xff;
	if(aa.c[0]==0xff)
	{
		T t=0;
		for(size_t i=0;i<sizeof(T);i++)
		{
			t+=(T)(((val>>(i*8)) & 0xff)<<(sizeof(T)-i-1)*8);
		}
		return t;
	}
	return val;
}

template<typename T>
T DeserializeVal(T val)
{
	return SerializeVal<T>(val);
}
template<typename T>
void* valmemcpy(void* buff,T val)
{
	T temp=SerializeVal<T>(val);
	return memcpy(buff,&temp,sizeof(T));
}

double mypow(int x1);

time_t dateadd(unsigned char interUnit,long number,const time_t date);
void bcd_dateadd(unsigned char interUnit,long number,char *date);
long datediff(unsigned char interUnit,const time_t date1,const time_t date2);
long bcddatediff(unsigned char interUnit,const char* bcdTime1,const char* bcdTime2);
time_t datetrim(unsigned char interUnit,const time_t date);
time_t upboundtime(time_t date,time_t stdTime,unsigned char interUnit,unsigned long interVal);
time_t downboundtime(time_t date,time_t stdTime,unsigned char interUnit,unsigned long interVal);
time_t gettimerpoint(time_t nearTime,unsigned char interUnit,unsigned long interVal,unsigned char offsetUnit,unsigned long offsetVal,bool ibDownBound=true);
bool issameperiod(time_t baseTime,time_t checkTime,unsigned char interUnit,unsigned long interVal);

char* timestr(time_t* t);
char* bcdtimestr(char* bcdTime7);
char* bcdtimestr(char* bcdTime7,char* bcdTime15);
void timestr(time_t t,string& str);

class SecondTimeout
{
public:
	inline SecondTimeout(unsigned long ilTimeout)
	{
		time(&m_tBeginTime);
		//printf("SecondTimeout constr %d\n",m_tBeginTime);
		m_lTimeout=ilTimeout;
	}
	inline SecondTimeout(time_t itBeginTime, unsigned long ilTimeout)
	{
		m_tBeginTime=itBeginTime;
		m_lTimeout=ilTimeout;
	}
	inline void Reset()
	{
		time(&m_tBeginTime);
	}
	 inline void Reset(unsigned long mm_lTimeout)
	{
		time(&m_tBeginTime);
		m_lTimeout=mm_lTimeout/1000;
	}
	bool IsNowTimeout()
	{
		time_t tNow;
		time(&tNow);
		double diff=difftime(tNow,m_tBeginTime);
		if (diff < 0) //向前对时
		{
			//printf("changed m_tBeginTime\n");
			m_tBeginTime=tNow+m_lTimeout/2;
		}
		return (((unsigned long)diff)>m_lTimeout);
	}
	static bool IsNowTimeout(time_t itBeginTime, unsigned long ilTimeout)
	{
		SecondTimeout st(itBeginTime,ilTimeout);
		return st.IsNowTimeout();
	}
	const time_t GetBeginTime() const
	{
		return m_tBeginTime;
	}
	const unsigned long GetTimeout() const
	{
		return m_lTimeout;
	}
protected:
	time_t m_tBeginTime;
	unsigned long m_lTimeout;
};

class MilliSecondTimeout:public SecondTimeout
{
public:
	inline MilliSecondTimeout(unsigned long ilTimeout):SecondTimeout(ilTimeout/1000+((ilTimeout % 1000)>0?1:0) )
	{
	}
	inline MilliSecondTimeout(time_t itBeginTime, unsigned long ilTimeout):SecondTimeout(itBeginTime,ilTimeout/1000+((ilTimeout % 1000)>0?1:0))
	{
	}
};

//double basechange(int begin,unsigned int num,const unsigned char* buffer);
//int basechange_reverse(double dValue,unsigned char xiaoshubit,unsigned char zhenshubit,unsigned char bytenum,unsigned char* buffer);
bool isbcdchar(char c);
char* strlower(char* str);

//unsigned char getcentury(unsigned char bcdYear);

//bool SetBitInArray(unsigned char *array,unsigned short len, unsigned int wbit);

void GetAddr_3or6_FixFor0(unsigned char CharLen,unsigned char Flag, char *i_addr,unsigned char *o_addr);
void GetAddr_6_FixF( char *i_addr,unsigned char *o_addr);
void GetAddr_6_Fix0( char *i_addr,unsigned char *o_addr);
void GetAddr_3_FixF( char *i_addr,unsigned char *o_addr);
void GetAddr_3_Fix0( char *i_addr,unsigned char *o_addr);
void GetAddr_6_FixA(char *i_addr,unsigned char *o_addr);

INT16U ByteCRC16(INT16U crcCode, unsigned char data);
void CRC16Table();	

unsigned short CRC16RTU(const unsigned char * pszBuf, unsigned int unLength);

//bool isValidTime(time_t& inputtime);
#endif
