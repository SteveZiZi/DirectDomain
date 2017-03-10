#include "ImportmentAlarmFile.h"
#include "../base/KLDump.h"
/*================================================================================================
ImpAlarmRecordReset()����˵����

һ���������ܣ���ʼ��������¼�ļ�
��������˵����
iLen��������¼�ļ�������¼������ͬ�ı��������в�ͬ��Ҫ��
iItenLen����¼��ĳ��ȣ���ͬ�ı��������в�ͬ��Ҫ��
���������㷨��
���ݼ�¼��ĳ��Ⱥͼ�¼�ļ�������¼����ʹ��0xff����ļ���ʹ��0x00����ļ�ͷ��ÿ����¼�ı�־��
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
	if (CONST_ImpAlarm_FILE_LENGTH % 10 != 0)		//�������10������������Ҫ�����µĽ��е�����ʼ��������ѭ���ж������Դ�˷�
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
AddImpAlarmRecord()˵����

һ���������ܣ����һ���¼�������¼
��������˵����
1��const char *FileName��Ҫд����ļ����ļ�����Ĭ��
2��INT16U FileLen��Ҫд����ļ�����¼����Ĭ��
3��const coid *iRecord��Ҫд����������ڵĵ�ַ������
4��INT8U iItemLen��Ҫд������ݵĶ�����λ��Ĭ��
5��INT16U iLen��Ҫд������ݵ�ʵ�ʳ���
���������㷨��
1���ж��ļ��Ƿ�Ϊ�գ����򴴽����ļ���
2��ȡ���ļ�ͷ���ж��Ƿ����¼�¼λ���Ƿ��Ѿ������ļ�ĩβ����������ļ�ĩβ��ִ�е�һ�ִ���
�㷨�����δ�����ļ�ĩβ��ִ�еڶ��ִ����㷨��
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
			KL_WARN("�����ļ�%s����!\n",FileName);
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
	INT16U writenIndex=write_index[0]*256+write_index[1];//д��¼����

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
	fseek(fp,0,SEEK_SET);//д�ļ�ͷ
	fwrite(file_len,2,1,fp);

	write_index[0] = writenIndex>>8 & 0xff;
	write_index[1] = writenIndex & 0xff;
	fseek(fp,2,SEEK_SET);//д�ļ�ͷ
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

//����-2��ʾ������¼�����ڣ�����-1��ʾ�ļ������ڣ�����0��ʾ�ɹ�
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

//����0��ʾ��ȡ�ɹ�������-1��ʾ�ļ������ڣ�����1��ʾû�м�¼�ɶ�������2��ʾ��ʶ�����ֱ�������
//i_TrueLen�ɵ��õĳ�����iRecord����ĳ��ȣ��ú�����ʹ�ø��β������ظ�����¼��ʵ�ʳ��ȣ�Ӧ�ó���Ӧ�ø��ݴ˳������������֡
//io_Offset�ɵ��õĳ����룬�����ظ����õĳ����Թ�SetImpAlarmRecordReaded���ڶ�λ��ȡ�ļ�¼��Ŀ���ַ
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

//����0��ʾ��ȡ�ɹ�������-1��ʾ�ļ������ڣ�����1��ʾû�м�¼�ɶ�������2��ʾ��ʶ�����ֱ�������
//i_TrueLen�ɵ��õĳ�����iRecord����ĳ��ȣ��ú�����ʹ�ø��β������ظ�����¼��ʵ�ʳ��ȣ�Ӧ�ó���Ӧ�ø��ݴ˳������������֡
//io_Offset�ɵ��õĳ����룬�����ظ����õĳ����Թ�SetImpAlarmRecordReaded���ڶ�λ��ȡ�ļ�¼��Ŀ���ַ
int ImpAlarmFile::GetAlarmRecord(INT8U *iRecord,INT8U &i_TrueLen,INT16U &io_Offset,INT8U iItemLen,INT16U iFileLen,char *FileName)
{
	int iResult = -1;
	FILE *fp = FileBase::OpenFile(FileName,"rb");
	if (fp == NULL)
	{
		return -1;//�ļ������ڣ��˳�
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
	for (INT16U i = 0;i != iFileLen;i++)//��ȡ������¼�ı�־λ������ñ�־λΪ1������ֹͣ����
	{
		FileBase::ReadBinFile(fp,&flag,4 + i*iItemLen,SEEK_SET,1);
		if (flag == 1)
		{
			FileBase::ReadBinFile(fp,iRecord,4 + i*iItemLen,SEEK_SET,CONST_ImpAlarm_RECORD_ITEM_LENGTH);
			ImpAlarmNo[0] = iRecord[12];
			ImpAlarmNo[1] = iRecord[13];
			ImpAlarmNo[2] = iRecord[14];
			iResult = GetTrueLen(ImpAlarmNo,i_TrueLen);
			if (iResult == 0)//����iResult != 0��˵��������¼����ȷ�򲻴��ڻ����Ǳ������޷�ʶ��ļ�¼
			{
				io_Offset = i;//��ȡ��ֵ�����ں�����¼�¼״̬
				break;
			}
		}
	}
	//SetImpAlarmRecordReaded(io_Offset);//ֻҪ��ȡ�ˣ�����Ϊ������¼�Ѿ��ϱ�����
	return iResult;//����0��ʾ��ȡ�ɹ�
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
	//KL_INFO("�򿪸澯�����ļ���%s\n",FileName);
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
			continue;//������¼������
		}

		if (tBeginTime == 0 && tEndTime == 0)//���紫���ʱ�䶼Ϊ0����˵����Ҫ�������еļ�¼����ʱ�����ж�ʱ�䣬ֱ�ӽ����м�¼ѹ���ջ����
		{
			ImpAlarmNo[0] = record[12];//���򱣴�
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
			//KL_VERBOSE("ȡ����%d�����ݣ��澯ʱ�䣺%02X%02X-%02X-%02X %02X:%02X:%02X���澯���룺%02X%02X\n",iResult,bcdtime[0],bcdtime[1],bcdtime[2],bcdtime[3],bcdtime[4],bcdtime[5],bcdtime[6],ImpAlarmNo[1],ImpAlarmNo[0]);
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
		if (tTime >= tBeginTime && tTime <= tEndTime)//������¼�������ʱ��֮��
		{
			ImpAlarmNo[0] = record[12];//���򱣴�
			ImpAlarmNo[1] = record[13];
			ImpAlarmNo[2] = record[14];
			GetTrueLen(ImpAlarmNo,i_TrueLen);
			if (i_TrueLen <= 0)
			{
				continue;
			}
			if (record[0] == 0x01)
			{
				SetAlarmRecordReaded(fp,i);//����ȡ���������ݻ�û���ϱ�������ô�����������ݵı�־λ��Ϊ�Ѿ��ϱ���
			}
			bstrRecord.append(record+1,i_TrueLen - 1);
			iRecords.push_back(bstrRecord);
			iResult += 1;
			KL_VERBOSE("ȡ����%d�����ݣ��澯ʱ�䣺%02X%02X-%02X-%02X %02X:%02X:%02X���澯���룺%02X%02X\n",iResult,bcdtime[0],bcdtime[1],bcdtime[2],bcdtime[3],bcdtime[4],bcdtime[5],bcdtime[6],ImpAlarmNo[1],ImpAlarmNo[0]);
			
		}

		if (irCount != 0 && iResult == irCount)//�Ѿ�ȡ���ˣ���ô�˳�
		{
			break;
		}

		//KL_VERBOSE("ȡ��һ������\n");

	}
	//KL_VERBOSE("�ر��ļ�\n");
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
