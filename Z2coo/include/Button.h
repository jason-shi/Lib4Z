#ifndef __Button_h__
#define __Button_h__

#include "Sprite.h"

enum {
	BTN_NORMAL,
	BTN_FOCUSED,
	BTN_PRESSED,
	BTN_DISABLE,
};

class CButton : public CSprite {
  public:
	CButton();
	CButton(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth);
	virtual ~CButton();

  public:
	void			 Set(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth);
	void			 SetSrcPos(CPoint xSrcPos);
	void			 SetSrcPos(int x, int y);
	void			 Disable(bool bDisable = true);

  public:
	CObject			*FindObject(const CPoint& point);
	CObject			*FindExcept(const CPoint& point, CObject *pExcept);

  public:
	void			 OnLoseFocus();
	void			 OnLButtonDown(UINT nFlags, CPoint point);
	void			 OnLButtonUp(UINT nFlags, CPoint point);
	bool			 OnMouseEnter(UINT nFlags, CPoint point);
	bool			 OnMouseLeave(UINT nFlags, CPoint point);
	void			 OnLClick(UINT nFlags, CPoint point);
	void			 OnRClick(UINT nFlags, CPoint point);
	void			 OnMClick(UINT nFlags, CPoint point);

  protected:
	void			 SetMapStyle();
	void			 SetMapSrcPos();

  protected:
	CRect			 m_xSrcRect; //”≥…‰‘¥
	int				 m_nMapStyle; //”≥…‰¿‡–Õ
	int				 m_nStatus; //∞¥≈•◊¥Ã¨
};

inline void CButton::SetSrcPos(CPoint xSrcPos)
{
	SetSrcPos(xSrcPos.x, xSrcPos.y);
}

#endif
