///*RegID.h中包括能量寄存器ID相关的定义*/

#ifndef INCLUDED_DATA_ID
#define INCLUDED_DATA_ID

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_DEFINE
#include "../include/Config.h"
#endif

#ifndef INCLUDED_KL_BASE
#include "../base/KLBase.h"
#endif

#ifndef INCLUDED_KL_DUMP
#include "../base/KLDump.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif



//数据量ID，为INT32U型，根据需要在DataID类中增加静态函数，编号规则请参考《EAC5000数据量编号表.doc》
class DataID
{
public:
	inline static unsigned char GetObjType(unsigned int iiID)
	{
		return ((iiID>>28) & 0x07);
	}
	
	inline static unsigned char GetTimeProp(unsigned int iiID)
	{
		return ((iiID>>24) & 0x0F);
	}
	inline static unsigned char GetDataType(unsigned int iiID)
	{
		return ((iiID>>16) & 0x0F);
	}
	inline static bool HasSD(unsigned int iiID)
	{
		return (iiID & 0x100F0000)<0x100C0000  && (((iiID>>20) & 0x0F)!=0x07);
	}
	inline static unsigned char GetSD(unsigned int iiID)
	{
		return ((iiID>>8) & 0x0F);
	}
	inline static unsigned int ChangeSd(unsigned int iiID,unsigned char icSdIndex)
	{
		return (iiID & 0xFFFFF0FF)+(icSdIndex<<8);
	}
	inline static unsigned char GetXW(unsigned int iiID)//获取iiID的相位值
	{
		return ((iiID>>20) & 0x07);
	}
	inline static unsigned char GetValueType(unsigned int iiID)
	{
		return (iiID & 0xFF);
	}
	inline static unsigned char GetXWType(unsigned int iiID)//获取相位类型
	{
		return ((iiID>>20) & 0x07);
	}
	inline static unsigned int ChangeDataType(unsigned int iiID,unsigned char icNewDataType)
	{
		return (iiID & 0xFFFFFFF0)+icNewDataType;
	}

	inline static unsigned int GetCurPeriodID(unsigned int iiID)//获取iiID对应的当前时期的ID（例如获取上日行度对应的当前日行度ID）
	{
		return (iiID & 0xF7FFFFFF);
	}
	inline static unsigned int GetPrePeriodID(unsigned int iiID)//获取iiID对应的上时期的ID
	{
		return (iiID | 0x08000000);
	}

	static unsigned char GetInterUnit(unsigned int iiID);
	static unsigned char GetInterVal(unsigned int iiID);
	enum TimeProp
	{
		Now=0,
		Cur5Min=1,
		Cur15Min=2,
		Cur30Min=3,
		CurHour=4,
		CurDay=5,
		CurMonth=6,
		CurYear=7,
		Pre=8,
		Pre5Min=9,
		Pre15Min=10,
		Pre30Min=11,
		PreHour=12,
		PreDay=13,
		PreMonth=14,
		PreYear=15,		
		MaxTimeProp=PreYear
	};
	enum ObjType
	{
		Device=0,
		Meter=1,
	};

	enum DataType
	{
		DataHD=0,//行度
		DataDL=1,//电量
		DataGL=4,//功率
		DataGLYS=5,//功率因数
		DataXL=6,//需量
		DataDV=8,//电压
		DataDA=9,//电流
		DataPL=10,//频率
		DataXBDV=12,//谐波电压
		DataXBDA=13 //谐波电流
	};
	enum SideType
	{
		NoSide=0,
		SecondSide=1,
		FirstSideCT=2,
		FirstSidePT=3,
		FirstSideCTPT=4
	};
	enum XWType
	{
		XWT=0,//总相
		XWA=1,//A相
		XWB=2,//B相
		XWC=3,//C相
	};
	enum QuadType //象限
	{
		QuadZX=0,//正向
		QuadFX=1,//反向
		QuadSZ=2,//视在
		Quad1=4,//一象限
		Quad4=5,//四象限
		Quad2=6,//二象限
		Quad3=7,//三象限
	};

	enum ValueType
	{
		OrigValue=0,//原始值

		SumValue=1,//累计值
		AverValue=2,//平均值
		//ReserveValue=3,

		MaxValue=4,//最大值
		MaxTime=5,//最大值时间

		MinValue=6,//最小值
		MinTime=7,//最小值时间

		ZeroSumTime=8,//为零累计时间
		YxSumTime=9,//越限累计时间
		YsxSumTime=10,//越上限累计时间
		YssxSumTime=11,//越上上限累计时间
		YxxSumTime=12,//越下限累计时间
		YxxxSumTime=13,//越下下限累计时间
		ValidSumTime=14,//合格累计时间
		NEqSumTime=15,//不平衡累计时间
		BetweenSumTime=16//位于定值1与2之间累计时间
	};
};

