#ifndef INCLUDED_KL_DUMP
#define INCLUDED_KL_DUMP

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_STRING
#include "String.h"
#endif

#ifndef INCLUDED_KL_TRACE
#include "KLTrace.h"
#endif

class KLDump
{
public:
	inline KLDump(FILE* pFile = stdout):m_Trace(TRACE_LEVEL_DISABLED,0)
	{
		m_pFile=pFile;
		m_Level=-1;
		m_Option=0;
		m_nDepth = 0;
	}

	inline KLDump(TRACE_LEVEL level,int option):m_Trace(level,option)
	{
		m_pFile=NULL;
		m_Level=level;
		m_Option=option;
		m_nDepth = 0;
	}

	inline int GetDepth() const      // 0 => this object, 1 => children objects
	{
		return m_nDepth;
	}
	inline void SetDepth(int nNewDepth)
	{
		m_nDepth = nNewDepth;
	}
	inline void Reset()
	{
		m_Trace.Reset();
	}

	KLDump& operator<<(const TRACE_LEVEL v);
	KLDump& operator<<(const EXPORT_WAY v);
	KLDump& operator<<(const TraceSetting& v);
	KLDump& operator<<(const char* str);
	KLDump& operator<<(char* str)
	{
		return operator<<((const char*)str);
	}

	KLDump& Indent();
	template<class V>
	KLDump& operator<<(const V& v)
	{
		return v.Dump(*this);
	}

	KLDump& operator<<(const strings& v);

	inline KLDump& operator<<(const string& v)
	{
		return operator<<(v.c_str());
	}
	inline KLDump& operator<<(const bstring& v)
	{
		return DumpAsHex(v.data(),v.size());
	}
	KLDump& operator<<(const bstrings& v);

	KLDump& operator<<(const time_t& v);

	//KLDump& operator<<(INT64U v);
	//KLDump& operator<<(INT64S v);
	KLDump& operator<<(INT32U v);
	KLDump& operator<<(INT32S v);
	
	inline KLDump& operator<<(INT16U v)
	{
		return operator<<((INT32U)v);
	}
	inline KLDump& operator<<(INT16S v)
	{
		return operator<<((INT32S)v);
	}
	inline KLDump& operator<<(INT8U v)
	{
		return operator<<((INT32U)v);
	}
	inline KLDump& operator<<(INT8S v)
	{
		return operator<<((INT32S)v);
	}


	KLDump& DumpChar(char c);
	KLDump& DumpBcd7Time(const unsigned char bcdTime[]);

	KLDump& DumpAsHex(INT8U v);
	KLDump& DumpAsHex(INT16U v);
	KLDump& DumpAsHex(INT32U v);
	KLDump& DumpAsHex(INT64U v);
	KLDump& DumpAsHex(const unsigned char* pData,size_t Len);
	KLDump& operator<<(FP32 v)
	{
		return operator<<((FP64)v);
	}
	KLDump& operator<<(FP64 v);

protected:
	inline void operator=(const KLDump& dcSrc){}
	void OutputString(const char* str);

	int m_nDepth;

public:
	FILE* m_pFile;
	KLTrace m_Trace;
	int m_Level;
	int m_Option;
};

#define KL_PRINTF_DUMP(dump) KLDump dump
#define KL_ERROR_DUMP(dump) KLDump dump(TRACE_LEVEL_ERROR,0)
#define KL_WARN_DUMP(dump) KLDump dump(TRACE_LEVEL_WARN,0)
#define KL_INFO_DUMP(dump) KLDump dump(TRACE_LEVEL_INFO,0)
#define KL_VERBOSE_DUMP(dump) KLDump dump(TRACE_LEVEL_VERBOSE,0)

#define KL_VERBOSE_DATA_DUMP(dump) KLDump dump(TRACE_LEVEL_VERBOSE,TRACE_OPTION_DATA_STREAM)
#define KL_VERBOSE_SCHEDULE_DUMP(dump) KLDump dump(TRACE_LEVEL_VERBOSE,TRACE_OPTION_SCHEDULE)

#endif
