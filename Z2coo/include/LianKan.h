#ifndef __LianKan_h__
#define __LianKan_h__

#include "Button.h"

class CLianKan : public CButton {
  public:
	CLianKan();
	CLianKan(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth);
	virtual ~CLianKan();

  public:
	void			 SetKind(int nKind);
	int				 GetKind() const;

  public:
	bool			 IsSelected() const;
	void			 OnSelect();
	void			 UnSelect();

  public:
	virtual void	 OnLClick(UINT nFlags, CPoint point);

  protected:
	int				 m_nKind;
	bool			 m_bSelected;
};

inline void CLianKan::SetKind(int nKind)
{
	m_nKind = nKind;
}

inline int CLianKan::GetKind() const
{
	return m_nKind;
}

inline bool CLianKan::IsSelected() const
{
	return m_bSelected;
}

#endif
