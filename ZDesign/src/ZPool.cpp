#include "StdAfx.h"
#include <memory>

CZPool::CZPool()
{
	m_pHead = NULL;
	m_pSpare = NULL;
}

CZPool::~CZPool()
{
	SAFE_DELETE(m_pHead);
}

bool CZPool::Alloc(int nSize, int nCount)
{
	m_nSize = (nSize + 3) & 0xFFFFFFFC;
	m_nCount = nCount;

	//重置资源
	Reclaim();

	//申请空间
	m_pHead = new char[m_nSize * m_nCount + sizeof(int)];
	return m_pHead != NULL;
}

void CZPool::MemFill(void *pBuffer)
{
	char *pTemp = m_pHead;
	for (int i = 0; i < m_nCount; i++)
	{
		memcpy(pTemp, pBuffer, m_nSize);
		pTemp += m_nSize;
	}
}

char *CZPool::GetNode()
{
	static int T[37] = { 0, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4, 7, 17,
						0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5, 20, 8, 19, 18 };

	//首先分配优选空节点
	if (m_pSpare)
	{
		char *pNode = m_pSpare;
		m_pSpare = NULL;
		return pNode;
	}
	//根据空闲位图进行分配
	for (int i = 0; i < MAX_POOL_FLAG; i++)
	{
		if (m_nFlag[i] == 0)
			continue;

		int nIndex = T[(unsigned int)(m_nFlag[i] & -m_nFlag[i]) % 37];
		m_nFlag[i] &= m_nFlag[i] - 1;
		return m_pHead + (nIndex + (i << 5)) * m_nSize;
	}
	return NULL;
}

void CZPool::RtnNode(char *pNode)
{
	//首先存储优选空节点
	if (m_pSpare == NULL)
		m_pSpare = pNode;
	else
	{
		int nIndex = (pNode - m_pHead) / m_nSize;
		m_nFlag[nIndex >> 5] |= 1 << (nIndex & 0x1F);
	}
}

void CZPool::Reclaim()
{
	m_pSpare = NULL;
	memset(m_nFlag, 0, sizeof(m_nFlag));

	for (int i = 0; i < (m_nCount >> 5); i++)
		m_nFlag[i] = 0xFFFFFFFF;
	m_nFlag[m_nCount >> 5] = (1 << (m_nCount & 0x1F)) - 1;
}

bool CZPool::ForEach(FN_WaterProc WaterProc, void *lpParam)
{
	if (WaterProc)
	{
		for (int i = 0; i < m_nCount; i++)
		{
			if (!WaterProc((CWater *)(m_pHead + i * m_nSize), lpParam))
				return false;
		}
	}
	return true;
}

CZPool *CreateZPool(int nSize, int nCount, void *pBuffer)
{
	CZPool *pPool = new CZPool();
	if (!pPool)
		return NULL;

	pPool->Alloc(nSize, nCount);
	pPool->MemFill(pBuffer);
	return pPool;
}
