#ifndef __Panel_h__
#define __Panel_h__

#include "Container.h"

class CImage;
class CPanel : public CContainer {
  public:
	CPanel();
	CPanel(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth);
	CPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CPanel();

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
	void			 ClearImage();

  protected:
	bool			 AllocImage(int nWidth, int nHeight);

  public:
	virtual bool	 OnMouseEnter(UINT nFlags, CPoint point);
	virtual bool	 OnMouseLeave(UINT nFlags, CPoint point);

  public:
	virtual CObject	*FindObject(const CPoint& point);
	virtual CObject	*FindExcept(const CPoint& point, CObject *pExcept);

  protected:
	CImage			*m_pImage;
	CPoint			 m_xSrcPos;
};

//将参数设定在构造好的对象上
inline void CPanel::Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
{
	m_pImage = pImage;
	m_xDrawPos = xDrawPos;
	m_xSrcPos = xSrcPos;
	m_xSize = xSize;
	m_nDepth = nDepth;
}

inline CImage* CPanel::GetImage() const
{
	return m_pImage;
}

inline void CPanel::SetSrcPos(CPoint xSrcPos)
{
	SetSrcPos(xSrcPos.x, xSrcPos.y);
}

inline CPoint CPanel::GetSrcPos() const
{
	return m_xSrcPos;
}

inline bool CPanel::OnMouseEnter(UINT nFlags, CPoint point)
{
	CContainer::OnMouseEnter(nFlags, point);
	return true;
}

inline bool CPanel::OnMouseLeave(UINT nFlags, CPoint point)
{
	CContainer::OnMouseLeave(nFlags, point);
	return true;
}

#endif
