#include "StdAfx.h"
#include "Number.h"

CNumber::CNumber()
{
}

CNumber::~CNumber()
{
}

bool CNumber::GetBinValue(dword *pResult, const TCHAR *wcsBuff, int &pos)
{
	const TCHAR *pTemp = wcsBuff + pos;

	while (*pTemp >= L'0' && *pTemp <= L'1') pTemp++;
	if (pTemp == wcsBuff)
		return false;

	*pResult = 0;
	while (pos < pTemp - wcsBuff)
	{
		*pResult <<= 1;
		*pResult += wcsBuff[pos++] - L'0';
	}
	return true;
}

bool CNumber::GetHexValue(dword *pResult, const TCHAR *wcsBuff, int &pos)
{
	const TCHAR *pTemp = wcsBuff + pos;

	while (*pTemp && iswxdigit(*pTemp)) pTemp++;
	if (pTemp == wcsBuff)
		return false;

	*pResult = 0;
	while (pos < pTemp - wcsBuff)
	{
		*pResult <<= 4;
		*pResult += HEX_2_DEC(wcsBuff[pos]);

		pos++;
	}
	return true;
}

bool CNumber::GetOctValue(dword *pResult, const TCHAR *wcsBuff, int &pos)
{
	const TCHAR *pTemp = wcsBuff + pos;

	while (*pTemp >= L'0' && *pTemp <= L'7') pTemp++;
	if (pTemp == wcsBuff)
		return false;

	*pResult = 0;
	while (pos < pTemp - wcsBuff)
	{
		*pResult <<= 3;
		*pResult += wcsBuff[pos++] - L'0';
	}
	return true;
}

bool CNumber::GetDecValue(dword *pResult, const TCHAR *wcsBuff, int &pos)
{
	const TCHAR *pTemp = wcsBuff + pos;

	while (*pTemp && iswdigit(*pTemp)) pTemp++;
	if (pTemp == wcsBuff)
		return false;

	*pResult = 0;
	while (pos < pTemp - wcsBuff)
	{
		*pResult *= 10;
		*pResult += wcsBuff[pos++] - L'0';
	}

	pos += pTemp - wcsBuff;
	return true;
}

bool CNumber::GetNumber(dword *pResult, const TCHAR* wcsBuff, int &pos)
{
	*pResult = 0;
	if (wcsBuff[pos] == L'0')
	{
		if (wcsBuff[pos + 1] == L'x' || wcsBuff[pos + 1] == L'X')
		{
			//十六进制数值
			pos += 2;
			return GetHexValue(pResult, wcsBuff, pos);
		}
		else if (wcsBuff[pos + 1] >= L'0' && wcsBuff[pos + 1] <= L'7')
		{
			//八进制数值
			pos += 1;
			return GetOctValue(pResult, wcsBuff, pos);
		}
		else if (!iswalnum(wcsBuff[1]) && wcsBuff[1] != L'_')
		{
			*pResult = 0;
			pos += 1;
		}
		else //非法的数值形式
		{
			return false;
		}
	}
	else if (iswdigit(wcsBuff[pos]))
	{
		return GetDecValue(pResult, wcsBuff, pos);
	}
	else
	{
		return false;
	}
	return true;
}
