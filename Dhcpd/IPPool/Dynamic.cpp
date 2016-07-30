#include "StdAfx.h"
#include "DynamicAP.h"

CDynamicAP::CDynamicAP()
{
}

CDynamicAP::~CDynamicAP()
{
}

bool CDynamicAP::Initial()
{
	return true;
}

bool CDynamicAP::Release()
{
	return true;
}

bool CDynamicAP::IsExist(dword dwAddr)
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (iter->dwAddr == dwAddr)
			return true;
	}
	return false;
}

dword CDynamicAP::AddAddr(dword dwAddr)
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

dword CDynamicAP::DelAddr(dword dwAddr)
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

dword CDynamicAP::GetBindedAddr(byte byMAC[6])
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (iter->bUsed && memcmp(iter->byMAC, byMAC, 6) == 0)
			return iter->dwAddr;
	}
	return INVALID_ADDR;
}

bool CDynamicAP::IsConflict(byte byMAC[6], dword dwAddr)
{
	VECCOUPLE::iterator iter;
	for (iter = m_vecCouple.begin(); iter != m_vecCouple.end(); iter++)
	{
		if (!iter->bUsing) //不在使用必然不会冲突
			continue;
		if (memcmp(iter->byMAC, byMAC, 6) == 0 && iter->dwAddr != dwAddr)
			return true;
		if (memcmp(iter->byMAC, byMAC, 6) != 0 && iter->dwAddr == dwAddr)
			return true;
	}
	return false;
}

dword CDynamicAP::SearchAddr(byte byMAC[6])
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

bool CDynamicAP::AcceptAddr(byte byMAC[6], dword dwAddr)
{
}

bool CDynamicAP::ReturnAddr(byte byMAC[6], dword dwAddr)
{
}
