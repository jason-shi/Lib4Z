#include "StdAfx.h"
#include "BrowsePanel.h"
#include <boost\bind.hpp>

CBrowsePanel::CBrowsePanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pBrowseView = NULL;
	m_pBrowseTitle = NULL;
	m_pVScrollBar = NULL;
	m_pHScrollBar = NULL;

	EnableAction(MA_MOUSEWHEEL);
	SetCease(); //吞噬鼠标响应
}

CBrowsePanel::~CBrowsePanel()
{
}

void CBrowsePanel::BindBrowseView(CBrowseView *pBrowseView)
{
	m_pBrowseView = pBrowseView;
	m_pBrowseView->SetUpdateView(boost::bind(&CBrowsePanel::OnUpdateView, this, _1));

	AddObject(m_pBrowseView);
}

void CBrowsePanel::BindBrowseTitle(CBrowseTitle *pBrowseTitle)
{
	m_pBrowseTitle = pBrowseTitle;
	m_pBrowseTitle->SetAdjustWidth(boost::bind(&CBrowsePanel::OnAdjustWidth, this, _1, _2, _3));

	AddObject(m_pBrowseTitle);
}

void CBrowsePanel::BindVScrollBar(CVScrollBar *pVScrollBar)
{
	m_pVScrollBar = pVScrollBar;
	m_pVScrollBar->SetFN_ScrollWindow(boost::bind(&CBrowsePanel::OnVScrollWindow, this, _1, _2));
	m_pVScrollBar->SetFN_SlideWindow(boost::bind(&CBrowsePanel::OnVSlideWindow, this, _1, _2, _3));

	AddObject(m_pVScrollBar);
}

void CBrowsePanel::BindHScrollBar(CHScrollBar *pHScrollBar)
{
	m_pHScrollBar = pHScrollBar;
	m_pHScrollBar->SetFN_ScrollWindow(boost::bind(&CBrowsePanel::OnHScrollWindow, this, _1, _2));
	m_pHScrollBar->SetFN_SlideWindow(boost::bind(&CBrowsePanel::OnHSlideWindow, this, _1, _2, _3));

	AddObject(m_pHScrollBar);
}

void CBrowsePanel::UpdateVScrollBar()
{
	if (!m_pVScrollBar)
		return;

	int nFront = m_pBrowseView->GetFrontHeight();
	int nField = m_pBrowseView->GetFieldHeight();
	int nTotal = m_pBrowseView->GetTotalHeight();

	m_pVScrollBar->OnUpdateView(nFront, nField, nTotal);
}

void CBrowsePanel::UpdateHScrollBar()
{
	if (!m_pHScrollBar)
		return;

	int nFront = m_pBrowseView->GetFrontWidth();
	int nField = m_pBrowseView->GetFieldWidth();
	int nTotal = m_pBrowseView->GetTotalWidth();

	m_pHScrollBar->OnUpdateView(nFront, nField, nTotal);
}

void CBrowsePanel::OnVScrollWindow(CScrollBar *pScrollBar, int nDelta)
{
	m_pBrowseView->OnVScrollWindow(nDelta);

	//同步由BrowseView处理滚动行为的结果
	m_pVScrollBar->SetAutoScroll(m_pBrowseView->IsAutoScroll());
	UpdateVScrollBar();
}

void CBrowsePanel::OnHScrollWindow(CScrollBar *pScrollBar, int nDelta)
{
	m_pBrowseView->OnHScrollWindow(nDelta);
	UpdateHScrollBar();

	if (m_pBrowseTitle)
		m_pBrowseTitle->OnHScrollWindow(nDelta);
}

void CBrowsePanel::OnVSlideWindow(CScrollBar *pScrollBar, bool bMouseDrop, bool bAutoScroll)
{
	//同步由VScrollBar处理拉动行为的结果
	m_pBrowseView->SetAutoScroll(bAutoScroll);

	int nSlideFront = pScrollBar->GetSlideFront();
	int nSlideTotal = pScrollBar->GetSlideTotal();
	m_pBrowseView->OnVSlideWindow(nSlideFront, nSlideTotal);

	int nFront = m_pBrowseView->GetFrontHeight();
	int nField = m_pBrowseView->GetFieldHeight();
	int nTotal = m_pBrowseView->GetTotalHeight();
	pScrollBar->OnUpdateView(nFront, nField, nTotal, bMouseDrop);
}

void CBrowsePanel::OnHSlideWindow(CScrollBar *pScrollBar, bool bMouseDrop, bool bAutoScroll)
{
	int nSlideFront = pScrollBar->GetSlideFront();
	int nSlideTotal = pScrollBar->GetSlideTotal();
	m_pBrowseView->OnHSlideWindow(nSlideFront, nSlideTotal);

	int nFront = m_pBrowseView->GetFrontWidth();
	int nField = m_pBrowseView->GetFieldWidth();
	int nTotal = m_pBrowseView->GetTotalWidth();
	pScrollBar->OnUpdateView(nFront, nField, nTotal, bMouseDrop);

	if (m_pBrowseTitle)
		m_pBrowseTitle->OnHSlideWindow(nSlideFront, nSlideTotal);
}

void CBrowsePanel::OnAdjustWidth(CBrowseTitle *pBrowseTitle, int nItem, int nWidth)
{
	m_pBrowseView->OnAdjustWidth(nItem, nWidth);
}

void CBrowsePanel::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	if (m_pVScrollBar)
	{
		int nSaltate = m_pVScrollBar->GetSaltate();
		OnVScrollWindow(m_pVScrollBar, zDelta == 255 ? nSaltate : -nSaltate);
	}
}

void CBrowsePanel::OnUpdateView(CBrowseView *pBrowseView)
{
	m_pBrowseView->UpdateView();

	UpdateVScrollBar();
	UpdateHScrollBar();
}
