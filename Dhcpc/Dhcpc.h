#ifndef __Dhcpc_h__
#define __Dhcpc_h__

#include "IDhcpClient.h"

class CDhcpc {
  public:
	SINGLETON(CDhcpc);
	~CDhcpc();
  private:
	CDhcpc();

  public:
	IDhcpClient		*CreateClient();
	void			 DeleteClient(IDhcpClient *pDhcpClient);
};

#define g_pDhcpc		CDhcpc::GetInstancePtr()

#endif
