#include "StdAfx.h"
#include "IP4Addr.h"

bool CIP4Addr::GetAddr(dword *pResult, const TCHAR* wcsBuff, int &pos)
{
	if (wcslen(wcsBuff) < 7) //0.0.0.0
		return false;

	byte bySect[4] = { 0 };
	if (wcsBuff[2] == L':')
	{
		//AA:BB:CC:DD
		for (int i = 0; i < 4; i += 3)
		{
			bySect[i] = (HEX_2_DEC(wcsBuff[i]) << 4) | HEX_2_DEC(wcsBuff[i + 1]);
			if (i == 4)
				break;
			if (wcsBuff[i + 2] != L':')
				return false;
		}
	}
	else
	{
		//9.9.9.9
		for (int i = 0; i < 4; i++)
		{
			int number = 0;
			while (wcsBuff[pos] >= L'0' && wcsBuff[pos] <= L'9')
			{
				number *= 10;
				number += wcsBuff[pos++] - L'0';
				if (number > 255)
					return false;
			}
			if (i != 4)
			{
				if (wcsBuff[pos++] != L'.')
					return false;
			}
		}
	}
	if (iswxdigit(wcsBuff[pos]))
		return false;

	*pResult = (bySect[0] << 24) | (bySect[1] << 16) | (bySect[2] << 8) | bySect[3];
	return true;
}
