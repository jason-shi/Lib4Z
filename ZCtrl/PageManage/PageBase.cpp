#include "StdAfx.h"
#include "PageBase.h"

CPageBase::CPageBase(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pNextPage = NULL;
}

CPageBase::~CPageBase()
{
}

void CPageBase::SetActive(bool bActive)
{
	m_bActive = true;
}

bool CPageBase::Initial()
{
	return true;
}

bool CPageBase::Rebuild()
{
	return true;
}

bool CPageBase::Release()
{
	return true;
}

void CPageBase::Destory()
{
}