template<unsigned int MaxCount>
struct DataIDS
{
	INT32U Count;
	unsigned int Item[MaxCount];
public:
	inline DataIDS()
	{
		Init();
	}
	inline void Init()
	{
		memset(this,0,sizeof(*this));
	}
	void AddID(unsigned int iiDataID)
	{
			if (Count>=MaxCount)
			{
				KL_WARN( "DataID is too many.MaxCount is %d\n",MaxCount);
				return;
			}
			const unsigned int* iter=lower_bound(Item,Item+Count,iiDataID);//二分法查找
			if (iter==Item+Count)
			{
				Item[Count++]=iiDataID;
			}
			else if ((*iter)!=iiDataID)
			{
				memmove((void*)(iter+1),iter,(Item+Count-iter)*sizeof(*iter)); //陈桂华 修改 严重系统bug
				Count++;
				Item[iter-Item]=iiDataID;
			}
	}
	int FindID(unsigned int iiDataID,unsigned int* opIndex) const
	{
		const unsigned int* iter=lower_bound(Item,Item+Count,iiDataID);//二分法查找
		if (iter==Item+Count || (*iter)!=iiDataID)
		{
			return -1;
		}
		if (opIndex!=NULL)
		{
			*opIndex=(unsigned int)(iter-Item);
		}
		return 0;
	}
	void AddParse(const char* valList)
	{
		strings strs;
		strs.addfrom(valList,",",false);
		for(strings::iterator iter=strs.begin();iter!=strs.end();iter++)
		{
			AddParseOne((*iter).c_str());
		}
	}
	void AddParseOne(const char* valStr)
	{
		unsigned int vmin=0;
		unsigned int vmax=0;
		strings strs;
		strs.addfrom(valStr,"-",false);
		if (strs.size()==0)
		{
			KL_WARN("Invalid ValStr %s\n",valStr);
			return;
		}
		const char* vs=(*strs.begin()).c_str();
		vmin=String_Asc8TouLong((char*)vs);
		if (vmin==0 && strchr(vs,'0')==0)
		{
			ASSERT(false);
			KL_ERROR("Invalid min param in DataIDS.AddParseOne(%s)\n",valStr);
			return;
		}
		if (strs.size()==1)
		{
			vmax=vmin;
		}
		else
		{
			vs=(*(strs.end()-1)).c_str();
			vmax=String_Asc8TouLong((char*)vs);
			if (vmax==0 && strchr(vs,'0')==0)
			{
				ASSERT(false);
				KL_ERROR("Invalid max param in DataIDS.AddParseOne(%s)\n",valStr);
				return;
			}
		}
		for(unsigned int i=vmin;i<=vmax;i++)
		{
			AddID(i);
		}
	}

	void GetItemsString(string &o_str,DataBitSet& i_DBS,unsigned int i_NumPerRow=0,unsigned int i_PreTabNum=0)
	{
		char cTempStr[20];
		unsigned int iRegID;

		bool bNewLine=true;
		unsigned int count=0;
		bool bAll=true;
		for (size_t i=0;i<i_DBS.size();i++)
		{
			if (i_DBS.test(i))
			{
				iRegID=Item[i];
				sprintf(cTempStr,"%08X",iRegID);
				if (count>0 && i_NumPerRow>0 && (count % i_NumPerRow)==0)
				{
					o_str.append(",\n");
					bNewLine=true;
				}
				else
				{
					if (count>0)
					{
						o_str.push_back(',');
					}
					bNewLine=(count==0);
				}
				if (bNewLine==true && i_PreTabNum>0)
				{
					o_str.append((size_t)i_PreTabNum,'\t');
				}
				o_str.append(cTempStr);
				count++;
			}
			else
			{
				bAll=false;
			}
		}
		if (bAll)
		{
			o_str.append("All");
		}
	}

	void SetBitSet(const char* valList,DataBitSet& oDBS)
	{
		strings strs;
		strs.addfrom(valList,",",false);
		for(strings::iterator iter=strs.begin();iter!=strs.end();iter++)
		{
			SetBitSetOne(1,(*iter).c_str(),oDBS);
		}
	}
	void ResetBitSet(const char* valList,DataBitSet& oDBS)
	{
		strings strs;
		strs.addfrom(valList,",",false);
		for(strings::iterator iter=strs.begin();iter!=strs.end();iter++)
		{
			SetBitSetOne(0,(*iter).c_str(),oDBS);
		}
	}
	//i_cSetType非0设对应位为1，i_cSetType=0设对应位为0
	void SetBitSetOne(unsigned char i_cSetType,const char* valStr,DataBitSet& oDBS)
	{
		unsigned int vmin=0;
		unsigned int vmax=0;
		strings strs;
		strs.addfrom(valStr,"-",false);
		if (strs.size()==0)
		{
			KL_WARN("Invalid ValStr %s\n",valStr);
			return;
		}
		const char* vs=(*strs.begin()).c_str();
		vmin=String_Asc8TouLong((char*)vs);
		if (vmin==0 && strchr(vs,'0')==0)
		{
			ASSERT(false);
			KL_ERROR("Invalid min param in DataIDS.SetBitSetOne(%s)\n",valStr);
			return;
		}
		if (strs.size()==1)
		{
			vmax=vmin;
		}
		else
		{
			vs=(*(strs.end()-1)).c_str();
			vmax=String_Asc8TouLong((char*)vs);
			if (vmax==0 && strchr(vs,'0')==0)
			{
				ASSERT(false);
				KL_ERROR("Invalid max param in DataIDS.SetBitSetOne(%s)\n",valStr);
				return;
			}
		}
		unsigned int index;
		for(unsigned int i=vmin;i<=vmax;i++)
		{
			if (FindID(i,&index)==0)
			{
				if (i_cSetType!=0) oDBS.set(index);
				else oDBS.reset(index);
			}
			else
			{
				KL_WARN("Cannot find DataID %08X\n",i);
			}
		}		
	}
	KLDump& Dump(KLDump& dump) const
	{
		dump<<"DataIDS(Size="<<Count<<"):";
		for(unsigned int i=0;i<Count;i++)
		{
			dump.DumpAsHex(Item[i])<<" ";
		}
		return dump;
	}
};

//寄存器ID集合：包括装置分时（能量寄存）中的ID以及电表分时中的ID
struct RegIDS:public DataIDS<CONS_REG_ID_MAX_COUNT>
{
};

////事件ID集合：包括装置事件的ID以及电表事件的ID
//struct EventIDS:public DataIDS<CONS_EVENT_ID_MAX_COUNT>
//{
//};
typedef vector <unsigned int> INT32UKLIDs;

#endif


