#include "StdAfx.h"
#include "ToolBar.h"

CToolBar::CToolBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pSelected = NULL;
}

CToolBar::~CToolBar()
{
}

void CToolBar::AddToolBtn(CToolBtn *pToolBtn)
{
	VECTOOLBTN::iterator iter;
	for (iter = m_vecToolBtn.begin(); iter != m_vecToolBtn.end(); iter++)
	{
		if (*iter == pToolBtn)
			return;
	}

	AddObject(pToolBtn);
	m_vecToolBtn.push_back(pToolBtn);
}

void CToolBar::DelToolBtn(CToolBtn *pToolBtn)
{
	VECTOOLBTN::iterator iter;
	for (iter = m_vecToolBtn.begin(); iter != m_vecToolBtn.end(); iter++)
	{
		if (*iter == pToolBtn)
		{
			DelObject(pToolBtn);
			m_vecToolBtn.erase(iter);
			break;
		}
	}
}

void CToolBar::OnSwitchSelect(CToolBtn *pToolBtn)
{
	if (m_pSelected)
		m_pSelected->UnSelect();

	m_pSelected = pToolBtn;
	m_pSelected->OnSelect();
}

bool CToolBar::ProcessLClick(CObject *pObject, UINT nFlags, CPoint point)
{
	if (m_pSelected)
	{
		if (m_pSelected->ProcessLClick(pObject, nFlags, point))
			return true;
	}
	return false;
}

bool CToolBar::ProcessRClick(CObject *pObject, UINT nFlags, CPoint point)
{
	if (m_pSelected)
	{
		if (m_pSelected->ProcessRClick(pObject, nFlags, point))
			return true;
	}
	return false;
}

bool CToolBar::ProcessDrawRect(CObject *pObject, UINT nFlags, const CRect& rect)
{
	if (m_pSelected)
	{
		if (m_pSelected->ProcessDrawRect(pObject, nFlags, rect))
			return true;
	}
	return false;
}
