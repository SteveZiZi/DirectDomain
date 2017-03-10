#ifndef INCLUDED_SERIAL_CONNECTION
#define INCLUDED_SERIAL_CONNECTION

#include "../base/PortSet.h"
#include "PortConnection.h"

class SerialConnection:public PortConnection
{
public:
	SerialConnection(Channel& channel);
	~SerialConnection(void);

	virtual void SetPortSet(PortSet& irPortSet,bool bForce,bool bSaved);
	void OnDisconnect(bool ibIsBreaked);//刘志强2006-11-27号迁移过来为了支持热拔插
	//inline void GetPortSet(PortSet& oPortSet)
	//{
	//	memcpy(&oPortSet,&m_Channel.Setting.Com.PortSet,sizeof(PortSet));
	//}

	/** @brief 设置端口映射表 
	 *  @param[in] pPortMap 端口映射表
	 *  @return void
	 */
	void SetPortMap(const INT8U *pPortMap){m_pPortMap = pPortMap;}

	PortSet& GetPortSet();
	INT8U GetPortNo();
	bool IsForModem();
	bool IsBaudChanged();
	inline bool IsReady() const
	{
#ifdef WIN32
		return m_hCom!=INVALID_HANDLE_VALUE;
#else
		return m_iPortFD>=0; 
#endif
	}
protected:
	bool OnConnect();
	int Config(const void* pSetting,int iiLen);

	#ifdef WIN32
	int OnWriteData(const INT8U* ipSendData,int iiCount);
	int OnReadData(INT8U* ipRecvBuff,int iiCount,int iiMilliSeconds);
	#endif

protected:
	#ifdef WIN32
	HANDLE m_hCom;
	#endif
	const INT8U *m_pPortMap; ///< 端口映射表
};

#endif

