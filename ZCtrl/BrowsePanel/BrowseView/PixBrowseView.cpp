#include "StdAfx.h"
#include "PixBrowseView.h"

CPixBrowseView::CPixBrowseView(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
					: CBrowseView(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
}

CPixBrowseView::~CPixBrowseView()
{
}

void CPixBrowseView::ShowBrowseView()
{
}

int CPixBrowseView::GetFrontHeight() const
{
	return 0;
}

void CPixBrowseView::OnVScrollWindow(int nDelta)
{
}

void CPixBrowseView::OnHScrollWindow(int nDelta)
{
}

void CPixBrowseView::OnVSlideWindow(int nFront, int nTotal)
{
}

void CPixBrowseView::OnHSlideWindow(int nFront, int nTotal)
{
}
