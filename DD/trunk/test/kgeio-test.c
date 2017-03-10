/*
 * KGEIO Driver Test Program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <linux/types.h>
#include "kgeio-test.h"

int fd;

void print_help(void)
{
	  fprintf(stderr, "#########################################\n");
	  fprintf(stderr, "# power down--------- -pw ###############\n");
	  fprintf(stderr, "# battery control---- -bt ###############\n");
	  fprintf(stderr, "# bell control------- -be ###############\n");
	  fprintf(stderr, "# esam reset  ------- -er ###############\n");
	  fprintf(stderr, "# battery charge  --- -bc ###############\n");
	  fprintf(stderr, "# gprs igt        --- -pi ###############\n");
	  fprintf(stderr, "# gprs power set  --- -pp ###############\n");
	  fprintf(stderr, "# gprs reset      --- -pr ###############\n");
	  fprintf(stderr, "# plc reset       --- -lr ###############\n");
	  fprintf(stderr, "# plc set         --- -ls ###############\n");
	  fprintf(stderr, "# YKLED1 set      --- -l1 ###############\n");
	  fprintf(stderr, "# YKLED2 set      --- -l2 ###############\n");
	  fprintf(stderr, "# YKLED3 set      --- -l3 ###############\n");
	  fprintf(stderr, "# YKLED4 set      --- -l4 ###############\n");
	  fprintf(stderr, "# GKLED           --- -gk ###############\n");
	  fprintf(stderr, "# DKLED           --- -dk ###############\n");
	  fprintf(stderr, "# BDLED           --- -bd ###############\n");
	  fprintf(stderr, "# RUNLED          --- -ru ###############\n");
	  fprintf(stderr, "# ALARMLED        --- -al ###############\n");
	  fprintf(stderr, "# YK1             --- -k1 ###############\n");
	  fprintf(stderr, "# YK2             --- -k2 ###############\n");
	  fprintf(stderr, "# YK3             --- -k3 ###############\n");
	  fprintf(stderr, "# YK4             --- -k4 ###############\n");
	  fprintf(stderr, "# YKA             --- -ka ###############\n");
	  fprintf(stderr, "# YK_EN           --- -ye ###############\n");
	  fprintf(stderr, "# LCD_BL          --- -bl ###############\n");
	  fprintf(stderr, "# sim_heater      --- -pm ###############\n");
	  fprintf(stderr, "# gsm_state       --- -ms ###############\n");
	  fprintf(stderr, "# ykrst      	 --- -kr ###############\n");
	  fprintf(stderr, "# ykstate      	 --- -ks ###############\n");
	  fprintf(stderr, "# button program  --- -bp ###############\n");
	  fprintf(stderr, "# button open     --- -bo ###############\n");
	  fprintf(stderr, "# check button    --- -bb ###############\n");
	  fprintf(stderr, "# check battery status -- -cb ###########\n");
	  fprintf(stderr, "# read rcu bell        -- -rb ###########\n");
	  fprintf(stderr, "#########################################\n");
}

/*
 * The main program.  Run the program with "-d" to disable the card,
 * or "-e" to enable the card.
 */
