#ifndef INCLUDED_IPC_INFO
#define INCLUDED_IPC_INFO

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_STRING
#include "String.h"
#endif

class IpcInfo
{
public:
	IpcInfo()
	{
		DestSvrID=0;
		SrcSvrID=0;
		InfoType=0;
		SendTime=0;
		TimeOutS=0;
	}
	size_t Serialize(unsigned char* ipBuff,size_t iiBuffLen);
	size_t Deserialize(unsigned char* ipBuff,size_t iiBuffLen);
	inline size_t GetSerialSize()
	{
		return GetMinSerialSize()+SrcInfo.size()+ResultInfo.size()+Tag.size();
	}
	static inline size_t GetMinSerialSize()
	{
		return sizeof(int)+sizeof(int)+sizeof(int)+sizeof(time_t)+sizeof(unsigned long)+3*sizeof(bstring::size_type);
	}
	bool IsValid();
public:
	int DestSvrID;
	int SrcSvrID;
	int InfoType;
	time_t SendTime;
	unsigned long TimeOutS;
	bstring SrcInfo;
	bstring ResultInfo;
	bstring Tag;
};

#endif

