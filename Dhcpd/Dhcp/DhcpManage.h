#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DhcpManage_h__
#define __DhcpManage_h__

class CDhcpInst;
class CDhcpManage {
	typedef std::vector<CDhcpInst *>	VECDHCP;
  public:
	CDhcpManage(CDhcpProc *pDhcpProc);
	~CDhcpManage();

  public:
	void			 AddInst(CDhcpInst *pDhcpInst);
	void			 DelInst(CDhcpInst *pDhcpInst);
	CDhcpInst		*FindInst(dword dwTransID);
	CDhcpInst		*ObtainInst(dword dwTransID);

  protected:
	VECDHCP			 m_vecDhcpInst;
	CDhcpProc		*m_pDhcpProc;
};

#endif

#include "PRIVATE_NAMESPACE_END"
