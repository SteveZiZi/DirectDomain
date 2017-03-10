#ifndef INCLUDED_SAVE_NODE_FILE_H
#define INCLUDED_SAVE_NODE_FILE_H

#include "../include/StdInclude.h"
#include "../include/Config.h"
#include "../base/Timer.h"
#include "../share/SysBase.h"
#include "../base/PortSet.h"

#define FILE_SAVE_MAX_SIZE_BYTE   6*1024
#define FILE_LSTNODE_HEADSIZE   14
#define FILE_LSTNODE_RECODE_HEADSIZE 7

class NodesOp
{
public:
	static int SaveFailedNode(const INT8U  *addr,const INT16U &count);
	static int ReadFailedNode(bstring &nodeInfo,const INT8U &lstSeq);
};
#endif


