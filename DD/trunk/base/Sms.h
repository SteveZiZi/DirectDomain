#ifndef INCLUDED_SMS
#define INCLUDED_SMS

//����֡��
#define SMS_DATA_LENGTH 140

//���忨��һ���Կ��յ������Ķ�����Ŀ
#define MAX_SMS_NUM 32

////������ŷ�ʽ��������Ĳ��ʱ�䳤�ȣ����ӣ���������ʱ�䳤��û��ͨѶ����Ϊ����
//#define SMS_MAX_UNLIVE_MIN 3

// ����Ϣ�����ṹ������/���빲��
// ���У��ַ�����0��β

typedef struct {
	char SCA[16];			// ����Ϣ�������ĺ���(SMSC��ַ)
	char TPA[16];			// Ŀ������ظ�����(TP-DA��TP-RA)
	char TP_PID;			// �û���ϢЭ���ʶ(TP-PID)
	char TP_DCS;			// �û���Ϣ���뷽ʽ(TP-DCS)
	char TP_SCTS[16];		// ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
	char TP_UD[161];		// ԭʼ�û���Ϣ(����ǰ�������TP-UD)
	char index;				// ����Ϣ��ţ��ڶ�ȡʱ�õ�
	int  TP_UD_length;
} SM_PARAM;

// �û���Ϣ���뷽ʽ
#define GSM_7BIT		0
#define GSM_8BIT		4
#define GSM_UCS2		8

int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst);
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst);
int cdmaDecodePdu(const char* pSrc, SM_PARAM* pDst,int srcLen);
int cdmaEncodePdu(const SM_PARAM* pSrc, char* pDst);
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);



#endif
