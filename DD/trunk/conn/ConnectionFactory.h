#ifndef INCLUDED_CONNECTION_FACTORY
#define INCLUDED_CONNECTION_FACTORY

class Connection;
struct Channel;

class ConnectionFactory
{
public:
	static Connection* CreateConnection(Channel& channel);
};

#endif
