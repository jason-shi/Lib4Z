#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __PermntAP_h__
#define __PermntAP_h__

struct TCouple {
	byte			 byMAC[6];
	dword			 dwAddr;
	bool			 bUsing;
	bool			 bLocked;
};

class CPermntAP {
	typedef std::vector<TCouple>		VECCOUPLE;
  public:
	CPermntAP();
	~CPermntAP();

  public:
	bool			 Initial();
	bool			 Release();

  public:
	bool			 IsExist(dword dwAddr);
	dword			 AddAddr(dword dwAddr);
	dword			 DelAddr(dword dwAddr);

  public:
	dword			 GetBindedAddr(byte byMAC[6]);
	bool			 IsConflict(byte byMAC[6], dword dwAddr);

  public:
	dword			 SearchAddr(byte byMAC[6]);               //查找可用的地址，Unlocked->Locked, Unused
	bool			 AcceptAddr(byte byMAC[6], dword dwAddr); //接受地址 Locked->Unlocked, Unused->Used
	bool			 ReturnAddr(byte byMAC[6], dword dwAddr); //归还地址 Used->Unused

  protected:
	VECCOUPLE		 m_vecCouple;
};

#endif

#include "PRIVATE_NAMESPACE_END"
