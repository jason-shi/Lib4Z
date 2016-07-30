#ifndef __RSpecial_h__
#define __RSpecial_h__

#include "RocketAI.h"

class CRSpecial {
  public:
	SINGLETON(CRSpecial);
	~CRSpecial();
  private:
	CRSpecial();

  public:
	void			 CallLiGun(TLiGunParam *pLiGunParam, TLiGunResult *pLiGunResult);
	void			 CallJueGun(TJueGunParam *pJueGunParam, TJueGunResult *pJueGunResult);
};

#define g_pRSpecial		CRSpecial::GetInstancePtr()

#endif
