#ifndef INCLUDED_MESSAGE
#define INCLUDED_MESSAGE

#ifndef INCLUDED_STD_INCLUDE
#include "../include/StdInclude.h"
#endif


struct klb_msgbuf
{
    long int mtype;		/* type of received/sent message */
    char mtext[1];		/* text of the message */
};

class Message
{
public:
	Message(char iiQueueFlag=0);
public:
	bool SendData(const unsigned char* ipData,unsigned int iDataLen,long ilMsgType);
	bool RecvData(unsigned char* ipData,unsigned int& ioDataLen,long ilMsgType);
	bool SendMsg(void* ipMsgBuff,unsigned int iDataLen);
	bool RecvMsg(void* ipMsgBuff,unsigned int& ioDataLen,long ilMsgType);
	bool HasMsg(long ilMsgType);//check whether exist any message for type ilMsgType
	void ClearMsg(long ilMsgType);//clear message for type ilMsgType
	void Destroy();//destroy message queue
	inline int GetMsgQID() const
	{
		return m_msqid;
	}
private:
	void InitMsg(char iiQueueFlag);
protected:
	int m_msqid;//message queue id
};


#endif

