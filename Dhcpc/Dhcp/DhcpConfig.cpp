#include "StdAfx.h"
#include "DhcpConfig.h"

using namespace std;

CDhcpConfig::CDhcpConfig()
{
}

CDhcpConfig::~CDhcpConfig()
{
}

bool CDhcpConfig::LoadConfig(wstring szConfigFile)
{
	FILE *fp = _wfopen(szConfigFile.c_str(), L"r");
	if (fp == NULL)
		return false;

	TCHAR wcsBuff[1024 + 1];
	while (fgetws(wcsBuff, 1024, fp))
	{
		vector<wstring> vecRet;
		int nItem = g_pZLexer->ParseLine(wcsBuff, vecRet, L'=');
		if (nItem != 2)
			continue;

		int nPos = 0;
		if (vecRet[0].compare(L"MAC") == 0)
		{
			g_pZLexer->GetMACAddr(&m_byMAC[0], vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"ClientIP") == 0)
		{
			g_pZLexer->GetIP4Addr(&m_dwClientIP, vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"SubnetMask") == 0)
		{
			g_pZLexer->GetIP4Addr(&m_dwSubnetMask, vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"AgentIP") == 0)
		{
			g_pZLexer->GetIP4Addr(&m_dwAgentIP, vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"RentMoment") == 0)
		{
			g_pZLexer->GetMoment(&m_dwRentMoment, vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"IPLeaseTime") == 0)
		{
			g_pZLexer->GetNumber(&m_dwIPLeaseTime, vecRet[1].c_str(), nPos);
			continue;
		}
	}
	fclose(fp);
	return true;
}
