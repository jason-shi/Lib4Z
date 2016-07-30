#ifndef __RPlayerSJ_h__
#define __RPlayerSJ_h__

#include "..\RPlayer.h"

class CRPlayerSJ : public CRPlayer {
  public:
	SINGLETON(CRPlayerSJ);
	~CRPlayerSJ();

  private:
	CRPlayerSJ();
};

#define g_pRPlayerSJ			CRPlayerSJ::GetInstancePtr()

#endif
