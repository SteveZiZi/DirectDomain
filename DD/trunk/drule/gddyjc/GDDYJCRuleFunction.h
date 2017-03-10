#ifndef INCLUDED_GDDYJC_RULE_FUNCTION
#define INCLUDED_GDDYJC_RULE_FUNCTION

#ifndef INCLUDED_STD_INCLUDE
#include "../../include/StdInclude.h"
#endif

#ifndef INCLUDED_CONTEXT
#include "../../share/Context.h"
#endif

#ifndef INCLUDED_FILE_BASE
#include "../../base/FileBase.h"
#endif

#ifndef DATA_CONVERT
#include "data_convert.h"
#endif

typedef struct ReportData
{
/*
	用于所有上报需要确认的命令
	使用时候,要把所有符合条件的数据一次性全部读入bs_AllData里面.
*/
public:
    bool bo_NotConfirmed;//可以组织上报帧则为假,否则就认为是已经上报,但还没有受到确认.
	bstring bs_req;//主站下来的请求帧
	bstring bs_rep;//已经上报的帧
	bstring bs_confirm;//本地组织的确认帧
	bstring bs_AllData;//所有符合条件的数据
	bstrings bss_AllFrame;//所有要发的帧
	time_t t_AllFrame;//所有帧组织完成的时间//当超过10分钟,则将这个上报的所有数据清空
	bool bo_RequestReset;//是否需要复位
	time_t t_SendTime;//帧被发送的时刻或者重发的时刻
	INT32U i_sendTimes;//发送次数
	INT32U i_maxSendTimes;//最大重发次数
	INT32U i_TimeOutSpace;//超时间隔  单位是分钟
	INT32U i_Counts;//上边下来的数据要求发送的报警数量
	unsigned char c_time[5];//要求上报的起始时间
	INT32U i_reported;//已经上报的数量
	INT32U i_FrameNum;//上次发出的是第几帧
	INT32U i_FrameCounts;//帧总数
	bool bo_checkTimeOutEnable;//是否允许超时检查.为真时允许,当第一次上报时将其置为真.当全部上报完毕时候,则将其置为假
	bool bo_complete;//是否完成没有完成则为假
	INT32U IN_packageLen;//包最大长度仅供参考

public:
	void init()
	{
		i_FrameNum=0; 
		i_sendTimes=0;
		i_maxSendTimes=3;
		i_TimeOutSpace=5;
		i_FrameCounts=0;
	    bo_NotConfirmed=false;
		i_reported=0;
		i_Counts=0;
		bo_checkTimeOutEnable=true;
		bo_complete=true;
		bo_RequestReset=false;

		for(int i=0;i<5;i++)
			c_time[i]=0x00;
	}
	void reset()
	{
		i_sendTimes=0;	 
		i_maxSendTimes=3;
		i_TimeOutSpace=5;
		i_reported=0;		
		IN_packageLen=0;
		bo_RequestReset=false;
		memset(c_time,0,5);
		bs_rep.clear();
		bs_confirm.clear();
		bs_req.clear();
		bs_AllData.clear();
		
		bo_complete=true;
		bo_checkTimeOutEnable=false; 
		bo_NotConfirmed=false;

		bss_AllFrame.clear();
		t_AllFrame=-1;
		i_Counts=0;
		i_FrameCounts=0;
	}

	void ClearFrames()
	{
		
		if(bo_RequestReset)
		{
			time_t timeNow;
			timeNow=time(&timeNow);
			if(datediff(2,t_AllFrame,timeNow)>=20)//帧已经组织完成了有20分钟了
			{
				reset();
			}
		}
	}

	//void sendData(Connection& irConnection)
	//{
	//	if(!bo_checkTimeOutEnable)//如果不需要检查超时
	//		return;
	//	//需要检查超时有两个原因,
	//	//第一:当前已经发送了一个帧了但还没有受到一个确认.
	//	//第二:发送了,也受到确认了,但还有数据需要上报
	//	if(!bo_NotConfirmed)//组织新的帧发送
	//		MakeNewFrame(irConnection);	

	//	time_t timeNow;
	//	timeNow=time(&timeNow);
	//	if((datediff(2,timeNow,t_SendTime)>=(long)i_TimeOutSpace)&&(i_sendTimes<3))
	//	{
	// 		bstrings send;
	//		bstring sed;
	//		sed=send.add();
	//		sed=bs_rep;
	//		irConnection.SendData(send);
	//		t_SendTime=time(&(t_SendTime));
	//		i_sendTimes+=1;
	//	}else
	//	{
	//		MakeNewFrame(irConnection);
	//	}
	//}			
	void MakeNewFrame(Connection& irConnection)//组织新的帧
	{
		bs_rep.clear();
		bs_confirm.clear();
		i_sendTimes=0;
		bs_confirm.clear();



	}
	bool TimeFixed(unsigned char* reqTime,unsigned char yy,unsigned char mm,unsigned char dd,unsigned char hh,unsigned char ff)
	{//数据时间早于请求时间,则返回真		
		  if(reqTime[0]<yy)
			  return true;

		  if((reqTime[0]==yy)&&(reqTime[1]<mm))
			  return true;

		  if((reqTime[0]==yy)&&(reqTime[1]==mm)&&(reqTime[2]<=dd))
			  return true;

		  /*if((reqTime[0]==yy)&&(reqTime[1]==mm)&&(reqTime[2]==dd)&&(reqTime[3]<hh))
			  return true;

		  if((reqTime[0]==yy)&&(reqTime[1]==mm)&&(reqTime[2]==dd)&&(reqTime[3]==hh)&&(reqTime[4]<=ff))
			  return true;*/

		  return false;
	}
}REPORTDATA;

/*0密码错误,1密码正确*/
INT32U check_PassWord(INT8U *pass,int which);

//判断密码权限，返回对该数据表示所代表的数据的操作权限。
INT32U check_PassWord(INT8U *pass,INT8U which);
/**/

bool getTaskByDataType(Task &task,DATA_TYPE datype,int _1_485_or_4_zbmulti_or_Null=0);
unsigned int Convert_Tn_to_bcd(bstring::iterator bg,bstring::iterator ed);
bool lsCompressBCD(INT8U ctemp);
bool lsCompressBCDstr(INT8U *ctemps,int length);

#endif

