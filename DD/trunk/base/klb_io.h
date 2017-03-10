#ifndef __BASE_IO_H
#define __BASE_IO_H

#ifndef INCLUDED_PORT_SET
#include "../base/PortSet.h"
#endif

//82C55 Node file
//#define FILE_DRIVER_PPC8255	"/dev/ppc8255"
//#include "../../include/kl_type.h"
//#include "../arch/powerpc/driver/ppc8255/ppc8255.h"
//#include "../arch/powerpc/driver/ppc16c554/ppc16c554.h"

//82C55 driver
//#define CONS_PPC8255_PORTA  0x7001	//Comand symbol:the magic code='p(0x70)'
//#define CONS_PPC8255_PORTB  0x7002
//#define CONS_PPC8255_PORTC  0x7003

#define INVALID_CMD_CODE -1
#define KGEIO_DEV_NAME "/dev/kgeio"

int io_SetSeriPortAttr(int iFd,bool i_bInpck=true,bool ibModem=false);

//Function:initialize attribute of serial port without block pattern
//In:
//	i_cNo:number of serial port
//Out:
//	o_iFd:handle of serial port
//Error:
//	0--success; other--failure
int io_InitSeriPort_Unblock(unsigned char i_cNo,int *o_iFd,bool i_bInpck=true,bool ibModem=false);

//Function:initialize attribute of serial port with block pattern
//In:
//	i_cNo:number of serial port
//Out:
//	o_iFd:handle of serial port
//Error:
//	0--success; other--failure
//int io_InitSeriPort_Block(unsigned char i_cNo,int *o_iFd,bool i_bInpck=true,bool ibModem=false);




//Function:set attribute of serial port
//In:
//	i_iFd:handle of serial port
//	i_struPortSet:struct variable of serial port attribute
//Out:
//	no;
//Error:
//	0--success; other--failure
int io_SetSeriPortLine(int i_iFd,PortSet i_struPortSet);

//Function:only set baud of serial port
//In:
//	i_iFd:handle of serial port
//	i_iBaud:baud of serial port attribute
//Out:
//	no;
//Error:
//	0--success; other--failure
//int io_SetSeriPortBaud(int i_iFd,int i_iBaud);

//Function:Ignite GPRS Mode
//In:no
//Out:no
//Error:
//	0--success; other--failure
int io_IgniteGprsModem();

void io_PowerOffGprsModem(int baseDelayMS=3000,int exDelayMS=5000);
//void io_PowerOnGprsModem(int baseDelayMS=3000,int exDelayMS=5000);
//void io_ResetGprsModem(bool bForce=true);

//Function:Clear Watchdog
//In:no
//Out:no
//Error:
//	0--success; other--failure
//int io_ClrWatchdog();

//Function:Set Watchdog
//In:no
//Out:no
//Error:
//	0--success; other--failure
//int io_SetWatchdog();


void io_SetRTS(int i_iFd,bool i_bClear);
void io_SetDTR(int i_iFd,bool i_bClear);
void io_SetCD(int i_iFd,bool i_bClear);

bool io_HasCD(int i_iFd);
bool io_HasRing(int i_iFd);
//bool io_IsBatteryUsed(int i_iFd);
//bool io_CutBattery(int i_iFd);

//Function:write data to serail port
//In:
//    i_Fd:handle of serial port
//    *i_cpBuf:pointer refer to the data to be send
//    i_iCount:number of data to send
//    i_iBaud:baud rate of serial port
//    i_Tms:ms time to delay
//Out:
//    number of data actually sent
//Error:
//	(Out==i_iCount)--success; (Out!=i_iCount)--failure
//int io_WriteBuffer(int i_iFd,unsigned char *i_cpBuf,int i_iCount,int i_iBaud,int i_iTms);

