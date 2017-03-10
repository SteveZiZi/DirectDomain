#include "RS485Rule.h"

#include <algorithm>
#include <numeric>

#include <stdio.h>

#ifndef WIN32
#include <sys/file.h>
#endif



inline RS485Rule:: RS485Rule(Connection *conn):BusChannelsRule(conn)
{
	InitRuleBaseInfo();
	b_conn->Config(&(b_RuleInfo.DefPortSet),sizeof(b_RuleInfo.DefPortSet));
}

void RS485Rule::InitRuleBaseInfo()
{
	
	b_RuleInfo.BusNo=DBCJ_1_BUSNO_485;
	b_RuleInfo.CommID = BUS_485;
	memset(b_RuleInfo.Name,0,sizeof(b_RuleInfo.Name));
	strncpy(b_RuleInfo.Name,RS485_RULE_NAME,sizeof(b_RuleInfo.Name)-1);
	b_RuleInfo.DefPortSet.Baud=1200;
	b_RuleInfo.DefPortSet.DataBit=8;
	b_RuleInfo.DefPortSet.StopBit=1;
	b_RuleInfo.DefPortSet.Parity=2;//偶校验
	b_RuleInfo.DefMinCommInterS=0;
	b_RuleInfo.DefCommRetryTimes=3;
	b_RuleInfo.CharReadTimeoutMs=RS485_CHAR_READ_TIMEOUT_MS;
	b_RuleInfo.SubReadTimeoutMS=RS485_SUB_READ_TIMEOUT_MS;
	b_RuleInfo.ReadTimeoutMS=RS485_READ_TIMEOUT_MS;
	b_RuleInfo.WriteTimeoutMS=RS485_WRITE_TIMEOUT_MS;
}

int RS485Rule:: MakePackage(Meter& imeter,const unsigned char* idata,int idataLen,INT8U iCtrlFlag,bstring& oSendPackage,bstring* iaddr)
{
	return 0;
}

PARSE_RESULT RS485Rule::ParsePackage(Meter& meter,bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& irSendData,bstring& opReturnData)
{

	bstring packagetmp;
	packagetmp.append(iterBegin,iterEnd);	
	bstring::iterator cStartFlagIter=find(iterBegin,iterEnd,0x68);
	bstring::difference_type iNum=distance(cStartFlagIter,iterEnd);
	iterNextBegin=cStartFlagIter;
	if (iNum==0)//no start flag 0x68
	{
		/*KL_VERBOSE( "GB645Rule::ParsePackage [1] 无开始标志\n");*/
		return PARSE_RESULT_INVALID_NO_HEADER_PACKAGE;
	}
	if (iNum<10)
	{
		//KL_VERBOSE( "GB645Rule::ParsePackage [2] 部分包\n");
		return PARSE_RESULT_PARTICAL_PACKAGE;//partical frame
	}

	unsigned short cLen=*(cStartFlagIter+9);

	if (iNum<cLen+12)
	{
		//KL_VERBOSE( "GB645Rule::ParsePackage [3] 部分包\n");
		return PARSE_RESULT_PARTICAL_PACKAGE;
	}

	iterNextBegin=cStartFlagIter+1;
	unsigned char checkSum=accumulate(cStartFlagIter,cStartFlagIter+cLen+10,0);
	if (checkSum!=(*(cStartFlagIter+cLen+10)))
	{
		KL_WARN( "CheckSum error in gather meter %d.(CalcSum=%X and RecvSum=%X)\n",meter.MeterNo,checkSum,*(cStartFlagIter+cLen+10));
		return PARSE_RESULT_INVALID_WITH_HEADER_PACKAGE;
	}
	if (*(cStartFlagIter+11+cLen)!=0x16)
	{
		KL_WARN( "Stop Flag error in gather meter %d.(RecvStopFlag=%X)\n",meter.MeterNo,*(cStartFlagIter+11+cLen));
		return PARSE_RESULT_INVALID_WITH_HEADER_PACKAGE;
	}

	iterNextBegin = cStartFlagIter+12+cLen;
	opReturnData.clear();
	opReturnData.append(cStartFlagIter+10,cStartFlagIter+10+cLen);
	return PARSE_RESULT_OK_PACKAGE;
}



