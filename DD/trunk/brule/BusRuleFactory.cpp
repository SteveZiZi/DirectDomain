#include "BusRuleFactory.h"




BusChannelsRuleItem GDBusRuleFactory::m_BusRuleItems[CONS_METER_RULE_MAX_COUNT];
INT32U GDBusRuleFactory::m_Count = 0;
PortSet GDBusRuleFactory::m_GatherMeterPortSet;
GDBusRuleFactory::GDBusRuleFactory()
{
	DECLARE_CONTEXT(oContext);
	m_Count=0;
	LoadBusRuleInfos(oContext.BusInfos);
}


/** 
*@brief 改变不同规约的通讯连接，由通道号决定 
*@function InstanceConnection(BusChannelsRule &io_pconnrule, const int &i_channelNo)
*@param io_pconnrule, 需要被改变的通道对象
*@param i_channelNo， 要设置到的通道号
*@retval 返回成功 0 不成功 -1；
*@date 
*@author 
*/
int  GDBusRuleFactory::InstanceConnection(BusChannelsRule &io_pconnrule,const int &i_channelNo)
{
    
	if (io_pconnrule.b_conn != NULL && i_channelNo != DBCJ_3_BUSRULE_GENERAL)
	{
        delete io_pconnrule.b_conn;
		io_pconnrule.b_conn = NULL;
	}

	if (io_pconnrule.b_conn == NULL)
	{
		Connection *tmp_conn = ConnectionFactory::CreateConnection(CONTEXT.ServerInfos.Dbcj.Channel[i_channelNo-1]);
		ASSERT(tmp_conn);

		if (tmp_conn)
		{
			if (tmp_conn->IsConnected()==false)
			{	
				if(tmp_conn->EnsureOnLine(true,true))
					tmp_conn->m_Channel.Status=CHANNEL_STATUS_CONNECTED;
				else
				{
					KL_VERBOSE( "InstanceConnection Failed \n");
					delete tmp_conn;
					return -1;
				}
			}

		}
		io_pconnrule.b_conn = tmp_conn; 
		io_pconnrule.b_RuleInfo.BusNo = i_channelNo;	    
	}

	return 0;

}

int GDBusRuleFactory::AdaptComm() //除了指定chip外的所有模块适应
{
	return -1;

}


int GDBusRuleFactory::LoadBusRuleInfos(struct BusRuleInfos &ruleInfos)
{	
	//------------>请使用ADD_METER_RULE宏增加电表规约
	static RS485Rule RS485_Zong;
	static RS485Rule RS485_Fen;
	AddRule(RS485_Zong);
	AddRule(RS485_Fen);

	m_BusRuleItems[0].ID=DBCJ_1_BUSNO_485;
	m_BusRuleItems[1].ID = DBCJ_2_BUSNO_485;


	for(INT32U i=0;i<m_Count;i++)
	{
		m_BusRuleItems[i].Rule->InitRuleBaseInfo();
		m_BusRuleItems[i].ID=i+1;
	}
	sort(m_BusRuleItems,m_BusRuleItems+m_Count);
	return 0;
}

int GDBusRuleFactory::OnSysInit(Context& oContext)
{
	for(INT32U i=0;i<m_Count;i++)
	{
		m_BusRuleItems[i].Rule->GetBusRuleInfo(oContext.BusInfos.bruleInfos[i]);
		m_BusRuleItems[i].Rule->OnSysInit();
	}
	oContext.BusInfos.Count=m_Count;
	return 0;
}

void GDBusRuleFactory::AddRule(BusChannelsRule& rule)
{
	if (m_Count>=CONS_METER_RULE_MAX_COUNT)
	{
		KL_ERROR( "Cannot add meter rule because of too many rules\n");
		return;
	}

	m_BusRuleItems[m_Count++].Rule=&rule;
}

