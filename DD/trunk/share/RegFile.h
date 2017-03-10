#ifndef INCLUDED_REG_FILE
#define INCLUDED_REG_FILE

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

//#ifndef INCLUDED_ITEM_FILE
//#include "../base/ItemFile.h"
//#endif

#ifndef INCLUDED_SHARE_MEM
#include "../base/ShareMem.h"
#endif

#ifndef INCLUDED_REG_ITEM
#include "RegItem.h"
#endif

//#ifndef METER_REGS_H
//#include "MeterRegs.h"
//#endif

//#ifndef SEMAPHORE_H
//#include "SemObject.h"
//#endif

struct RegIDS;
struct DataBitSet;

class RegFile;

//typedef void (*BEFORE_REG_UPDATE_PROC)(RegFile& regFile);
//typedef void (*AFTER_REG_UPDATE_PROC)(RegFile& regFile);
//typedef int (*BEFORE_REG_WRITE_PROC)(RegFile& regFile,const RegItem& irRegItem);
//typedef void (*AFTER_REG_WRITE_PROC)(RegFile& regFile,const RegItem& irRegItem,const RegItem& irOldRegItem);

typedef void (*BEFORE_REG_UPDATE_PROC)(unsigned short isObjID);
typedef void (*AFTER_REG_UPDATE_PROC)(unsigned short isObjID);
typedef int (*BEFORE_REG_WRITE_PROC)(const RegItem& irRegItem);
typedef void (*AFTER_REG_WRITE_PROC)(const RegItem& irRegItem,const RegItem& irOldRegItem);

struct RegUpdateIntercept
{
	BEFORE_REG_UPDATE_PROC BeforeUpdate;
	AFTER_REG_UPDATE_PROC AfterUpdate;
	BEFORE_REG_WRITE_PROC BeforeWrite;
	AFTER_REG_WRITE_PROC AfterWrite;

	inline RegUpdateIntercept()
	{
		memset(this,0,sizeof(*this));
	}
};



struct RegInfo
{
public:
	unsigned char Flag;//0xFF表示无效，0x00表示有效，0x80表示过期
private:
	unsigned char Reserve[2];//保留
	unsigned char Time[7];//YYYYMMDDHHNNSS,BCD码
public:
	unsigned char RegFlag;
	unsigned char RegFormat;
	unsigned char RegData[8];
public:
	void GetTime(unsigned char* bcdTime7);
	void SetTime(const unsigned char* bcdTime7);
public:
	enum {ValidFlag=0x00};
	enum {InValidFlag=0xFF};
	enum {ExpiredFlag=0x80};
};

struct RegInfos
{
	RegInfo Regs[CONS_REG_ID_MAX_COUNT];	
};


struct RegDirtyFlag
{
	unsigned char Flag[CONS_REG_MAX_OBJ_COUNT][CONS_REG_ID_MAX_COUNT/CONS_REG_ID_BLOCK_COUNT];//每CONS_REGID_BLOCK_COUNT个能量寄存器信息一个脏标志

	RegDirtyFlag()
	{
		memset(this,0,sizeof(*this));
	}
	
};
//只为已经存在的表创建信号量，新增表，删除表怎么办？
struct RegHeader
{
	struct HeaderChecker Checker;
	RegDirtyFlag DirtyFlag;

};
class RegItemShareFile
{
public:
	RegInfos Infos[CONS_REG_MAX_OBJ_COUNT];
	RegHeader Header;

	enum {RegInfosCount=CONS_REG_MAX_OBJ_COUNT};
	enum {DefaultChar=0xFF};
public:

	static RegItemShareFile& Instance();

	inline static RegUpdateIntercept& GetIntercept()
	{
		return m_Intercept;
	}
	//CGH 2010-05-19 Add var:index for read multiple regitem
	int Read(unsigned short isObjID,unsigned int iiRegID,RegIDS& irRegIDS,RegItem& orRegItem,const INT8U &index=0);
	int Write(const RegItem& irRegItem,RegIDS& irRegIDS);
	int Clear(unsigned short isObjID,DataBitSet& irRBS,RegIDS& irRegIDS);
	int Reset(unsigned short isObjID);
	int Reset();

	int Save(bool bForce,bool async);

public:
	inline static void BeforeUpdate(unsigned short isObjID)
	{
		if (m_Intercept.BeforeUpdate!=0)
		{
			(*m_Intercept.BeforeUpdate)(isObjID);
		}
	}
	inline static void AfterUpdate(unsigned short isObjID)
	{
		if (m_Intercept.AfterUpdate!=0)
		{
			(*m_Intercept.AfterUpdate)(isObjID);
		}
	}	

private:

	inline static int BeforeWrite(const RegItem& irRegItem,RegIDS& irRegIDS)
	{
		if (m_Intercept.BeforeWrite!=0)
		{
			(*m_Intercept.BeforeWrite)(irRegItem);
		}
		return 0;
	}
	inline static void AfterWrite(const RegItem& irRegItem,const RegItem& irOldRegItem,RegIDS& irRegIDS)
	{
		if (m_Intercept.AfterWrite!=0)
		{
			(*m_Intercept.AfterWrite)(irRegItem,irOldRegItem);
		}
	}
private:
	static bool InitFile(FILE* fp, void* ipOther);
	static bool CheckFile(FILE* fp, void* ipOther);
	//CGH 2010-05-19 Add var:index for read multiple regitem
	int Read(unsigned int iiIndex,unsigned int iiRegID,unsigned int iiObjID,RegItem& orItem,const INT8U &index=0);
	int Write(RegIDS& irRegIDS,unsigned int iiIndex,const RegItem& irRegItem,RegItem* ipOldRegItem);
	int PrepareWrite(const RegItem& irRegItem,RegIDS& irRegIDS);

	RegInfo* GetRegInfo(unsigned short iiObjID,unsigned int iiRegID,unsigned int iiIndex,unsigned char** ppDdirtyFlag);
	void MakeRegItem(RegInfo& info,RegItem& orItem);
	void MakeRegInfo(RegInfo& info,const RegItem& item);
private:
	static RegUpdateIntercept m_Intercept;
	static ShareMem m_ShareMem;
#ifndef WIN32
	//SemObject sems[5];//5个信号量集，每个最多包含250个信号量，总共1250个信号量，对应1250个电表。
#endif
};

#define REG_FILE RegItemShareFile::Instance()
#define DECLARE_REG_FILE(regFile) RegItemShareFile& regFile=REG_FILE

class RegAccessor
{
public:
	inline RegAccessor(unsigned short isObjID):m_ObjID(isObjID)
	{
		RegItemShareFile::BeforeUpdate(m_ObjID);
	}
	inline ~RegAccessor()
	{
		RegItemShareFile::AfterUpdate(m_ObjID);
	}

	inline int Read(unsigned short isObjID,unsigned int iiRegID,RegIDS& irRegIDS,RegItem& orRegItem,const INT8U index=0)
	{
		return REG_FILE.Read(isObjID,iiRegID,irRegIDS,orRegItem,index);
	}
	int Write(const RegItem& irRegItem,RegIDS& irRegIDS)
	{
		return REG_FILE.Write(irRegItem,irRegIDS);
	}

private:
	unsigned short m_ObjID;
};



//class RegFile:protected ItemFile<RegItem>
//{
//public:
//	inline RegFile(unsigned short isObjID,const char*ipFileName,unsigned int iiMaxItems):ItemFile<RegItem>(ipFileName,iiMaxItems),m_ObjID(isObjID)
//	{
//		BeforeUpdate();
//	};
//	inline ~RegFile()
//	{
//		AfterUpdate();
//	}
//	int Read(unsigned short isObjID,unsigned int iiRegID,RegIDS& irRegIDS,RegItem& orRegItem);
//	int Write(const RegItem& irRegItem,RegIDS& irRegIDS);
//	int Clear(DataBitSet& irRBS,RegIDS& irRegIDS);
//	static const char* GetFileName(unsigned short iiObjID,unsigned int iiRegID);
//	static const char* GetMeterFileName(unsigned short iiObjID);
//	static RegUpdateIntercept& GetIntercept()
//	{
//		return m_Intercept;
//	}
//private:
//	int Read(unsigned int iiIndex,RegItem& orItem);
//	int Write(unsigned int iiIndex,const RegItem& irRegItem,RegItem* ipOldRegItem);
//	int PrepareWrite(const RegItem& irRegItem,RegIDS& irRegIDS);
//	inline void BeforeUpdate()
//	{
//		if (m_Intercept.BeforeUpdate!=0)
//		{
//			(*m_Intercept.BeforeUpdate)(*this);
//		}
//	}
//	inline void AfterUpdate()
//	{
//		if (m_Intercept.AfterUpdate!=0)
//		{
//			(*m_Intercept.AfterUpdate)(*this);
//		}
//	}	
//	inline int BeforeWrite(const RegItem& irRegItem)
//	{
//		if (m_Intercept.BeforeWrite!=0)
//		{
//			return (*m_Intercept.BeforeWrite)(*this,irRegItem);
//		}
//		return 0;
//	}
//	inline void AfterWrite(const RegItem& irRegItem,const RegItem& irOldRegItem)
//	{
//		if (m_Intercept.AfterWrite!=0)
//		{
//			(*m_Intercept.AfterWrite)(*this,irRegItem,irOldRegItem);
//		}
//	}
//public:
//	unsigned short m_ObjID;
//private:
//	static RegUpdateIntercept m_Intercept;
//
//};



#endif
