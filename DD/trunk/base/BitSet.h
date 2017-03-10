///*BitSet.h*/

#ifndef INCLUDED_BIT_SET
#define INCLUDED_BIT_SET

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_DEFINE
#include "../include/Define.h"
#endif


#ifndef INCLUDED_KL_DUMP
#include "../base/KLDump.h"
#endif

template <size_t _Bits>
struct FastBitSet:public bitset<_Bits>
{
	typedef unsigned long _Ty;	// base type for a storage word
	typedef INT16U SizeType;

	SizeType FromPos;
	SizeType ToPos;

public:

	bitset<_Bits>& operator-=(const bitset<_Bits>& _Right)
	{
		return operator ^=(_Right & (*this) );
	}
	/*bitset<_Bits>& operator+=(const bitset<_Bits>& _Right)
	{
		return operator |=(_Right | (*this));
	}*/
	/*FastBitSet<_Bits>& operator=(const FastBitSet<_Bits>& _Right)
	{
		this->reset();
		size_t len=_Right.size();
		for (size_t i=0; i<len; ++i)
		{
			if (_Right.test(i)) this->set(i);
		}
		this->Stat();
		return *this;
	}*/
	//FastBitSet& operator|()
	//{
	//
	//}
	size_t PosNumber(size_t _Pos) const
	{
		bitset<_Bits> _newBS= this->operator <<(this->size()-1-_Pos);
		return _newBS.count();
	}

	void Stat()
	{
		ASSERT(this->size()<=0xFFFF);
		bool first=true;
		size_t from=0;
		size_t to=0;
		for(size_t i=0;i<this->size();i++)
		{
			if (this->test(i))
			{
				if (first)
				{
					first=false;
					from=i;
				}
				to=i+1;
			}
		}
		FromPos=(INT16U)from;
		ToPos=(INT16U)to;
	}
	void Parse(const char* list)
	{
		Parse(list,0,this->size()-1);
	}
	void Parse(const char* list,size_t min,size_t max)
	{
		strings strs;
		strs.addfrom(list,",",false);
		for(strings::iterator iter=strs.begin();iter!=strs.end();iter++)
		{
			ParseOne((*iter).c_str(),min,max);
		}
		Stat();
	}
	void ToString(string& oStr,size_t min,size_t max) const
	{
		if (this->count()==max+1-min)
		{
			oStr.append("All");
		}
		else
		{
			ToString(oStr);
		}
	}
	void ToString(string& oStr) const
	{
		size_t iSize=this->size();
		if (this->count()==iSize)
		{
			oStr.append("All");
			return;
		}
		long iIndexFrom=-1;
		long iIndexTo=-1;
		long iPreIndex=-1;
		bool bFinded=false;
		bool bFirst=true;
		oStr.reserve(oStr.size()+256);
		for(unsigned int i=0;i<iSize;i++)
		{
			if (this->test(i))
			{
				if (iIndexFrom==-1)
				{
					iIndexFrom=i;
				}
				iIndexTo=i;
				iPreIndex=i;
				bFinded=true;
				if (i!=iSize-1)
				{
					continue;
				}
			}
			if(bFinded==true)
			{
				if (iIndexFrom>=0 && iIndexTo>=0 && iIndexFrom<=iIndexTo)
				{
					char temp[20];
					if (iIndexFrom==iIndexTo)
					{
						sprintf(temp,"%ld",iIndexFrom);
					}
					else
					{
						sprintf(temp,"%ld-%ld",iIndexFrom,iIndexTo);
					}
					if (bFirst==false)
					{
						oStr.push_back(',');
					}
					oStr.append(temp);
					iIndexFrom=-1;
					iIndexTo=-1;
					bFirst=false;
				}
				bFinded=false;
			}
		}

		//string str;
		//str=to_string<char, char_traits<char>, allocator<char> >();
	}	

	KLDump& Dump(KLDump& dump) const
	{
		string str;
		ToString(str);
		dump<<str;
		return dump;
	}

private:
	void ParseOne(const char* valStr,size_t min,size_t max)
	{
		if (strcasecmp(valStr,"All")==0)
		{
			for(size_t i=min;i<=max;i++)
			{
				this->set(i,true);
			}
			return;
		}

		size_t vmin=-1;
		size_t vmax=-1;
		strings strs;
		strs.addfrom(valStr,"-",false);
		if (strs.size()==0)
		{
			KL_WARN("Invalid ValStr %s\n",valStr);
			return;
		}
		const char* vs=(*strs.begin()).c_str();
		vmin=atoi(vs);
		if (vmin==0 && strchr(vs,'0')==0)
		{
			ASSERT(false);
			KL_WARN("Invalid ValStr %s\n",vs);
			return;
		}
		if (strs.size()==1)
		{
			vmax=vmin;
		}
		else
		{
			vs=(*(strs.end()-1)).c_str();
			vmax=atoi(vs);
			if (vmax==0 && strchr(vs,'0')==0)
			{
				ASSERT(false);
				KL_WARN("Invalid ValStr %s\n",vs);
				return;
			}
		}
		if (vmin<min)
		{
			KL_WARN("In para %s,max val(%d) should not less than %d\n",valStr,vmin,min);
			vmin=min;
		}
		if (vmax>max)
		{
			KL_WARN("In para %s,max val(%d) should not greater than %d\n",valStr,vmax,max);
			vmax=max;
		}
		if (vmax>=this->size())
		{
			vmax=this->size()-1;
		}
		for(size_t i=vmin;i<=vmax;i++)
		{
			this->set(i,true);
		}
	}
};


#endif
