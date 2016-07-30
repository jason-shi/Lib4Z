#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DhcpClient_h__
#define __DhcpClient_h__

#include "IDhcpClient.h"

#define PID_DLINK				0
#define PID_DHCP				1

class CDhcpClient : public IDhcpClient {
  public:
	CDhcpClient();
	virtual ~CDhcpClient();

  public:
	bool			 Initial(dword dwClientID, dword dwProcBase);
	void			 Run();

  public:
	dword			 GetJDLink() const;
	void			 AddJDest(dword dwJDest);
	void			 DelJDest(dword dwJDest);

  protected:
	dword			 m_dwClientID;
	dword			 m_dwProcBase;

  protected:
	dword			 m_dwJDLink;
	dword			 m_dwJDhcp;
};

#endif

#include "PRIVATE_NAMESPACE_END"
