#include "StdAfx.h"

int CZXml::ReadBitValue(int &nNumber, byte *pBuff)
{
	byte *pTemp = pBuff;

	while (*pTemp == '0' || *pTemp == '1') pTemp++;
	int nLength = pTemp - pBuff;

	nNumber = 0;
	for (int i = 0; i < nLength - 1; i++)
	{
		nNumber += *pBuff++ - '0';
		nNumber <<= 1;
	}
	nNumber = *pBuff++ - '0';

	return nLength;
}

int CZXml::ReadHexValue(int &nNumber, byte *pBuff)
{
	byte *pTemp = pBuff;

	while (*pTemp && isxdigit(*pTemp)) pTemp++;
	int nLength = pTemp - pBuff;

	nNumber = 0;
	for (int i = 0; i < nLength - 1; i++)
	{
		nNumber += HEX_2_DEC(pBuff[i]);
		nNumber <<= 4;
	}
	nNumber += HEX_2_DEC(pBuff[nLength - 1]);

	return nLength;
}

int CZXml::ReadOctValue(int &nNumber, byte *pBuff)
{
	byte *pTemp = pBuff;

	while (*pTemp >= '0' && *pTemp <= '7') pTemp++;
	int nLength = pTemp - pBuff;

	nNumber = 0;
	for (int i = 0; i < nLength - 1; i++)
	{
		nNumber += pBuff[i] - '0';
		nNumber <<= 3;
	}
	nNumber += pBuff[nLength - 1] - '0';

	return nLength;
}

int CZXml::ReadDecValue(int &nNumber, byte *pBuff)
{
	byte *pTemp = pBuff;

	while (*pTemp && isdigit(*pTemp)) pTemp++;
	int nLength = pTemp - pBuff;

	nNumber = 0;
	for (int i = 0; i < nLength - 1; i++)
	{
		nNumber += pBuff[i] - '0';
		nNumber *= 10;
	}
	nNumber += pBuff[nLength - 1] - '0';

	return nLength;
}

int CZXml::ReadNumber(int &nNumber, byte *pBuff)
{
	int nOffset = -1;
	if (pBuff[0] == '0')
	{
		if (pBuff[1] == '{')
		{
			//二进制数值
			nOffset = ReadBitValue(nNumber, pBuff + 2);
			nOffset += 2;
				;;;;if (pBuff[nOffset] != '}') return -1;
			nOffset += 1;
		}
		else if (pBuff[1] == 'x' || pBuff[1] == 'X')
		{
			//十六进制数值
				;;;;if (!isxdigit(pBuff[2])) return -1;
			nOffset = ReadHexValue(nNumber, pBuff + 2);
			nOffset += 2;
		}
		else if (pBuff[1] >= '0' && pBuff[1] <= '7')
		{
			//八进制数值
			nOffset = ReadOctValue(nNumber, pBuff + 1);
			nOffset += 1;
		}
		else if (!isdigit(pBuff[1]))
		{
			//八进制数值
			nNumber = 0;
			nOffset = 1;
		}
		else //非法的数值形式
		{
			return -1;
		}
	}
	else
	{
		//十进制数值
		nOffset = ReadDecValue(nNumber, pBuff);
	}

	m_vecState.push_back(m_byState);
	m_byState = LS_NUMBER;

	m_vecNumber.push_back(nNumber);
	m_byStatus = LS_NUMBER;

	return nOffset;
}
