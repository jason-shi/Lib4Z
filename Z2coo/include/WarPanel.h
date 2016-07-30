#ifndef __WarPanel_h__
#define __WarPanel_h__

#include "Panel.h"

class CWarCraft;
class CWarPanel : public CPanel {
	typedef std::vector<CWarCraft *>		VECWAR;
  public:
	CWarPanel(CImage *pImage, CPoint point, CSize size, int nDepth);
	virtual ~CWarPanel();

  public:
	void			 SetSelectLimit(int nSelectLimit);
	void			 AddVisual(CWarCraft *pWarCraft);
	void			 Select(CWarCraft *pWarCraft);
	void			 Select(int nKind);
	void			 SelectVisual(const CRect& rect);
	void			 UnSelect(CWarCraft *pWarCraft);
	void			 UnSelect(int nKind);
	void			 UnSelect();
	void			 SetDrawThrs(int nDrawThrs);
	CRect			 GetDrawingRect() const;
	VECWAR&			 GetSelected() { return m_vecSelect; }

  public:
	bool			 OnMouseEnter(UINT nFlags, CPoint point);
	bool			 OnMouseLeave(UINT nFlags, CPoint point);
	void			 OnLClick(UINT nFlags, CPoint point);
	bool			 CheckDrawMode(UINT nFlags);
	bool			 CheckDrawThrs(const CRect& rect);
	void			 OnDrawRect(UINT nFlags, const CRect& rect);
	void			 OnDrawOver(UINT nFlags, const CRect& rect);

  private:
	int				 m_nSelectLimit;
	int				 m_nDrawThrs;
	CRect			 m_xDrawingRect;
	VECWAR			 m_vecVisual;
	VECWAR			 m_vecSelect;
};

inline void CWarPanel::SetSelectLimit(int nSelectLimit)
{
	m_nSelectLimit = nSelectLimit;
}

inline void CWarPanel::SetDrawThrs(int nDrawThrs)
{
	m_nDrawThrs = nDrawThrs;
}

inline CRect CWarPanel::GetDrawingRect() const
{
	return m_xDrawingRect;
}

inline bool CWarPanel::CheckDrawThrs(const CRect& rect)
{
	return rect.Width() > m_nDrawThrs || rect.Height() > m_nDrawThrs;
}

#endif