//Function:write data to serail port or net
//In:
//    i_iFd:handle of serial port or net
//    *i_cpBuf:pointer refer to the data to be send
//    i_iCount:number of data to send
//Out:
//    number of data actually sent
//Error:
//	(Out==i_iCount)--success; (Out!=i_iCount)--failure
//int io_WriteBuffer(int i_iFd,unsigned char *i_cpBuf,int i_Count);

//Function:get one-byte data from serail port or net
//In:
//    i_iFd:handle of serial port or net
//    *o_cpBuf:pointer refer to the data to be send
//Out:
//    number of data actually sent
//Error:
//	0--success; other--failure
//int io_WaitPort(int i_iFd,unsigned char *o_cpBuf);

//int IO_8255BC_SetOrClearBit(unsigned char PortNo,unsigned char Bit,unsigned char SetOrClear);

//Function:get one-byte data from serail port--16c554
//In:
//    i_iFd:handle of serial port or net
//    *o_cpBuf:pointer refer to the data to be send
//Out:
//    number of data actually sent
//Error:
//	0--success; other--failure
//int io_WaitPort_16c554(int i_iFd,unsigned char *o_cpBuf);

//Function:delay milliseconds
// In:
//	i_imSec:milliseconds
// Out: no
//Error:no
//void delay_ms(int i_imSec);

//Function:delay seconds
// In:
//	i_iSec:seconds
// Out: no
//Error:no
//void delay_s(int i_iSec);

//
//new define
//
#ifndef WIN32

#ifdef __KERNEL__
#  include <linux/ioctl.h>
#else
#  include <sys/ioctl.h>
#endif

#define KGESG_IO_MAGIC        'K'

/* led control, no data exchanged */
#define KGESG_IO_RUNLED_ON             _IO(KGESG_IO_MAGIC, 1)
#define KGESG_IO_RUNLED_OFF            _IO(KGESG_IO_MAGIC, 2)

/* left-box (debugging box) */
#define KGESG_IO_LEFTBOX_RST_HIGH    _IO(KGESG_IO_MAGIC, 3)
#define KGESG_IO_LEFTBOX_RST_LOW     _IO(KGESG_IO_MAGIC, 4)

#define KGESG_IO_LEFTBOX_SET_HIGH      _IO(KGESG_IO_MAGIC, 5)
#define KGESG_IO_LEFTBOX_SET_LOW       _IO(KGESG_IO_MAGIC, 6)

/* so-called state-value from Right-box */
#define KGESG_IO_GET_STATE0   _IOR(KGESG_IO_MAGIC, 7, int)
#define KGESG_IO_GET_STATE1   _IOR(KGESG_IO_MAGIC, 8, int)
#define KGESG_IO_GET_STATE2   _IOR(KGESG_IO_MAGIC, 9, int)
#define KGESG_IO_GET_STATE3   _IOR(KGESG_IO_MAGIC, 10, int)
#define KGESG_IO_GET_STATE4   _IOR(KGESG_IO_MAGIC, 11, int)

/* so-called switch input */
#define KGESG_IO_GET_SW1      _IOR(KGESG_IO_MAGIC, 12, int)
#define KGESG_IO_GET_SW2      _IOR(KGESG_IO_MAGIC, 13, int)

/* the fucking Meng Jie Dian : the state of case-cover */
#define KGESG_IO_GET_CASECOVER    _IOR(KGESG_IO_MAGIC, 14, int)

#define KGESG_IO_GPRSIGT_HIGH     _IO(KGESG_IO_MAGIC, 15)
#define KGESG_IO_GPRSIGT_LOW      _IO(KGESG_IO_MAGIC, 16)

#define KGESG_IO_GPRSPWR_HIGH     _IO(KGESG_IO_MAGIC, 17)
#define KGESG_IO_GPRSPWR_LOW      _IO(KGESG_IO_MAGIC, 18)

#define KGESG_IO_GPRSRST_HIGH     _IO(KGESG_IO_MAGIC, 19)
#define KGESG_IO_GPRSRST_LOW      _IO(KGESG_IO_MAGIC, 20)

