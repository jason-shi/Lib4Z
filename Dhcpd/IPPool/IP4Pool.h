#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __IP4Pool_h__
#define __IP4Pool_h__

class CIP4Set;
class CIP4Pool {
	typedef std::vector<CIP4Set *>		VECIP4SET;
  public:
	CIP4Pool();
	~CIP4Pool();

  public:
	bool			 Initial();

  public:
	CIP4Set			*Alloc(byte byMAC[6]);
	CIP4Set			*Alloc(byte byMAC[6], dword dwAddr);
	bool			 Release(byte byMAC[6]);

  protected:
	CIP4Set 		*FindIP4Set(VECIP4SET& vecIP4Set, byte byMAC[6]);
	CIP4Set 		*FindIP4Set(VECIP4SET& vecIP4Set, dword dwAddr);
	dword			 FindFreeAddr4(VECIP4SET& vecIP4Set, dword dwAddr, dword dwMask);
	void			 InsertIP4Set(VECIP4SET& vecIP4Set, CIP4Set *pIP4Set);
	void			 RemoveIP4Set(VECIP4SET& vecIP4Set, CIP4Set *pIP4Set);
	void			 ModifyIP4Set(VECIP4SET& vecIP4Set, CIP4Set *pIP4Set);

  protected:
	dword			 m_dwBegin;
	dword			 m_dwEnd;
	dword			 m_dwMask;
	dword			 m_dwGate;
	dword			 m_dwPeriod;

  protected:
	VECIP4SET		 m_vecBinding;
	VECIP4SET		 m_vecBinded;
	VECIP4SET		 m_vecRecord;

  protected:
	VECIP4SET		 m_vecExcept;
};

#endif

#include "PRIVATE_NAMESPACE_END"
