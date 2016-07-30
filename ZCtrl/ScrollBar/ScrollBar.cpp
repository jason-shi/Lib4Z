#include "StdAfx.h"
#include "ScrollBar.h"

CScrollBar::CScrollBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, 0)
{
	m_bAutoScroll = false;
	m_nSaltate = 3;
	OnScrollWindow = NULL;
}

CScrollBar::~CScrollBar()
{
}

void CScrollBar::OnUpdateView(int nFront, int nField, int nTotal, bool bUpdate)
{
	m_nFront = nFront;
	m_nField = nField;
	m_nTotal = nTotal == 0 ? nField : nTotal;

	if (bUpdate)
		UpdateSlideBar();
}
