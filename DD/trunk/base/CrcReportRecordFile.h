#ifndef INCLUDED_REPORT_CYC_RECORD_FILE
#define INCLUDED_REPORT_CYC_RECORD_FILE

#ifndef INCLUDED_CRC_RECORD_FILE
#include "CrcRecordFile.h"
#endif

class CrcReportRecordFile:public CrcRecordFile
{
public:
	inline CrcReportRecordFile(const char* ipFileName,unsigned long ilMaxDots,unsigned long ilDotLen,unsigned long iInterVal=1,unsigned short isInterUnit=2,unsigned long iOffsetVal=0,unsigned short isOffsetUnit=2,unsigned short iMeterDataLen=0):CrcRecordFile(ipFileName,ilMaxDots,ilDotLen,iInterVal,isInterUnit,iOffsetVal,isOffsetUnit,iMeterDataLen)
	{
		m_tMaxReportDataTime=m_tMaxTime;
	}

	bool GetReportRecords(time_t itStartTime,unsigned long iiMaxDots,unsigned int iFreq,bstring* oDataResult,unsigned long* oRetDots,char* opBcdTime7,unsigned int iiCutOffset=0,unsigned int  iiCutLen=0,unsigned char icBaseDots=1,LAST_RECORD_CHECK_PROC lrcProc=NULL,void* ipOther=NULL,bool ibUpdateReport=true);

protected:
	inline CrcReportRecordFile()
	{
		m_tMaxReportDataTime=m_tMaxTime;
	}

	bool CreateHeader(unsigned char* ipHead128);
	bool ParseHeader(unsigned char* ipHead128);
	void UpdateHeader(unsigned char* ipHead128);
	void OnRecordChanged(bool bFirstRecordChanged);
public:
	time_t m_tMaxReportDataTime;
};

#endif
