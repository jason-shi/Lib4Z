#include "StdAfx.h"
#include "IP6Set.h"

using namespace std;

IP6Set::IP6Set(byte byMAC[6], word wAddr[8], dword dwPeriod, dword dwElapse)
{
	memcpy(this->byMAC, byMAC, 6);
	memcpy(this->wAddr, wAddr, 128);
	this->dwPeriod = dwPeriod;
	this->dwElapse = dwElapse;
}

IP6Set *FindIP6Set(VECIP6SET& vecIP6Set, byte byMAC[6])
{
	VECIP6SET::iterator iter;
	for (iter = vecIP6Set.begin(); iter != vecIP6Set.end(); iter++)
	{
		if (memcmp((*iter)->byMAC, byMAC, 6))
			return *iter;
	}

	return NULL;
}

IP6Set *FindIP6Set(VECIP6SET& vecIP6Set, word wAddr[8])
{
	VECIP6SET::iterator iter;
	for (iter = vecIP6Set.begin(); iter != vecIP6Set.end(); iter++)
	{
		if (memcmp((*iter)->wAddr, wAddr, 128))
			return *iter;
	}

	return NULL;
}

void InsertIP6Set(VECIP6SET& vecIP6Set, IP6Set *pIP6Set)
{
	VECIP6SET::iterator iter, iterN;
	for (iter = vecIP6Set.begin(); iter != vecIP6Set.end(); iter++)
	{
		iterN = iter + 1;
		if (memcmp((*iterN)->wAddr, pIP6Set->wAddr, 128))
			break;
	}
	vecIP6Set.insert(iter, pIP6Set);
}

void RemoveIP6Set(VECIP6SET& vecIP6Set, IP6Set *pIP6Set)
{
	VECIP6SET::iterator iter;
	for (iter = vecIP6Set.begin(); iter != vecIP6Set.end(); iter++)
	{
		if (*iter == pIP6Set)
		{
			vecIP6Set.erase(iter);
			break;
		}
	}
}

void ModifyIP6Set(VECIP6SET& vecIP6Set, IP6Set *pIP6Set)
{
	VECIP6SET::iterator iter;
	for (iter = vecIP6Set.begin(); iter != vecIP6Set.end(); iter++)
	{
		if (memcmp((*iter)->byMAC, pIP6Set->byMAC, 16))
		{
			memcpy((*iter)->wAddr, pIP6Set->wAddr, 128);
			(*iter)->dwPeriod = pIP6Set->dwPeriod;
			(*iter)->dwElapse = pIP6Set->dwElapse;
			break;
		}
	}
}
