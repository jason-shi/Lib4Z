#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DLink_h__
#define __DLink_h__

#define EV_ADD_JDEST			(EV_USER + 100)
#define EV_DEL_JDEST			(EV_USER + 101)

class CDLink : public JProcess {
	typedef std::vector<dword>		VECDWORD;
  public:
	CDLink(dword dwProcID, byte byPriority);
	virtual ~CDLink();

  public:
 	 void			 DefProcEntry(byte byState, dword dwEvent, byte *pBuff, int nLength);

  public:
	void			 OnAddJDest(byte *pPacket, int nLength);
	void			 OnDelJDest(byte *pPacket, int nLength);

  public:
	VECDWORD		 m_vecJDest;
};

#endif

#include "PRIVATE_NAMESPACE_END"
