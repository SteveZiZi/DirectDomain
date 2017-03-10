#ifndef INCLUDED_STRING
#define INCLUDED_STRING

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

using namespace std;
#include <string>


#ifndef WIN32
	//#ifdef ARCH_PPC
	//	#include  "../arch/powerpc/char_traits.h"
	//#endif
	#ifdef ARCH_ARM
		#include  "../arch/arm/char_traits.h"
	#endif
#endif



//#define PSTRING_DATA(pStr) (&(*pStr->begin()))
#define STRING_DATA(str) (&(*str.begin()))

typedef basic_string<unsigned char> bstring;


class bstrings:public vector<bstring>
{
public:
	bstring& add()
	{
		bstring str;
		push_back(str);
		return *(end()-1);
	}
};

class strings:public vector<string>
{
public:
	string& add()
	{
		string str;
		push_back(str);
		return *(end()-1);
	}
	int addfrom(const char* list,const char* delimiters,bool enableEmpty);
};

void PRINT_BSTRING(bstring &rep);

void GetPrintString(string& str,const void* pData,size_t Len,bool bHexMode,size_t MaxLen=0);
inline void GetHexString(string& str,const void* pData,size_t Len,size_t MaxLen=0)
{
	return GetPrintString(str,pData,Len,true,MaxLen);
}
inline void GetAscString(string& str,const void* pData,size_t Len,size_t MaxLen=0)
{
	return GetPrintString(str,pData,Len,false,MaxLen);
}

void GetBcdTimeString(string& str,const unsigned char bcdTime[]);
int ParseHexStr(const char* pHexStr,bstring& oData);
int ParseHexFromFile(const char* fileName,bstring& oData);
char * InterceptStr(char *target,const char* src ,int start,int end);
#endif
