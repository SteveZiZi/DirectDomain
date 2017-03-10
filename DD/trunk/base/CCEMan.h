// CCEMan.h: interface for the CCEMan class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CCEMAN_H_
#define _CCEMAN_H_


//#define MAXSBUFLEN 2048
#define MAXSBUFLEN 16384      //Special for KeLi 

#define EXE_COMPRESS_NEW	0x01//ÐÂµÄÑ¹Ëõ 

//#include "CompressFunNew.h"

class CCEMan  
{
public:
	CCEMan();
	virtual ~CCEMan();
	int EnData(unsigned char * DataBuf, int DataLen, unsigned char Oper);
	int DeData(unsigned char * DataBuf, int DataLen);

	unsigned char SendBuf[MAXSBUFLEN];
	unsigned char RecvBuf[MAXSBUFLEN];

private:
//	CCompressFunNew mynewcompress;
	//unsigned char m_key[4][MAXKC];
	
	int FormFrame(unsigned char Oper, unsigned char * buf,int buflen);
};

#endif //(_CCEMAN_H_)
