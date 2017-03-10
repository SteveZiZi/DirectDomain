/** @file
 *  @brief QT进程与后台监控进程共享内存通讯接口类
 *  @author Steven Wan
 *  @date 2015-08/30
 *  @version 0.1
*/
 
#include "ipcSharemem.h"

ShareMem SIpcShareMem::m_IpcShareMem;

SIpcShareMem::SIpcShareMem()
{
	memset(this,0,sizeof(*this));
	Checker.Init();
}
/*
	brief: 共享文件初始化
	param [in] FILE* fp：共享文件指针
	returnt true：格式正确，false：格式错误
	author:Steven Wan
	date: 2015-08-30
*/
bool SIpcShareMem::InitFile(FILE* fp, void* ipOther)
{
	ASSERT(fp);
	if (fseek(fp,0L ,SEEK_SET)!=0)
	{
		printf("fseek(fp,0L ,SEEK_SET) error in SIpcShareMem::InitFile\n");
		return false;
	}
	SIpcShareMem* pIpcShareMem=new SIpcShareMem;
	bool bResult=(fwrite(pIpcShareMem,sizeof(SIpcShareMem),1,fp)==1);
	delete pIpcShareMem;
	return bResult;
}
/*
	brief: 共享文件头校验检测
	param [in] FILE* fp：共享文件指针
	return bool：true：格式正确，false：格式错误
	author:Steven Wan
	date: 2015-08-30
*/
bool SIpcShareMem::CheckFile(FILE* fp, void* ipOther)
{
	if (fseek(fp,0L,SEEK_END)!=0)
	{
		printf("fseek(fp,0L,SEEK_END) error in SIpcShareMem::InitFile\n");
		return false;
	}
	long len=ftell(fp);
	if (len>0 && (unsigned int)len==sizeof(SIpcShareMem))
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
	printf("SIpcShareMem file check return false\n");
	return false;
}
/*
	brief: 共享内存获取入口
	param [in] void
	param [out] none
	return SIpcShareMem：共享内存对象
	author:Steven Wan
	date: 2015-08-30
*/
SIpcShareMem& SIpcShareMem::Instance(void)
{	
	static SIpcShareMem* pIpcShareMem=NULL;
	if (pIpcShareMem==NULL)
	{
		if (m_IpcShareMem.IsInited()==false)//第一次使用打开内存文件
		{
			m_IpcShareMem.Init(FILE_IPC_SHAREMEM,sizeof(SIpcShareMem),SIpcShareMem::InitFile,SIpcShareMem::CheckFile,NULL);
			m_IpcShareMem.OpenShareMemFile(SIpcShareMem::InitFile,SIpcShareMem::CheckFile,NULL);
		}
		pIpcShareMem=(SIpcShareMem*)m_IpcShareMem.GetMem();
	}
	ASSERT(pIpcShareMem!=NULL);
	ASSERT(pIpcShareMem==m_IpcShareMem.GetMem());
	return *pIpcShareMem;
}

