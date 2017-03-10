// CrypFun.h: interface for the CCrypFun class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CRYPFUN_H_
#define _CRYPFUN_H_

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/

//#define MAXBUFFER		20000	//��������ڴ泤��
#define MAXBC				4
#define MAXKC				4
#define MAXROUNDS			10
#define ROUNDS              10
#define BUF_DEF				1024

//
#ifndef DATAZIP
	typedef unsigned char BYTE;
#endif


typedef unsigned char	word8;
typedef unsigned short  word16;
typedef unsigned long	word32;

#define   BIAOSHI1  0xDD//periodѹ����ʶ
#define   BIAOSHI2  0xBB//RAYѹ����ʶ

#define   COMPARE_CHAR  0x9A//ΪѰ���������õıȽ�ֵ
#define   GUESS_SIZE  30

typedef struct
{
	word8 *x;
	word32 length;
}DATA;

#endif // !defined(AFX_CRYPFUN_H__1EC6029B_0E82_49BF_A978_9F0CBDDAE293__INCLUDED_)

