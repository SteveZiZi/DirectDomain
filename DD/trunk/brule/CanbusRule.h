#ifndef _CANBUSRULE_H_
#define _CANBUSRULE_H_

#include "../include/StdInclude.h"

/* special address description flags for the CAN_ID */
#define CAN_EFF_FLAG 0x80000000U /* EFF/SFF is set in the MSB */
#define CAN_RTR_FLAG 0x40000000U /* remote transmission request */
#define CAN_ERR_FLAG 0x20000000U /* error frame */

/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x0000FFFFU /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */

#define DEVICE_CAN_NAME	"can0"

class CCanbus
{
public:
	static bool Open();
	static void Close();
	static int Send(INT8U srcaddr,INT8U destaddr,INT8U cmd,INT8U data[8],INT8U  len);
	static bool  Recv(INT8U srcAddr,INT8U destAddr,INT8U data[8],INT8U  &retlen,INT8U timeOuts);
	
private:
	static int m_sockfd;
};

#endif



