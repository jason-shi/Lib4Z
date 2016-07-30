#include "StdAfx.h"
#include "SlideBar.h"
#include "SlideBtn.h"

CSlideBar::CSlideBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize)
			: CPanel(pImage, xDrawPos, xSrcPos, xSize, 0)
{
}

CSlideBar::~CSlideBar()
{
	m_vecButton.clear();
}

void CSlideBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	VECBUTTON::iterator iter;
	for (iter = m_vecButton.begin(); iter != m_vecButton.end(); iter++)
		(*iter)->OnLButtonDown(nFlags, point);
}

void CSlideBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	VECBUTTON::iterator iter;
	for (iter = m_vecButton.begin(); iter != m_vecButton.end(); iter++)
		(*iter)->OnLButtonUp(nFlags, point);
}

bool CSlideBar::OnMouseEnter(UINT nFlags, CPoint point)
{
	VECBUTTON::iterator iter;
	for (iter = m_vecButton.begin(); iter != m_vecButton.end(); iter++)
		(*iter)->OnMouseEnter(nFlags, point);

	return true;
}

bool CSlideBar::OnMouseLeave(UINT nFlags, CPoint point)
{
	VECBUTTON::iterator iter;
	for (iter = m_vecButton.begin(); iter != m_vecButton.end(); iter++)
		(*iter)->OnMouseLeave(nFlags, point);

	return true;
}
