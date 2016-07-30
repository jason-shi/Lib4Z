#ifndef __WarCraft_h__
#define __WarCraft_h__

#include "Button.h"

class CWarCraft : public CButton {
  public:
	CWarCraft();
	CWarCraft(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth);
	virtual ~CWarCraft();

  public:
	void			 SetKind(int nKind);
	int				 GetKind() const;

  public:
	bool			 IsSelected() const;
	void			 OnSelect();
	void			 UnSelect();

  public:
	virtual bool	 RectIn(const CRect& rect);
	virtual void	 OnLClick(UINT nFlags, CPoint point);
	virtual void	 OnLDblClk(UINT nFlags, CPoint point);

  protected:
	int				 m_nKind;
	bool			 m_bSelected;
};

inline void CWarCraft::SetKind(int nKind)
{
	m_nKind = nKind;
}

inline int CWarCraft::GetKind() const
{
	return m_nKind;
}

inline bool CWarCraft::IsSelected() const
{
	return m_bSelected;
}

#endif
