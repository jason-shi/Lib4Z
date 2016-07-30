#include "StdAfx.h"
#include "HScrollBar.h"
#include <boost\bind.hpp>

CHScrollBar::CHScrollBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize)
				: CScrollBar(pImage, xDrawPos, xSrcPos, xSize)
{
	m_pLDcrt = NULL;
	m_pLArrow = NULL;
	m_pLPad = NULL;
	m_pSlideBar = NULL;
	m_pRPad = NULL;
	m_pRArrow = NULL;
	m_pRDcrt = NULL;
}

CHScrollBar::~CHScrollBar()
{
	delete m_pLDcrt;
	delete m_pLArrow;
	delete m_pLPad;
	delete m_pSlideBar;
	delete m_pRPad;
	delete m_pRArrow;
	delete m_pRDcrt;
}

bool CHScrollBar::Initial()
{
	if (!CPanel::Initial())
		return false;

	m_pLDcrt = new CSprite(&ImageCtrl, CPoint(0, 0), CPoint(399, 13), CSize(6, 15), 1);
	if (!m_pLDcrt || !m_pLDcrt->Initial())
		return false;
	AddObject(m_pLDcrt);

	m_pLArrow = new CButton(&ImageCtrl, CRect(2, 1, 17, 14), CRect(413, 14, 428, 27), 0);
	if (!m_pLArrow || !m_pLArrow->Initial())
		return false;
	m_pLArrow->EnableAction(MA_KEEPLDOWN);
	m_pLArrow->SetMouseProc(MP_LCLICK, boost::bind(&CHScrollBar::OnScrollLeft, this, _1));
	AddObject(m_pLArrow);

	m_pLPad = new CButton(&ImageCtrl, CRect(0, 1, 1, 14), CRect(399, 28, 412, 29), 1);
	if (!m_pLPad || !m_pLPad->Initial())
		return false;
	m_pLPad->EnableAction(MA_KEEPLDOWN);
	m_pLPad->SetCease();
	m_pLPad->SetMouseProc(MP_LCLICK, boost::bind(&CHScrollBar::OnSaltateLeft, this, _1));
	AddObject(m_pLPad);

	m_pSlideBar = new CHSlideBar(NULL, CPoint(0, 1), CPoint(0, 0), CSize(1024, 15));
	if (!m_pSlideBar || !m_pSlideBar->Initial())
		return false;
	m_pSlideBar->Initial();
	m_pSlideBar->SetMouseProc(MP_MOUSEPICK, boost::bind(&CHScrollBar::OnMousePick, this, _1, _2, _3));
	m_pSlideBar->SetMouseProc(MP_MOUSEDRAG, boost::bind(&CHScrollBar::OnMouseDrag, this, _1, _2, _3));
	m_pSlideBar->SetMouseProc(MP_MOUSEDROP, boost::bind(&CHScrollBar::OnMouseDrop, this, _1, _2, _3));
	AddObject(m_pSlideBar);

	m_pRPad = new CButton(&ImageCtrl, CRect(0, 1, 1, 14), CRect(399, 28, 412, 29), 1);
	if (!m_pRPad || !m_pRPad->Initial())
		return false;
	m_pRPad->EnableAction(MA_KEEPLDOWN);
	m_pRPad->SetCease();
	m_pRPad->SetMouseProc(MP_LCLICK, boost::bind(&CHScrollBar::OnSaltateRight, this, _1));
	AddObject(m_pRPad);

	m_pRArrow = new CButton(&ImageCtrl, CRect(m_xSize.cx - 17, 1, m_xSize.cx - 2, 14), CRect(428, 14, 429, 28), 0);
	if (!m_pRArrow || !m_pRArrow->Initial())
		return false;
	m_pRArrow->EnableAction(MA_KEEPLDOWN);
	m_pRArrow->SetMouseProc(MP_LCLICK, boost::bind(&CHScrollBar::OnScrollRight, this, _1));
	AddObject(m_pRArrow);

	m_pRDcrt = new CSprite(&ImageCtrl, CPoint(m_xSize.cx - 6, 0), CPoint(406, 13), CSize(6, 15), 1);
	if (!m_pRDcrt || !m_pRDcrt->Initial())
		return false;
	AddObject(m_pRDcrt);

	return true;
}

