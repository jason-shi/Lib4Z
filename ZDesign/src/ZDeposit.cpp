#include "StdAfx.h"
#include <memory>

CZDeposit::CZDeposit()
{
	m_pHead = NULL;
}

CZDeposit::~CZDeposit()
{
	SAFE_DELETE(m_pHead);
}

bool CZDeposit::Alloc(int nSize, int nCount, int nNext, int nPrev)
{
	m_nSize = (nSize + 3) & 0xFFFFFFFC;
	m_nCount = nCount;
	m_nNext = nNext;
	m_nPrev = nPrev;
	m_nFlag = ((_int64)1 << m_nCount) - 1;
	//申请空间
	m_pHead = new char[m_nSize * m_nCount + sizeof(int)];
	return m_pHead != NULL;
}

void CZDeposit::MemFill(void *pBuffer)
{
	char *pTemp = m_pHead;
	for (int i = 0; i < m_nCount; i++)
	{
		memcpy(pTemp, pBuffer, m_nSize);
		pTemp += m_nSize;
	}
}

void* CZDeposit::GetNode()
{
	static int T[37] = { 0, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4, 7, 17,
						0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5, 20, 8, 19, 18 };
	int nIndex = T[(m_nFlag & -m_nFlag) % 37];
	m_nFlag &= m_nFlag - 1;
	return m_pHead + nIndex * m_nSize;
}

void CZDeposit::RtnNode(char *pNode)
{
	int nIndex = (pNode - m_pHead) / m_nSize;
	m_nFlag |= (_int64)1 << nIndex;
}

void CZDeposit::Impawn(char *pNode)
{
	//队头前向指针指向对头指针的地址
	int nNextAddr = *(int *)(pNode + m_nNext);
	int nPrevAddr = *(int *)(pNode + m_nPrev);

	if (*(int *)nPrevAddr == (int)pNode)
		*(int *)nPrevAddr = nNextAddr;
	else
		*(int *)(nPrevAddr + m_nNext) = nNextAddr;

	if (nNextAddr != 0)
		*(int *)(nNextAddr + m_nPrev) = nPrevAddr;
}

void CZDeposit::Redeem(char *pNode)
{
	//队头前向指针指向对头指针的地址
	int nNextAddr = *(int *)(pNode + m_nNext);
	int nPrevAddr = *(int *)(pNode + m_nPrev);

	if (*(int *)nPrevAddr == nNextAddr)
		*(int *)nPrevAddr = (int)pNode;
	else
		*(int *)(nPrevAddr + m_nNext) = (int)pNode;

	if (nNextAddr != 0)
		*(int *)(nNextAddr + m_nPrev) = (int)pNode;
}

void CZDeposit::Reclaim()
{
	m_nFlag = ((_int64)1 << m_nCount) - 1;
}
