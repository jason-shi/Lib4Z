#ifndef __VScrollBar_h__
#define __VScrollBar_h__

#include "ScrollBar.h"

class CVSlideBar;
class CVScrollBar : public CScrollBar {
  public:
	CVScrollBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize);
	virtual ~CVScrollBar();

  public:
	bool			 Initial();
	void			 Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual void	 SetHeight(int nBarHeight);
	virtual void	 UpdateSlideBar();

  private:
	void			 _UpdateSlideBar(int nOffset, int nHeight); //指定位置和高度
	void			 _UpdateSlideBar(UINT nFlags, CPoint point); //按滑动方式更新

  public:
	int				 GetSlideFront() const;
	int				 GetSlideTotal() const;

  protected:
	void			 OnScrollUp(CObject *pButton);
	void			 OnScrollDown(CObject *pButton);
	void			 OnSaltateUp(CObject *pButton);
	void			 OnSaltateDown(CObject *pButton);

  protected:
	void			 OnMousePick(CObject *pSlideBar, UINT nFlags, CPoint point);
	void			 OnMouseDrag(CObject *pSlideBar, UINT nFlags, CPoint point);
	void			 OnMouseDrop(CObject *pSlideBar, UINT nFlags, CPoint point);

  protected:
	CSprite			*m_pUDcrt;
	CButton			*m_pUArrow;
	CButton			*m_pUPad;
	CVSlideBar		*m_pSlideBar;
	CButton			*m_pDPad;
	CButton			*m_pDArrow;
	CSprite			*m_pDDcrt;
};

inline int CVScrollBar::GetSlideFront() const
{
	int nMargin = 2 + 9;
	return m_pSlideBar->GetDrawPos().y - nMargin;
}

inline int CVScrollBar::GetSlideTotal() const
{
	int nMargin = 2 + 9;
	return m_xSize.cy - 2 * nMargin;
}

#endif
