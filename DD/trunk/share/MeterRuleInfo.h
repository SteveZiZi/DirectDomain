///*MeterRuleInfo.h电表信息*/

#ifndef INCLUDED_METER_RULE_INFO
#define INCLUDED_METER_RULE_INFO

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

#ifndef INCLUDED_DATA_ID
#include "DataID.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

#ifndef INCLUDED_PORT_SET
#include "../base/PortSet.h"
#endif


struct MeterRuleInfo
{
	INT8U RuleID;//规约ID，  GB645,GD645,MK,LGR按顺序编码
	char Name[CONS_RULE_NAME_MAX_LEN+1];//表规约名
	struct PortSet DefPortSet;//默认端口设置
	char DefUser[CONS_METER_USER_MAX_LEN+1];//默认用户名
	char DefPwd[CONS_METER_PWD_MAX_LEN+1];//默认密码

	INT32U CharReadTimeoutMs;//字符超时
	INT32U WriteTimeoutMS;//默认的写超时
	INT32U ReadTimeoutMS;//默认的读超时
	INT32U SubReadTimeoutMS;//尝试的连续读超时

	INT16U DefMinCommInterS;//默认的两次通讯之间的最小间隔
	INT16U DefCommRetryTimes;//默认的通讯重试次数

	DataBitSet RBS;//支持的能量寄存器掩码
	DataBitSet EBS;//支持的事件掩码
	DataBitSet FBS;//支持的分时掩码

	static char* GetKey(INT8U ruleID);
	inline char* GetKey() const
	{
		return GetKey(RuleID);
	}
	INT8U GetTrueSetTimeMethod(INT8U method);
};

struct MeterRuleInfos
{
	INT32U Count;
	MeterRuleInfo Item[CONS_METER_RULE_MAX_COUNT];
public:
	inline MeterRuleInfos()
	{
		Init();
	}
	inline void Init()
	{
		memset(this,0,sizeof(*this));
	}
	bool HasRule(INT8U ruleID) const;
};

#endif
