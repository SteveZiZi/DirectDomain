#include "DeviceRuleFactory.h"
#include "../share/DeviceRuleInfo.h"

#define ADD_DEVICE_RULE(ruleClass) static ruleClass _##ruleClass;AddRule(_##ruleClass);

DeviceRuleFactory::DeviceRuleFactory()
{
	DECLARE_CONTEXT(context);
	LoadDeviceRuleInfos(context.DeviceRuleInfos);
}


int DeviceRuleFactory::LoadDeviceRuleInfos(DeviceRuleInfos& ruleInfos)
{
	m_Count=0;

	//<------------请使用ADD_DEVICE_RULE宏增加电表规约

	ADD_DEVICE_RULE(GDDYJCRule);
#ifdef P8090_RULE
	ADD_DEVICE_RULE(P8090Rule);
#endif
	//------------>请使用ADD_DEVICE_RULE宏增加电表规约

	DeviceRuleInfo ruleInfo;
	for(INT32U i=0;i<m_Count;i++)
	{
		m_RuleItems[i].Rule->GetRuleBaseInfo(ruleInfo);
		m_RuleItems[i].ID=ruleInfo.RuleID;
	}
	sort(m_RuleItems,m_RuleItems+m_Count);
	for(INT32U i=0;i<m_Count;i++)
	{
		m_RuleItems[i].Rule->SetDeviceRuleInfo(&ruleInfos.Item[i]);
	}
	return 0;
}

int DeviceRuleFactory::OnSysInit(Context& oContext)
{
	for(INT32U i=0;i<m_Count;i++)
	{
		m_RuleItems[i].Rule->GetRuleBaseInfo(oContext.DeviceRuleInfos.Item[i]);
		m_RuleItems[i].Rule->OnSysInit();
	}
	oContext.DeviceRuleInfos.Count=m_Count;
	return 0;
}

void DeviceRuleFactory::AddRule(DeviceRule& rule)
{
	if (m_Count>=CONS_DEVICE_RULE_MAX_COUNT)
	{
		ASSERT(false);
		KL_ERROR("Cannot add meter rule because of too many rules\n");
		return;
	}
	m_RuleItems[m_Count++].Rule=&rule;
}

DeviceRule* DeviceRuleFactory::FindRule(INT8U ruleID)
{
	const DeviceRuleItem* iter=lower_bound(m_RuleItems,m_RuleItems+m_Count,ruleID);//二分法查找
	if (iter!=m_RuleItems+m_Count && (*iter).ID==ruleID)
	{
		return (*iter).Rule;
	}
	return NULL;
}

