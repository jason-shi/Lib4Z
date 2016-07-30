#include "StdAfx.h"
#include "SlideBtn.h"

CSlideBtn::CSlideBtn(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth)
				: CButton(pImage, xDrawRect, xSrcRect, nDepth)
{
}

CSlideBtn::~CSlideBtn()
{
}

void CSlideBtn::SetWidth(int nWidth)
{
	m_xSize.cx = nWidth;
	m_xSrcRect.right = m_xSrcRect.left + nWidth;

	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

void CSlideBtn::SetHeight(int nHeight)
{
	m_xSize.cy = nHeight;
	m_xSrcRect.bottom = m_xSrcRect.top + nHeight;

	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

void CSlideBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return;
	m_nStatus = BTN_PRESSED;
	SetMapSrcPos();
	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

bool CSlideBtn::OnMouseEnter(UINT nFlags, CPoint point)
{
	if (CButton::OnMouseEnter(nFlags, point))
	{
		m_pParent->OnMouseEnter(nFlags, point);
		return true;
	}
	return false;
}

bool CSlideBtn::OnMouseLeave(UINT nFlags, CPoint point)
{
	if (CButton::OnMouseLeave(nFlags, point))
	{
		m_pParent->OnMouseLeave(nFlags, point);
		return true;
	}
	return false;
}
