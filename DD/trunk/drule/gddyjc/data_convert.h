#ifndef DATA_CONVERT
#define DATA_CONVERT

typedef enum _ERROR_NO					//定义所有的错误编码
{
	RIGHT_COMMAND = 0x00,				//正确无误
	RELY_NO_BACK = 0x01,				//中继命令没有返回
	WRONG_SET_CODE = 0x02,				//设置内容非法
	PSW_PERMISSION_IS_WRONG = 0x03,		//密码权限不足
	NO_SUCH_DATA = 0x04,				//无此数据项
	COMMAND_TIMEOUT = 0x05,				//命令时间失效
	AIM_ADDRESS_IS_NONE = 0x11,			//目标地址不存在
	FAIL_TO_SEND = 0x12,				//发送失败
	FRAME_IS_TOO_LONG = 0x13,			//短消息帧太长
	FAIL_TO_SET = WRONG_SET_CODE,					//参数数据不符合要求
	INVALID_METER = WRONG_SET_CODE,				//无效电表
	//FAIL_TO_LINK_SLAVE = WRONG_SET_CODE,			//与从集中器连接失败
	REC_FROM_SLAVE_TIMEOUT = 0x01,		//中继命令没有返回
}ERROR_NO;

#endif

