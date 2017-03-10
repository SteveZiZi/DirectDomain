/** @file
 *  @brief �����������Ͷ���
 *  @author Steven Wan
 *  @date 2016-02/11
 *  @version 0.1
*/

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

/* �豸��ض���*/
#define CONS_MAP_DEVICE_MAX_COUNT	12+1//������1��ʼ
#define CONS_MAP_DEVICE_SUBNO_MAX_COUNT	32+1//������1��ʼ
#define CONS_DEVICE_MAX_COUNT		160+1//������1��ʼ
#define CONS_IORELAY_INPUTPORT_MAX_COUNT	12
#define CONS_CLRELAY_MAX_COUNT	16+1//������1��ʼ
#define CONS_DEVICE_LUXSENSOR_STARTNO	97
#define CONS_REAL_CONTROL_COMMAND_MAX_COUNT	20 

/* �̿�������ض���*/
#define CONS_NORTASK_MAX_COUNT		12+1//������1��ʼ
#define CONS_TASKPLAN_MAX_COUNT		12//������0��ʼ
#define CONS_CLRELAY_TASKACTION_NUM	24//������0��ʼ
#define MANUAL_CTL 0x01
#define TASK_CTL 0x02
#define RESUME_CTL 0x03


/* ���������ض��� */
#define CONS_LUXTASK_MAX_COUNT		2+1//������1��ʼ
#define CONS_LUXTASKPLAN_MAX_COUNT		12//������0��ʼ

/* ����������ض���*/
#define CONS_CONSTANTLUXTASK_MAX_COUNT		2+1//������1��ʼ
#define CONS_TIME_PERIOD_IN_DAY_MAX_COUNT	1//������Чʱ��

/* �������Ͷ���*/
typedef unsigned char			INT8U;
typedef INT8U                   		UCHAR;
typedef signed char			INT8S;
typedef unsigned short			INT16U;
typedef signed short			INT16S;
typedef unsigned int			INT32U;
typedef signed int				INT32S;
typedef unsigned long long		INT64U;
typedef signed long long		INT64S;
typedef float					FP32;
typedef double				FP64;
typedef unsigned char 			byte;


typedef enum
{
	emFail = 0,
	emSucess =1	
}T_emResult;



#endif