int main(int argc, char *argv[])
{
	int sw1,sw2, sw3;
	int sel;
	struct timeval timeout;
	fd_set  rfds;
    fd = open("/dev/kgeio", O_RDWR );

    if (fd == -1) {
	fprintf(stderr, "device not enabled.\n");
	fflush(stderr);
	exit(-1);
    }

    if (argc > 1) {
	if (!strncasecmp(argv[1], "-pw", 3)) {
	  ioctl(fd, KGESG_IO_GET_PWRDW, &sw1);
	  fprintf(stderr, "power down is %2x.\n", sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-bt", 3)) {
	  fprintf(stderr, "battery control set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_BATCTRL, &sw1);
	  fprintf(stderr, "battery control is %2x.\n",sw1);
	    fflush(stderr);
	}
       else if (!strncasecmp(argv[1], "-be", 3)) {
	  fprintf(stderr, "bell control set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_BELL, &sw1);
	  fprintf(stderr, "bell control is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-er", 3)) {
	    fprintf(stderr, "esam reset set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_ESAM_RST, &sw1);
	  fprintf(stderr, "esam reset is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-bc", 3)) {
	    fprintf(stderr, "battery charge set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_BATCHARGE, &sw1);
	  fprintf(stderr, "battery charge is %2x.\n",sw1);
	    fflush(stderr);
	 }
	else if (!strncasecmp(argv[1], "-pi", 3)) {
	    fprintf(stderr, "gprs igt set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_GPRSIGT, &sw1);
	    fflush(stderr);
	 }
	else if (!strncasecmp(argv[1], "-pp", 3)) {
	    fprintf(stderr, "gprs power set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_GPRSPWR, &sw1);
	  fprintf(stderr, "gprs power is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-pr", 3)) {
	fprintf(stderr, "gprs reset set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_GPRSRST, &sw1);
	  fprintf(stderr, "gprs reset is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-lr", 3)) {
	  fprintf(stderr, "plc reset set or clear: 1/0.\n");
          while(1)
            {
              scanf("%d",&sw1);
              if((sw1 == 1) || (sw1 == 0))
                break;
            }
          ioctl(fd, KGESG_IO_SET_PLC_RESET, &sw1);
          fprintf(stderr, "plc reset is %2x.\n",sw1);
	  fflush(stderr);
        }
	else if (!strncasecmp(argv[1], "-ls", 3)) {
	  fprintf(stderr, "plc set set or clear: 1/0.\n");
          while(1)
            {
              scanf("%d",&sw1);
              if((sw1 == 1) || (sw1 == 0))
                break;
            }
          ioctl(fd, KGESG_IO_SET_PLC_SET, &sw1);
          fprintf(stderr, "plc set is %2x.\n",sw1);
	  fflush(stderr);
        }
	else if (!strncasecmp(argv[1], "-l1", 3)) {
	fprintf(stderr, "YKLED1 set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YKLED1, &sw1);
	  fprintf(stderr, "YKLED1 is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-l2", 3)) {
	fprintf(stderr, "YKLED2 set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YKLED2, &sw1);
	  fprintf(stderr, "YKLED2 is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-l3", 3)) {
	fprintf(stderr, "YKLED3 set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YKLED3, &sw1);
	  fprintf(stderr, "YKLED3 is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-l4", 3)) {
	fprintf(stderr, "YKLED4 set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YKLED4, &sw1);
	  fprintf(stderr, "YKLED4 is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-gk", 3)) {
	fprintf(stderr, "GKLED set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_GKLED, &sw1);
	  fprintf(stderr, "GKLED is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-bd", 3)) {
	fprintf(stderr, "BDLED set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_BDLED, &sw1);
	  fprintf(stderr, "BDLED is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-dk", 3)) {
	fprintf(stderr, "DKLED set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_DKLED, &sw1);
	  fprintf(stderr, "DKLED is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-ru", 3)) {
	fprintf(stderr, "RUNLED set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_RUNLED, &sw1);
	  fprintf(stderr, "RUNLED is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-al", 3)) {
	fprintf(stderr, "ALARMLED set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_ALARMLED, &sw1);
	  fprintf(stderr, "ALARMLED is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-k1", 3)) {
	fprintf(stderr, "YK1 set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YK1, &sw1);
	  fprintf(stderr, "YK1 is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-k2", 3)) {
	fprintf(stderr, "YK2 set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YK2, &sw1);
	  fprintf(stderr, "YK2 is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-k3", 3)) {
	fprintf(stderr, "YK3 set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YK3, &sw1);
	  fprintf(stderr, "YK3 is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-k4", 3)) {
	fprintf(stderr, "YK4 set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YK4, &sw1);
	  fprintf(stderr, "YK4 is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-ka", 3)) {
	fprintf(stderr, "YKA set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YKA, &sw1);
	  fprintf(stderr, "YKA is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-ye", 3)) {
	fprintf(stderr, "YK_EN set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YK_EN, &sw1);
	  fprintf(stderr, "YK_EN is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-pm", 3)) {
	fprintf(stderr, "sim card heater set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_M_POWER, &sw1);
	  fprintf(stderr, "M_POWER is %2x.\n",sw1);
	  fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-kr", 3)) {
	fprintf(stderr, "yk module reset set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_YK_RST, &sw1);
	  fprintf(stderr, "YK_RST is %2x.\n",sw1);
	  fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-bl", 3)) {
	  fprintf(stderr, "LCD Backlight set or clear: 1/0.\n");
	  while(1)
	    {
	      scanf("%d",&sw1);
	      if((sw1 == 1) || (sw1 == 0))
		break;
	    }
	  ioctl(fd, KGESG_IO_SET_LCD_BL, &sw1);
	  fprintf(stderr, "LCD_BL is %2x.\n",sw1);
	    fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-bp", 3)) {
	  ioctl(fd, KGESG_IO_GET_BUTTON_P, &sw1);
	  fprintf(stderr, "button program is %2x.\n", sw1);
	  fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-ms", 3)) {
	  ioctl(fd, KGESG_IO_GET_M_STATE, &sw1);
	  fprintf(stderr, "gsm state is %2x.\n", sw1);
	  fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-ks", 3)) {
	  ioctl(fd, KGESG_IO_GET_YK_STATE, &sw1);
	  fprintf(stderr, "gsm state is %2x.\n", sw1);
	  fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-bo", 3)) {
	  ioctl(fd, KGESG_IO_GET_BUTTON_O, &sw1);
	  fprintf(stderr, "button open is %2x.\n", sw1);
	  fflush(stderr);
	}
    else if (!strncasecmp(argv[1], "-cb", 3)) {
	  ioctl(fd, KGESG_IO_GET_BATCHECK, &sw1);
	  fprintf(stderr, "check battery status is %2x.\n", sw1);
	  fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-rb", 3)) {
	  ioctl(fd, KGESG_IO_GET_RCUBELL, &sw1);
	  fprintf(stderr, "RCUBELL is %2x.\n", sw1);
	  fflush(stderr);
	}
	else if (!strncasecmp(argv[1], "-bb", 3)) {
	  fprintf(stderr, "check button begin\n");
	  while(1){
	  FD_ZERO (&rfds);
	  FD_SET (fd, &rfds);
	  timeout.tv_sec = 0;
	  timeout.tv_usec = 10000000;
	  sel = select (fd + 1, &rfds, NULL, NULL, &timeout);
	  if(sel > 0)
	    {
	      fprintf(stderr, "sel > 0.\n");
	      if(FD_ISSET (fd, &rfds))
		{
		  sw2 = read(fd,&sw1, 1);
		  fprintf(stderr, "button val is %2x.\n", sw1);
		}
	    }
	  else{
	    fprintf(stderr, "sel fail.\n");
	  }
	  }
	}
	else {
	  fprintf(stderr, "Kicking Away!\n");
	  print_help();
	fflush(stderr);
    }
    }else{
      print_help();
    }
    close(fd);

}
