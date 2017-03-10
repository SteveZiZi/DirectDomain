///*Action.h操作*/

#ifndef INCLUDED_ACTION
#define INCLUDED_ACTION

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

//SysConfig中的设置将在出厂时设置，即使装置参数复位也不会掉失
struct Action
{
	INT8U Type;//操作类型
	char Key[CONS_ACTION_KEY_MAX_LEN+1];//操作名称
	char Name[CONS_ACTION_NAME_MAX_LEN+1];//操作名称
	char Param[CONS_ACTION_PARAM_MAX_LEN+1];//操作参数
public:
	inline Action()
	{
		Init();
	}
	inline void Init()
	{
		memset(this,0,sizeof(*this));
	}
	inline bool IsValid() const
	{
		return IsValidActionType(Type);
	}
	KLDump& Dump(KLDump& dump) const;
};

struct Actions
{
	INT32U Count;
	Action Item[CONS_ACTION_MAX_COUNT];
public:
	inline Actions()
	{
		Init();
	}
	void Init()
	{
		memset(this,0,sizeof(*this));
	}
	void Default();

	Action* Add(INT8U type,const char* key,const char* name,const char* param);
	void SetBitSet(const char* valList,DataBitSet& oDBS);
	Action* FindItem(const char* key,INT32U* opIndex);
	void GetItemsString(string &o_str,DataBitSet& i_DBS,unsigned int i_NumPerRow=0,unsigned int i_PreTabNum=0);
	KLDump& Dump(KLDump& dump) const;
};


#endif

