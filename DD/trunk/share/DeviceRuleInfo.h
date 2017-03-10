///*DeviceRuleInfo.hװ�ù�Լ��Ϣ*/

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
	INT8U RuleID;//��ԼID
	char Name[CONS_RULE_NAME_MAX_LEN+1];//��Լ����

	INT32U PackageMaxLen;


	//DataBitSet EventDBS;//���õ��¼�ID����

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

