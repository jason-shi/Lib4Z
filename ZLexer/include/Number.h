#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Decimal_h__
#define __Decimal_h__

class CNumber {
  public:
	CNumber();
	~CNumber();

  public:
	static bool		 GetBinValue(dword *pResult, const TCHAR *wcsBuff, int &pos);
	static bool		 GetHexValue(dword *pResult, const TCHAR *wcsBuff, int &pos);
	static bool		 GetOctValue(dword *pResult, const TCHAR *wcsBuff, int &pos);
	static bool		 GetDecValue(dword *pResult, const TCHAR *wcsBuff, int &pos);
	static bool		 GetNumber(dword *pResult, const TCHAR *wcsBuff, int &pos);
};

#endif

#include "PRIVATE_NAMESPACE_END"
