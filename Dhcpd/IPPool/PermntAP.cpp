#include "StdAfx.h"
#include "PermntAP.h"

CPermntAP::CPermntAP()
{
}

CPermntAP::~CPermntAP()
{
}

bool CPermntAP::Initial()
{
	return true;
}

bool CPermntAP::Release()
{
	return true;
}

bool CPermntAP::IsExist(dword dwAddr)
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (iter->dwAddr == dwAddr)
			return true;
	}
	return false;
}

dword CPermntAP::AddAddr(dword dwAddr)
{
	if (IsExist(dwAddr))
		return DHCPD_PERMNTAP_IP_ISEXIST;

	TCouple tCouple;
	memset(tCouple.byMAC, '0', 6);
	tCouple.dwAddr = dwAddr;
	tCouple.bUsing = false;
	tCouple.bLocked = false;
	m_vecCouple.push_back(tCouple);
	return DHCPD_OK;
}

dword CPermntAP::DelAddr(dword dwAddr)
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (iter->dwAddr == dwAddr)
		{
			m_vecCouple.erase(iter);
			return DHCPD_OK;
		}
	}
	return DHCPD_PERMNTAP_IP_NOT_EXIST;
}

dword CPermntAP::GetBindedAddr(byte byMAC[6])
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (iter->bUsed && memcmp(iter->byMAC, byMAC, 6) == 0)
			return iter->dwAddr;
	}
	return INVALID_ADDR;
}

bool CPermntAP::IsConflict(byte byMAC[6], dword dwAddr)
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (memcmp(iter->byMAC, "000000", 6) == 0) //尚未绑定必然不会冲突
			continue;
		if (memcmp(iter->byMAC, byMAC, 6) == 0 && iter->dwAddr != dwAddr)
			return true;
		if (memcmp(iter->byMAC, byMAC, 6) != 0 && iter->dwAddr == dwAddr)
			return true;
	}
	return false;
}

dword CPermntAP::SearchAddr(byte byMAC[6])
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (memcmp(iter->byMAC, byMAC, 6) == 0) //分配绑定的IP地址
			return iter->dwAddr;
	}
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (memcmp(iter->byMAC, "000000", 6) == 0) //分配空闲的IP地址
			return iter->dwAddr;
	}
	return INVALID_IP; //没有资源
}

bool CPermntAP::AcceptAddr(byte byMAC[6], dword dwAddr)
{
}

bool CPermntAP::ReturnAddr(byte byMAC[6], dword dwAddr)
{
}
