#ifndef PBZD_CLASS
#define PBZD_CLASS

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_PORT_SET
#include "../base/PortSet.h"
#endif

#ifndef INCLUDED_DATA_ID
#include "DataID.h"
#endif

#ifndef INCLUDED_SYS_BASE
#include "SysBase.h"
#endif

struct JilianSlaves
{
public:
	INT8U Enable;
	char CzdAddrs[16 + 1];
	char PbzdAddr[4 + 1];	//������ն˵�ַ8822
	INT16U CT;//��������ϵ����0��ʾδ֪
	INT16U PT;//��ѹ����ϵ����0��ʾδ֪	

	const char* SetJilianSlaveMachines(const char* jilianslavesmachines);
	const char* SetPbzdAddr(const char* PbzdAddr);
	void init();
};
#endif

