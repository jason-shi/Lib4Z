#ifndef __RPlayerXJ_h__
#define __RPlayerXJ_h__

#include "..\RPlayer.h"

class CRPlayerXJ : public CRPlayer {
  public:
	SINGLETON(CRPlayerXJ);
	~CRPlayerXJ();

  private:
	CRPlayerXJ();
};

#define g_pRPlayerXJ			CRPlayerXJ::GetInstancePtr()

#endif
