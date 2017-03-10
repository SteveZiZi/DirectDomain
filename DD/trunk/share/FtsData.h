#ifndef INCLUDED_FTS_DATA
#define INCLUDED_FTS_DATA

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif

#ifndef INCLUDED_STRING
#include "../base/String.h"
#endif

#ifndef INCLUDED_FILE_BASE
#include "../base/FileBase.h"
#endif

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif


struct FtsData
{
	void Init()
	{
		Lock_UpdateFlag = false;
		UpdateFlag = false;
	}
	bool SetUpdateFlag()
	{
		while (1)
		{
			if (!Lock_UpdateFlag)
			{
				UpdateFlag = true;
				Lock_UpdateFlag = true;
				return true;
			}
		}
		return false;
	}
	bool ReadUpdateFlag()
	{
		if (Lock_UpdateFlag)
		{
			return UpdateFlag;
		}
		return false;
	}
	inline int RenameUpdateKey()
	{
		return rename(CONS_AUT0_UPDATE_KEY_FILE_NAME_TEMP,CONS_AUTO_UPDATE_KEY_FILE_NAME);
	}
	inline int DeleteUpdateKey()
	{
		return unlink(CONS_AUTO_UPDATE_KEY_FILE_NAME);
	}
	bool UpdateFlag;//������־��֪ͨ�ػ���������ѹ�����ļ�
	bool Lock_UpdateFlag;
	int ReadUpdateSlaveFlag(const char *icFileName,unsigned char *UpdateSlaveFlag,bool DeleteOrNot);//���ļ��ж�ȡUpdateSlaveFlag��־
	int WriteUpdateSlaveFlag(bstring &UpdateSlaveFlag);//дUpdateSlaveFlag��־���ļ���
};

#endif
