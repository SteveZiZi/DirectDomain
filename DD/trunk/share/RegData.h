///*RegData.h�а��������Ĵ���������صĶ���*/

#ifndef INCLUDED_REG_DATA
#define INCLUDED_REG_DATA

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONFIG
#include "../include/Config.h"
#endif

class KLDump;


struct RegData
{
public:
	//1��Flag,����λ��ʾ�����׼��
    //	1.1�����׼Ϊ0ʱ������λ��ʾ�������ͣ�0��ʾԭʼ�ִ���1��ʾBCD��Ǹ�����2��ʾBCD�븺����3��ʾ��׼BCD��ʱ�䣬15��ʾ��Ч����
	unsigned char Flag;

	//2��Format:������׼���������Ͳ�ͬ����ͬ��
	//	2.1, �������׼Ϊ0ʱ:
	//		21.1.1��������Ϊ0ʱ������λ����Ϊ0������λ��ʾ��ԭʼ�ִ�����
	//		2.1.2��������Ϊ1��2ʱ��Format����λ��ʾ����λ����Format����λ��ʾС��λ����
	//		2.1.3��������Ϊ3ʱ��Format��ʾ��׼��ʱ���ʽ����׼ʱ���ʽ��BCD�밴CCYYMMDD(WW)HHNNSS˳��������У�
	//			Format�ĺ��壺D7��1��ʾ���ܣ���0��ʾû���ܣ�D4~D6��ʾ��ʼ��λֵ��D3������D0~D2��ʾ������λֵ
	//			ʱ�䵥λֵ��CC=7��YY=6��MM=5��DD=4��HH=3��NN=2��SS=1ע�⣬WW���ܣ��Ƚ����⣬������DD��HH֮�䡣
	//			���磬0x64��ʾYYMMDD��0x46��ʾDDMMYY�� 0xE4��ʾYYMMDDWW,0x52��ʾMMDDHHNN��0x25��ʾNNHHDDMM,0xA5��ʾNNHHWWDDMM
	unsigned char Format;

public:
	//3��Data�������򣬿���Ϊ��ֵ��ʱ�䡢�ִ��������������ݣ�ȫ����������뷽ʽ�����򱣴�
	unsigned char Data[8];
public:
	inline RegData()
	{
		SetEmpty();
	}
	inline void SetEmpty()
	{
		memset(this,CONS_REG_DEFAULT_CHAR,sizeof(*this));
	}
	static bool IsValidFlag(unsigned char icFlag)
	{
		return ((icFlag & FlagInvalid)!=FlagInvalid);
	}
	inline unsigned char GetType() const
	{
		return GetType(Flag);
	}
	inline static unsigned char GetType(unsigned char icFlag)
	{
		return (icFlag & 0x0F);
	}

	inline unsigned int GetLen() const
	{
		return GetLen(*this);
	}

	static unsigned int GetLen(const RegData& irRegData);
	static unsigned int GetValFormatLen(unsigned char icFormat);
	inline bool IsValid() const
	{
		return IsValid(Flag);
	}
	inline static bool IsValid(unsigned char icFlag)
	{
		return GetType(icFlag)!=TypeUnknown;
	}
	static int ConvertTo(unsigned char icDestFlag,unsigned char icDestFormat,RegData& iorDestData);
	inline int ConvertTo(unsigned char icDestFlag,unsigned char icDestFormat)
	{
		return ConvertTo(icDestFlag,icDestFormat,*this);
	}
	int ConvertTo(unsigned char iValFormat,unsigned char* opData,INT32U iDataLen)const;
	int Set(unsigned char icFlag,unsigned char icFormat,const unsigned char* ipData,unsigned int iiLen);

	int GetVal(double* opVal) const;
	int GetTime(char* pBcdTime7) const;
	int GetVal(time_t* opTime) const;
	int SetVal(double idVal,unsigned char icFormat);
	int SetVal(time_t itTime,unsigned char icFormat);
	int SetVal(char* bcdTime,unsigned char icFormat);
	int OpVal(double val,unsigned char Op,unsigned char icFormat=0x00);

	int OpRegData(const RegData& irRegData,unsigned char Op)
	{
		double val;
		if (irRegData.GetVal(&val)==0)
		{
			return OpVal(val,Op);
		}
		return -1;
	}
	void ToString(string& str) const;
	KLDump& Dump(KLDump& dump) const;

	enum {TypeChar=0};//�ִ�
	enum {TypeBCDNum=1};//�Ǹ���
	enum {TypeBCDNegaNum=2};//����
	enum {TypeBCDTime=3};//ʱ��
	enum {TypeUnknown=0x0F};//��Ч����
	enum {FlagInvalid=0x0F};//��Ч��ʶ

	enum {OpPlus=0};
	enum {OpMinus=1};
	enum {OpMulti=2};
	enum {OpDivide=3};
	enum {OpZfg=4};//������
	enum {OpWindowMinus=5};//���ڲ�������㣩

	enum {TimeWW=8};
	enum {TimeCC=7};
	enum {TimeYY=6};
	enum {TimeMM=5};
	enum {TimeDD=4};
	enum {TimeHH=3};
	enum {TimeNN=2};
	enum {TimeSS=1};
};

#endif

