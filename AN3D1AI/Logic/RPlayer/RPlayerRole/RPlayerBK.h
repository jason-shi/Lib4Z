#ifndef __RPlayerBK_h__
#define __RPlayerBK_h__

#include "..\RPlayer.h"

class CRPlayerBK : public CRPlayer {
  public:
	SINGLETON(CRPlayerBK);
	~CRPlayerBK();

  private:
	CRPlayerBK();
};

#define g_pRPlayerBK			CRPlayerBK::GetInstancePtr()

#endif
