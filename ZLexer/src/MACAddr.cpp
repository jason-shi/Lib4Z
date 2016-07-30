#include "StdAfx.h"
#include "MACAddr.h"

bool CMACAddr::GetAddr(byte *pResult, const TCHAR* wcsBuff, int &pos)
{
	//AA:BB:CC:DD:EE:FF
	if (wcslen(wcsBuff) < 17)
		return false;

	for (int i = 0, j = 0; i < 6; i++, j += 3)
	{
		pResult[i] = (HEX_2_DEC(wcsBuff[j]) << 4) + HEX_2_DEC(wcsBuff[j + 1]);
		if (i != 6)
		{
			if (wcsBuff[j + 2] != L':')
				return false;
		}
	}
	return true;
}
