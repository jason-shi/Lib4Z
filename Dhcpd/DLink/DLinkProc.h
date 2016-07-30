#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DLinkProc_h__
#define __DLinkProc_h__

#define DLINK_INIT_STATE		0
#define DLINK_WORK_STATE		1
#define DLINK_QUIT_STATE		2

class CDLinkInst;
class CDLinkProc : public JProcess {
  public:
	CDLinkProc(dword dwProcID, byte byPriority);
	~CDLinkProc();

  public:
 	void			 ProcEntry(byte byState, dword dwEvent, byte *pBuff, int nLength);

  public:
	void			 OnCreate();
	void			 OnDestroy();

  public:
	void			 OnSetJDhcp(byte *pPacket, int nLength);
	void			 OnAddJDest(byte *pPacket, int nLength);
	void			 OnDelJDest(byte *pPacket, int nLength);

  protected:
	void			 OnRecvNetPacket(byte *pPacket, int nLength);
	void			 OnRecvDLinkData(byte *pPacket, int nLength);

  protected:
	CDLinkInst		*m_pDLinkInst;

  public:
	dword			 m_dwJDhcp;
	dword			 m_dwJDest;
	VECDWORD		 m_vecJDest;
};

class CDLinkProcCreator : public JProcessCreator {
  public:
	CDLinkProcCreator()
	{
	}
	CDLinkProc *CreateJProcess(dword dwProcID, byte byPriority)
	{
		return new CDLinkProc(dwProcID, byPriority);
	}
};

#endif

#include "PRIVATE_NAMESPACE_END"
