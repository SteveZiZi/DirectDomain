///*RegItem.h中包括能量寄存器项相关的定义*/

#ifndef INCLUDED_REG_ITEM
#define INCLUDED_REG_ITEM

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_REG_DATA
#include "../share/RegData.h"
#endif

#ifndef INCLUDED_KL_BASE
#include "../base/KLBase.h"
#endif

class KLDump;

struct DataBitSet;

class RegItem
{
public:	
	RegItem()
	{
		memset(this,0xFF,sizeof(*this));
	};
	int Save();
	//CGH 2010-05-19 Add var:index for read multiple regitem
	int Load(unsigned short isObjID,unsigned int iiRegID,const INT8U &index=0);

	//CGH 2010-05-19 重载运算符以运用STL算法
	inline operator unsigned int() const
	{
		return RegID;
	}

	inline time_t GetTime() const
	{
		time_t t;
		Time_BCDTime7TotTime((const char*)Time,&t);
		return t;
	}
	inline void SetTime(time_t* ipTime)
	{
		if (ipTime==NULL)
		{
			Get_CurBCDTime7((char*)Time);
		}
		else
		{
			Time_tTimeToBCDTime7(*ipTime,(char*)Time);
		}
	}
public:
	unsigned int RegID;//能量寄存器ID
	unsigned char Time[7];//YYYYMMDDHHNNSS,BCD码
	unsigned short ObjID;//对象ID
	RegData Data;
	INT8U index; //CGH 2010-05-19 增加下标 解决一个regitem可能包含多个数据项的问题


public:
	KLDump& Dump(KLDump& dump) const;
	void ToString(string& str) const;
};

class RegItems:public vector<RegItem>
{
public:
	int Save();
	KLDump& Dump(KLDump& dump) const;

	static int ClearMeterRegs(unsigned short isObjID,DataBitSet& irRBS);
	static int DeleteMeterRegs(unsigned short isObjID);
};

#endif

