#include "StdAfx.h"
#include "PhotoPanel.h"
#include <boost\bind.hpp>

CPhotoPanel::CPhotoPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
			: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pVScrollBar = NULL;
	m_pHScrollBar = NULL;

	EnableAction(MA_MOUSEWHEEL);
	SetCease(); //吞噬鼠标响应
}

CPhotoPanel::~CPhotoPanel()
{
}

void CPhotoPanel::BindPhotoView(CPhotoView *pPhotoView)
{
	m_pPhotoView = pPhotoView;
	m_pPhotoView->SetUpdateView(boost::bind(&CPhotoPanel::OnUpdateView, this, _1));

	AddObject(m_pPhotoView);
}

void CPhotoPanel::BindVScrollBar(CVScrollBar *pVScrollBar)
{
	m_pVScrollBar = pVScrollBar;
	m_pVScrollBar->SetFN_ScrollWindow(boost::bind(&CPhotoPanel::OnVScrollWindow, this, _1, _2));
	m_pVScrollBar->SetFN_SlideWindow(boost::bind(&CPhotoPanel::OnVSlideWindow, this, _1, _2, _3));

	AddObject(m_pVScrollBar);
}

void CPhotoPanel::BindHScrollBar(CHScrollBar *pHScrollBar)
{
	m_pHScrollBar = pHScrollBar;
	m_pHScrollBar->SetFN_ScrollWindow(boost::bind(&CPhotoPanel::OnHScrollWindow, this, _1, _2));
	m_pHScrollBar->SetFN_SlideWindow(boost::bind(&CPhotoPanel::OnHSlideWindow, this, _1, _2, _3));

	AddObject(m_pHScrollBar);
}

void CPhotoPanel::UpdateVScrollBar()
{
	if (!m_pVScrollBar)
		return;

	int nFront = m_pPhotoView->GetFrontHeight();
	int nField = m_pPhotoView->GetFieldHeight();
	int nTotal = m_pPhotoView->GetTotalHeight();

	m_pVScrollBar->OnUpdateView(nFront, nField, nTotal);
}

void CPhotoPanel::UpdateHScrollBar()
{
	if (!m_pHScrollBar)
		return;

	int nFront = m_pPhotoView->GetFrontWidth();
	int nField = m_pPhotoView->GetFieldWidth();
	int nTotal = m_pPhotoView->GetTotalWidth();

	m_pHScrollBar->OnUpdateView(nFront, nField, nTotal);
}

void CPhotoPanel::OnVScrollWindow(CScrollBar *pScrollBar, int nDelta)
{
	m_pPhotoView->OnVScrollWindow(nDelta);

	//同步由PhotoView处理滚动行为的结果
	UpdateVScrollBar();
}

void CPhotoPanel::OnHScrollWindow(CScrollBar *pScrollBar, int nDelta)
{
	m_pPhotoView->OnHScrollWindow(nDelta);
	UpdateHScrollBar();
}

void CPhotoPanel::OnVSlideWindow(CScrollBar *pScrollBar, bool bMouseDrop, bool bAutoScroll)
{
	int nSlideFront = pScrollBar->GetSlideFront();
	int nSlideTotal = pScrollBar->GetSlideTotal();
	m_pPhotoView->OnVSlideWindow(nSlideFront, nSlideTotal);

	int nFront = m_pPhotoView->GetFrontHeight();
	int nField = m_pPhotoView->GetFieldHeight();
	int nTotal = m_pPhotoView->GetTotalHeight();
	pScrollBar->OnUpdateView(nFront, nField, nTotal, bMouseDrop);
}

void CPhotoPanel::OnHSlideWindow(CScrollBar *pScrollBar, bool bMouseDrop, bool bAutoScroll)
{
	int nSlideFront = pScrollBar->GetSlideFront();
	int nSlideTotal = pScrollBar->GetSlideTotal();
	m_pPhotoView->OnHSlideWindow(nSlideFront, nSlideTotal);

	int nFront = m_pPhotoView->GetFrontWidth();
	int nField = m_pPhotoView->GetFieldWidth();
	int nTotal = m_pPhotoView->GetTotalWidth();
	pScrollBar->OnUpdateView(nFront, nField, nTotal, bMouseDrop);
}

void CPhotoPanel::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	if (nFlags == 0)
	{
		if (m_pVScrollBar)
		{
			int nSaltate = m_pVScrollBar->GetSaltate();
			int nRepeat = -zDelta / 120;
			OnVScrollWindow(m_pVScrollBar, nSaltate * nRepeat);
		}
	}
	if (nFlags == MK_CONTROL)
	{
		if (m_pHScrollBar)
		{
			int nSaltate = m_pHScrollBar->GetSaltate();
			int nRepeat = -zDelta / 120;
			OnHScrollWindow(m_pHScrollBar, nSaltate * nRepeat);
		}
	}
}

void CPhotoPanel::OnUpdateView(CPhotoView *pPhotoView)
{
	m_pPhotoView->UpdateView();

	UpdateVScrollBar();
	UpdateHScrollBar();
}