PARSE_RESULT RS485Rule::ParsePackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& opReturnData)
{

	bstring packagetmp;
	packagetmp.append(iterBegin,iterEnd);	
	bstring::iterator cStartFlagIter=find(iterBegin,iterEnd,0x68);
	bstring::difference_type iNum=distance(cStartFlagIter,iterEnd);
	iterNextBegin=cStartFlagIter;

	if (iNum==0)//no start flag 0x68
	{
			return PARSE_RESULT_INVALID_NO_HEADER_PACKAGE;
	}
	if (iNum<10)
	{
		return PARSE_RESULT_PARTICAL_PACKAGE;//partical frame
	}

	unsigned short cLen=*(cStartFlagIter+9);

	if (iNum<cLen+12)
	{
		return PARSE_RESULT_PARTICAL_PACKAGE;
	}

	iterNextBegin=cStartFlagIter+1;
	unsigned char checkSum=accumulate(cStartFlagIter,cStartFlagIter+cLen+10,0);
	if (checkSum!=(*(cStartFlagIter+cLen+10)))
	{
		return PARSE_RESULT_INVALID_WITH_HEADER_PACKAGE;
	}
	if (*(cStartFlagIter+11+cLen)!=0x16)
	{
		return PARSE_RESULT_INVALID_WITH_HEADER_PACKAGE;
	}

	iterNextBegin = cStartFlagIter+12+cLen;
	opReturnData.clear();
	opReturnData.append(cStartFlagIter+10,cStartFlagIter+10+cLen);
	return PARSE_RESULT_OK_PACKAGE;
}

PARSE_RESULT RS485Rule::ParseModbusPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,INT8U key,bstring& opReturnData)
{

	bstring packagetmp;
	unsigned short crc_code = 0;
	unsigned short value = 0;
	packagetmp.append(iterBegin,iterEnd);	
	bstring::iterator cStartFlagIter;
	if (STATUS.deviceStatus.m_AirConditionType == 2)
	{
		if (*iterBegin == 0x40 && *(iterBegin+1) == 0x20)//失真报文，特殊处理
		{
			opReturnData.clear();
			opReturnData.push_back(0x03);
			opReturnData.push_back(0x00);
			opReturnData.append(iterBegin+2,iterEnd);	
			return PARSE_RESULT_OK_PACKAGE;
		}
		cStartFlagIter=find(iterBegin,iterEnd,MODBUS_B_COMMNO);
	}
	else if (STATUS.deviceStatus.m_AirConditionType == 1)
	{
		cStartFlagIter=find(iterBegin,iterEnd,MODBUS_A_COMMNO);
	}
	else
	{
		cStartFlagIter=find(iterBegin,iterEnd,key);
	}
	bstring::difference_type iNum=distance(cStartFlagIter,iterEnd);
	iterNextBegin=cStartFlagIter;

	if (iNum==0)//no start flag 0x68
	{
			return PARSE_RESULT_INVALID_NO_HEADER_PACKAGE;
	}
	if (iNum<6)
	{
		return PARSE_RESULT_PARTICAL_PACKAGE;//partical frame
	}

	unsigned char ctl = *(cStartFlagIter+1);

	if (ctl != 0x03 && ctl != 0x06)
	{
		return PARSE_RESULT_ERROR_PACKAGE;
	}

	if (ctl == 0x06)
	{
		return PARSE_RESULT_OK_PACKAGE;
	}
	
	if (STATUS.deviceStatus.m_AirConditionType == 2)
	{
		crc_code = CRC16RTU( &(*cStartFlagIter), 6);
		value = *(iterEnd-2) | *(iterEnd-1)<<8;
		if (crc_code != value)
		{
			KL_ERROR("MODBUS数据包校验码错误\n");
			return PARSE_RESULT_ERROR_PACKAGE;
		}
		opReturnData.clear();
		opReturnData.append(cStartFlagIter+1,iterEnd);
		return PARSE_RESULT_OK_PACKAGE;
	}
	else if (STATUS.deviceStatus.m_AirConditionType == 1)
	{
		unsigned short cLen=*(cStartFlagIter+2)*256 + *(cStartFlagIter+3);
		crc_code = CRC16RTU( &(*cStartFlagIter), cLen+4);

		value = *(cStartFlagIter+cLen+4) | *(cStartFlagIter+cLen+5)<<8;

		if (crc_code != value)
		{
			KL_ERROR("MODBUS数据包校验码错误\n");
			return PARSE_RESULT_ERROR_PACKAGE;
		}
		iterNextBegin = cStartFlagIter+6+cLen;
		opReturnData.clear();
		opReturnData.append(cStartFlagIter+1,cStartFlagIter+4+cLen);
		return PARSE_RESULT_OK_PACKAGE;
	}
	else
	{
		unsigned short cLen= *(cStartFlagIter+2);
		
		if (iNum < cLen+5)
			return PARSE_RESULT_PARTICAL_PACKAGE;//partical frame

		crc_code = CRC16RTU( &(*cStartFlagIter), cLen+3);
		value = *(cStartFlagIter+cLen+3) | *(cStartFlagIter+cLen+4)<<8;

		if (crc_code != value)
		{
			KL_ERROR("MODBUS数据包校验码错误\n");
			return PARSE_RESULT_ERROR_PACKAGE;
		}
		iterNextBegin = cStartFlagIter+5+cLen;
		opReturnData.clear();
		opReturnData.append(cStartFlagIter+1,cStartFlagIter+3+cLen);
		return PARSE_RESULT_OK_PACKAGE;
	}
}




bool RS485Rule::SendPackage(Meter& imeter,const unsigned char* idata,int idataLen,INT8U iCtrlFlag,bstring& oSendPackage,bstring* iaddr)

