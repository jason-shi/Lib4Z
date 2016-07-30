#ifndef __HScrollBar_h__
#define __HScrollBar_h__

#include "ScrollBar.h"

class CHSlideBar;
class CHScrollBar : public CScrollBar {
  public:
	CHScrollBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize);
	virtual ~CHScrollBar();

  public:
	bool			 Initial();
	void			 Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual void	 SetWidth(int nBarWidth);
	virtual void	 UpdateSlideBar();

  private:
	void			 _UpdateSlideBar(int nOffset, int nWidth); //指定位置和宽度
	void			 _UpdateSlideBar(UINT nFlags, CPoint point); //按滑动方式更新

  public:
	int				 GetSlideFront() const;
	int				 GetSlideTotal() const;

  protected:
	void			 OnScrollLeft(CObject *pButton);
	void			 OnScrollRight(CObject *pButton);
	void			 OnSaltateLeft(CObject *pButton);
	void			 OnSaltateRight(CObject *pButton);

  protected:
	void			 OnMousePick(CObject *pSlideBar, UINT nFlags, CPoint point);
	void			 OnMouseDrag(CObject *pSlideBar, UINT nFlags, CPoint point);
	void			 OnMouseDrop(CObject *pSlideBar, UINT nFlags, CPoint point);

  protected:
	CSprite			*m_pLDcrt;
	CButton			*m_pLArrow;
	CButton			*m_pLPad;
	CHSlideBar		*m_pSlideBar;
	CButton			*m_pRPad;
	CButton			*m_pRArrow;
	CSprite			*m_pRDcrt;
};

inline int CHScrollBar::GetSlideFront() const
{
	int nMargin = 2 + 9;
	return m_pSlideBar->GetDrawPos().x - nMargin;
}

inline int CHScrollBar::GetSlideTotal() const
{
	int nMargin = 2 + 9;
	return m_xSize.cx - 2 * nMargin;
}

#endif
