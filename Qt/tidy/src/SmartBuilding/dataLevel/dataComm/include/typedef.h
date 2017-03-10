/** @file
 *  @brief 基础数据类型定义
 *  @author Steven Wan
 *  @date 2016-02/11
 *  @version 0.1
*/

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

/* 设备相关定义*/
#define CONS_MAP_DEVICE_MAX_COUNT	12+1//从索引1开始
#define CONS_MAP_DEVICE_SUBNO_MAX_COUNT	32+1//从索引1开始
#define CONS_DEVICE_MAX_COUNT		160+1//从索引1开始
#define CONS_IORELAY_INPUTPORT_MAX_COUNT	12
#define CONS_CLRELAY_MAX_COUNT	16+1//从索引1开始
#define CONS_DEVICE_LUXSENSOR_STARTNO	97
#define CONS_REAL_CONTROL_COMMAND_MAX_COUNT	20 

/* 程控任务相关定义*/
#define CONS_NORTASK_MAX_COUNT		12+1//从索引1开始
#define CONS_TASKPLAN_MAX_COUNT		12//从索引0开始
#define CONS_CLRELAY_TASKACTION_NUM	24//从索引0开始
#define MANUAL_CTL 0x01
#define TASK_CTL 0x02
#define RESUME_CTL 0x03


/* 光控任务相关定义 */
#define CONS_LUXTASK_MAX_COUNT		2+1//从索引1开始
#define CONS_LUXTASKPLAN_MAX_COUNT		12//从索引0开始

/* 恒照任务相关定义*/
#define CONS_CONSTANTLUXTASK_MAX_COUNT		2+1//从索引1开始
#define CONS_TIME_PERIOD_IN_DAY_MAX_COUNT	1//当天有效时段

/* 数据类型定义*/
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
