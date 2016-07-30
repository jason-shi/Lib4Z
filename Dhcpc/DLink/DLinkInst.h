#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DLinkInst_h__
#define __DLinkInst_h__

class CDLinkProc;
class CDLinkInst {
  public:
	CDLinkInst(CDLinkProc *pDLinkProc);
	~CDLinkInst();

  public:
  	void			 SetDLinkInst(TDLinkMsg *pDLinkMsg);
  	void			 GetDLinkInst(TDLinkMsg *pDLinkMsg);
	void			 Transposition();

  public:
	void 			 OnRecvNetPacket(TDLinkMsg *pDLinkMsg);
	void 			 OnRecvDLinkData(TDLinkData *pDLinkData);

  protected:
	char			 DA[6];
	char			 SA[6];
	word			 wProtocal;
	byte			 byVersion;
	byte			 byWhat;
	word			 wServType;
	word			 wLength;
	byte			 byFlag;
	byte			 byOffset;
	byte			 byServival;
	byte			 byUDPTCP;
	dword			 SIP;
	dword			 DIP;
	word			 SPort;
	word			 DPort;

  protected:
	CDLinkProc		*m_pJSelf;
};

#endif

#include "PRIVATE_NAMESPACE_END"
