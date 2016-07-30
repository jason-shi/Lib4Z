#ifndef __Dhcpd_h__
#define __Dhcpd_h__

#include "IDhcpServer.h"

#define INVALID_IP									0x00000000

#define DHCPD_OK									0
#define DHCPD_STATICAP_MACORIP_ISEXIST				1
#define DHCPD_STATICAP_MAC_IP_DISMATCH				2
#define DHCPD_PERMNTAP_IP_NOT_EXIST					3
#define DHCPD_MAC_ILLEGAL							4
#define DHCPD_IPADDR_ILLEGAL						5

class CDhcpd {
  public:
	SINGLETON(CDhcpd);
	~CDhcpd();
  private:
	CDhcpd();

  public:
	IDhcpServer		*CreateServer();
	void			 DeleteServer(IDhcpServer *pDhcpServer);
};

#define g_pDhcpd		CDhcpd::GetInstancePtr()

#endif
