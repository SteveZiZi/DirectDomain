#ifndef INCLUDED_SMS
#define INCLUDED_SMS

//定义帧长
#define SMS_DATA_LENGTH 140

//定义卡内一次性可收到的最多的短信数目
#define MAX_SMS_NUM 32

////定义短信方式充许的最大的不活动时间长度（分钟），超过此时间长度没有通讯，视为断链
//#define SMS_MAX_UNLIVE_MIN 3

// 短消息参数结构，编码/解码共用
// 其中，字符串以0结尾

typedef struct {
	char SCA[16];			// 短消息服务中心号码(SMSC地址)
	char TPA[16];			// 目标号码或回复号码(TP-DA或TP-RA)
	char TP_PID;			// 用户信息协议标识(TP-PID)
	char TP_DCS;			// 用户信息编码方式(TP-DCS)
	char TP_SCTS[16];		// 服务时间戳字符串(TP_SCTS), 接收时用到
	char TP_UD[161];		// 原始用户信息(编码前或解码后的TP-UD)
	char index;				// 短消息序号，在读取时用到
	int  TP_UD_length;
} SM_PARAM;

// 用户信息编码方式
#define GSM_7BIT		0
#define GSM_8BIT		4
#define GSM_UCS2		8

int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst);
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst);
int cdmaDecodePdu(const char* pSrc, SM_PARAM* pDst,int srcLen);
int cdmaEncodePdu(const SM_PARAM* pSrc, char* pDst);
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);



#endif
