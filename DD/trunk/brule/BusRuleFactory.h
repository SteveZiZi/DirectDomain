#ifndef INCLUDED_BUSFACTORY
#define INCLUDED_BUSFACTORY

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "../share/SysBase.h"
#endif

#ifndef INCLUDED_RS485RULE
#include "RS485Rule.h"
#endif


#ifndef INCLUDED_METER
#include "../share/Meter.h"
#endif


#ifndef INCLUDED_METER_RULE
#include "../share/MeterRule.h"
#endif

#ifndef INCLUDED_CONNECTION
#include "../share/Connection.h"
#endif

#ifndef INCLUDED_BUSCHANNELSRULE
#include "../share/BusChannelsRule.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../share/Config.h"
#endif

#ifndef INCLUDED_CONNECTION_FACTORY
#include "../conn/ConnectionFactory.h"
#endif

//#ifndef INCLUDED_CONTEXT
//#include "../share/Context.h"
//#endif
#include "../base/PortSet.h"
//
class BusChannelsRule;
class Connection;

struct BusChannelsRuleItem
{
	INT8U ID;
	BusChannelsRule* Rule;

	BusChannelsRuleItem()
	{
		ID=METER_RULE_UNKNOWN;
		Rule=NULL;
	}
	//≈≈–Ú ± π”√
	inline operator INT8U() const
	{
		return ID;
	}


};

class GDBusRuleFactory
{

public:
	static GDBusRuleFactory& Instance()
	{
		static GDBusRuleFactory sFactory;
		return sFactory;
	}

    static BusChannelsRule* GetBusRule(const Meter& meter);
	static BusChannelsRule* GetBusRule(const INT8U& busno);
    
	inline static BusChannelsRule* GetRule(INT8U RuleID)
	{
		return Instance().FindRule(RuleID);
	}

	static BusChannelsRule* GetAnyRule();
	int OnSysInit(struct Context& oContext);

	inline INT32U Count() const
	{
		return m_Count;
	}
	BusChannelsRuleItem* Item(INT32U index)
	{
		return (index<m_Count?&m_BusRuleItems[index]:NULL);
	}
	static int AdaptComm();
private:
	GDBusRuleFactory();

	static void AddRule(BusChannelsRule& rule);

	static int  InstanceConnection(BusChannelsRule &io_pconnrule,const int &i_channelNo);

	int LoadBusRuleInfos(struct BusRuleInfos& busruleInfos);

	BusChannelsRule* FindRule(INT8U RuleID);
private:
	static BusChannelsRuleItem m_BusRuleItems[CONS_METER_RULE_MAX_COUNT];
	static INT32U m_Count;
	static PortSet m_GatherMeterPortSet;
	
};


#endif

