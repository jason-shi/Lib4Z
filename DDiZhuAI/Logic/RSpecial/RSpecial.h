#ifndef __RSpecial_h__
#define __RSpecial_h__

#include "DDiZhuAI.h"

class CRSpecial {
  public:
	SINGLETON(CRSpecial);
	~CRSpecial();
  private:
	CRSpecial();

  public:
	void			 CallScore(TScoreParam *pScoreParam, TScoreResult *pScoreResult);
};

#define g_pRSpecial		CRSpecial::GetInstancePtr()

#endif
