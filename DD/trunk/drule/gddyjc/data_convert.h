#ifndef DATA_CONVERT
#define DATA_CONVERT

typedef enum _ERROR_NO					//�������еĴ������
{
	RIGHT_COMMAND = 0x00,				//��ȷ����
	RELY_NO_BACK = 0x01,				//�м�����û�з���
	WRONG_SET_CODE = 0x02,				//�������ݷǷ�
	PSW_PERMISSION_IS_WRONG = 0x03,		//����Ȩ�޲���
	NO_SUCH_DATA = 0x04,				//�޴�������
	COMMAND_TIMEOUT = 0x05,				//����ʱ��ʧЧ
	AIM_ADDRESS_IS_NONE = 0x11,			//Ŀ���ַ������
	FAIL_TO_SEND = 0x12,				//����ʧ��
	FRAME_IS_TOO_LONG = 0x13,			//����Ϣ̫֡��
	FAIL_TO_SET = WRONG_SET_CODE,					//�������ݲ�����Ҫ��
	INVALID_METER = WRONG_SET_CODE,				//��Ч���
	//FAIL_TO_LINK_SLAVE = WRONG_SET_CODE,			//��Ӽ���������ʧ��
	REC_FROM_SLAVE_TIMEOUT = 0x01,		//�м�����û�з���
}ERROR_NO;

#endif

