///*RegData.h中包括能量寄存器数据相关的定义*/

#ifndef INCLUDED_REG_DATA
#define INCLUDED_REG_DATA

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

class KLDump;


struct RegData
{
public:
	//1、Flag,高四位表示分类标准；
    //	1.1分类标准为0时，低四位表示数据类型，0表示原始字串，1表示BCD码非负数，2表示BCD码负数，3表示标准BCD码时间，15表示无效数据
	unsigned char Flag;

	//2、Format:因分类标准和数据类型不同而不同：
	//	2.1, 当分类标准为0时:
	//		21.1.1数据类型为0时，高四位保留为0，低四位表示有原始字串长度
	//		2.1.2数据类型为1或2时，Format高四位表示整数位数，Format低四位表示小数位数；
	//		2.1.3数据类型为3时，Format表示标准的时间格式，标准时间格式即BCD码按CCYYMMDD(WW)HHNNSS顺序或反序排列，
	//			Format的含义：D7置1表示有周，置0表示没有周，D4~D6表示开始单位值，D3保留，D0~D2表示结束单位值
	//			时间单位值：CC=7，YY=6，MM=5，DD=4，HH=3，NN=2，SS=1注意，WW（周）比较特殊，它总在DD与HH之间。
	//			例如，0x64表示YYMMDD，0x46表示DDMMYY， 0xE4表示YYMMDDWW,0x52表示MMDDHHNN，0x25表示NNHHDDMM,0xA5表示NNHHWWDDMM
	unsigned char Format;

public:
	//3、Data数据区域，可以为数值、时间、字串或其它类型数据，全部采用左对齐方式，反序保存
	unsigned char Data[8];
public:
	inline RegData()
	{
		SetEmpty();
	}
	inline void SetEmpty()
	{
		memset(this,CONS_REG_DEFAULT_CHAR,sizeof(*this));
	}
	static bool IsValidFlag(unsigned char icFlag)
	{
		return ((icFlag & FlagInvalid)!=FlagInvalid);
	}
	inline unsigned char GetType() const
	{
		return GetType(Flag);
	}
	inline static unsigned char GetType(unsigned char icFlag)
	{
		return (icFlag & 0x0F);
	}

	inline unsigned int GetLen() const
	{
		return GetLen(*this);
	}

	static unsigned int GetLen(const RegData& irRegData);
	static unsigned int GetValFormatLen(unsigned char icFormat);
	inline bool IsValid() const
	{
		return IsValid(Flag);
	}
	inline static bool IsValid(unsigned char icFlag)
	{
		return GetType(icFlag)!=TypeUnknown;
	}
	static int ConvertTo(unsigned char icDestFlag,unsigned char icDestFormat,RegData& iorDestData);
	inline int ConvertTo(unsigned char icDestFlag,unsigned char icDestFormat)
	{
		return ConvertTo(icDestFlag,icDestFormat,*this);
	}
	int ConvertTo(unsigned char iValFormat,unsigned char* opData,INT32U iDataLen)const;
	int Set(unsigned char icFlag,unsigned char icFormat,const unsigned char* ipData,unsigned int iiLen);

	int GetVal(double* opVal) const;
	int GetTime(char* pBcdTime7) const;
	int GetVal(time_t* opTime) const;
	int SetVal(double idVal,unsigned char icFormat);
	int SetVal(time_t itTime,unsigned char icFormat);
	int SetVal(char* bcdTime,unsigned char icFormat);
	int OpVal(double val,unsigned char Op,unsigned char icFormat=0x00);

	int OpRegData(const RegData& irRegData,unsigned char Op)
	{
		double val;
		if (irRegData.GetVal(&val)==0)
		{
			return OpVal(val,Op);
		}
		return -1;
	}
	void ToString(string& str) const;
	KLDump& Dump(KLDump& dump) const;

	enum {TypeChar=0};//字串
	enum {TypeBCDNum=1};//非负数
	enum {TypeBCDNegaNum=2};//负数
	enum {TypeBCDTime=3};//时间
	enum {TypeUnknown=0x0F};//无效数据
	enum {FlagInvalid=0x0F};//无效标识

	enum {OpPlus=0};
	enum {OpMinus=1};
	enum {OpMulti=2};
	enum {OpDivide=3};
	enum {OpZfg=4};//均方根
	enum {OpWindowMinus=5};//窗口差（电量计算）

	enum {TimeWW=8};
	enum {TimeCC=7};
	enum {TimeYY=6};
	enum {TimeMM=5};
	enum {TimeDD=4};
	enum {TimeHH=3};
	enum {TimeNN=2};
	enum {TimeSS=1};
};

#endif

