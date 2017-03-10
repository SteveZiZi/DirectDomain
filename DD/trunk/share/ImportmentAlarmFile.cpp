#include "ImportmentAlarmFile.h"
#include "../base/KLDump.h"
/*================================================================================================
ImpAlarmRecordReset()函数说明：

一、函数功能：初始化报警记录文件
二、参数说明：
iLen：报警记录文件的最大记录数，不同的报警类型有不同的要求。
iItenLen：记录项的长度，不同的报警类型有不同的要求。
三、函数算法：
根据记录项的长度和记录文件的最大记录数，使用0xff填充文件、使用0x00填充文件头和每条记录的标志。
================================================================================================*/
int ImpAlarmRecordInfo::ImpAlarmRecordReset(FILE* fp,INT16U iLen,INT8U iItemLen)
{
	KL_WARN("Will reset ImpAlarm Data\n");
	ImpAlarmData[0]=0x00;
	fseek(fp,0,SEEK_SET);
	fwrite(ImpAlarmData,1,1,fp);
	fseek(fp,1,SEEK_SET);
	fwrite(ImpAlarmData,1,1,fp);
	unsigned char date_tmp[CONST_ImpAlarm_RECORD_ITEM_LENGTH * 10];
	memset(date_tmp, 0xff, CONST_ImpAlarm_RECORD_ITEM_LENGTH * 10);
	for (int i = 0; i < CONST_ImpAlarm_FILE_LENGTH/10; i++)
	{
		fseek(fp, 2 + CONST_ImpAlarm_RECORD_ITEM_LENGTH * 10 * i, SEEK_SET);
		fwrite(date_tmp, CONST_ImpAlarm_RECORD_ITEM_LENGTH * 10, 1, fp);
	}
	if (CONST_ImpAlarm_FILE_LENGTH % 10 != 0)		//如果不是10的整数倍，需要将余下的进行单独初始化，避免循环判断造成资源浪费
	{
		fseek(fp, 2 + CONST_ImpAlarm_RECORD_ITEM_LENGTH * 10 * (CONST_ImpAlarm_FILE_LENGTH/10), SEEK_SET);
		fwrite(date_tmp, CONST_ImpAlarm_RECORD_ITEM_LENGTH * (CONST_ImpAlarm_FILE_LENGTH % 10), 1, fp);
	}
	return 0;
}

int ImpAlarmFile::AddAlarmRecord(const INT8U *iRecordContent,INT8U iTrueLen,INT16U iImpAlarmNo,INT16U iMeterNo,INT8U iItemLen,INT16U FileLen,char *FileName)
{
	char bcdtime[7];
	Get_CurBCDTime7(bcdtime);
	return AddAlarmRecord(iRecordContent,iTrueLen,iImpAlarmNo,bcdtime,iMeterNo,iItemLen,FileLen,FileName);
}

