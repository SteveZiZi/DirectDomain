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
	���������ϱ���Ҫȷ�ϵ�����
	ʹ��ʱ��,Ҫ�����з�������������һ����ȫ������bs_AllData����.
*/
public:
    bool bo_NotConfirmed;//������֯�ϱ�֡��Ϊ��,�������Ϊ���Ѿ��ϱ�,����û���ܵ�ȷ��.
	bstring bs_req;//��վ����������֡
	bstring bs_rep;//�Ѿ��ϱ���֡
	bstring bs_confirm;//������֯��ȷ��֡
	bstring bs_AllData;//���з�������������
	bstrings bss_AllFrame;//����Ҫ����֡
	time_t t_AllFrame;//����֡��֯��ɵ�ʱ��//������10����,������ϱ��������������
	bool bo_RequestReset;//�Ƿ���Ҫ��λ
	time_t t_SendTime;//֡�����͵�ʱ�̻����ط���ʱ��
	INT32U i_sendTimes;//���ʹ���
	INT32U i_maxSendTimes;//����ط�����
	INT32U i_TimeOutSpace;//��ʱ���  ��λ�Ƿ���
	INT32U i_Counts;//�ϱ�����������Ҫ���͵ı�������
	unsigned char c_time[5];//Ҫ���ϱ�����ʼʱ��
	INT32U i_reported;//�Ѿ��ϱ�������
	INT32U i_FrameNum;//�ϴη������ǵڼ�֡
	INT32U i_FrameCounts;//֡����
	bool bo_checkTimeOutEnable;//�Ƿ�����ʱ���.Ϊ��ʱ����,����һ���ϱ�ʱ������Ϊ��.��ȫ���ϱ����ʱ��,������Ϊ��
	bool bo_complete;//�Ƿ����û�������Ϊ��
	INT32U IN_packageLen;//����󳤶Ƚ����ο�

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
			if(datediff(2,t_AllFrame,timeNow)>=20)//֡�Ѿ���֯�������20������
			{
				reset();
			}
		}
	}

	//void sendData(Connection& irConnection)
	//{
	//	if(!bo_checkTimeOutEnable)//�������Ҫ��鳬ʱ
	//		return;
	//	//��Ҫ��鳬ʱ������ԭ��,
	//	//��һ:��ǰ�Ѿ�������һ��֡�˵���û���ܵ�һ��ȷ��.
	//	//�ڶ�:������,Ҳ�ܵ�ȷ����,������������Ҫ�ϱ�
	//	if(!bo_NotConfirmed)//��֯�µ�֡����
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
	void MakeNewFrame(Connection& irConnection)//��֯�µ�֡
	{
		bs_rep.clear();
		bs_confirm.clear();
		i_sendTimes=0;
		bs_confirm.clear();



	}
	bool TimeFixed(unsigned char* reqTime,unsigned char yy,unsigned char mm,unsigned char dd,unsigned char hh,unsigned char ff)
	{//����ʱ����������ʱ��,�򷵻���		
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

/*0�������,1������ȷ*/
INT32U check_PassWord(INT8U *pass,int which);

//�ж�����Ȩ�ޣ����ضԸ����ݱ�ʾ����������ݵĲ���Ȩ�ޡ�
INT32U check_PassWord(INT8U *pass,INT8U which);
/**/

bool getTaskByDataType(Task &task,DATA_TYPE datype,int _1_485_or_4_zbmulti_or_Null=0);
unsigned int Convert_Tn_to_bcd(bstring::iterator bg,bstring::iterator ed);
bool lsCompressBCD(INT8U ctemp);
bool lsCompressBCDstr(INT8U *ctemps,int length);

#endif

