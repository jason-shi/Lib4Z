#include "StdAfx.h"
#include "IP4Pool.h"
#include "IPSet\IP4Set.h"

CIP4Pool::CIP4Pool()
{
}

CIP4Pool::~CIP4Pool()
{
}

bool CIP4Pool::Initial()
{
	//m_dwMask    = g_pZXml->Load("Config.txt", "IP4Mask");
	//m_dwBegin   = g_pZXml->Load("Config.txt", "IP4Begin");
	//m_dwEnd     = g_pZXml->Load("Config.txt", "IP4End");
	//m_vecExcept = g_pZXml->Load("Config.txt", "IP4Except");

	m_vecBinding.clear();
	m_vecBinded.clear();
	m_vecRecord.clear();
	return true;
}

CIP4Set *CIP4Pool::Alloc(byte byMAC[6])
{
	//首先根据MAC查找分配记录
	CIP4Set *pIP4Set = FindIP4Set(m_vecBinding, byMAC);
	if (pIP4Set)
		return pIP4Set;

	pIP4Set = FindIP4Set(m_vecBinded, byMAC);
	if (pIP4Set)
	{
		RemoveIP4Set(m_vecBinded, pIP4Set);
		InsertIP4Set(m_vecBinding, pIP4Set);
		return pIP4Set;
	}

	//分配空闲的地址
	dword dwFreeAddr = FindFreeAddr4(m_vecRecord, m_dwBegin, m_dwMask);
	if (dwFreeAddr != 0)
	{
		pIP4Set = new CIP4Set(byMAC, dwFreeAddr, m_dwMask, m_dwGate, m_dwPeriod, 0);
		InsertIP4Set(m_vecBinding, pIP4Set);
		InsertIP4Set(m_vecRecord, pIP4Set);
		return pIP4Set;
	}

	//支持动态分配
	if (!m_vecBinded.empty())
	{
		pIP4Set = m_vecBinded.back();
		m_vecBinded.pop_back();
		InsertIP4Set(m_vecBinding, pIP4Set);
		return pIP4Set;
	}

	//没有可以分配的地址
	return NULL;
}

CIP4Set *CIP4Pool::Alloc(byte byMAC[6], dword dwAddr)
{
	//首先根据MAC查找分配记录
	CIP4Set *pIP4Set = FindIP4Set(m_vecBinding, byMAC);
	if (pIP4Set)
	{
		if (pIP4Set->dwAddr != dwAddr)
			pIP4Set->dwAddr = dwAddr;
		return pIP4Set;
	}

	pIP4Set = FindIP4Set(m_vecBinded, byMAC);
	if (pIP4Set)
	{
		if (pIP4Set->dwAddr != dwAddr)
			pIP4Set->dwAddr = dwAddr;

		RemoveIP4Set(m_vecBinded, pIP4Set);
		InsertIP4Set(m_vecBinding, pIP4Set);
		return pIP4Set;
	}

	//分配空闲的地址
	dword dwFreeAddr = FindFreeAddr4(m_vecRecord, m_dwBegin, m_dwMask);
	if (dwFreeAddr != 0)
	{
		pIP4Set = new CIP4Set(byMAC, dwFreeAddr, m_dwMask, m_dwGate, m_dwPeriod, 0);
		InsertIP4Set(m_vecBinding, pIP4Set);
		InsertIP4Set(m_vecRecord, pIP4Set);
		return pIP4Set;
	}
	return NULL;
}

bool CIP4Pool::Release(byte byMAC[6])
{
	CIP4Set *pIP4Set = FindIP4Set(m_vecBinding, byMAC);
	if (pIP4Set == NULL)
		return false;

	RemoveIP4Set(m_vecBinding, pIP4Set);
	InsertIP4Set(m_vecBinded, pIP4Set);
	return true;
}

CIP4Set *CIP4Pool::FindIP4Set(VECIP4SET& vecIP4Set, byte byMAC[6])
{
	VECIP4SET::iterator iter;
	for (iter = vecIP4Set.begin(); iter != vecIP4Set.end(); iter++)
	{
		if (memcmp((*iter)->byMAC, byMAC, 6))
			return *iter;
	}

	return NULL;
}

CIP4Set *CIP4Pool::FindIP4Set(VECIP4SET& vecIP4Set, dword dwAddr)
{
	VECIP4SET::iterator iter;
	for (iter = vecIP4Set.begin(); iter != vecIP4Set.end(); iter++)
	{
		if ((*iter)->dwAddr == dwAddr)
			return *iter;
	}

	return NULL;
}

dword CIP4Pool::FindFreeAddr4(VECIP4SET& vecIP4Set, dword dwAddr, dword dwMask)
{
	dword dwStart = dwAddr & dwMask;
	if (vecIP4Set.size() == 0)
		return dwStart + 1;

	if (vecIP4Set[0]->dwAddr != dwStart + 1)
		return dwStart + 1;

	VECIP4SET::iterator iter, iterN;
	for (iter = vecIP4Set.begin(), iterN = iter + 1; iterN != vecIP4Set.end(); iterN++, iter++)
	{
		if ((*iterN)->dwAddr - (*iter)->dwAddr != 1)
			break;
	}
	return (*iter)->dwAddr + 1;
}

void CIP4Pool::InsertIP4Set(VECIP4SET& vecIP4Set, CIP4Set *pIP4Set)
{
	VECIP4SET::iterator iter, iterN;
	for (iter = vecIP4Set.begin(); iter != vecIP4Set.end(); iter++)
	{
		iterN = iter + 1;
		if ((*iterN)->dwAddr > pIP4Set->dwAddr)
			break;
	}
	vecIP4Set.insert(iter, pIP4Set);
}

void CIP4Pool::RemoveIP4Set(VECIP4SET& vecIP4Set, CIP4Set *pIP4Set)
{
	VECIP4SET::iterator iter;
	for (iter = vecIP4Set.begin(); iter != vecIP4Set.end(); iter++)
	{
		if (*iter == pIP4Set)
		{
			vecIP4Set.erase(iter);
			break;
		}
	}
}

void CIP4Pool::ModifyIP4Set(VECIP4SET& vecIP4Set, CIP4Set *pIP4Set)
{
	VECIP4SET::iterator iter;
	for (iter = vecIP4Set.begin(); iter != vecIP4Set.end(); iter++)
	{
		if (memcmp((*iter)->byMAC, pIP4Set->byMAC, 6))
		{
			(*iter)->dwAddr = pIP4Set->dwAddr;
			(*iter)->dwMask = pIP4Set->dwMask;
			(*iter)->dwGate = pIP4Set->dwGate;
			(*iter)->dwPeriod = pIP4Set->dwPeriod;
			(*iter)->dwElapse = pIP4Set->dwElapse;
			break;
		}
	}
}