{
	//MakePackage(Meter& meter,INT8U control, const unsigned char* data,int dataLen,bstring& oPackage,bool broadcast)
	//485传输与表规约一致 无需封装与解析
	oSendPackage.clear();
	oSendPackage.append(idata, idataLen);
	return b_conn->Send(idata,idataLen, b_conn->m_Channel.WriteTimeoutMS);

}

bool RS485Rule::SendData(Meter& meter,const unsigned char* data,int dataLen,bstring &retPackage,bstring* iaddr)
{
     int i, control= 0x01;

	 if (data != NULL && dataLen!=0)
	 {
		 for (i = 0; i<3; i++)
		 {
			 if (!SendPackage(meter,data+1,dataLen-1,control,retPackage))
				 continue;
			 else
				 break;
		 }

		 if (i == 3)
			 return false;
	 }

	return true;

}
bool RS485Rule::SendPackage(const unsigned char* idata,int idataLen)
{
	return b_conn->Send(idata,idataLen, b_conn->m_Channel.WriteTimeoutMS);
}


int  RS485Rule::GetMeterData(Meter& meter,const unsigned char* data,int dataLen,bstring& opReturnData,bstring* iaddr,const int &iiTimesOuts) 
{

	//* str已经表规约层解析，接下来需要被总线通讯层解析
	int ret;
	bstring cRecvData;
	bstring sndPackage;
	sndPackage.clear();
	cRecvData.clear();
   
    if (!SendData(meter,data,dataLen,sndPackage))
		return -1;

	
	ret =OnReceiveData(meter,sndPackage,cRecvData);


	opReturnData.assign(cRecvData);
	return ret;
}

int  RS485Rule::GetDeviceData(Device& device,const unsigned char* data,int dataLen,bstring& opReturnData,bstring* iaddr,const int &iiTimesOuts) 
{

	//* str已经表规约层解析，接下来需要被总线通讯层解析
	int ret;
	bstring cRecvData;
	cRecvData.clear();
   
    	SendPackage(data,dataLen);
#ifdef WIN32
		msleep(200);
#endif
	msleep(40);
	ret =OnReceiveDeviceData(device,cRecvData);


	opReturnData.assign(cRecvData);
	return ret;
}

int  RS485Rule::GetRtfData(Device& device,const unsigned char* data,int dataLen,bstring& opReturnData,bstring* iaddr,const int &iiTimesOuts) 
{

	//* str已经表规约层解析，接下来需要被总线通讯层解析
	int ret;
	bstring cRecvData;
	cRecvData.clear();
   
    	SendPackage(data,dataLen);
#ifdef WIN32
		msleep(500);
#endif
	msleep(40);
	ret =OnReceiveRtfData(device,cRecvData);


	opReturnData.assign(cRecvData);
	return ret;
}
PARSE_RESULT RS485Rule::ParseRtfDataPackage(bstring::iterator iterBegin,bstring::iterator iterEnd,bstring::iterator& iterNextBegin,bstring& opReturnData)
{

	bstring packagetmp;
	packagetmp.append(iterBegin,iterEnd);	
	bstring::iterator cStartFlagIter=find(iterBegin,iterEnd,0x7E);
	bstring::difference_type iNum=distance(cStartFlagIter,iterEnd);
	iterNextBegin=cStartFlagIter;

	if (iNum==0)//no start flag 0x7E
	{
			return PARSE_RESULT_INVALID_NO_HEADER_PACKAGE;
	}
	if (iNum<6)
	{
		return PARSE_RESULT_PARTICAL_PACKAGE;//partical frame
	}

	unsigned short cLen=bcdc(*(cStartFlagIter+2));

	if (iNum<cLen+5)
	{
		return PARSE_RESULT_PARTICAL_PACKAGE;
	}

	iterNextBegin=cStartFlagIter+1;
	unsigned char checkSum=accumulate(cStartFlagIter+1,cStartFlagIter+cLen+1,0);
	checkSum = cbcd(checkSum);
/*	if (checkSum!=(*(cStartFlagIter+cLen+3)))
	{
		return PARSE_RESULT_INVALID_WITH_HEADER_PACKAGE;
	}*/
	if (*(cStartFlagIter+4+cLen)!=0x0d)
	{
		return PARSE_RESULT_INVALID_WITH_HEADER_PACKAGE;
	}

	iterNextBegin = cStartFlagIter+5+cLen;
	opReturnData.clear();
	opReturnData.append(cStartFlagIter+1,cStartFlagIter+2+cLen);
	return PARSE_RESULT_OK_PACKAGE;
}

int RS485Rule::BroadCastData(Meter &meter, INT8U *data, const INT8U &dataLen)
{
    bstring oretData;
    int i;
	
	for(i = 0; i < 3; i++)
	   if (SendPackage(meter,data+1,dataLen-1,0x01,oretData))
			break;

	return 0;

}

int RS485Rule::GetTimeOutMS(Meter& meter)
{
	return b_RuleInfo.ReadTimeoutMS;
}
