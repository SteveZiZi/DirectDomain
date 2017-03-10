#ifndef INCLUDED_METERDATAITEM
#define INCLUDED_METERDATAITEM

struct MeterDataItem
{
	unsigned int KLID;
	unsigned char Flag;//参考RegData的Flag说明,为15时表示自定义类型
	unsigned char Format;//参考RegData的Format说明

	MeterDataItem(unsigned int klid,unsigned char flag,unsigned char format):
	KLID(klid),Flag(flag),Format(format){}

	MeterDataItem() {}

	virtual inline ~MeterDataItem() 
	{
	}
	inline operator unsigned int() const
	{
		return KLID;
	}

	virtual  int Length() {return 0;}

	virtual unsigned short MapIID(){return 0;}
	virtual bool IsValid() const
	{
		return (KLID!=0xFFFFFFFF);
	}
};
#endif

