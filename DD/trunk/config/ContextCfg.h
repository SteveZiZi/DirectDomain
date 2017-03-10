#ifndef INCLUDED_CONTEXT_CFG
#define INCLUDED_CONTEXT_CFG

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

#ifndef INCLUDED_STATUS_CFG
#include "StatusCfg.h"
#endif

#include "../brule/BusRuleFactory.h"

struct Context;
class DirScan;
struct Meter;
class MeterRule;

class ContextCfg
{
public:
	static int BaseInit(DirScan& dirScan);

	static int InitContext(DirScan& dirScan,Context& oContext);
	static int LoadContext(Context& oContext);

	static int MemReset(Context& oContext);
	static int DataReset(Context& oContext);
	static int ParamReset(Context& oContext);
	static int ClearLogs(Context& oContext);
	static int FactoryReset(Context& oContext);
	static int ParamChanged(Context& oContext);
	static int TimeChanged(Context& oContext);
	static int DataFileRebuilded(Context& oContext);

	static int SoftwareReset(Context& oContext);
	static int HardwareReset(Context& oContext);
	static int SetPowerSource(Context& oContext,INT8U newVal);
	static INT8U GetMeterSetTimeTrueMethod(Context& oContext,INT16U meterNo,INT8U settingMethod);

	static int ExecScript(Context& context,const char* scriptName);
	static void SetLocalIp(Context& oContext);
	static int Eth0IpConfig(Context& oContext);
private:
	static void AdjustMeters(Context& oContext);
	static void AdjustMeter(Meter& meter,MeterRule& rule);
	static void ClearMeterReg(Context& oContext);
};
#endif
