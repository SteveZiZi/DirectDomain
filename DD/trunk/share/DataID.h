///*RegID.h�а��������Ĵ���ID��صĶ���*/

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



//������ID��ΪINT32U�ͣ�������Ҫ��DataID�������Ӿ�̬��������Ź�����ο���EAC5000��������ű�.doc��
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
	inline static unsigned char GetXW(unsigned int iiID)//��ȡiiID����λֵ
	{
		return ((iiID>>20) & 0x07);
	}
	inline static unsigned char GetValueType(unsigned int iiID)
	{
		return (iiID & 0xFF);
	}
	inline static unsigned char GetXWType(unsigned int iiID)//��ȡ��λ����
	{
		return ((iiID>>20) & 0x07);
	}
	inline static unsigned int ChangeDataType(unsigned int iiID,unsigned char icNewDataType)
	{
		return (iiID & 0xFFFFFFF0)+icNewDataType;
	}

	inline static unsigned int GetCurPeriodID(unsigned int iiID)//��ȡiiID��Ӧ�ĵ�ǰʱ�ڵ�ID�������ȡ�����жȶ�Ӧ�ĵ�ǰ���ж�ID��
	{
		return (iiID & 0xF7FFFFFF);
	}
	inline static unsigned int GetPrePeriodID(unsigned int iiID)//��ȡiiID��Ӧ����ʱ�ڵ�ID
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
		DataHD=0,//�ж�
		DataDL=1,//����
		DataGL=4,//����
		DataGLYS=5,//��������
		DataXL=6,//����
		DataDV=8,//��ѹ
		DataDA=9,//����
		DataPL=10,//Ƶ��
		DataXBDV=12,//г����ѹ
		DataXBDA=13 //г������
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
		XWT=0,//����
		XWA=1,//A��
		XWB=2,//B��
		XWC=3,//C��
	};
	enum QuadType //����
	{
		QuadZX=0,//����
		QuadFX=1,//����
		QuadSZ=2,//����
		Quad1=4,//һ����
		Quad4=5,//������
		Quad2=6,//������
		Quad3=7,//������
	};

	enum ValueType
	{
		OrigValue=0,//ԭʼֵ

		SumValue=1,//�ۼ�ֵ
		AverValue=2,//ƽ��ֵ
		//ReserveValue=3,

		MaxValue=4,//���ֵ
		MaxTime=5,//���ֵʱ��

		MinValue=6,//��Сֵ
		MinTime=7,//��Сֵʱ��

		ZeroSumTime=8,//Ϊ���ۼ�ʱ��
		YxSumTime=9,//Խ���ۼ�ʱ��
		YsxSumTime=10,//Խ�����ۼ�ʱ��
		YssxSumTime=11,//Խ�������ۼ�ʱ��
		YxxSumTime=12,//Խ�����ۼ�ʱ��
		YxxxSumTime=13,//Խ�������ۼ�ʱ��
		ValidSumTime=14,//�ϸ��ۼ�ʱ��
		NEqSumTime=15,//��ƽ���ۼ�ʱ��
		BetweenSumTime=16//λ�ڶ�ֵ1��2֮���ۼ�ʱ��
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
			const unsigned int* iter=lower_bound(Item,Item+Count,iiDataID);//���ַ�����
			if (iter==Item+Count)
			{
				Item[Count++]=iiDataID;
			}
			else if ((*iter)!=iiDataID)
			{
				memmove((void*)(iter+1),iter,(Item+Count-iter)*sizeof(*iter)); //�¹� �޸� ����ϵͳbug
				Count++;
				Item[iter-Item]=iiDataID;
			}
	}
	int FindID(unsigned int iiDataID,unsigned int* opIndex) const
	{
		const unsigned int* iter=lower_bound(Item,Item+Count,iiDataID);//���ַ�����
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
	//i_cSetType��0���ӦλΪ1��i_cSetType=0���ӦλΪ0
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

//�Ĵ���ID���ϣ�����װ�÷�ʱ�������Ĵ棩�е�ID�Լ�����ʱ�е�ID
struct RegIDS:public DataIDS<CONS_REG_ID_MAX_COUNT>
{
};

////�¼�ID���ϣ�����װ���¼���ID�Լ�����¼���ID
//struct EventIDS:public DataIDS<CONS_EVENT_ID_MAX_COUNT>
//{
//};
typedef vector <unsigned int> INT32UKLIDs;

#endif


