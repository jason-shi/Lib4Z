#include "StdAfx.h"
#include "ZXml.h"

using namespace std;

int CZXml::ReadBString(string &szString, byte *pBuff)
{
	byte *pTemp = pBuff;

	while (*pTemp == '_' || isalnum(*pTemp)) pTemp++;
	int nLength = pTemp - pBuff;

	szString.clear();
	for (int i = 0; i < nLength; i++)
		szString.push_back(*pBuff++);

	return nLength;
}

int CZXml::ReadXString(string &szString, byte *pBuff)
{
	byte *pTemp = pBuff;

	while (isxdigit(*pTemp)) pTemp++;
	int nLength = pTemp - pBuff;
		;;;;if ((nLength & 1) == 1) return -1;

	szString.clear();
	for (int i = 0; i < nLength; i += 2)
	{
		byte byValue = HEX_2_DEC(pBuff[i]);
		byValue <<= 4;
		byValue |= HEX_2_DEC(pBuff[i + 1]);

		szString.push_back(byValue);
	}

	return nLength;
}

int CZXml::ReadQString(string &szString, byte *pBuff)
{
	byte *pTemp = pBuff;

	while (*pTemp != '"') pTemp++;
	int nLength = pTemp - pBuff;

	szString.clear();
	for (int i = 0; i < nLength; i++)
		szString.push_back(*pBuff++);

	return nLength;
}

int CZXml::ReadString(string &szString, byte *pBuff)
{
	int nOffset = -1;
	if (m_byStatus == LS_QUOTATION)
	{
		//Ë«ÒýºÅ×Ö·û´®
		nOffset = ReadQString(szString, pBuff + 1);
		nOffset += 1;
			;;;;if (pBuff[nOffset] != '"') return -1;
		nOffset += 1;
	}
	else if (pBuff[0] == 'X')
	{
		if (pBuff[1] == '"')
		{
			//Ê®Áù½øÖÆ×Ö·û´®
			nOffset = ReadXString(szString, pBuff + 2);
			nOffset += 2;
				;;;;if (pBuff[nOffset] != '"') return -1;
			nOffset += 1;
		}
		else
		{
			//µ¥×Ö½Ú×Ö·û´®
			nOffset = ReadBString(szString, pBuff);
		}
	}
	else
	{
		//µ¥×Ö½Ú×Ö·û´®
		nOffset = ReadBString(szString, pBuff);
	}

	m_vecState.push_back(m_byState);
	m_byState = m_byStatus == LS_QUOTATION ? LS_STRING : LS_NAMESTR;

	m_vecString.push_back(szString);
	m_byStatus = m_byStatus == LS_QUOTATION ? LS_STRING : LS_NAMESTR;

	return nOffset;
}
