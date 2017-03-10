/** @file
 *  @brief QT�������̨��ؽ��̹���״̬����ͨѶ�ӿ���
 *  @author Steven Wan
 *  @date 2015-09/26
 *  @version 0.1
*/
 
#include "ipcStatus.h"

ShareMem SIpcStatus::m_IpcStatus;

SIpcStatus::SIpcStatus()
{
	memset(this,0,sizeof(*this));
	Checker.Init();
}
/*
	brief: �����ļ���ʼ��
	param [in] FILE* fp�������ļ�ָ��
	returnt true����ʽ��ȷ��false����ʽ����
	author:Steven Wan
	date: 2015-09-26
*/
bool SIpcStatus::InitFile(FILE* fp, void* ipOther)
{
	ASSERT(fp);
	if (fseek(fp,0L ,SEEK_SET)!=0)
	{
		printf("fseek(fp,0L ,SEEK_SET) error in SIpcStatus::InitFile\n");
		return false;
	}
	SIpcStatus* pIpcShareMem=new SIpcStatus;
	bool bResult=(fwrite(pIpcShareMem,sizeof(SIpcStatus),1,fp)==1);
	delete pIpcShareMem;
	return bResult;
}
/*
	brief: �����ļ�ͷУ����
	param [in] FILE* fp�������ļ�ָ��
	return bool��true����ʽ��ȷ��false����ʽ����
	author:Steven Wan
	date: 2015-09-26
*/
bool SIpcStatus::CheckFile(FILE* fp, void* ipOther)
{
	if (fseek(fp,0L,SEEK_END)!=0)
	{
		printf("fseek(fp,0L,SEEK_END) error in SIpcShareMem::InitFile\n");
		return false;
	}
	long len=ftell(fp);
	if (len>0 && (unsigned int)len==sizeof(SIpcStatus))
	{
		fseek(fp,0 ,SEEK_SET);
		HeaderChecker checker;
		if (fread(&checker,sizeof(HeaderChecker),1,fp)==1)
		{
			HeaderChecker initChecker;
			if (memcmp(&checker,&initChecker,sizeof(HeaderChecker))==0)
			{
				return true;
			}
		}
	}
	printf("SIpcStatus file check return false\n");
	return false;
}

/*
	brief: ����״̬���ݻ�ȡ���
	param [in] void
	param [out] none
	return SIpcShareMem�������ڴ����
	author:Steven Wan
	date: 2015-09-26
*/
SIpcStatus& SIpcStatus::Instance(void)
{	
	static SIpcStatus* pIpcStatus=NULL;
	if (pIpcStatus==NULL)
	{
		if (m_IpcStatus.IsInited()==false)
		{
			m_IpcStatus.Init(FILE_IPC_STATUS,sizeof(SIpcStatus),SIpcStatus::InitFile,SIpcStatus::CheckFile,NULL);
		}
		pIpcStatus=(SIpcStatus*)m_IpcStatus.GetMem();
	}
	ASSERT(pIpcStatus!=NULL);
	ASSERT(pIpcStatus==m_IpcStatus.GetMem());
	return *pIpcStatus;
}


