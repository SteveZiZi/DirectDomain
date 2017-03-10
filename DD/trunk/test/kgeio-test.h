#ifndef __KGEIO_H
#define __KGEIO_H

#  include <sys/ioctl.h>

#define KGESG_IO_MAGIC        'K'

#define KGESG_IO_GET_PWRDW              _IOR(KGESG_IO_MAGIC, 1, int)
#define KGESG_IO_SET_BATCTRL            _IOW(KGESG_IO_MAGIC, 2, int)
#define KGESG_IO_SET_ESAM_RST           _IOW(KGESG_IO_MAGIC, 3, int)
#define KGESG_IO_SET_BATCHARGE          _IOW(KGESG_IO_MAGIC, 4, int)
#define KGESG_IO_SET_GPRSIGT            _IOW(KGESG_IO_MAGIC, 5, int)
#define KGESG_IO_SET_GPRSPWR            _IOW(KGESG_IO_MAGIC, 6, int)
#define KGESG_IO_SET_GPRSRST            _IOW(KGESG_IO_MAGIC, 7, int)

#define KGESG_IO_SET_YKLED1             _IOW(KGESG_IO_MAGIC, 8, int)
#define KGESG_IO_SET_YKLED2             _IOW(KGESG_IO_MAGIC, 9, int)
#define KGESG_IO_SET_YKLED3             _IOW(KGESG_IO_MAGIC, 10, int)
#define KGESG_IO_SET_YKLED4             _IOW(KGESG_IO_MAGIC, 11, int)
#define KGESG_IO_SET_GKLED              _IOW(KGESG_IO_MAGIC, 12, int)
#define KGESG_IO_SET_DKLED              _IOW(KGESG_IO_MAGIC, 13, int)
#define KGESG_IO_SET_RUNLED             _IOW(KGESG_IO_MAGIC, 14, int)
#define KGESG_IO_SET_ALARMLED           _IOW(KGESG_IO_MAGIC, 15, int)


#define KGESG_IO_SET_YK1                _IOW(KGESG_IO_MAGIC, 16, int)
#define KGESG_IO_SET_YK2                _IOW(KGESG_IO_MAGIC, 17, int)
#define KGESG_IO_SET_YK3                _IOW(KGESG_IO_MAGIC, 18, int)
#define KGESG_IO_SET_YK4                _IOW(KGESG_IO_MAGIC, 19, int)
#define KGESG_IO_SET_YKA                _IOW(KGESG_IO_MAGIC, 20, int)

#define KGESG_IO_GET_BUTTON_P           _IOR(KGESG_IO_MAGIC, 21, int)
#define KGESG_IO_GET_BUTTON_O           _IOR(KGESG_IO_MAGIC, 22, int)

#define KGESG_IO_SET_YK_EN              _IOW(KGESG_IO_MAGIC, 23, int)
#define KGESG_IO_SET_LCD_BL             _IOW(KGESG_IO_MAGIC, 24, int)

#define KGESG_IO_SET_GJ_LED             _IOW(KGESG_IO_MAGIC, 25, int)
#define KGESG_IO_SET_BELL               _IOW(KGESG_IO_MAGIC, 26, int)
#define KGESG_IO_SET_PLC_RESET          _IOW(KGESG_IO_MAGIC, 27, int)
#define KGESG_IO_SET_PLC_SET            _IOW(KGESG_IO_MAGIC, 28, int)

#define KGESG_IO_SET_BDLED             _IOW(KGESG_IO_MAGIC, 29, int)

#define KGESG_IO_SET_M_POWER            _IOW(KGESG_IO_MAGIC, 30, int)
#define KGESG_IO_GET_M_STATE            _IOR(KGESG_IO_MAGIC, 31, int)
#define KGESG_IO_SET_YK_RST             _IOW(KGESG_IO_MAGIC, 32, int)
#define KGESG_IO_GET_YK_STATE           _IOR(KGESG_IO_MAGIC, 33, int)
#define KGESG_IO_GET_BATCHECK           _IOR(KGESG_IO_MAGIC, 34, int)

#define KGESG_IO_SET_RCUBELL            _IOW(KGESG_IO_MAGIC, 35, int)
#define KGESG_IO_SET_RCUDND             _IOW(KGESG_IO_MAGIC, 36, int)
#define KGESG_IO_SET_RCUCLEAN           _IOW(KGESG_IO_MAGIC, 37, int)
#define KGESG_IO_SET_RCUWAIT            _IOW(KGESG_IO_MAGIC, 38, int)
#define KGESG_IO_SET_RCUCTRL            _IOW(KGESG_IO_MAGIC, 39, int)
#define KGESG_IO_GET_RCUCARD            _IOR(KGESG_IO_MAGIC, 40, int)
#define KGESG_IO_GET_RCUDND             _IOR(KGESG_IO_MAGIC, 41, int)
#define KGESG_IO_GET_RCUCLEAN           _IOR(KGESG_IO_MAGIC, 42, int)
#define KGESG_IO_GET_RCUWAIT            _IOR(KGESG_IO_MAGIC, 43, int)
#define KGESG_IO_GET_RCUSOS             _IOR(KGESG_IO_MAGIC, 44, int)
#define KGESG_IO_GET_RCUDOOR            _IOR(KGESG_IO_MAGIC, 45, int)
#define KGESG_IO_GET_RCUBELL             _IOR(KGESG_IO_MAGIC, 46, int)
#define KGESG_IO_GET_RCUIR             _IOR(KGESG_IO_MAGIC, 47, int)

#endif
