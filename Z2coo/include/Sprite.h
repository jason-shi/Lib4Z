#ifndef	__Sprite_h__
#define	__Sprite_h__

#include "Object.h"

class CImage;
class CSprite : public CObject {
  public:
	CSprite();
	CSprite(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth);
	CSprite(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	CSprite(CImage *pImage, const CRect& xRect, CPoint xSrcPos, int nDepth);
	virtual ~CSprite();

  public:
	void			 Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	void			 SetImage(CImage *pImage);
	CImage			*GetImage() const;
	void			 SetSrcPos(CPoint xSrcPos);
	void			 SetSrcPos(int x, int y);
	CPoint			 GetSrcPos() const;
	void			 Draw(CImage *pImage);
	void			 Draw(CImage *pImage, const CRect &rect);
	bool			 PtInImage(CPoint pt);
	bool			 operator<(const CSprite &SpriteCmp) const;

  protected:
	CImage			*m_pImage;
	CPoint			 m_xSrcPos;
};

inline CSprite::CSprite() : CObject()
{
	m_pImage = NULL;
}

inline CSprite::CSprite(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth)
					: CObject(xDrawPos, xSize, nDepth)
{
	m_pImage = pImage;
	m_xSrcPos = CPoint(0, 0);
}

inline CSprite::CSprite(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
					: CObject(xDrawPos, xSize, nDepth)
{
	m_pImage = pImage;
	m_xSrcPos = xSrcPos;
}

inline CSprite::CSprite(CImage *pImage, const CRect& xRect, CPoint xSrcPos, int nDepth)
					: CObject(xRect.TopLeft(), xRect.Size(), nDepth)
{
	m_pImage = pImage;
	m_xSrcPos = xSrcPos;
}

//将参数设定在构造好的对象上
inline void CSprite::Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
{
	m_pImage = pImage;
	m_xDrawPos = xDrawPos;
	m_xSrcPos = xSrcPos;
	m_xSize = xSize;
	m_nDepth = nDepth;
}

inline CImage* CSprite::GetImage() const
{
	return m_pImage;
}

inline void CSprite::SetSrcPos(CPoint xSrcPos)
{
	SetSrcPos(xSrcPos.x, xSrcPos.y);
}

inline CPoint CSprite::GetSrcPos() const
{
	return m_xSrcPos;
}

inline bool CSprite::operator<(const CSprite &SpriteCmp) const
{
	int y1 = m_xDrawPos.y + m_xSize.cy;
	int y2 = SpriteCmp.m_xDrawPos.y + SpriteCmp.m_xSize.cy;
	if (y1 == y2) {
		int x1 = m_xDrawPos.x + (m_xSize.cx + 1) / 2;
		int x2 = SpriteCmp.m_xDrawPos.x + (SpriteCmp.m_xSize.cx + 1) / 2;
		if (x1 == x2) {
			return m_nDepth < SpriteCmp.m_nDepth;
		}
		return x1 < x2;
	}
	return y1 < y2;
}

#endif
