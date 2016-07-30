#include "StdAfx.h"

CSprite::~CSprite()
{
}

void CSprite::SetImage(CImage *pImage)
{
	m_pImage = pImage;
	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

void CSprite::SetSrcPos(int x, int y)
{
	m_xSrcPos.x = x;
	m_xSrcPos.y = y;
	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

void CSprite::Draw(CImage *pImage)
{
	CRect r;
	GetRect(&r);
	pImage->MixImage(m_pImage, r, m_xSrcPos);
}

void CSprite::Draw(CImage *pImage, const CRect &rect)
{
	CRect r;
	GetRect(&r);
	r &= rect;
	if (!r.IsRectEmpty())
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
}

bool CSprite::PtInImage(CPoint pt)
{
	if (PtIn(pt) && m_pImage) {
		if ((m_pImage->GetPixel(pt + -m_xOffset + -m_xDrawPos + m_xSrcPos) & 0xFF000000) != 0x00000000)
			return true;
	}
	return false;
}
