#ifndef __PhotoPanel_h__
#define __PhotoPanel_h__

class CPhotoView;
class CVScrollBar;
class CHScrollBar;
class CPhotoPanel : public CPanel {
  public:
	CPhotoPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CPhotoPanel();

  public:
	void			 BindPhotoView(CPhotoView *pPhotoView);
	void			 BindVScrollBar(CVScrollBar *pVScrollBar);
	void			 BindHScrollBar(CHScrollBar *pHScrollBar);

  public:
	virtual void	 UpdateVScrollBar();
	virtual void	 UpdateHScrollBar();

  public: //¹ö¶¯Ìõ
	virtual void	 OnVScrollWindow(CScrollBar *pScrollBar, int nDelta);
	virtual void	 OnHScrollWindow(CScrollBar *pScrollBar, int nDelta);
	virtual void	 OnVSlideWindow(CScrollBar *pScrollBar, bool bMouseDrop, bool bAutoScroll);
	virtual void	 OnHSlideWindow(CScrollBar *pScrollBar, bool bMouseDrop, bool bAutoScroll);

  public: //¹öÂÖ
	virtual void	 OnMouseWheel(UINT nFlags, short zDelta, CPoint point);

  public: //ÊÓÍ¼
	virtual void	 OnUpdateView(CPhotoView *pPhotoView);

  protected:
	CPhotoView		*m_pPhotoView;
	CVScrollBar		*m_pVScrollBar;
	CHScrollBar		*m_pHScrollBar;
};

#endif
