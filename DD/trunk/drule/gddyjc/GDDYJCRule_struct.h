#ifndef INCLUDED_GDDYJC_STRUCT_DATA
#define INCLUDED_GDDYJC_STRUCT_DATA

#ifndef DATA_CONVERT
#include "data_convert.h"
#endif

//typedef struct M_Meter
//{
//	unsigned char meter_code[6];
//}m_Meter;

struct 	sRTUA_Struct
{
	unsigned char l_cRTUA_ll;
	unsigned char l_cRTUA_l;
	unsigned char l_cRTUA_h;
	unsigned char l_cRTUA_hh;
};

typedef struct _heartbeat
{
	int i_HeartBeat_times;//+1 ��Ϊ��ǰ�ڼ����������������ڵ��ڶ������BEATIMES��ͷ��ؼ�
	int i_BEATIMES;//�����������Ӧ���������
	int i_BeatReply;//�����Ӽ��㣬�������         ���������sysconfig�ж��ж���
	time_t tim_LastBeatTime;

public:
	int init(int HeartBeating_times=0,time_t LastBeatTime=0,int BEATIMES=3,int BeatReply=30)
	{
		 i_HeartBeat_times=HeartBeating_times;
		 i_BEATIMES=BEATIMES;
		 i_BeatReply=BeatReply;
		 tim_LastBeatTime=LastBeatTime;
		 return 0;
	}
	//�������ʾ��������������������������Ӧ����ǰ��N�ε�Ӧ���Ѿ��յ�
	bool HeartBeat(Connection& irConnection,bstring beat_str)
	{
			time_t tim_BeatNow;
			time(&tim_BeatNow);
			if(i_HeartBeat_times>=i_BEATIMES)
			{
				//KL_VERBOSE("����%d��û�л�Ӧ\n",i_BEATIMES);
				i_HeartBeat_times=0;
				tim_LastBeatTime=0;
				return false;
			}
			if(tim_LastBeatTime==0)
			{
				time(&tim_LastBeatTime);
				if(irConnection.IsConnected())
				{
					//KL_VERBOSE("���͵�%d������֡\n",i_HeartBeat_times);
					if(irConnection.SendData(beat_str))
					{
						i_HeartBeat_times+=1;
						tim_LastBeatTime=tim_BeatNow;
						return true;
					}
				}else{
					return false;
				}
			}	
			int dif=datediff(2,tim_LastBeatTime,tim_BeatNow);			
			//if(datediff(2,tim_LastBeatTime,tim_BeatNow)>i_BeatReply)
			if(dif>=i_BeatReply)
			{
				if(irConnection.IsConnected())
				{
					//KL_VERBOSE("%d minutes later Now send the %dth frame\n",i_BeatReply,i_HeartBeat_times);
					if(irConnection.SendData(beat_str))
					{
						i_HeartBeat_times+=1;
						tim_LastBeatTime=tim_BeatNow;
						return true;
					}
				}else{
					return false;
				}
			}
			return true;	
	}
	int HeartBeat_confirm()
	{
		//KL_VERBOSE("�յ�������Ӧȷ��\n",i_HeartBeat_times);
		i_HeartBeat_times=0;
		//tim_LastBeatTime=0;
		return 0;
		  
	}

	bool time_out()
	{
		if(tim_LastBeatTime==0)
			return true;

		time_t tim_BeatNow;
		time(&tim_BeatNow);
		int dif=datediff(2,tim_LastBeatTime,tim_BeatNow);	
		
		if(dif>=i_BeatReply)//��ʱ
		{
			return true;
		}else
		{
			return false;
		}
	}

	bool Set_Now()
	{
		time(&tim_LastBeatTime);
		return true;
	}
}HEARTBEATING;

#endif
