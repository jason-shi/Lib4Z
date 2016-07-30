#include "StdAfx.h"
#include "PhotoView.h"
#include "ToolBar\ToolBar.h"

CPhotoView::CPhotoView(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pToolBar = NULL;
}

CPhotoView::~CPhotoView()
{
}

void CPhotoView::ShowPhotoView()
{
	g_pZ2coo->Redraw(this);
}

void CPhotoView::UpdateView()
{
}

int CPhotoView::GetFrontWidth() const
{
	return m_xSrcPos.x;
}

int CPhotoView::GetFrontHeight() const
{
	return m_xSrcPos.y;
}

int CPhotoView::GetFieldWidth() const
{
	return m_xSize.cx;
}

int CPhotoView::GetFieldHeight() const
{
	return m_xSize.cy;
}

int CPhotoView::GetTotalWidth() const
{
	if (!m_pImage)
		return m_xSize.cx;

	return m_pImage->Width();
}

int CPhotoView::GetTotalHeight() const
{
	if (!m_pImage)
		return m_xSize.cy;

	return m_pImage->Height();
}

void CPhotoView::OnVScrollWindow(int nDelta)
{
	m_xSrcPos.y += nDelta;
	if (m_xSrcPos.y < 0)
		m_xSrcPos.y = 0;
	if (m_xSrcPos.y > m_pImage->Height() - m_xSize.cy)
		m_xSrcPos.y = m_pImage->Height() - m_xSize.cy;

	ShowPhotoView();
}

void CPhotoView::OnHScrollWindow(int nDelta)
{
	m_xSrcPos.x += nDelta;
	if (m_xSrcPos.x < 0)
		m_xSrcPos.x = 0;
	if (m_xSrcPos.x > m_pImage->Width() - m_xSize.cx)
		m_xSrcPos.x = m_pImage->Width() - m_xSize.cx;

	ShowPhotoView();
}

void CPhotoView::OnVSlideWindow(int nFront, int nTotal)
{
	m_xSrcPos.y = (float)nFront / nTotal * m_pImage->Height();
	if (m_xSrcPos.y < 0)
		m_xSrcPos.y = 0;
	if (m_xSrcPos.y > m_pImage->Height() - m_xSize.cy)
		m_xSrcPos.y = m_pImage->Height() - m_xSize.cy;

	ShowPhotoView();
}

void CPhotoView::OnHSlideWindow(int nFront, int nTotal)
{
	m_xSrcPos.x = (float)nFront / nTotal * m_pImage->Width();
	if (m_xSrcPos.x < 0)
		m_xSrcPos.x = 0;
	if (m_xSrcPos.x > m_pImage->Width() - m_xSize.cx)
		m_xSrcPos.x = m_pImage->Width() - m_xSize.cx;

	ShowPhotoView();
}

void CPhotoView::OnLClick(UINT nFlags, CPoint point)
{
	if (m_pToolBar)
		m_pToolBar->ProcessLClick(this, nFlags, point);
}

void CPhotoView::OnRClick(UINT nFlags, CPoint point)
{
	if (m_pToolBar)
		m_pToolBar->ProcessRClick(this, nFlags, point);
}

void CPhotoView::OnDrawRect(UINT nFlags, const CRect& rect)
{
	if (m_pToolBar)
		m_pToolBar->ProcessDrawRect(this, nFlags, rect);
}
