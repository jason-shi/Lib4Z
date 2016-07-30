#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DhcpServer_h__
#define __DhcpServer_h__

#include "IDhcpServer.h"

#define PID_DLINK				0
#define PID_DHCP				1

class CDhcpServer : public IDhcpServer {
  public:
	CDhcpServer();
	virtual ~CDhcpServer();

  public:
	bool			 Initial(dword dwServerID, dword dwProcBase);
	void			 Run();

  public:
	dword			 GetJDLink() const;
	void			 AddJDest(dword dwJDest);
	void			 DelJDest(dword dwJDest);

  protected:
	dword			 m_dwServerID;
	dword			 m_dwProcBase;

  protected:
	dword			 m_dwJDLink;
	dword			 m_dwJDhcp;
};

#endif

#include "PRIVATE_NAMESPACE_END"
