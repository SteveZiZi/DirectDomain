#ifndef INCLUDED_DEVICE_RULE_FACTORY
#define INCLUDED_DEVICE_RULE_FACTORY

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../share/SysBase.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_CONTEXT
#include "../share/Context.h"
#endif
#ifndef INCLUDED_DYJC_RULE
#include "gddyjc/GDDYJCRule.h"
#endif

#ifdef P8090_RULE
#ifndef INCLUDED_P8090_RULE
#include "p8090rule/P8090Rule.h"
#endif
#endif

class DeviceRule;
struct DeviceRuleInfos;
struct Context;

struct DeviceRuleItem
{
	INT8U ID;
	DeviceRule* Rule;

	DeviceRuleItem()
	{
		ID=DEVICE_RULE_UNKNOWN;
		Rule=NULL;
	}
	//≈≈–Ú ± π”√
	inline operator INT8U() const
	{
		return ID;
	}
};

class DeviceRuleFactory
{
public:
	static DeviceRuleFactory& Instance()
	{
		static DeviceRuleFactory sFactory;
		return sFactory;
	}
	inline static DeviceRule* GetRule(INT8U ruleID)
	{
		return Instance().FindRule(ruleID);
	}
	int OnSysInit(Context& oContext);
	inline INT32U Count() const
	{
		return m_Count;
	}
	DeviceRuleItem* Item(INT32U index)
	{
		return (index<m_Count?&m_RuleItems[index]:NULL);
	}
private:
	DeviceRuleFactory();
	void AddRule(DeviceRule& rule);
	int LoadDeviceRuleInfos(DeviceRuleInfos& ruleInfos);
	DeviceRule* FindRule(INT8U ruleID);
private:
	DeviceRuleItem m_RuleItems[CONS_DEVICE_RULE_MAX_COUNT];
	INT32U m_Count;
};

#endif

