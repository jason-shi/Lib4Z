#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __IP4Addr_h__
#define __IP4Addr_h__

#define IP4_FORMAT_VALUE		0
#define IP4_FORMAT_COMMA		1
#define IP4_FORMAT_COLON		2

class CIP4Addr {
  public:
	static bool		 GetAddr(dword *pResult, const TCHAR* wcsBuff, int &pos);
	static bool		 GetMACAddr(byte *pResult, const TCHAR* wcsBuff, int &pos);

  public:
	static void		 Format(dword dwAddr, int nFormatter);
	static void		 Format(std::wstring szAddr, int nFormatter);
};

#endif

#include "PRIVATE_NAMESPACE_END"
