#include "StdAfx.h"
#include "DhcpConfig.h"
#include "DhcpServer.h"

using namespace std;

bool CDhcpConfig::LoadConfig(wstring szConfigFile)
{
#if 0
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
		if (vecRet[0].compare(L"IP4Server") == 0)
		{
			g_pZLexer->GetIP4Addr(&m_pDhcpServer->dwIP4Server, vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"IP4Mask") == 0)
		{
			g_pZLexer->GetIP4Addr(&m_pDhcpServer->dwIP4Mask, vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"IP4Gate") == 0)
		{
			g_pZLexer->GetIP4Addr(&m_pDhcpServer->dwIP4Gate, vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"IP4Begin") == 0)
		{
			g_pZLexer->GetIP4Addr(&m_pDhcpServer->dwIP4Begin, vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"IP4End") == 0)
		{
			g_pZLexer->GetIP4Addr(&m_pDhcpServer->dwIP4End, vecRet[1].c_str(), nPos);
			continue;
		}
		if (vecRet[0].compare(L"IP4Except") == 0)
		{
			ParseIP4Except(vecRet[1].c_str());
			continue;
		}
	}
	fclose(fp);
#endif
	return true;
}

void CDhcpConfig::ParseIP4Except(const TCHAR* wcsExcept)
{
#if 0
	vector<wstring> vecRet;
	dword dwTemp;

	int nItem = g_pZLexer->ParseLine(wcsExcept, vecRet, L',');
	int nPos = 0;
	for (int i = 0; i < nItem; i++)
	{
		if (!g_pZLexer->GetIP4Addr(&dwTemp, vecRet[i].c_str(), nPos))
			return;
		m_pDhcpServer->vecIP4Except.push_back(dwTemp);
	}
#endif
}
