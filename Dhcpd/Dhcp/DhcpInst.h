#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DhcpInst_h__
#define __DhcpInst_h__

#define MAX_NAME_LEN			64
#define MAX_FILE_LEN			128

class CDhcpProc;
class CDhcpInst {
  public:
	CDhcpInst(CDhcpProc *pDhcpProc);
	~CDhcpInst();

  public:
	void			 SetTransID(dword dwTransID);
	dword			 GetTransID() const;
	void			 SetState(byte byState);
	byte			 GetState() const;

  public:
	void			 OnRecvDiscover(TDhcpMsg *pDhcpMsg);
	void			 OnRecvRequest(TDhcpMsg *pDhcpMsg);
	void			 OnRecvDecline(TDhcpMsg *pDhcpMsg);
	void			 OnRecvRelease(TDhcpMsg *pDhcpMsg);
	void			 OnRecvInform(TDhcpMsg *pDhcpMsg);

  protected:
	void			 SendOffer();
	void			 SendAck();
	void			 SendNack();
	void			 SendRelease();

  protected:
	void			 SendDLinkData(TDhcpMsg *pDhcpMsg);
	void			 OnTimerOut(int nTimerID);

  protected:
	dword			 m_dwTransID;
	byte			 m_byState;

  protected:
	byte			 m_byHWType;
	byte			 m_byHWLen;
	word			 m_wSeconds;
	dword			 m_dwClientIP;
	dword			 m_dwUserIP;
	dword			 m_dwServerIP;
	dword			 m_dwAgentIP;
	byte			 m_byMAC[6];
	char			 m_szHostName[MAX_NAME_LEN];
	char			 m_szBootFile[MAX_FILE_LEN];
	//Option Item
	dword			 m_dwSubnetMask;
	VECDWORD		 m_vecRouter;
	VECDWORD		 m_vecDNSServer;
	VECDWORD		 m_vecLogServer;
	word			 m_wInterfMTU;
	VECSTATICROUTE	 m_vecStaticRoute;
	dword			 m_dwARPCacheTime;
	VECDWORD		 m_vecNTPServer;
	dword			 m_dwIPLeaseTime;
	byte			 m_byMsgType;
	dword			 m_dwServerID;

  protected:
	CDhcpProc		*m_pJSelf;
};

inline void CDhcpInst::SetTransID(dword dwTransID)
{
	m_dwTransID = dwTransID;
}

inline dword CDhcpInst::GetTransID() const
{
	return m_dwTransID;
}

inline void CDhcpInst::SetState(byte byState)
{
	m_byState = byState;
}

inline byte CDhcpInst::GetState() const
{
	return m_byState;
}

#endif

#include "PRIVATE_NAMESPACE_END"
