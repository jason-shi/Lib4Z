#ifndef __PhotoView_h__
#define __PhotoView_h__

#include <boost/function.hpp>

class CToolBar;
class CPhotoView : public CPanel {
	typedef boost::function<void(CPhotoView *)>			FN_UpdateView;
  public:
	CPhotoView(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CPhotoView();

  public:
	virtual void	 ShowPhotoView();
	virtual void	 UpdateView();

  public:
	virtual int		 GetFrontWidth() const;
	virtual int		 GetFrontHeight() const;
	virtual int		 GetFieldWidth() const;
	virtual int		 GetFieldHeight() const;
	virtual int		 GetTotalWidth() const;
	virtual int		 GetTotalHeight() const;

  public:
	virtual void	 OnVScrollWindow(int nDelta);
	virtual void	 OnHScrollWindow(int nDelta);
	virtual void	 OnVSlideWindow(int nFront, int nTotal);
	virtual void	 OnHSlideWindow(int nFront, int nTotal);

  public:
	void			 BindToolBar(CToolBar *pToolBar);

  public:
	void			 OnLClick(UINT nFlags, CPoint point);
	void			 OnRClick(UINT nFlags, CPoint point);
	void			 OnDrawRect(UINT nFlags, const CRect& rect);

  public:
	void			 SetUpdateView(FN_UpdateView UpdateView);

  public:
	FN_UpdateView	 OnUpdateView;

  protected:
	CToolBar		*m_pToolBar;
};

inline void CPhotoView::SetUpdateView(FN_UpdateView UpdateView)
{
	OnUpdateView = UpdateView;
}

inline void CPhotoView::BindToolBar(CToolBar *pToolBar)
{
	m_pToolBar = pToolBar;
}

#endif
