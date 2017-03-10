#ifndef INCLUDED_TIMER
#define INCLUDED_TIMER

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_KL_BASE
#include "KLBase.h"
#endif

#ifndef INCLUDED_BIT_SET
#include "BitSet.h"
#endif

typedef	enum _TIME_UNIT
{
	TIME_UNIT_INVALID=0,
	TIME_UNIT_SECOND=1,
	TIME_UNIT_MINUTE=2,
	TIME_UNIT_HOUR=3,
	TIME_UNIT_DAY=4,
	TIME_UNIT_MONTH=5,
	TIME_UNIT_YEAR=6,
	TIME_UNIT_WEEK
}TIME_UNIT;

KLDump& operator<< (KLDump& dump, TIME_UNIT v);

class InterTimer
{
public:
	InterTimer()
	{
		StartUnit=TIME_UNIT_INVALID;
		StartVal=0;
		IntervalUnit=TIME_UNIT_INVALID;
		IntervalVal=0;
	}

	inline bool IsValid() const
	{
		return (StartUnit>=TIME_UNIT_SECOND && StartUnit<=TIME_UNIT_MONTH && IntervalUnit<=TIME_UNIT_MONTH && StartUnit<=IntervalUnit && IntervalUnit>=1);
	}
	bool IsTimer(time_t itCheckTime,time_t itLastTimer,time_t* otTimer);
	inline bool IsNowTimer(time_t itLastTimer,time_t* otTimer)
	{
		return IsTimer(time(NULL),itLastTimer,otTimer);
	}

	inline time_t GetTimerPoint(time_t nearTime,bool bDownBound=true)
	{
		return gettimerpoint(nearTime,IntervalUnit,IntervalVal,StartUnit,StartVal,bDownBound);
	}
	inline InterTimer& operator =(const InterTimer& irOther)
	{
		memcpy(this,&irOther,sizeof(InterTimer));
		return *this;
	}

	KLDump& Dump(KLDump& dump) const;
public:
	unsigned char StartUnit;//start time unit
	unsigned short StartVal;//start time value
	unsigned char IntervalUnit;//interval time unit
	unsigned short IntervalVal;//interval time value
};
inline bool operator== (const InterTimer& lhs,const InterTimer& rhs)
{
	return memcmp(&lhs,&rhs,sizeof(InterTimer))==0;
}

struct TimeBitSet:public FastBitSet<64>
{
	//public:
	//	void Parse(const char* list,unsigned char min,unsigned char max);
	//private:
	//	void ParseOne(const char* valStr,unsigned char min,unsigned char max);
};

struct MatchTimer
{
public:
	TimeBitSet MinuteBS;
	TimeBitSet HourBS;
	TimeBitSet DayBS;
	TimeBitSet MonthBS;
public:
	bool Match(struct tm* time) const;
	inline bool Match(time_t& time) const
	{
		return Match(localtime(&time));
	}
	inline void Reset()
	{
		MinuteBS.reset();
		HourBS.reset();
		DayBS.reset();
		MonthBS.reset();
		MinuteBS.Stat();
		HourBS.Stat();
		DayBS.Stat();
		MonthBS.Stat();
	}
	inline bool Match() const
	{
		time_t tNow = time(NULL);
		return Match(tNow);
	}
	void Set(unsigned char interUnit,long interVal,const time_t date);
	bool IsValid() const;
	KLDump& Dump(KLDump& dump) const;
};

#endif

