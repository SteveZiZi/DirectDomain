
#ifndef INCLUDED_PORT_CONNECTION
#define INCLUDED_PORT_CONNECTION

#include "../share/Connection.h"

class PortConnection:public Connection
{
public:
	PortConnection(Channel& channel);
	~PortConnection(void);
public:
	inline SOCKET GetFD() const
	{//�򿪵Ķ˿ں���
	 //��klb_io.cpp->io_InitSeriPort_Unblock�����ɸ�ֵlinux
	 //PC16C554Connection.inc->OnConnect�����ɸ�ֵ
	 //
		return m_iPortFD;
	}

	int GetPortFd();

protected:
	bool OnConnect()=0;
	void OnDisconnect(bool ibIsBreaked);
	int OnWriteData(const unsigned char* ipSendData,int iiCount);
	int OnReadData(unsigned char* ipRecvBuff,int iiCount,int iiTimeoutMS);
	int OnLineCheck();
	virtual int CheckCD();
private:
	int BlockReadData(unsigned char* ipRecvBuff,int iiCount,int iiTimeoutMS);
	int NonBlockReadData(unsigned char* ipRecvBuff,int iiCount,int iiTimeoutMS);
	bool SocketLinkCheck(void);
public:
	bool p_bBlockRead;
protected:
	SOCKET m_iPortFD;
};

#endif


