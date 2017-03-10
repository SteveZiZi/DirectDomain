#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../base/KLDump.h"
#include "../base/KLTrace.h"
#include "../include/Define.h"
#include"CanbusRule.h"

#include "../base/klb_base.h"
#include "../base/klb_io.h"
#include "../share/context.h"
#include "../include/FileDir.h"
#include "../config/ContextCfg.h"

#ifndef WIN32
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#endif

int CCanbus::m_sockfd = -1;

bool CCanbus::Open()
{
#ifndef WIN32
	struct sockaddr_can addr;
	int family = PF_CAN; 
	int type = SOCK_RAW;
	int proto = CAN_RAW;

	if (m_sockfd != -1)
		return true;

	//ContextCfg::ExecScript(CONTEXT, NAME_SCRIPT_CAN0_IPCONFIG);
	
	if ((m_sockfd = socket(family, type, proto)) < 0) 
	{
		KL_ERROR("SOCKET 创建失败\n");
		return false;
	}

	addr.can_family = family;
	addr.can_ifindex = if_nametoindex(DEVICE_CAN_NAME);

	if (bind(m_sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
	{
		KL_ERROR("CAN0 绑定失败\n");
		close(m_sockfd);
		return false;
	} 
	KL_VERBOSE("Can 打开成功");
#endif
	return true;
}

void CCanbus::Close()
{
#ifndef WIN32
	close(m_sockfd);
#endif
}
int CCanbus:: Send(INT8U srcaddr,INT8U destaddr,INT8U cmd,INT8U data[8],INT8U  len)
{
#ifndef WIN32
	struct can_frame frame;

	frame.can_id = (INT32U)(CAN_EFF_FLAG|(0x00<<24|cmd<<16|destaddr<<8|srcaddr));

	if (len > 8)//数据区最大支持8字节
	{
		return -1;
	}
	
	frame.can_dlc = len;
	for (int i=0; i<len; i++)
	{
		frame.data[i] = data[i];
	}
	int nbytes = send(m_sockfd, &frame, sizeof(frame),0);  //发送frame
	if (nbytes == -1)
		nbytes = send(m_sockfd, &frame, sizeof(frame),0); 
	
	KL_VERBOSE("nbytes = %d,sizeof frame = %d\n",nbytes,sizeof(frame));
	if (nbytes != sizeof(frame)) 
	{
	   // ContextCfg::ExecScript(CONTEXT, NAME_SCRIPT_CAN0_IPCONFIG);
	  //  msleep(2000);
	    return -1;        //发送错误，退出
	}

	bstring sendstr;
	INT8U *pdata = (INT8U *)(&frame);
	sendstr.append(pdata,sizeof(frame));

	KL_INFO_DUMP(dump);
	dump<<"Canbus Send "<<"("<<nbytes<<"):"<<sendstr<<"\n";

	return nbytes;
#else
	return -1;
#endif

}

bool CCanbus:: Recv(INT8U srcAddr,INT8U destAddr,INT8U data[8],INT8U  &retlen,INT8U timeOuts)
{
#ifndef WIN32
	struct can_frame frame;
	struct can_filter rfilter;
	 //定义接收规则，只接收表示符等于0x11的报文
	rfilter.can_id   = srcAddr<<8|destAddr;//只接收这个地址的回复报文
	rfilter.can_mask = CAN_SFF_MASK;
	setsockopt(m_sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

	/* 接收数据*/
	int counter = 0;
	while(1)
	{
		int nbytes = recv(m_sockfd, &frame, sizeof(frame),MSG_DONTWAIT);   //接收报文

		if (nbytes < 0)
		{
			msleep(5);
			counter++;
			if (counter > timeOuts*1000/5)
			{
				KL_ERROR("接受数据超时\n");
				system("ifconfig can0 down &");
				msleep(1000);
				system("ifconfig can0 up &");
				msleep(2000);
				return false;
			}
			continue;
			
		}

		bstring recstr;
		INT8U *pdata = (INT8U *)(&frame);
		recstr.append(pdata,sizeof(frame));

		KL_INFO_DUMP(dump);
		dump<<"Canbus Recv "<<"("<<nbytes<<"):"<<recstr<<"\n";
		KL_VERBOSE("ID=0x%X DLC=%d \n", frame.can_id,frame.can_dlc);
		for (int i=0; i<frame.can_dlc; i++)
		{
			data[i] = frame.data[i];
		}
		break;
	}
	retlen = frame.can_dlc;
	return true;
#else
	return false;
#endif
}


