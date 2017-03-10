#ifndef INCLUDED_NET_CONNECTION
#define INCLUDED_NET_CONNECTION

#include "PortConnection.h"

class NetConnection:public PortConnection
{
public:
	NetConnection(Channel& channel);
	~NetConnection();
public:
	bool OnConnect();
	//void OnDisconnect(bool ibIsBreaked);
protected:
	int OnWriteData(const unsigned char* ipSendData,int iiCount);
	int OnReadData(unsigned char* ipRecvBuff,int iiCount,int iiTimeoutMS);

	SOCKET SocketConnect(const char* pIPStr,int portNo,int timeoutS);
	SOCKET SocketAccept(SOCKET sockfd,struct sockaddr_in& oRemoteAddr,int timeoutS);
	SOCKET SocketListen(int portNo);
	SOCKET GetUdpSocket(int portNo);
	int UdpOnWriteData(const char* pIPStr,int portNo,const byte* ipSendData,int iiCount);
	int UdpOnReadData(int portNo,byte* ipRecvBuff,int iiCount,int iiMilliSeconds);
	void CloseSocket(SOCKET sockfd);
	int SetKeepAlive(SOCKET sockfd);
	virtual bool IsOnLine();
protected:
	SOCKET m_ListenSocket;
};
#endif


