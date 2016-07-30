#ifndef __ScrollBar_h__
#define __ScrollBar_h__

#include <boost/function.hpp>

class CScrollBar;
typedef boost::function<void(CScrollBar *, int nDelta)>							FN_ScrollWindow;
typedef boost::function<void(CScrollBar *, bool bMouseDrop, bool bAutoScroll)>	FN_SlideWindow;

class CScrollBar : public CPanel {
  public:
	CScrollBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize);
	virtual ~CScrollBar();

  public:
	virtual void	 SetAutoScroll(bool bAutoScroll);
	virtual bool	 IsAutoScroll() const;
	void			 OnUpdateView(int nFront, int nView, int nTotal, bool bUpdate = true);

  protected:
	virtual bool	 Initial() = 0;
	virtual void	 UpdateSlideBar() = 0;

  public:
	virtual int		 GetSlideFront() const = 0;
	virtual int		 GetSlideTotal() const = 0;

  public:
	void			 SetSaltate(int nSaltate);
	int				 GetSaltate() const;
	void			 SetFN_ScrollWindow(FN_ScrollWindow ScrollWindow);
	void			 SetFN_SlideWindow(FN_SlideWindow SlideWindow);

  protected:
	bool			 m_bAutoScroll;
	int				 m_nFront;
	int				 m_nField;
	int				 m_nTotal;

  protected:
	int				 m_nSaltate;
	FN_ScrollWindow	 OnScrollWindow;
	FN_SlideWindow	 OnSlideWindow;
};

inline void CScrollBar::SetAutoScroll(bool bAutoScroll)
{
	m_bAutoScroll = bAutoScroll;
}

inline bool CScrollBar::IsAutoScroll() const
{
	return m_bAutoScroll;
}

inline void CScrollBar::SetSaltate(int nSaltate)
{
	m_nSaltate = nSaltate;
}

inline int CScrollBar::GetSaltate() const
{
	return m_nSaltate;
}

inline void CScrollBar::SetFN_ScrollWindow(FN_ScrollWindow ScrollWindow)
{
	OnScrollWindow = ScrollWindow;
}

inline void CScrollBar::SetFN_SlideWindow(FN_SlideWindow SlideWindow)
{
	OnSlideWindow = SlideWindow;
}

#endif
