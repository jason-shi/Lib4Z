#include "StdAfx.h"
#include "VScrollBar.h"
#include <boost\bind.hpp>

CVScrollBar::CVScrollBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize)
				: CScrollBar(pImage, xDrawPos, xSrcPos, xSize)
{
	m_pUDcrt = NULL;
	m_pUArrow = NULL;
	m_pUPad = NULL;
	m_pSlideBar = NULL;
	m_pDPad = NULL;
	m_pDArrow = NULL;
	m_pDDcrt = NULL;
}

CVScrollBar::~CVScrollBar()
{
	delete m_pUDcrt;
	delete m_pUArrow;
	delete m_pUPad;
	delete m_pSlideBar;
	delete m_pDPad;
	delete m_pDArrow;
	delete m_pDDcrt;
}

bool CVScrollBar::Initial()
{
	if (!CPanel::Initial())
		return false;

	m_pUDcrt = new CSprite(&ImageCtrl, CPoint(0, 0), CPoint(0, 0), CSize(15, 6), 1);
	if (!m_pUDcrt || !m_pUDcrt->Initial())
		return false;
	AddObject(m_pUDcrt);

	m_pUArrow = new CButton(&ImageCtrl, CRect(1, 2, 14, 17), CRect(1, 14, 14, 29), 0);
	if (!m_pUArrow || !m_pUArrow->Initial())
		return false;
	m_pUArrow->EnableAction(MA_KEEPLDOWN);
	m_pUArrow->SetMouseProc(MP_LCLICK, boost::bind(&CVScrollBar::OnScrollUp, this, _1));
	AddObject(m_pUArrow);

	m_pUPad = new CButton(&ImageCtrl, CRect(1, 0, 14, 1), CRect(28, 0, 41, 1), 1);
	if (!m_pUPad || !m_pUPad->Initial())
		return false;
	m_pUPad->EnableAction(MA_KEEPLDOWN);
	m_pUPad->SetCease();
	m_pUPad->SetMouseProc(MP_LCLICK, boost::bind(&CVScrollBar::OnSaltateUp, this, _1));
	AddObject(m_pUPad);

	m_pSlideBar = new CVSlideBar(NULL, CPoint(1, 0), CPoint(0, 0), CSize(15, 1024));
	if (!m_pSlideBar || !m_pSlideBar->Initial())
		return false;
	m_pSlideBar->Initial();
	m_pSlideBar->SetMouseProc(MP_MOUSEPICK, boost::bind(&CVScrollBar::OnMousePick, this, _1, _2, _3));
	m_pSlideBar->SetMouseProc(MP_MOUSEDRAG, boost::bind(&CVScrollBar::OnMouseDrag, this, _1, _2, _3));
	m_pSlideBar->SetMouseProc(MP_MOUSEDROP, boost::bind(&CVScrollBar::OnMouseDrop, this, _1, _2, _3));
	AddObject(m_pSlideBar);

	m_pDPad = new CButton(&ImageCtrl, CRect(1, 0, 14, 1), CRect(28, 0, 41, 1), 1);
	if (!m_pDPad || !m_pDPad->Initial())
		return false;
	m_pDPad->EnableAction(MA_KEEPLDOWN);
	m_pDPad->SetCease();
	m_pDPad->SetMouseProc(MP_LCLICK, boost::bind(&CVScrollBar::OnSaltateDown, this, _1));
	AddObject(m_pDPad);

	m_pDArrow = new CButton(&ImageCtrl, CRect(1, m_xSize.cy - 17, 14, m_xSize.cy - 2), CRect(1, 29, 14, 44), 0);
	if (!m_pDArrow || !m_pDArrow->Initial())
		return false;
	m_pDArrow->EnableAction(MA_KEEPLDOWN);
	m_pDArrow->SetMouseProc(MP_LCLICK, boost::bind(&CVScrollBar::OnScrollDown, this, _1));
	AddObject(m_pDArrow);

	m_pDDcrt = new CSprite(&ImageCtrl, CPoint(0, m_xSize.cy - 6), CPoint(0, 7), CSize(15, 6), 1);
	if (!m_pDDcrt || !m_pDDcrt->Initial())
		return false;
	AddObject(m_pDDcrt);

	return true;
}

