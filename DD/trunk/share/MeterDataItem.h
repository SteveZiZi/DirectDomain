#ifndef INCLUDED_METERDATAITEM
#define INCLUDED_METERDATAITEM

struct MeterDataItem
{
	unsigned int KLID;
	unsigned char Flag;//�ο�RegData��Flag˵��,Ϊ15ʱ��ʾ�Զ�������
	unsigned char Format;//�ο�RegData��Format˵��

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

