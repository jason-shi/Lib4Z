#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __IP4Set_h__
#define __IP4Set_h__

class CIP4Set {
  public:
	CIP4Set();
	CIP4Set(byte byMAC[6], dword dwAddr, dword dwMask, dword dwGate, dword dwPeriod, dword dwElapse);

  public:
	byte			 byMAC[6];
	dword			 dwAddr;
	dword			 dwMask;
	dword			 dwGate;
	dword			 dwPeriod;
	dword			 dwElapse;

  public:
};

#endif

#include "PRIVATE_NAMESPACE_END"
