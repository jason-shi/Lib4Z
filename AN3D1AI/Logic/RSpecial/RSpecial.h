#ifndef __RSpecial_h__
#define __RSpecial_h__

#include "AN3D1AI.h"

class CRSpecial {
  public:
	SINGLETON(CRSpecial);
	~CRSpecial();
  private:
	CRSpecial();

  public:
	void			 CallScore(TScoreParam *pScoreParam, TScoreResult *pScoreResult);
	void			 CallTrump(TTrumpParam *pTrumpParam, TTrumpResult *pTrumpResult);
	void			 CallMieDi(TMieDiParam *pMieDiParam, TMieDiResult *pMieDiResult);
};

#define g_pRSpecial		CRSpecial::GetInstancePtr()

#endif