void CVScrollBar::Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
{
	CScrollBar::Set(pImage, xDrawPos, xSrcPos, xSize, nDepth);

	m_pUDcrt->Set(&ImageCtrl, CPoint(0, 0), CPoint(0, 0), CSize(15, 6), 1);
	m_pUArrow->Set(&ImageCtrl, CRect(1, 2, 14, 17), CRect(1, 14, 14, 29), 0);
	m_pUPad->Set(&ImageCtrl, CRect(1, 0, 14, 1), CRect(28, 0, 41, 1), 1);
	m_pSlideBar->Set(NULL, CPoint(1, 0), CPoint(0, 0), CSize(15, 1024), 0);
	m_pDPad->Set(&ImageCtrl, CRect(1, 0, 14, 1), CRect(28, 0, 41, 1), 1);
	m_pDArrow->Set(&ImageCtrl, CRect(1, m_xSize.cy - 17, 14, m_xSize.cy - 2), CRect(1, 29, 14, 44), 0);
	m_pDDcrt->Set(&ImageCtrl, CPoint(0, m_xSize.cy - 6), CPoint(0, 7), CSize(15, 6), 1);
}

void CVScrollBar::SetHeight(int nBarHeight)
{
	SetSize(m_xSize.cx, nBarHeight);

	//更新滚动条的各部分
	UpdateSlideBar();

	m_pUDcrt->SetDrawPos(0, 0);
	m_pUArrow->SetDrawPos(1, 2);
	m_pDArrow->SetDrawPos(1, m_xSize.cy - 17);
	m_pDDcrt->SetDrawPos(0, m_xSize.cy - 6);
}

void CVScrollBar::UpdateSlideBar()
{
	int nMargin = 2 + 9;
	int nRegion = m_xSize.cy - 2 * nMargin;

	//滑块上端位置和高度
	int nOffset = (float)m_nFront / m_nTotal * nRegion + nMargin;
	int nHeight = (float)m_nField / m_nTotal * nRegion;

	//当自动滚屏时，调整为底端对齐
	if (IsAutoScroll())
	{
		//调整因浮点计算导致的偏差
		if (nOffset - nMargin + nHeight < nRegion)
			nHeight = nRegion - nOffset + nMargin;
	}

	_UpdateSlideBar(nOffset, nHeight);
}

void CVScrollBar::_UpdateSlideBar(int nOffset, int nHeight)
{
	m_pSlideBar->SetDrawPos(m_pSlideBar->GetDrawPos().x, nOffset);
	m_pSlideBar->SetHeight(nHeight);

	//滑块中间位置
	int nMargin = 2 + 9;
	int nMiddlePos = nOffset + nHeight / 2;

	m_pUPad->SetDrawPos(m_pUPad->GetDrawPos().x, nMargin);
	m_pUPad->SetSize(m_pUPad->GetSize().cx, nMiddlePos - nMargin);

	m_pDPad->SetDrawPos(m_pDPad->GetDrawPos().x, nMiddlePos);
	m_pDPad->SetSize(m_pDPad->GetSize().cx, m_xSize.cy - nMargin - nMiddlePos);
}

void CVScrollBar::_UpdateSlideBar(UINT nFlags, CPoint point)
{
	int nMargin = 2 + 9;
	int nDelta = point.y - g_pZ2coo->GetLDown().y;

	//除拉到底以外都不自动滚屏
	m_bAutoScroll = false;

	int nOffset = DragOffset.y + nDelta;
	if (nOffset < nMargin)
		nOffset = nMargin;
	if (nOffset + m_pSlideBar->GetSize().cy > m_xSize.cy - nMargin)
	{
		m_bAutoScroll = true;
		nOffset = m_xSize.cy - nMargin - m_pSlideBar->GetSize().cy;
	}

	_UpdateSlideBar(nOffset, m_pSlideBar->GetSize().cy);
}

void CVScrollBar::OnScrollUp(CObject *pButton)
{
	if (OnScrollWindow)
		OnScrollWindow(this, -1);
}

void CVScrollBar::OnScrollDown(CObject *pButton)
{
	if (OnScrollWindow)
		OnScrollWindow(this, +1);
}

void CVScrollBar::OnSaltateUp(CObject *pButton)
{
	if (OnScrollWindow)
		OnScrollWindow(this, -m_nSaltate);
}

void CVScrollBar::OnSaltateDown(CObject *pButton)
{
	if (OnScrollWindow)
		OnScrollWindow(this, +m_nSaltate);
}

void CVScrollBar::OnMousePick(CObject *pSlideBar, UINT nFlags, CPoint point)
{
	DragOffset = pSlideBar->GetDrawPos();
}

void CVScrollBar::OnMouseDrag(CObject *pSlideBar, UINT nFlags, CPoint point)
{
	_UpdateSlideBar(nFlags, point);

	//滑动窗口，但不更新滑动条
	if (OnSlideWindow)
		OnSlideWindow(this, false, m_bAutoScroll);
}

void CVScrollBar::OnMouseDrop(CObject *pSlideBar, UINT nFlags, CPoint point)
{
	_UpdateSlideBar(nFlags, point);

	//滑动窗口，并且更新滑动条
	if (OnSlideWindow)
		OnSlideWindow(this, true, m_bAutoScroll);
}
