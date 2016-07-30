#include "StdAfx.h"

CLianPanel::CLianPanel() : CPanel()
{
	m_pLianKan = NULL;
}

CLianPanel::CLianPanel(CImage *pImage, CPoint point, CSize size, int nDepth)
				: CPanel(pImage, point, size, nDepth)
{
	m_pLianKan = NULL;
}

CLianPanel::~CLianPanel()
{
}

void CLianPanel::Select(CLianKan *pLianKan)
{
	pLianKan->OnSelect();
	m_pLianKan = pLianKan;
}

void CLianPanel::UnSelect()
{
	if (m_pLianKan)
	{
		m_pLianKan->UnSelect();
		m_pLianKan = NULL;
	}
}

void CLianPanel::OnRemoveSame(CLianKan *pObject1, CLianKan *pObject2)
{
	DelObject(pObject1);
	DelObject(pObject2);
}

void CLianPanel::OnLClick(CLianKan *pLianKan)
{
	if (m_pLianKan)
	{
		if (m_pLianKan == pLianKan)
			UnSelect();
		else if (m_pLianKan->GetKind() == pLianKan->GetKind())
		{
			OnRemoveSame(m_pLianKan, pLianKan);
			m_pLianKan = NULL;
		}
		else
		{
			UnSelect();
			Select(pLianKan);
		}
	}
	else
	{
		Select(pLianKan);
	}
}

bool CLianPanel::OnMouseEnter(UINT nFlags, CPoint point)
{
	return true;
}

bool CLianPanel::OnMouseLeave(UINT nFlags, CPoint point)
{
	return true;
}

void CLianPanel::OnLClick(UINT nFlags, CPoint point)
{
	UnSelect();
}
