#include "StdAfx.h"
#include "IP4Set.h"

using namespace std;

CIP4Set::CIP4Set()
{
}

CIP4Set::CIP4Set(byte byMAC[6], dword dwAddr, dword dwMask, dword dwGate, dword dwPeriod, dword dwElapse)
{
	memcpy(this->byMAC, byMAC, 6);
	this->dwAddr = dwAddr;
	this->dwMask = dwMask;
	this->dwGate = dwGate;
	this->dwPeriod = dwPeriod;
	this->dwElapse = dwElapse;
}