/*=============================================================================================
AddImpAlarmRecord()说明：

一、函数功能：添加一条事件报警记录
二、参数说明：
1、const char *FileName：要写入的文件的文件名，默认
2、INT16U FileLen：要写入的文件最大记录数，默认
3、const coid *iRecord：要写入的内容所在的地址，传入
4、INT8U iItemLen：要写入的内容的度量单位，默认
5、INT16U iLen：要写入的内容的实际长度
三、函数算法：
1、判断文件是否为空，是则创建新文件。
2、取出文件头，判断是否最新记录位置是否已经到达文件末尾，如果到达文件末尾则执行第一种处理
算法，如果未到达文件末尾则执行第二种处理算法。
=============================================================================================*/
int ImpAlarmFile::AddAlarmRecord(const INT8U* iRecord,INT8U iTrueLen,INT16U iImpAlarmNo,const char *bcdtime,INT16U iMeterNo,INT8U iItemLen,INT16U FileLen,char *FileName)
{
	//if (iTrueLen<=0 || iItemLen<=0)
	//{
	//	return -1;
	//}
	FILE* fp;
	INT16U fileLen;	
	fp=FileBase::OpenFile(FileName,"rb+");
	if (fp == NULL)
	{
		ImpAlarmRecordInfo recordInfo;
		if (access(FileName, 0) == 0)
		{
			unlink(FileName);
		}
		FILE* fp1=FileBase::OpenFile(FileName,"wb+");        
		if (fp1 == NULL)
		{
			KL_WARN("创建文件%s错误!\n",FileName);
			return -1;
		}			
		recordInfo.Init(fp1,CONST_ImpAlarm_FILE_LENGTH,CONST_ImpAlarm_RECORD_ITEM_LENGTH);
		FileBase::Close(fp1);
		fp = FileBase::OpenFile(FileName,"rb+");
		if (fp == NULL)
		{
			return -1;
		}
	}
	AutoFileFP autofp(fp,true,true,true);
	fseek(fp,0,SEEK_SET);
	INT8U file_len[2];
	fread(file_len,2,1,fp);
	if (file_len[0]==0xff && file_len[1]==0xff)
	{
		memset(file_len,0x00,2);
	}
	INT16U fileLen0 = file_len[0];
	INT16U fileLen1 = file_len[1];

	INT8U write_index[2];
	fseek(fp,2,SEEK_SET);
	fread(write_index,2,1,fp);
	INT16U writenIndex=write_index[0]*256+write_index[1];//写记录索引

	writenIndex++; 
	if (writenIndex == CONST_ImpAlarm_FILE_LENGTH)
	{
		writenIndex = 0;
	}
	fileLen = fileLen0*0x0100 + fileLen1;
	if(fileLen >= FileLen)
	{
		//fileLen = (fileLen+1) % FileLen;
		fileLen = FileLen;
	}
	else
	{
		fileLen += 1;
	}

	file_len[1]=fileLen & 0xff;
	file_len[0]=fileLen>>8 & 0xff;
	fseek(fp,0,SEEK_SET);//写文件头
	fwrite(file_len,2,1,fp);

	write_index[0] = writenIndex>>8 & 0xff;
	write_index[1] = writenIndex & 0xff;
	fseek(fp,2,SEEK_SET);//写文件头
	fwrite(write_index,2,1,fp);
	
	INT8U *record = new INT8U[iItemLen];
	memset(record,0xff,iItemLen);
	record[0] = 0x01;
	DECLARE_CONTEXT(context);
	if (iMeterNo == CONS_UNVALID_METER_NUMBER)
	{
		record[1] = 0x99;
		record[2] = 0x99;
		record[3] = 0x99;
		record[4] = 0x99;
		record[5] = 0x99;
		record[6] = 0x99;
	}
	else
	{
		INT8U MeterAddr[6];
		Meter &meter = context.Meters.Item[iMeterNo];
		GetAddr_6_Fix0(meter.Addr,MeterAddr);
		record[1] = MeterAddr[0];
		record[2] = MeterAddr[1];
		record[3] = MeterAddr[2];
		record[4] = MeterAddr[3];
		record[5] = MeterAddr[4];
		record[6] = MeterAddr[5];
	}
	record[7] = bcdtime[1];
	record[8] = bcdtime[2];
	record[9] = bcdtime[3];
	record[10] = bcdtime[4];
	record[11] = bcdtime[5];
	record[12] = (INT8U)iImpAlarmNo;
	record[13] = (INT8U)(iImpAlarmNo>>8);
	memcpy(record+14,iRecord,iTrueLen);
	//fseek(fp,2+(fileLen-1)*iItemLen,SEEK_SET);
	fseek(fp,4+writenIndex*iItemLen,SEEK_SET);

	bstring ImpAlarmdata;
	ImpAlarmdata.append(record,iItemLen);
	KL_INFO_DUMP(dump);
	dump<<"ImpAlarm Log Data:"<<ImpAlarmdata<<"\n";
	
	fwrite(record,iItemLen,1,fp);	
	delete[] record;
	return 0;
}

//返回-2表示该条记录不存在，返回-1表示文件不存在，返回0表示成功
int ImpAlarmFile::SetAlarmRecordReaded(INT16U io_Offset,INT8U iItemLen,INT16U FileLen,char *FileName)
{
	if (io_Offset == CONST_ImpAlarm_RECORD_OFFSET || io_Offset >= FileLen)
	{
		return -2; 
	}
	FILE *fp = FileBase::OpenFile(FileName,"rb+");
	if (fp == NULL)
	{
		return -1;
	}
	AutoFileFP autofp(fp,true,true,true);
	INT8U flag = 0;
	FileBase::WriteBinFile(fp,&flag,2 + io_Offset*iItemLen,SEEK_SET,1);
	return 0;
}

int ImpAlarmFile::SetAlarmRecordReaded(FILE* fp,INT16U io_Offset,INT8U iItemLen,INT16U FileLen)
{
	if (io_Offset == CONST_ImpAlarm_RECORD_OFFSET || io_Offset >= FileLen)
	{
		return -2; 
	}
	if (fp == NULL)
	{
		return -1;
	}
	INT8U flag = 0;
	FileBase::WriteBinFile(fp,&flag,2 + io_Offset*iItemLen,SEEK_SET,1);
	return 0;
}

