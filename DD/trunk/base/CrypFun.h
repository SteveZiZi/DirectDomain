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

//#define MAXBUFFER		20000	//最大申请内存长度
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

#define   BIAOSHI1  0xDD//period压缩标识
#define   BIAOSHI2  0xBB//RAY压缩标识

#define   COMPARE_CHAR  0x9A//为寻找周期设置的比较值
#define   GUESS_SIZE  30

typedef struct
{
	word8 *x;
	word32 length;
}DATA;

#endif // !defined(AFX_CRYPFUN_H__1EC6029B_0E82_49BF_A978_9F0CBDDAE293__INCLUDED_)

