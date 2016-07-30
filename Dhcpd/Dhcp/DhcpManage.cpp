#include "StdAfx.h"
#include "DhcpInst.h"
#include "DhcpProc.h"
#include "DhcpManage.h"

using namespace std;

CDhcpManage::CDhcpManage(CDhcpProc *pDhcpProc)
{
	m_pDhcpProc = pDhcpProc;
}

CDhcpManage::~CDhcpManage()
{
	m_pDhcpProc = NULL;
	m_vecDhcpInst.clear();
}

void CDhcpManage::AddInst(CDhcpInst *pDhcpInst)
{
	m_vecDhcpInst.push_back(pDhcpInst);
}

void CDhcpManage::DelInst(CDhcpInst *pDhcpInst)
{
	VECDHCP::iterator iter;
	for (iter = m_vecDhcpInst.begin(); iter != m_vecDhcpInst.end(); iter++)
	{
		if (*iter == pDhcpInst)
		{
			m_vecDhcpInst.erase(iter);
			break;
		}
	}
}

CDhcpInst *CDhcpManage::FindInst(dword dwTransID)
{
	VECDHCP::iterator iter;
	for (iter = m_vecDhcpInst.begin(); iter != m_vecDhcpInst.end(); iter++)
	{
		if ((*iter)->GetTransID() == dwTransID)
			return *iter;
	}
	return NULL;
}

CDhcpInst *CDhcpManage::ObtainInst(dword dwTransID)
{
	CDhcpInst *pDhcpInst = FindInst(dwTransID);
	if (pDhcpInst == NULL)
	{
		pDhcpInst = new CDhcpInst(m_pDhcpProc);
		if (pDhcpInst == NULL)
			return NULL;
	}

	return pDhcpInst;
}