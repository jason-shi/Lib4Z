#include "StdAfx.h"
#include "ZLexer.h"
#include "IP4Addr.h"
#include "IP6Addr.h"
#include "MACAddr.h"
#include "Number.h"
#include "DateTime.h"

using namespace std;

CZLexer::CZLexer()
{
}

CZLexer::~CZLexer()
{
}

int CZLexer::ParseLine(wstring strLine, std::vector<wstring>& vecReturn, const TCHAR cTrim)
{
	wstring::size_type pos1, pos2;

	pos2 = 0;
	while (pos2 != wstring::npos)
	{
		pos1 = strLine.find_first_not_of(cTrim, pos2);
		if (pos1 == wstring::npos)
			break;
		pos2 = strLine.find_first_of(cTrim, pos1 + 1);
		if (pos2 == wstring::npos)
		{
			if (pos1 != strLine.size())
			{
				if (strLine[(int)strLine.size() - 1] == L'\n')
					vecReturn.push_back(strLine.substr(pos1, (int)strLine.size() - pos1 - 1));
				else
					vecReturn.push_back(strLine.substr(pos1, (int)strLine.size() - pos1));
			}
			break;
		}
		vecReturn.push_back(strLine.substr(pos1, pos2 - pos1));
	}
	return (int)vecReturn.size();
}

int CZLexer::GetNumber(TCHAR *wcsBuff, int &pos, const TCHAR cTrim)
{
	int number = 0;
	while(wcsBuff[pos++] != cTrim && wcsBuff[pos] != L'\0')
	{
		number *= 10;
		number += wcsBuff[pos - 1] - L'0';
	}
	return number;
}

bool CZLexer::GetNumber(dword *pResult, const TCHAR* wcsBuff, int &pos)
{
	return CNumber::GetNumber(pResult, wcsBuff, pos);
}

bool CZLexer::GetIP4Addr(dword *pResult, const TCHAR* wcsBuff, int &pos)
{
	return CIP4Addr::GetAddr(pResult, wcsBuff, pos);
}

bool CZLexer::GetIP6Addr(dword *pResult, const TCHAR* wcsBuff, int &pos)
{
	return false;
}

bool CZLexer::GetMACAddr(byte *pResult, const TCHAR* wcsBuff, int &pos)
{
	return CMACAddr::GetAddr(pResult, wcsBuff, pos);
}

bool CZLexer::GetDecimal(double *pResult, const TCHAR* wcsBuff, int &pos)
{
	return false;
}

bool CZLexer::GetMoment(dword *pResult, const TCHAR* wcsBuff, int &pos)
{
	return CDateTime::GetMoment(pResult, wcsBuff, pos);
}

wstring CZLexer::ReplaceLString(wstring szInput, wstring szOld, wstring szNew)
{
	int nIndex = szInput.find(szOld);
	if (nIndex == -1)
		return szInput;

	wstring szOutput = szInput.substr(0, nIndex);
	szOutput += szNew;

	szOutput += szInput.substr(nIndex + szOld.size());

	return szOutput;
}

wstring CZLexer::ReplaceRString(wstring szInput, wstring szOld, wstring szNew)
{
	int nIndex = szInput.rfind(szOld);
	if (nIndex == -1)
		return szInput;

	wstring szOutput = szInput.substr(0, nIndex);
	szOutput += szNew;

	szOutput += szInput.substr(nIndex + szOld.size());

	return szOutput;
}

TCHAR *CZLexer::TrimLString(const TCHAR* wcsBuff)
{
	if (wcsBuff == NULL)
		return NULL;

	TCHAR *pTemp = (TCHAR *)wcsBuff;
	while (iswspace(*pTemp)) pTemp++;

	return pTemp;
}

TCHAR *CZLexer::TrimRString(const TCHAR* wcsBuff)
{
	if (wcsBuff == NULL)
		return NULL;

	TCHAR *pTemp = (TCHAR *)wcsBuff;
	while (*pTemp != L'\0') pTemp++;
	pTemp--;
	while (iswspace(*pTemp)) pTemp--;
	*(pTemp + 1) = L'\0';

	return (TCHAR *)wcsBuff;
}

TCHAR *CZLexer::TrimString(const TCHAR* wcsBuff)
{
	if (wcsBuff == NULL)
		return NULL;

	return TrimRString(TrimLString(wcsBuff));
}

bool CZLexer::GetAttribute(TCHAR *wcsBuff, wstring attribute, int& nReturn)
{
	vector<wstring> vecReturn;

	int nItem = ParseLine(wcsBuff, vecReturn, L'=');
	if (nItem == 2 && vecReturn[0].compare(attribute) == 0)
	{
		nReturn = _wtoi(vecReturn[1].c_str());
		return true;
	}
	return false;
}

bool CZLexer::GetAttributeStr(TCHAR *wcsBuff, wstring attribute, wstring& wcsReturn)
{
	vector<wstring> vecReturn;

	int nItem = ParseLine(wcsBuff, vecReturn, L'=');
	if (nItem == 2 && vecReturn[0].compare(attribute) == 0)
	{
		wcsReturn = vecReturn[1];
		return true;
	}
	return false;
}