void CHScrollBar::Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
{
	CScrollBar::Set(pImage, xDrawPos, xSrcPos, xSize, nDepth);

	m_pLDcrt->Set(&ImageCtrl, CPoint(0, 0), CPoint(399, 13), CSize(6, 15), 1);
	m_pLArrow->Set(&ImageCtrl, CRect(2, 1, 17, 14), CRect(413, 14, 428, 27), 0);
	m_pLPad->Set(&ImageCtrl, CRect(0, 1, 1, 14), CRect(399, 28, 412, 29), 1);
	m_pSlideBar->Set(NULL, CPoint(0, 1), CPoint(0, 0), CSize(1024, 15), 0);
	m_pRPad->Set(&ImageCtrl, CRect(0, 1, 1, 14), CRect(399, 28, 412, 29), 1);
	m_pRArrow->Set(&ImageCtrl, CRect(m_xSize.cx - 17, 1, m_xSize.cx - 2, 14), CRect(428, 14, 429, 28), 0);
	m_pRDcrt->Set(&ImageCtrl, CPoint(m_xSize.cx - 6, 0), CPoint(406, 13), CSize(6, 15), 1);
}

void CHScrollBar::SetWidth(int nBarWidth)
{
	SetSize(nBarWidth, 15);

	//更新滚动条的各部分
	UpdateSlideBar();

	m_pLDcrt->SetDrawPos(0, 0);
	m_pLArrow->SetDrawPos(2, 1);
	m_pRArrow->SetDrawPos(m_xSize.cx - 17, 1);
	m_pRDcrt->SetDrawPos(m_xSize.cx - 6, 0);
}

void CHScrollBar::UpdateSlideBar()
{
	int nMargin = 2 + 9;
	int nRegion = m_xSize.cx - 2 * nMargin;

	//滑块上端位置和高度
	int nOffset = (float)m_nFront / m_nTotal * nRegion + nMargin;
	int nWidth = (float)m_nField / m_nTotal * nRegion;

	_UpdateSlideBar(nOffset, nWidth);
}

void CHScrollBar::_UpdateSlideBar(int nOffset, int nWidth)
{
	m_pSlideBar->SetDrawPos(nOffset, m_pSlideBar->GetDrawPos().y);
	m_pSlideBar->SetWidth(nWidth);

	//滑块中间位置
	int nMargin = 2 + 9;
	int nMiddlePos = nOffset + nWidth / 2;

	m_pLPad->SetDrawPos(nMargin, m_pLPad->GetDrawPos().y);
	m_pLPad->SetSize(nMiddlePos - nMargin, m_pLPad->GetSize().cy);

	m_pRPad->SetDrawPos(nMiddlePos, m_pRPad->GetDrawPos().y);
	m_pRPad->SetSize(m_xSize.cx - nMargin - nMiddlePos, m_pRPad->GetSize().cy);
}

void CHScrollBar::_UpdateSlideBar(UINT nFlags, CPoint point)
{
	int nMargin = 2 + 9;
	int nDelta = point.x - g_pZ2coo->GetLDown().x;

	int nOffset = DragOffset.x + nDelta;
	if (nOffset < nMargin)
		nOffset = nMargin;
	if (nOffset + m_pSlideBar->GetSize().cx >= m_xSize.cx - nMargin)
		nOffset = m_xSize.cx - nMargin - m_pSlideBar->GetSize().cx;

	_UpdateSlideBar(nOffset, m_pSlideBar->GetSize().cx);
}

void CHScrollBar::OnScrollLeft(CObject *pButton)
{
	if (OnScrollWindow)
		OnScrollWindow(this, -1);
}

void CHScrollBar::OnScrollRight(CObject *pButton)
{
	if (OnScrollWindow)
		OnScrollWindow(this, +1);
}

void CHScrollBar::OnSaltateLeft(CObject *pButton)
{
	if (OnScrollWindow)
		OnScrollWindow(this, -m_nSaltate);
}

void CHScrollBar::OnSaltateRight(CObject *pButton)
{
	if (OnScrollWindow)
		OnScrollWindow(this, +m_nSaltate);
}

void CHScrollBar::OnMousePick(CObject *pSlideBar, UINT nFlags, CPoint point)
{
	DragOffset = pSlideBar->GetDrawPos();
}

void CHScrollBar::OnMouseDrag(CObject *pSlideBar, UINT nFlags, CPoint point)
{
	_UpdateSlideBar(nFlags, point);

	//滑动窗口，但不更新滑动条
	if (OnSlideWindow)
		OnSlideWindow(this, false, m_bAutoScroll);
}

void CHScrollBar::OnMouseDrop(CObject *pSlideBar, UINT nFlags, CPoint point)
{
	_UpdateSlideBar(nFlags, point);

	//滑动窗口，并且更新滑动条
	if (OnSlideWindow)
		OnSlideWindow(this, true, m_bAutoScroll);
}
