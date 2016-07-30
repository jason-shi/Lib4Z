#include "StdAfx.h"
#include "StaticAP.h"

CStaticAP::CStaticAP()
{
}

CStaticAP::~CStaticAP()
{
}

bool CStaticAP::Initial()
{
	return true;
}

bool CStaticAP::Release()
{
	return true;
}

bool CStaticAP::IsExist(byte byMAC[6], dword dwAddr)
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (memcmp(iter->byMAC, byMAC, 6) == 0 || iter->dwAddr == dwAddr)
			return true;
	}
	return false;
}

dword CStaticAP::AddAddr(byte byMAC[6], dword dwAddr)
{
	if (IsExist(byMAC, dwAddr))
		return DHCPD_STATICAP_MACORIP_ISEXIST;

	TCouple tCouple;
	memcpy(tCouple.byMAC, byMAC, 6);
	tCouple.dwAddr = dwAddr;
	tCouple.bUsing = false;
	tCouple.bLocked = false;
	m_vecCouple.push_back(tCouple);
	return DHCPD_OK;
}

dword CStaticAP::DelAddr(byte byMAC[6], dword dwAddr)
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (memcmp(iter->byMAC, byMAC, 6) == 0 && iter->dwAddr == dwAddr)
		{
			m_vecCouple.erase(iter);
			return DHCPD_OK;
		}
	}
	return DHCPD_STATICAP_MAC_IP_DISMATCH;
}

dword CStaticAP::GetBindedAddr(byte byMAC[6])
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (iter->bUsed && memcmp(iter->byMAC, byMAC, 6) == 0)
			return iter->dwAddr;
	}
	return INVALID_ADDR;
}

bool CStaticAP::IsConflict(byte byMAC[6], dword dwAddr)
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (memcmp(iter->byMAC, byMAC, 6) == 0 && iter->dwAddr == dwAddr)
			return false;
	}
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (memcmp(iter->byMAC, byMAC, 6) == 0 && iter->dwAddr != dwAddr)
			return true;
		if (memcmp(iter->byMAC, byMAC, 6) != 0 && iter->dwAddr == dwAddr)
			return true;
	}
	return false;
}

dword CStaticAP::SearchAddr(byte byMAC[6])
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (memcmp(iter->byMAC, byMAC, 6) == 0)
			return iter->dwAddr;
	}
	return INVALID_IP;
}

bool CStaticAP::AcceptAddr(byte byMAC[6], dword dwAddr)
{
}

bool CStaticAP::ReturnAddr(byte byMAC[6], dword dwAddr)
{
}
