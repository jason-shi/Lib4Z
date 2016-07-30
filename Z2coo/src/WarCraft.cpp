#include "StdAfx.h"

CWarCraft::CWarCraft() : CButton()
{
	m_nKind = 0;
	m_bSelected = false;
}

CWarCraft::CWarCraft(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth)
				: CButton(pImage, xDrawRect, xSrcRect, nDepth)
{
	m_nKind = 0;
	m_bSelected = false;
}

CWarCraft::~CWarCraft()
{
}

void CWarCraft::OnSelect()
{
	m_bSelected = true;
}

void CWarCraft::UnSelect()
{
	m_bSelected = false;
}

bool CWarCraft::RectIn(const CRect& rect)
{
	CRect r;
	GetRect(&r);
	r.InflateRect(-10, -10); //内缩
	return !(r & rect).IsRectEmpty();
}

void CWarCraft::OnLClick(UINT nFlags, CPoint point)
{
	CWarPanel *pWarPanel = dynamic_cast<CWarPanel *>(m_pParent);
	if (pWarPanel == NULL)
		return;

	if (nFlags == 0)
	{
		pWarPanel->UnSelect();
		pWarPanel->Select(this);
	}
	else if (nFlags == MK_SHIFT)
		m_bSelected ? pWarPanel->UnSelect(this) : pWarPanel->Select(this);
	else if (nFlags == MK_CONTROL)
	{
		pWarPanel->UnSelect();
		pWarPanel->Select(m_nKind);
	}
	else if (nFlags == (MK_SHIFT | MK_CONTROL))
		m_bSelected ? pWarPanel->UnSelect(m_nKind) : pWarPanel->Select(m_nKind);
}

void CWarCraft::OnLDblClk(UINT nFlags, CPoint point)
{
	CWarPanel *pWarPanel = dynamic_cast<CWarPanel *>(m_pParent);
	if (pWarPanel == NULL)
		return;

	nFlags &= ~MK_CONTROL; //Ctrl在双击时无效
	if (nFlags == 0)
	{
		pWarPanel->UnSelect();
		pWarPanel->Select(m_nKind);
	}
	else if (nFlags == MK_SHIFT)
		m_bSelected ? pWarPanel->UnSelect(m_nKind) : pWarPanel->Select(m_nKind);
}
