///*MeterRuleInfo.h�����Ϣ*/

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
	INT8U RuleID;//��ԼID��  GB645,GD645,MK,LGR��˳�����
	char Name[CONS_RULE_NAME_MAX_LEN+1];//���Լ��
	struct PortSet DefPortSet;//Ĭ�϶˿�����
	char DefUser[CONS_METER_USER_MAX_LEN+1];//Ĭ���û���
	char DefPwd[CONS_METER_PWD_MAX_LEN+1];//Ĭ������

	INT32U CharReadTimeoutMs;//�ַ���ʱ
	INT32U WriteTimeoutMS;//Ĭ�ϵ�д��ʱ
	INT32U ReadTimeoutMS;//Ĭ�ϵĶ���ʱ
	INT32U SubReadTimeoutMS;//���Ե���������ʱ

	INT16U DefMinCommInterS;//Ĭ�ϵ�����ͨѶ֮�����С���
	INT16U DefCommRetryTimes;//Ĭ�ϵ�ͨѶ���Դ���

	DataBitSet RBS;//֧�ֵ������Ĵ�������
	DataBitSet EBS;//֧�ֵ��¼�����
	DataBitSet FBS;//֧�ֵķ�ʱ����

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