//返回0表示读取成功，返回-1表示文件不存在，返回1表示没有记录可读，返回2表示不识别这种报警类型
//i_TrueLen由调用的程序传入iRecord数组的长度，该函数将使用该形参来返回该条记录的实际长度，应用程序应该根据此长度来打包上送帧
//io_Offset由调用的程序传入，并返回给调用的程序，以供SetImpAlarmRecordReaded用于定位读取的记录的目标地址
int ImpAlarmFile::GetAlarmRecord(bstring &iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen,INT16U iFileLen,char *FileName)
{
	INT8U record[CONST_ImpAlarm_RECORD_ITEM_LENGTH];
	memset(record,0,sizeof(record));
	iRecord.clear();
	int iResult = GetAlarmRecord(record,i_TrueLen,io_Offset,iItemLen,iFileLen,FileName);
	if (iResult == 0)
	{
		iRecord.append(record,i_TrueLen);
	}
	return iResult;
}

//返回0表示读取成功，返回-1表示文件不存在，返回1表示没有记录可读，返回2表示不识别这种报警类型
//i_TrueLen由调用的程序传入iRecord数组的长度，该函数将使用该形参来返回该条记录的实际长度，应用程序应该根据此长度来打包上送帧
//io_Offset由调用的程序传入，并返回给调用的程序，以供SetImpAlarmRecordReaded用于定位读取的记录的目标地址
int ImpAlarmFile::GetAlarmRecord(INT8U *iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen,INT16U iFileLen,char *FileName)
{
	int iResult = -1;
	FILE *fp = FileBase::OpenFile(FileName,"rb");
	if (fp == NULL)
	{
		return -1;//文件不存在，退出
	}
	AutoFileFP autofp(fp,true,false,true);
	INT8U fileLen[2] = {0,0};
	FileBase::ReadBinFile(fp,fileLen,0,SEEK_SET,2);
	INT16U FileLen = fileLen[0]*0x100 + fileLen[1];
	if (FileLen > iFileLen)
	{
		return -1;
	}
	INT8U flag = 0x00;
	INT8U ImpAlarmNo[4] = {0,0,0,0};
	for (INT16U i = 0;i != iFileLen;i++)//读取逐条记录的标志位，假如该标志位为1则立刻停止搜索
	{
		FileBase::ReadBinFile(fp,&flag,4 + i*iItemLen,SEEK_SET,1);
		if (flag == 1)
		{
			FileBase::ReadBinFile(fp,iRecord,4 + i*iItemLen,SEEK_SET,CONST_ImpAlarm_RECORD_ITEM_LENGTH);
			ImpAlarmNo[0] = iRecord[12];
			ImpAlarmNo[1] = iRecord[13];
			ImpAlarmNo[2] = iRecord[14];
			iResult = GetTrueLen(ImpAlarmNo,i_TrueLen);
			if (iResult == 0)//假如iResult != 0则说明该条记录不正确或不存在或者是本机器无法识别的记录
			{
				io_Offset = i;//获取该值，用于后面更新记录状态
				break;
			}
		}
	}
	//SetImpAlarmRecordReaded(io_Offset);//只要读取了，就认为该条记录已经上报过了
	return iResult;//返回0表示读取成功
}

int ImpAlarmFile::GetTrueLen(INT8U *iImpAlarmNo,INT8U &i_TrueLen)
{
	int iResult = -1;
	if (iImpAlarmNo[1] == 0x01)
	{
		switch (iImpAlarmNo[0])
		{
		case 0x01:
			i_TrueLen = 3 + 14;
			iResult = 0;
			break;

		default:
			iResult = 2;
			i_TrueLen = 0;
			break;
		}
	}
	else
	{
		iResult = 2;
		i_TrueLen = 0;
	}
	return iResult;
}

