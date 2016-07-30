#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __MACAddr_h__
#define __MACAddr_h__

class CMACAddr {
  public:
	static bool		 GetAddr(byte *pResult, const TCHAR* wcsBuff, int &pos);
};

#endif

#include "PRIVATE_NAMESPACE_END"
