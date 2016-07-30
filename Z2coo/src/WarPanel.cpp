#include "StdAfx.h"

CWarPanel::CWarPanel(CImage *pImage, CPoint point, CSize size, int nDepth)
				: CPanel(pImage, point, size, nDepth)
{
	m_nSelectLimit = 12; //Commemorate The WarCraft
	m_nDrawThrs = 7;
}

CWarPanel::~CWarPanel()
{
}

void CWarPanel::AddVisual(CWarCraft *pWarCraft)
{
	m_vecVisual.push_back(pWarCraft);
}

void CWarPanel::Select(CWarCraft *pWarCraft)
{
	if (m_vecSelect.size() >= m_nSelectLimit)
		return;
	pWarCraft->OnSelect();
	m_vecSelect.push_back(pWarCraft);
}

void CWarPanel::Select(int nKind)
{
	VECWAR::iterator iter;
	for (iter = m_vecVisual.begin(); iter < m_vecVisual.end(); iter++)
	{
		//视野内未选中的同类兵种
		if ((*iter)->GetKind() == nKind && !(*iter)->IsSelected())
			Select(*iter);
		if (m_vecSelect.size() >= m_nSelectLimit)
			break;
	}
}

void CWarPanel::SelectVisual(const CRect& rect)
{
	VECWAR::iterator iter;
	for (iter = m_vecVisual.begin(); iter < m_vecVisual.end(); iter++)
	{
		//视野内框选
		if ((*iter)->RectIn(rect))
			Select(*iter);
		if (m_vecSelect.size() >= m_nSelectLimit)
			break;
	}
}

void CWarPanel::UnSelect(CWarCraft *pWarCraft)
{
	pWarCraft->UnSelect();

	VECWAR::iterator iter;
	for (iter = m_vecSelect.begin(); iter < m_vecSelect.end(); iter++)
	{
		if (*iter == pWarCraft)
		{
			m_vecSelect.erase(iter);
			break;
		}
	}
}

void CWarPanel::UnSelect(int nKind)
{
	VECWAR::iterator iter;
	for (iter = m_vecSelect.begin(); iter < m_vecSelect.end(); )
	{
		//已选中的同类兵种
		if ((*iter)->GetKind() == nKind && (*iter)->IsSelected())
		{
			UnSelect(*iter);
			iter = m_vecSelect.begin();
		}
		else
			iter++;
	}
}

void CWarPanel::UnSelect()
{
	VECWAR::iterator iter;
	for (iter = m_vecSelect.begin(); iter < m_vecSelect.end(); iter++)
		(*iter)->UnSelect();

	m_vecSelect.clear();
}

bool CWarPanel::OnMouseEnter(UINT nFlags, CPoint point)
{
	return true;
}

bool CWarPanel::OnMouseLeave(UINT nFlags, CPoint point)
{
	return true;
}

void CWarPanel::OnLClick(UINT nFlags, CPoint point)
{
	UnSelect();
}

bool CWarPanel::CheckDrawMode(UINT nFlags)
{
	return nFlags & MK_LBUTTON;
}

void CWarPanel::OnDrawRect(UINT nFlags, const CRect& rect)
{
	g_pZ2coo->Redraw(m_xDrawingRect | rect);
	if (CheckDrawThrs(rect))
	{
		m_xDrawingRect = rect;
	}
	else
	{
		//当选框小于门限时不绘制
		m_xDrawingRect.SetRectEmpty();
	}
}

void CWarPanel::OnDrawOver(UINT nFlags, const CRect& rect)
{
	g_pZ2coo->Redraw(m_xDrawingRect);
	m_xDrawingRect.SetRectEmpty();
	if (CheckDrawThrs(rect))
	{
		if (nFlags & MK_SHIFT)
			SelectVisual(rect);
		else
		{
			UnSelect();
			SelectVisual(rect);
		}
	}
}
