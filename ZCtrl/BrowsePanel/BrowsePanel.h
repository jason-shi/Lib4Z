#ifndef __BrowsePanel_h__
#define __BrowsePanel_h__

class CBrowseView;
class CBrowseTitle;
class CVScrollBar;
class CHScrollBar;
class CBrowsePanel : public CPanel {
  public:
	CBrowsePanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CBrowsePanel();

  public:
	void			 BindBrowseView(CBrowseView *pBrowseView);
	void			 BindBrowseTitle(CBrowseTitle *pBrowseTitle);
	void			 BindVScrollBar(CVScrollBar *pVScrollBar);
	void			 BindHScrollBar(CHScrollBar *pHScrollBar);

  public:
	virtual void	 UpdateVScrollBar();
	virtual void	 UpdateHScrollBar();

  public: //滚动条
	virtual void	 OnVScrollWindow(CScrollBar *pScrollBar, int nDelta);
	virtual void	 OnHScrollWindow(CScrollBar *pScrollBar, int nDelta);
	virtual void	 OnVSlideWindow(CScrollBar *pScrollBar, bool bMouseDrop, bool bAutoScroll);
	virtual void	 OnHSlideWindow(CScrollBar *pScrollBar, bool bMouseDrop, bool bAutoScroll);

  public: //滚轮
	virtual void	 OnMouseWheel(UINT nFlags, short zDelta, CPoint point);

  public: //标题
	virtual void	 OnAdjustWidth(CBrowseTitle *pBrowseTitle, int nItem, int nWidth);

  public: //视图
	virtual void	 OnUpdateView(CBrowseView *pBrowseView);

  protected:
	CBrowseView		*m_pBrowseView;
	CBrowseTitle	*m_pBrowseTitle;
	CVScrollBar		*m_pVScrollBar;
	CHScrollBar		*m_pHScrollBar;
};

#endif
