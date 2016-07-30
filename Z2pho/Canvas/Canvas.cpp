#include "StdAfx.h"
#include "Canvas.h"

CCanvas::CCanvas(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth)
			: CSprite(pImage, xDrawPos, xSize, nDepth)
{
}

CCanvas::~CCanvas()
{
}

void CCanvas::Set(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth)
{
	m_xDrawPos = xDrawPos;
	m_xSize = xSize;
	m_nDepth = nDepth;

	AllocImage(m_xSize.cx, m_xSize.cy);
}

void CCanvas::ResetSize(int nAlignMode, int nWidth, int nHeight)
{
	int nOldWidth = m_pImage->Width();
	int nOldHeight = m_pImage->Height();

	//备份图象
	CImage *pTemp = new CImage(nOldWidth, nOldHeight);
	pTemp->Copy(m_pImage);
	//调整旧图象的大小
	if (nOldWidth > nWidth)
		nOldWidth = nWidth;
	if (nOldHeight > nHeight)
		nOldHeight = nHeight;

	//重建图象
	AllocImage(nWidth, nHeight);

	//用背景色填充
	COLORREF bkcolor = g_pZ2pho->GetBkColor();
	m_pImage->FillRect(CRect(0, 0, nWidth, nHeight), bkcolor);

	//居中对齐时的偏移
	int nOffsetX = (nWidth - nOldWidth) / 2;
	int nOffsetY = (nHeight - nOldHeight) / 2;
	switch (nAlignMode)
	{
	case CNV_ALIGN_CENTER:
		m_pImage->Copy(pTemp, CRect(nOffsetX, nOffsetY, nOffsetX + nOldWidth, nOffsetY + nOldHeight), CPoint(0, 0));
		break;
	case CNV_ALIGN_LEFT:
		m_pImage->Copy(pTemp, CRect(0, nOffsetY, nOldWidth, nOffsetY + nOldHeight), CPoint(0, 0));
		break;
	case CNV_ALIGN_RIGHT:
		m_pImage->Copy(pTemp, CRect(nWidth - nOldWidth, nOffsetY, nWidth, nOffsetY + nOldHeight), CPoint(0, 0));
		break;
	case CNV_ALIGN_TOP:
		m_pImage->Copy(pTemp, CRect(nOffsetX, 0, nOffsetX + nOldWidth, nOldHeight), CPoint(0, 0));
		break;
	case CNV_ALIGN_BOTTOM:
		m_pImage->Copy(pTemp, CRect(nOffsetX, nHeight - nOldHeight, nOffsetX + nOldWidth, nHeight), CPoint(0, 0));
		break;
	case CNV_ALIGN_TOPLEFT:
		m_pImage->Copy(pTemp, CRect(0, 0, nOldWidth, nOldHeight), CPoint(0, 0));
		break;
	case CNV_ALIGN_TOPRIGHT:
		m_pImage->Copy(pTemp, CRect(nWidth - nOldWidth, 0, nWidth, nOldHeight), CPoint(0, 0));
		break;
	case CNV_ALIGN_BOTTOMLEFT:
		m_pImage->Copy(pTemp, CRect(0, nHeight - nOldHeight, nOldWidth, nHeight), CPoint(0, 0));
		break;
	case CNV_ALIGN_BOTTOMRIGHT:
		m_pImage->Copy(pTemp, CRect(nWidth - nOldWidth, nHeight - nOldHeight, nWidth, nHeight), CPoint(0, 0));
		break;
	}
	delete pTemp;

	//画布大小发生变化，并重绘
	g_pZ2coo->Redraw(this);
	m_xSize.cx = nWidth;
	m_xSize.cy = nHeight;
	g_pZ2coo->Redraw(this);
}

bool CCanvas::AllocImage(int nWidth, int nHeight)
{
	SAFE_DELETE(m_pImage);

	m_pImage = new CImage(nWidth, nHeight);
	return m_pImage != NULL;
}
