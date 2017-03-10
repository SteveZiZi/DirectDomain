///*DeviceRuleInfo.h装置规约信息*/

#ifndef INCLUDED_DEVICE_RULE_INFO
#define INCLUDED_DEVICE_RULE_INFO

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_DATA_ID
#include "DataID.h"
#endif


class KLDump;

struct DeviceRuleInfo
{
	INT8U RuleID;//规约ID
	char Name[CONS_RULE_NAME_MAX_LEN+1];//规约名称

	INT32U PackageMaxLen;


	//DataBitSet EventDBS;//配置的事件ID集合

	char* GetKey() const;

	KLDump& Dump(KLDump& dump) const;

};

struct DeviceRuleInfos
{
	INT32U Count;
	DeviceRuleInfo Item[CONS_DEVICE_RULE_MAX_COUNT];
public:
	inline DeviceRuleInfos()
	{
		Init();
	}
	inline void Init()
	{
		memset(this,0,sizeof(*this));
	}
	bool HasRule(INT8U ruleID) const;

	KLDump& Dump(KLDump& dump) const;
};

#endif

