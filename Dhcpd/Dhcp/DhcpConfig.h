#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DhcpConfig_h__
#define __DhcpConfig_h__

class CDhcpServer;
class CDhcpConfig {
  public:
	CDhcpConfig();
  	~CDhcpConfig();

  public:
	bool			 LoadConfig(std::wstring szConfigFile);
	void			 ParseIP4Except(const TCHAR* wcsExcept);

  protected:
	CDhcpServer		*m_pDhcpServer;
};

#endif

#include "PRIVATE_NAMESPACE_END"
