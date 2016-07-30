#include "StdAfx.h"

CPanel::CPanel() : CContainer()
{
	m_pImage = NULL;
}

CPanel::CPanel(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth)
			: CContainer(xDrawPos, xSize, nDepth)
{
	m_pImage = pImage;
	m_xSrcPos = CPoint(0, 0);
}

CPanel::CPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
			: CContainer(xDrawPos, xSize, nDepth)
{
	m_pImage = pImage;
	m_xSrcPos = xSrcPos;
}

CPanel::~CPanel()
{
}

void CPanel::SetImage(CImage *pImage)
{
	m_pImage = pImage;
	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

void CPanel::SetSrcPos(int x, int y)
{
	m_xSrcPos.x = x;
	m_xSrcPos.y = y;
	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

void CPanel::Draw(CImage* pImage)
{
	CRect rect;
	GetRect(&rect);
	if (m_pImage != NULL)
		pImage->MixImage(m_pImage, rect, m_xSrcPos);
	CContainer::Draw(pImage);
}

void CPanel::Draw(CImage *pImage, const CRect& rect)
{
	CRect r;
	GetRect(&r);
	r &= rect;
	if (!r.IsRectEmpty())
	{
		//首先绘制自身
		if (m_pImage != NULL)
		{
			CPoint src = m_xSrcPos;
			if (m_xOffset.x + m_xDrawPos.x < r.left)
			{
				src.x += r.left - (m_xOffset.x + m_xDrawPos.x);
			}
			if (m_xOffset.y + m_xDrawPos.y < r.top)
			{
				src.y += r.top - (m_xOffset.y + m_xDrawPos.y);
			}
			pImage->MixImage(m_pImage, r, src);
		}
		CContainer::Draw(pImage, r);
	}
}

bool CPanel::PtInImage(CPoint pt)
{
	if (PtIn(pt) && m_pImage) {
		if ((m_pImage->GetPixel(pt + -m_xOffset + -m_xDrawPos + m_xSrcPos) & 0xFF000000) != 0x00000000)
			return true;
	}
	return false;
}

bool CPanel::AllocImage(int nWidth, int nHeight)
{
	SAFE_DELETE(m_pImage);

	m_pImage = new CImage(nWidth, nHeight);
	return m_pImage != NULL;
}

void CPanel::ClearImage()
{
	m_pImage->Clear();
	g_pZ2coo->Redraw(this);
}

CObject* CPanel::FindObject(const CPoint& point)
{
	CObject *pObject = CContainer::FindObject(point);
	if (pObject)
		return pObject;

	//自身响应
	if (m_bCease || PtInImage(point))
		return this;

	return NULL;
}

CObject* CPanel::FindExcept(const CPoint& point, CObject *pExcept)
{
	CObject *pObject = CContainer::FindObject(point);
	if (pObject)
		return pObject;

	//本身是排除对象
	if (pExcept == this)
		return NULL;

	//自身响应
	if (m_bCease || PtInImage(point))
		return this;

	return NULL;
}
