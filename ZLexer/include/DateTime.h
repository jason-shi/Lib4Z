#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DateTime_h__
#define __DateTime_h__

class CDateTime {
  public:
	static bool		 GetDate(dword *pResult, const TCHAR* wcsBuff, int &pos);
	static bool		 GetTime(dword *pResult, const TCHAR* wcsBuff, int &pos);
	static bool		 GetMoment(dword *pResult, const TCHAR* wcsBuff, int &pos);
};

#endif

#include "PRIVATE_NAMESPACE_END"