#define KGESG_IO_FEED_WDT         _IO(KGESG_IO_MAGIC, 21)
#define KGESG_IO_WDT_EN           _IO(KGESG_IO_MAGIC, 22)
#define KGESG_IO_WDT_DIS          _IO(KGESG_IO_MAGIC, 23)

#define KGESG_IO_ALARMLED_ON             _IO(KGESG_IO_MAGIC, 26)
#define KGESG_IO_ALARMLED_OFF            _IO(KGESG_IO_MAGIC, 27)

#define KGESG_IO_ESAM_RST         _IO(KGESG_IO_MAGIC, 24)

#define KGESG_IO_SET_RCUBELL            _IOW(KGESG_IO_MAGIC, 35, int)//门铃
#define KGESG_IO_SET_RCUDND             _IOW(KGESG_IO_MAGIC, 36, int)//请勿打扰
#define KGESG_IO_SET_RCUCLEAN           _IOW(KGESG_IO_MAGIC, 37, int)//清理房间
//#define KGESG_IO_SET_RCUWAIT            _IOW(KGESG_IO_MAGIC, 38, int)//请稍后
#define KGESG_IO_SET_RCUPOWERLIGHT     _IOW(KGESG_IO_MAGIC, 38, int)//总电源指示灯
#define KGESG_IO_SET_RCUBELLBG         _IOW(KGESG_IO_MAGIC, 39, int)//门铃背光灯
//#define KGESG_IO_SET_RCUSET             _IOW(KGESG_IO_MAGIC, 48, int)
#define KGESG_IO_SET_NIGHTLIGHT             _IOW(KGESG_IO_MAGIC, 48, int)//夜灯指示灯

#define KGESG_IO_GET_RCUCARD            _IOR(KGESG_IO_MAGIC, 40, int)//门卡
#define KGESG_IO_GET_RCUDND             _IOR(KGESG_IO_MAGIC, 41, int)//请勿打扰
#define KGESG_IO_GET_RCUCLEAN           _IOR(KGESG_IO_MAGIC, 42, int)//清理房间
#define KGESG_IO_GET_RCUWAIT            _IOR(KGESG_IO_MAGIC, 43, int)//请稍后
#define KGESG_IO_GET_RCUSOS             _IOR(KGESG_IO_MAGIC, 44, int)//紧急按钮
#define KGESG_IO_GET_RCUDOOR            _IOR(KGESG_IO_MAGIC, 45, int)//大门检测
#define KGESG_IO_GET_RCUBELL             _IOR(KGESG_IO_MAGIC, 46, int)//门铃
#define KGESG_IO_GET_RCUIR             _IOR(KGESG_IO_MAGIC, 47, int)//人体检测


#ifdef GDLOWER
#define KGESG_IO_GET_PWRDW              _IOR(KGESG_IO_MAGIC, 1, int)
#define KGESG_IO_SET_BATCTRL            _IOW(KGESG_IO_MAGIC, 2, int)
#define KGESG_IO_SET_ESAM_RST           _IOW(KGESG_IO_MAGIC, 3, int)
#define KGESG_IO_SET_BATCHARGE          _IOW(KGESG_IO_MAGIC, 4, int)
#define KGESG_IO_SET_GPRSIGT            _IOW(KGESG_IO_MAGIC, 5, int)
#define KGESG_IO_SET_GPRSPWR            _IOW(KGESG_IO_MAGIC, 6, int)
#define KGESG_IO_SET_GPRSRST            _IOW(KGESG_IO_MAGIC, 7, int)

#define KGESG_IO_SET_ALARMLED           _IOW(KGESG_IO_MAGIC, 15, int)
#endif

#endif

int KgeioWriteCmd(char *pCmd, int state);
int KgeioReadState(char *pCmd);
int io_Watchdog(void);

#endif
