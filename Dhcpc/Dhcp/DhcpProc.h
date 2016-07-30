#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DhcpProc_h__
#define __DhcpProc_h__

#define DHCP_INIT_STATE			0
#define DHCP_WORK_STATE			1

class CDhcpInst;
class CDhcpProc : public JProcess {
  public:
	CDhcpProc(dword dwProcID, byte byPriority);
	~CDhcpProc();

  public:
 	void			 ProcEntry(byte byState, dword dwEvent, byte *pBuff, int nLength);

  public:
	void			 OnCreate();
	void			 OnDestroy();
	void			 OnSetJDLink(byte *pPacket, int nLength);

  public:
	void			 OnRecvDLinkData(byte *pPacket, int nLength);

  protected:
	CDhcpInst		*m_pDhcpInst;

  public:
	dword			 m_dwJDLink;
};

class CDhcpProcCreator : public JProcessCreator {
  public:
	CDhcpProcCreator()
	{
	}
	CDhcpProc *CreateJProcess(dword dwProcID, byte byPriority)
	{
		return new CDhcpProc(dwProcID, byPriority);
	}
};

#endif

#include "PRIVATE_NAMESPACE_END"
