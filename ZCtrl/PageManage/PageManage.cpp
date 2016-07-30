#include "StdAfx.h"
#include "PageBase.h"
#include "PageManage.h"

CPageManage::CPageManage(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pHeadPage = new CPageBase(NULL, CPoint(), CPoint(), CSize(), 0);
	m_pActived = NULL;
}

CPageManage::~CPageManage()
{
	delete m_pHeadPage;
	m_pHeadPage = NULL;
}

void CPageManage::AddPage(CPageBase *pPage)
{
	if (pPage == NULL)
		return;
	_DelPage(pPage);
	_AddPage(pPage);
}

void CPageManage::DelPage(CPageBase *pPage)
{
	if (pPage == NULL)
		return;
	_DelPage(pPage);
}

void CPageManage::ActivePage(CPageBase *pPage)
{
	if (!pPage || m_pActived == pPage)
		return;

	if (m_pActived)
	{
		//释放旧激活页
		m_pActived->Release();
		DelObject(m_pActived);
	}

	m_pActived = pPage;

	//重建新激活页
	m_pActived->Rebuild();
	AddObject(m_pActived);
}

void CPageManage::_AddPage(CPageBase *pPage)
{
	pPage->m_pNextPage = m_pHeadPage->m_pNextPage;
	m_pHeadPage->m_pNextPage = pPage;
}

void CPageManage::_DelPage(CPageBase *pPage)
{
	CPageBase *pNode = m_pHeadPage;
	//查找目标并删除
	while (pNode->m_pNextPage)
	{
		if (pNode->m_pNextPage == pPage)
		{
			pNode->m_pNextPage = pPage->m_pNextPage;
			pPage->m_pNextPage = NULL;
			break;
		}
		pNode = pNode->m_pNextPage;
	}
}
