#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DhcpConfig_h__
#define __DhcpConfig_h__

class CDhcpConfig {
  public:
	CDhcpConfig();
  	~CDhcpConfig();

  public:
	bool			 LoadConfig(std::wstring szConfigFile);

  public:
	dword			 m_dwRentMoment;
	byte			 m_byMAC[6];
	dword			 m_dwClientIP;
	dword			 m_dwSubnetMask;
	dword			 m_dwAgentIP;
	VECDWORD		 m_vecRouter;
	VECDWORD		 m_vecDNSServer;
	VECDWORD		 m_vecLogServer;
	word			 m_wInterfMTU;
	VECSTATICROUTE	 m_vecStaticRoute;
	dword			 m_dwARPCacheTime;
	VECDWORD		 m_vecNTPServer;
	dword			 m_dwIPLeaseTime;
};

#endif

#include "PRIVATE_NAMESPACE_END"
