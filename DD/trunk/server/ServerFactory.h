#ifndef INCLUDED_SERVER_FACTORY
#define INCLUDED_SERVER_FACTORY

class Server;

class ServerFactory
{
public:
	static Server* GetServer(const char* pathFile,const char* serverName,int instanceID);
};

#endif
