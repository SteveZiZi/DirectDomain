#ifndef _MCP3208_H_
#define _MCP3208_H_

#include <stdio.h>


extern "C"
{
	int mcp3208_read(int *rxbuf, int a2dChannel);
}

#endif

