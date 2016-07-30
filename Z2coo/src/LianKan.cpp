#include "StdAfx.h"

CLianKan::CLianKan() : CButton()
{
	m_nKind = 0;
	m_bSelected = false;
}

CLianKan::CLianKan(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth)
				: CButton(pImage, xDrawRect, xSrcRect, nDepth)
{
	m_nKind = 0;
	m_bSelected = false;
}

CLianKan::~CLianKan()
{
}

void CLianKan::OnSelect()
{
	m_bSelected = true;
	SetSize(20, 20);
}

void CLianKan::UnSelect()
{
	m_bSelected = false;
	SetSize(24, 24);
}

void CLianKan::OnLClick(UINT nFlags, CPoint point)
{
	CLianPanel *pLianPanel = dynamic_cast<CLianPanel *>(m_pParent);
	if (pLianPanel == NULL)
		return;

	pLianPanel->OnLClick(this);
}
