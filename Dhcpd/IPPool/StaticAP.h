#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __StaticAP_h__
#define __StaticAP_h__

struct TCouple {
	byte			 byMAC[6];
	dword			 dwAddr;
	bool			 bUsing;
	bool			 bLocked;
};

class CStaticAP {
	typedef std::vector<TCouple>		VECCOUPLE;
  public:
	CStaticAP();
	~CStaticAP();

  public:
	bool			 Initial();
	bool			 Release();

  public:
	bool			 IsExist(byte byMAC[6], dword dwAddr);
	dword			 AddAddr(byte byMAC[6], dword dwAddr);
	dword			 DelAddr(byte byMAC[6], dword dwAddr);

  public:
	dword			 GetBindedAddr(byte byMAC[6]);
	bool			 IsConflict(byte byMAC[6], dword dwAddr);

  public:
	dword			 SearchAddr(byte byMAC[6]);               //���ҿ��õĵ�ַ��Unlocked->Locked, Unused
	bool			 AcceptAddr(byte byMAC[6], dword dwAddr); //���ܵ�ַ Locked->Unlocked, Unused->Used
	bool			 ReturnAddr(byte byMAC[6], dword dwAddr); //�黹��ַ Used->Unused

  protected:
	VECCOUPLE		 m_vecCouple;
};

#endif

#include "PRIVATE_NAMESPACE_END"
