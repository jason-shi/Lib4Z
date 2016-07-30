#ifndef __RPlayerDJ_h__
#define __RPlayerDJ_h__

#include "..\RPlayer.h"

class CRPlayerDJ : public CRPlayer {
  public:
	SINGLETON(CRPlayerDJ);
	~CRPlayerDJ();

  private:
	CRPlayerDJ();
};

#define g_pRPlayerDJ			CRPlayerDJ::GetInstancePtr()

#endif
