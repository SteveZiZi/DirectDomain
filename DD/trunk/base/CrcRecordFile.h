#ifndef INCLUDED_CRC_RECORD_FILE
#define INCLUDED_CRC_RECORD_FILE

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_KL_BASE
#include "KLBase.h"
#endif


#include "String.h"

#define DEFAULTCHAR 0xFF
#define SUB_RECORD_HEAD_LEN 8
#define SUB_RECORD_DATA_LEN 16
#define SECONDS_OF_HOUR (3600)//每小时的秒数

typedef bool (*LAST_RECORD_CHECK_PROC)(unsigned char* ipRecordData,unsigned int iiRecordLen,void* ipOther);

class CrcRecordFile
{
public:
	inline CrcRecordFile(const char* ipFileName,unsigned long ilMaxDots,unsigned long ilDotLen,unsigned long iInterVal=1,unsigned short isInterUnit=2,unsigned long iOffsetVal=0,unsigned short isOffsetUnit=2,unsigned short iMeterDataLen=0)
	{
		Set(ipFileName,ilMaxDots,ilDotLen,iInterVal,isInterUnit,iOffsetVal,isOffsetUnit,iMeterDataLen);
	}
	void Set(const char* ipFileName,unsigned long ilMaxDots,unsigned long ilDotLen,unsigned long iInterVal,unsigned short isInterUnit,unsigned long iOffsetVal,unsigned short isOffsetUnit,unsigned short iMeterDataLen);
	virtual ~CrcRecordFile()
	{
		free(m_pFileName);
	}
protected:
	CrcRecordFile();
public:
	bool AddRecord(const char* ipBcdTime7,const unsigned char* ipData,const unsigned int iiDataLen,const unsigned char* ipFixData);
	bool AddRecord(time_t itTime,const unsigned char* ipData,const unsigned int iiDataLen,const unsigned char* ipFixData);
	bool AddRecord(const unsigned char* ipData,const unsigned int iiDataLen,const unsigned char* ipFixData);

	bool UpdateRecords(time_t tDeviceTime,bstrings& recordChips,bstring* fixChip,unsigned int iiOffset);
	bool UpdateRecords(time_t tDeviceTime,bstring& recordChip,INT16U meterPos_from,INT16U meter_count);
	
	//用于补抄写入数据文件
	/******************第一条记录的时间***间隔的时间值***所有的记录************此电表在这个任务的位置*/
	/*InterVal表示间隔了多少天或多少小时，应和recordChips.size()相等*/
	bool UpdateRecords(time_t tDeviceTime,INT8U InterVal,bstrings& recordChips,INT16U meterPos);



	bool ParseFile();
	bool DeleteFile();

	//可用于取第一条记录
	bool GetFirstRecord(bstring &bstr_data,unsigned int index_meter);
	
	//可用于取重点用户数据较为方便
	bool GetRecords(time_t fromTime,time_t toTime,INT16U meterPos,bstring& oDataResult);
	
	//可用于取日、月数据，较为方便
	bool GetRecords(const char* ipBcdTime3_start,INT16U meterPos_from,INT16U meterPos_to,bstring& oDataResult);
   
	bool UpdateRecords(bstring& recordChip,unsigned int iiOffsetMeter);
	
	inline const char* GetFileName() const
	{
		return m_pFileName;
	}
	inline time_t GetFileTime() const
	{
		return m_tFileTime;
	}
	inline time_t GetStdTime() const
	{
		return m_tStdTime;
	}
	inline time_t GetMinTime() const
	{
		return m_tMinTime;
	}
	inline time_t GetMaxTime() const
	{
		return m_tMaxTime;
	}
	inline unsigned long GetMaxDots() const
	{
		return m_lMaxDots;
	}
	inline unsigned long GetCurDots() const
	{
		return m_lCurDots;
	}
	inline unsigned long GetDotLen() const
	{
		return m_lDotLen;
	}
	inline unsigned long GetInterVal() const
	{
		return m_lInterVal;
	}
	inline unsigned short GetInterUnit() const
	{
		return m_siInterUnit;
	}
	inline unsigned long GetOffsetVal() const
	{
		return m_lOffsetVal;
	}
	inline unsigned short GetOffsetUnit() const
	{
		return m_siOffsetUnit;
	}
	enum {RecordHeaderLen=8};
protected:
	bool CreateFile();
	virtual bool CreateHeader(unsigned char* ipHead128);
	virtual bool ParseHeader(unsigned char* ipHead128);
	virtual void UpdateHeader(unsigned char* ipHead128);
	virtual bool CheckHeader(unsigned char* ipHead128);
	virtual void OnRecordChanged(bool bFirstRecordChanged);
	virtual void OnFileRebuilded();
	bool SaveHead(FILE* fp,unsigned char* pHead);
	inline time_t AdjustToStdTime(time_t nearTime)
	{
		time_t newTime=gettimerpoint(nearTime,(unsigned char)m_siInterUnit,m_lInterVal,(unsigned char)m_siOffsetUnit,m_lOffsetVal);
		return newTime;
	}
	bool InnerGetRecords(FILE* fp,unsigned char* pHead,time_t minTime,time_t maxTime,unsigned int iFreq,bstring* oDataResult,unsigned long* oRetDots,time_t* oMinTime,unsigned int iiCutOffset=0,unsigned int  iiCutLen=0,LAST_RECORD_CHECK_PROC lrcProc=NULL,void* ipOther=NULL);
	//bool InnerUpdateRecords(FILE* fp,time_t fromTime,time_t toTime,bstrings& recordChips,unsigned int iiOffset);
	//bool InnerUpdateRecord(FILE* fp,time_t destTime,unsigned char* pChipData,unsigned int chipDataLen,unsigned int iiOffset,unsigned int iiValuesOffset=0);
	bool InnerUpdateRecord(FILE* fp,time_t destTime,long dotIndex, unsigned char* chipData,unsigned int chipDataLen,unsigned int iiOffset,unsigned char recordFlag,unsigned int iiDayOffset=0);

	
	FILE* PreUpdate(unsigned char* cHead,bool bLockFile);

	virtual bool GetFixData(FILE* fp,time_t fromTime,time_t toTime,const unsigned char* data,unsigned int dataLen,unsigned int iiOffset,bstring& oFixData);
private:
	bool FixRecord(FILE* fp,time_t fromTime,time_t toTime,const unsigned char* fixData,unsigned int fixDataLen,unsigned int iiOffset);
	bool UpdateRecordWithDefaultChar(FILE* fp,unsigned int iiOffsetday);
	int GetOffsetTime(unsigned int iiOffsetValues);
	INT32U GetHourInterval(char beginTime[7],char endTime[7]);
	INT8S CompareTime(char time1[7],char time2[7],INT8U N);
	
	//判断记录中时间的有效性，并更改文件的最大和最小时间，如果正确返回此条记录在数据区的偏移位置
	bool IsValidTime(FILE* fp ,time_t tRecordTime,long &offset);

public:
	unsigned char DefaultChar;
protected:
	char* m_pFileName;
	time_t m_tFileTime;
	time_t m_tStdTime;
	time_t m_tMinTime;
	time_t m_tMaxTime;
	unsigned long m_lMaxDots;
	unsigned long m_lCurDots;
	unsigned long m_lDotLen;
	unsigned long m_lInterVal;
	unsigned short m_siInterUnit;
	unsigned long m_lOffsetVal;
	unsigned short m_siOffsetUnit;
	unsigned long m_MeterDataLen;
	bool m_bInited;
	INT8U taskNo;
};


#endif



