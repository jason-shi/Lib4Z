#include "StdAfx.h"

IRspblty::IRspblty()
{
}

IRspblty::~IRspblty()
{
}

IRspbZler::IRspbZler()
{
	m_pNext = NULL;
}

IRspbZler::~IRspbZler()
{
}

bool IRspbZler::ProcessRspb(int nType)
{
	return false;
}

bool IRspbZler::ProcessRspb(IRspblty *pRspblty)
{
	return false;
}

CRspbManage::CRspbManage()
{
	m_pHead = new IRspbZler();
}

CRspbManage::~CRspbManage()
{
	delete m_pHead;
}

void CRspbManage::AddRspbZler(IRspbZler *pRspbZler)
{
	if (pRspbZler)
	{
		//插入链尾
		IRspbZler *pNode = m_pHead;
		while (pNode->m_pNext)
			pNode = pNode->m_pNext;
		pNode->m_pNext = pRspbZler;
	}
}

void CRspbManage::DelRspbZler(IRspbZler *pRspbZler)
{
	IRspbZler *pNode = m_pHead;
	//查找目标并删除
	while (pNode->m_pNext)
	{
		if (pNode->m_pNext == pRspbZler)
		{
			pNode->m_pNext = pRspbZler->m_pNext;
			pRspbZler->m_pNext = NULL;
			break;
		}
		pNode = pNode->m_pNext;
	}
}

bool CRspbManage::ProcessRspb(int nType)
{
	IRspbZler *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		if (pNode->ProcessRspb(nType))
			return true;
	}
	return false;
}

bool CRspbManage::ProcessRspb(IRspblty *pRspblty)
{
	IRspbZler *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		if (pNode->ProcessRspb(pRspblty))
			return true;
	}
	return false;
}
