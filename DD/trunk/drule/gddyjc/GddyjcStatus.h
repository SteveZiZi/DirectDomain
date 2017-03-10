#ifndef INCLUDED_GDDYJC_STATUS
#define INCLUDED_GDDYJC_STATUS

#ifndef INCLUDED_CONTEXT
#include "../../share/Context.h"
#endif

#ifndef INCLUDED_GPRS_INFO
#include "../../share/GprsInfo.h"
#endif

#ifndef INCLUDED_STATUS
#include "../../share/Status.h"
#endif

#ifndef INCLUDED_GDDYJC_SET_SETTINGS
#include "GDDYJCRule_set_settings.h"
#endif

#ifndef INCLUDED_TASK_DATA_FILE
#include "../../share/TaskDataFile.h"
#endif

#ifndef __KLB_BASE_H
#include "../../base/klb_base.h"
#endif

class GddyjcStatus
{
public:
	void GetUplinkStatus(bstring &irStatus);
	void GetGatherStatus(bstring &irStatus);
private:
	void GetTaskGatherStatus(Task &task, INT8U *irTaskStatus);
	Task &GetTask(DATA_TYPE irDataType);
	INT16U GetMeterCount(METER_PROPERTY irProp);
	INT16U GetMeterCount(METER_CONNECTTYPE irType);
	unsigned char *GetSimCardIP();
};

#endif