BusChannelsRule* GDBusRuleFactory::FindRule(INT8U RuleID)
{
	INT8U ruleID=RuleID;
	const BusChannelsRuleItem* iter=lower_bound(m_BusRuleItems,m_BusRuleItems+m_Count,ruleID);//二分法查找

	//！测试当前所支持的表规约ID
	if (iter!=m_BusRuleItems+m_Count && (*iter).ID==ruleID)
	{
		return (*iter).Rule;
	}
	return NULL; 
}




BusChannelsRule* GDBusRuleFactory::GetBusRule(const Meter& meter)
{
	if (meter.MeterNo != 0 && CONTEXT.Meters.ValidMBS.test(meter.MeterNo) == 0)
	{

		KL_ERROR( "当前表号%d非法\n",meter.MeterNo);
		return NULL;
	}

	//KL_VERBOSE( " 下行口通道占用情况：\n");
	//for (INT8U i=0; i<m_Count; i++)
	//{

	//	if ( m_BusRuleItems[i].Rule->b_conn!=NULL)
	//		KL_VERBOSE( "%d, %d\n", m_BusRuleItems[i].ID, m_BusRuleItems[i].Rule->b_conn->m_Channel.Setting.Com.PortNo);
	//}


	BusChannelsRule *busrule =  GetBusRule(meter.BusNo);

	if (busrule == NULL || busrule->b_conn == NULL)
		return NULL;

	//下行只有485与表直连  与电表直连时 需考虑实时改变电表配 
	if (meter.BusNo == DBCJ_1_BUSNO_485|| meter.BusNo==DBCJ_2_BUSNO_485)
	{  
		if (busrule->b_conn->m_Channel.Setting.Com.PortSet != meter.PortSet
			|| m_GatherMeterPortSet != meter.PortSet)
		{
			busrule->b_conn->m_Channel.Setting.Com.PortSet = m_GatherMeterPortSet;

			if (busrule->ChangeConnConfig(meter.PortSet) != 0)
			{
				KL_WARN( "改变电表通讯参数失败，使用RS485总线规约默认通道");
			}	   
			busrule->b_conn->m_Channel.Setting.Com.PortSet = meter.PortSet;
		}
		m_GatherMeterPortSet = busrule->b_conn->m_Channel.Setting.Com.PortSet;
	}

	return busrule;
}



BusChannelsRule*  GDBusRuleFactory::GetBusRule(const INT8U & busno)
{

	BusChannelsRule *busrule;
	busrule = Instance().FindRule(busno);
	static PortSet m_BeforePortSet;

	if (busrule == NULL)
	{
		KL_ERROR( "找不到总线规约\n");
		return NULL;
	}

	/*ASSERT(busrule->b_conn!=NULL);*/

#ifdef WIN32
	for ( INT8U i = 0; i<m_Count; i++)
	{
		if (m_BusRuleItems[i].Rule->b_conn != NULL)
		{
			m_BusRuleItems[i].Rule->b_conn->EnsureOffLine(true);	
			m_BusRuleItems[i].Rule->b_conn = NULL;
		}
	}

#endif

	if (busrule->b_conn == NULL)
	{
		if(InstanceConnection(*busrule,busno) != 0)
		{
			return NULL;
		}
	}

	//CGH 2010-05-13 陈桂华 增加 不调整可能导致错误
/*	if (busrule->b_conn->m_Channel.Setting.Com.PortSet != busrule->b_RuleInfo.DefPortSet)
	{
		if (busrule->ChangeConnConfig(busrule->b_RuleInfo.DefPortSet) != 0)
		{
			KL_WARN("改变电表通讯参数失败，使用RS485总线规约默认通道");   
		}
	}*/

	/* 如果上行规约改变采集通道通讯参数，将按新参数重新打开串口*/
	if (busrule->b_conn->m_Channel.Setting.Com.PortSet != m_BeforePortSet)
	{
		if (busrule->ChangeConnConfig(busrule->b_conn->m_Channel.Setting.Com.PortSet) != 0)
		{
			KL_ERROR("改变电表通讯参数失败");   
		}
		else
		{
			KL_ERROR("改变电表通讯参数成功");   
		}
	}
	m_BeforePortSet = busrule->b_conn->m_Channel.Setting.Com.PortSet;
	return busrule;
}





