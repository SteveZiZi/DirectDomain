#ifndef _STDINCLUDE_H_
#define _STDINCLUDE_H_

#ifndef WIN32
#include <unistd.h> 
#include <dirent.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h> 
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "sys/ioctl.h"
#include <sys/types.h>
#include <sys/io.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/sem.h>

#else
#include <io.h>
#include <Windows.h>
#include <direct.h>
#include <sys/stat.h>
#define EINPROGRESS WSAEINPROGRESS
#endif

#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include "type.h"
#include <string>
using namespace std;




#define MEMORY_BUF_SIZE 100



typedef struct _SHARE_MEM
{
	long minuteTick;
	int dbcjpid;
	int httxpid;
	int ppppid;
	char buf[100];
}T_SHARE_MEM;

#endif