int ImpAlarmFile::GetAlarmRecords(bstrings &iRecords,INT8U irCount,time_t tBeginTime,time_t tEndTime,INT8U iItemLen,INT16U iFileLen,char *FileName)
{
	int iResult = 0;
	//KL_INFO("打开告警数据文件：%s\n",FileName);
	INT8U record[CONST_ImpAlarm_RECORD_ITEM_LENGTH];
	//memset(record,0xff,sizeof(record));
	INT8U ImpAlarmNo[4] = {0,0,0,0};
	INT8U i_TrueLen = 0;
	char bcdtime[7];
	Get_CurBCDTime7(bcdtime);
	time_t tTime = time(NULL);
	bstring bstrRecord;
	if (iFileLen > CONST_ImpAlarm_FILE_LENGTH)
	{
		iFileLen = CONST_ImpAlarm_FILE_LENGTH;
		KL_INFO("iFileLen = %d\n",iFileLen);
	}
	KL_INFO("iFileLen = %d\n",iFileLen);

	FILE *fp = FileBase::OpenFile(FileName,"rb");
	if (fp == NULL)
	{
		return -1;
	}
	AutoFileFP autofp(fp,true,false,true);

	if (irCount == 0XFF)
	{
		tBeginTime = 0;
		tEndTime = 0;
	}

	for (INT16U i = 0;i != iFileLen; i++)
	{
		bstrRecord.clear();
		memset(record,0xff,sizeof(record));
	
		FileBase::ReadBinFile(fp,record,4 + i*iItemLen,SEEK_SET,sizeof(record));

		if (record[0] != 0x00 && record[0] != 0x01)
		{
			continue;//该条记录不存在
		}

		if (tBeginTime == 0 && tEndTime == 0)//假如传入的时间都为0，则说明需要抄收所有的记录，此时无需判断时间，直接将所有记录压入堆栈即可
		{
			ImpAlarmNo[0] = record[12];//反序保存
			ImpAlarmNo[1] = record[13];
			ImpAlarmNo[2] = record[14];
			GetTrueLen(ImpAlarmNo,i_TrueLen);
			if (i_TrueLen <= 0)
			{
				continue;
			}
			bstrRecord.append(record+1,i_TrueLen - 1);

	//		KL_INFO_DUMP(dump);
		//	dump<<"bstrRecord:"<<bstrRecord<<"\n";
	
			iRecords.push_back(bstrRecord);
			iResult += 1;
			//KL_VERBOSE("取出第%d条数据，告警时间：%02X%02X-%02X-%02X %02X:%02X:%02X，告警编码：%02X%02X\n",iResult,bcdtime[0],bcdtime[1],bcdtime[2],bcdtime[3],bcdtime[4],bcdtime[5],bcdtime[6],ImpAlarmNo[1],ImpAlarmNo[0]);
			if (irCount != 0 && iResult == irCount)
			{
				break;
			}
			continue;
		}

		bcdtime[1] = record[7];//YY
		bcdtime[2] = record[8];//MM
		bcdtime[3] = record[9];//DD
		bcdtime[4] = record[10];//HH
		bcdtime[5] = record[11];//MM


		Time_BCDTime7TotTime(bcdtime,&tTime);
		if (tTime >= tBeginTime && tTime <= tEndTime)//该条记录在请求的时间之内
		{
			ImpAlarmNo[0] = record[12];//反序保存
			ImpAlarmNo[1] = record[13];
			ImpAlarmNo[2] = record[14];
			GetTrueLen(ImpAlarmNo,i_TrueLen);
			if (i_TrueLen <= 0)
			{
				continue;
			}
			if (record[0] == 0x01)
			{
				SetAlarmRecordReaded(fp,i);//假如取出来的数据还没有上报过，那么，将该条数据的标志位置为已经上报过
			}
			bstrRecord.append(record+1,i_TrueLen - 1);
			iRecords.push_back(bstrRecord);
			iResult += 1;
			KL_VERBOSE("取出第%d条数据，告警时间：%02X%02X-%02X-%02X %02X:%02X:%02X，告警编码：%02X%02X\n",iResult,bcdtime[0],bcdtime[1],bcdtime[2],bcdtime[3],bcdtime[4],bcdtime[5],bcdtime[6],ImpAlarmNo[1],ImpAlarmNo[0]);
			
		}

		if (irCount != 0 && iResult == irCount)//已经取够了，那么退出
		{
			break;
		}

		//KL_VERBOSE("取出一条数据\n");

	}
	//KL_VERBOSE("关闭文件\n");
	sort(iRecords.begin(),iRecords.end());
	return iResult;
}

int ImpAlarmFile::GetAlarmRecords(bstrings &iRecords,INT8U irCount,char *tBeginTimeBCD7,char *tEndTimeBCD7,INT8U iItemLen,INT16U iFileLen,char *FileName)
{
	time_t tBeginTime = 0,tEndTime = 0;
	if (tBeginTimeBCD7 != NULL && tEndTimeBCD7 != NULL)
	{
		Time_BCDTime7TotTime(tBeginTimeBCD7,&tBeginTime);
		Time_BCDTime7TotTime(tEndTimeBCD7,&tEndTime);
	}
	return GetAlarmRecords(iRecords,irCount,tBeginTime,tEndTime,iItemLen,iFileLen,FileName);
}

//int ImpAlarmFile::ConvertMeterNoToBCD(INT16U iMeterNo,INT8U *iBcdMeterNo)
//{
//	iMeterNo = ((iMeterNo%10000)/1000)*0x1000 + ((iMeterNo%1000)/100)*0x100 + ((iMeterNo%100)/10)*0x10 + (iMeterNo%10);
//	iBcdMeterNo[0] = (INT8U)iMeterNo >> 8;
//	iBcdMeterNo[1] = (INT8U)iMeterNo;
//	return 0;
//}

//int ImpAlarmFile::AddImpAlarmRecord(bstring &iRecord,INT8U iTrueLen,INT8U iItemLen,INT16U FileLen,char *FileName)
//{
//	return AddImpAlarmRecord((INT8U *)iRecord.c_str(),iTrueLen,iItemLen,FileLen,FileName);
//}
//
