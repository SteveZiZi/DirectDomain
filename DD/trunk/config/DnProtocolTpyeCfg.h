#ifndef INCLUDED_DNPROTOCOL_TYPE_H_
#define INCLUDED_DNPROTOCOL_TYPE_H_

#ifndef INCLUDED_FILE_DIR
#include "../include/FileDir.h"
#endif

struct Context;

typedef struct _T_DN_PROTOCOL_TYPE
{
	int index;
	char *key;
}T_DN_PROTOCOL_TYPE;

class DnProtocolTpye
{
public:
	static int LoadDnProtocolTpye(Context& oContext,const char* name=NAME_SET_DNPROTOCOL);
	static int SaveDnProtocolTpye(Context& oContext,const char* name=NAME_SET_DNPROTOCOL);
	static int GetDnProtocolTpyeType(const char *key);
	static char *GetDnProtocolTpyeString(const int type);
};
#endif
