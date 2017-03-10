#ifndef _RTF_CTRL_H_
#define _RTF_CTRL_H_

#include "context.h"

class CRtfCtrl
{
public:
	int RtfVolCurSetCtl(SSmartGarageItem &command, INT16U vol,INT16U cur);
	int RtfClVolCurSetCtl(SSmartGarageItem &command, INT16U vol,INT16U cur);
	int RtfClOnCtl(SSmartGarageItem &command);
	int RtfClOffCtl(SSmartGarageItem &command);
	int RtfSwitchSetCtl(SSmartGarageItem &command, INT8U state, INT8U delayTime);
	bool GetRtfClVol(INT16U clNo,INT16U& clVol);
};

#endif


