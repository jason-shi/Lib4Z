#include "StdAfx.h"
#include "DateTime.h"

bool CDateTime::GetDate(dword *pResult, const TCHAR* wcsBuff, int &pos)
{
	//2010.07.03
	if (wcslen(wcsBuff) < 11)
		return false;
	if (wcsBuff[4] != L'.' || wcsBuff[7] != L'.' || wcsBuff[10] != L'.')
		return false;

	word wSect[3];
	wSect[0] = (wcsBuff[0] - L'0') * 1000 + (wcsBuff[1] - L'0') * 100
			 + (wcsBuff[2] - L'0') * 10   + (wcsBuff[3] - L'0');
	wSect[1] = (wcsBuff[5] - L'0') * 10   + (wcsBuff[6] - L'0');
	wSect[2] = (wcsBuff[8] - L'0') * 10   + (wcsBuff[9] - L'0');

	//ÈÕÆÚ¼ì²â
	if (wSect[1] < 1 || wSect[1] > 12)
		return false;
	if (wSect[2] > 31)
		return false;

	*pResult = wSect[0] * 365 + wSect[1] * 31 + wSect[2];
	pos += 11;
	return true;
}

bool CDateTime::GetTime(dword *pResult, const TCHAR* wcsBuff, int &pos)
{
	//HH:MM:SS
	if (wcslen(wcsBuff) < 8)
		return false;
	if (wcsBuff[2] != L':' || wcsBuff[5] != L':')
		return false;

	byte bySect[3];
	for (int i = 0, j = 0; i < 3; i++, j += 3)
		bySect[i] = (wcsBuff[j] - L'0') * 10 + (wcsBuff[j + 1] - L'0');

	if (bySect[0] >= 24 || bySect[1] >= 60 || bySect[2] >= 60)
		return false;

	*pResult = bySect[0] * 3600 + bySect[1] * 60 + bySect[2];
	pos += 8;
	return true;
}

bool CDateTime::GetMoment(dword *pResult, const TCHAR* wcsBuff, int &pos)
{
	dword dwDate;
	if (!GetDate(&dwDate, wcsBuff, pos))
		return false;

	dword dwTime;
	if (!GetTime(&dwTime, wcsBuff, pos))
		return false;

	*pResult = dwDate * 24 * 3600 + dwTime;
	return true;
}
