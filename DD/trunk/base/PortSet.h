///*PortSet.h中端口设置定义*/

#ifndef INCLUDED_PORT_SET
#define INCLUDED_PORT_SET

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

class KLDump;

struct PortSet
{
	INT32U Baud;	/*115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600,300*/
	INT8S DataBit; /*5,6,7,8 */
	INT8S StopBit; /*1->1位,2->2位,3->1.5位*/
	INT8S Parity; /*0->no parity;1->odd parity;2->even parity; */

	void Init();
	void FullMerge(const PortSet& irOther);
	void PartMerge(const PortSet& irOther);
	void MergeDefault();
	inline bool IsFullValid() const
	{
		return (Baud>0 && DataBit>0 && StopBit>=0 && Parity>=0);
	}
	KLDump& Dump(KLDump& dump) const;
};

inline bool operator== (const PortSet& lhs,const PortSet& rhs)
{
	return (lhs.Baud==rhs.Baud && lhs.DataBit==rhs.DataBit && lhs.StopBit==rhs.StopBit && lhs.Parity==rhs.Parity);
}

//CGH 2010-05-13 增加非等于运算符重载
inline bool operator!= (const PortSet& lhs,const PortSet& rhs)
{
	return (lhs.Baud!=rhs.Baud || lhs.DataBit!=rhs.DataBit || lhs.StopBit!=rhs.StopBit || lhs.Parity!=rhs.Parity);
}

#endif
