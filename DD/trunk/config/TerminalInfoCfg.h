#ifndef _TERMINAL_INFO_CFG_H_
#define _TERMINAL_INFO_CFG_H_

class TerminalInfoCfg
{
public:
	static int LoadTerminalConfig(Context& oContext,const char* cfgFile=FILE_TERMINAL_CONFIG);
	static int SaveTerminalConfig(Context& oContext,const char* cfgFile=FILE_TERMINAL_CONFIG);
};		
#endif

