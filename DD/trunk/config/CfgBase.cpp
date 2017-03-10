#include "CfgBase.h"
#include "../include/Config.h"
#include "../base/KLTrace.h"
#include "../base/KLBase.h"
#include "../base/FileBase.h"
#include "../config/confuse/confuse.h"



//解析配置文件，先从当前用户设置区域/mnt/user/eac/set搜寻，再从系统预配置中搜寻
int CfgBase::ParseSetFile(cfg_t* cfg,const char* name,const char* userCode,const char* planKey,string* oCfgFile)
{
	char cfgFile[CONS_PATH_FILE_MAX_LEN];
	while(1)
	{
#ifdef PRINTF_DATA
printf("CfgBase\n");
#endif
		FileBase::GetUserFile(PATH_SET_USER,name,userCode,planKey,cfgFile);
		if (ParseCfg(cfg,cfgFile)==0)
		{
			break;
		}
		const char* code=(userCode && userCode[0]!=0) ?userCode:"default";
		const char* key=(planKey && planKey[0]!=0) ?planKey:"default";
		sprintf(cfgFile,"%s/%s/%s/%s/%s",PATH_SET_SOURCE,CONS_SYSTEM_MODEL_LOWER,code,key,name);//such as "/mnt/eac/set/eac5000d/guangdong/default/meter.ini"
		if (ParseCfg(cfg,cfgFile)==0)
		{
			break;
		}
		sprintf(cfgFile,"%s/%s/%s/%s",PATH_SET_SOURCE,CONS_SYSTEM_MODEL_LOWER,code,name);//such as "/mnt/eac/set/eac5000d/guangdong/meter.ini"
		if (ParseCfg(cfg,cfgFile)==0)
		{
			break;
		}
		sprintf(cfgFile,"%s/%s/%s/%s",PATH_SET_SOURCE,CONS_SYSTEM_MODEL_LOWER,key,name);//such as "/mnt/eac/set/eac5000d/default/meter.ini"
		if (ParseCfg(cfg,cfgFile)==0)
		{
			break;
		}

		sprintf(cfgFile,"%s/%s/%s",PATH_SET_SOURCE,CONS_SYSTEM_MODEL_LOWER,name);//such as "/mnt/eac/set/eac5000d/meter.ini"
		if (ParseCfg(cfg,cfgFile)==0)
		{
			break;
		}
		sprintf(cfgFile,"%s/%s",PATH_SET_SOURCE,name);//such as "/mnt/eac/set/meter.ini"
		if (ParseCfg(cfg,cfgFile)==0)
		{
			break;
		}
		return -1;
	}
	KL_VERBOSE("Parse CfgFile %s succeed\n",cfgFile);
	return 0;
}

int CfgBase::DelSetFile(const char* name,const char* userCode,const char* planKey)
{
	char cfgFile[CONS_PATH_FILE_MAX_LEN];
	FileBase::GetUserFile(PATH_SET_USER,name,userCode,planKey,cfgFile);
	return FileBase::DeleteFile(cfgFile);
	
}
int CfgBase::SaveSetFile(cfg_t* cfg,const char* name,const char* userCode,const char* planKey)
{
	char cfgFile[CONS_PATH_FILE_MAX_LEN];
	FileBase::GetUserFile(PATH_SET_USER,name,userCode,planKey,cfgFile);
	FILE *fp =FileBase::OpenFile(cfgFile, "w");
	if (fp==NULL)
	{
		KL_ERROR("Open file %s failed\n",cfgFile);
		return -1;
	}
	cfg_print(cfg, fp);
	FileBase::Close(fp);
	KLSystem(CONS_SYNC_COMMAND"&");
	return 0;
}

int CfgBase::SaveSetFile(string& str,const char* name,const char* userCode,const char* planKey)
{
	char cfgFile[CONS_PATH_FILE_MAX_LEN];
	FileBase::GetUserFile(PATH_SET_USER,name,userCode,planKey,cfgFile);
	int ret = FileBase::WriteCfgFile(&str,cfgFile);
	KLSystem(CONS_SYNC_COMMAND"&");
	return ret;
}


int CfgBase::ParseCfg(cfg_t* cfg,const char* cfgFile)
{
	int result=cfg_parse(cfg,cfgFile);
	if (result== CFG_SUCCESS)
	{
		return 0;
	}
	if (result==CFG_PARSE_ERROR)
	{
		ASSERT(false);
		KL_ERROR("Parse cfg file[%s] failed\n",cfgFile);
	}
	return -1;
}
