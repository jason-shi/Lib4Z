#ifndef __SlideBar_h__
#define __SlideBar_h__

class CSlideBtn;
class CSlideBar : public CPanel {
	typedef std::vector<CSlideBtn *>		VECBUTTON;
  public:
	CSlideBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize);
	virtual ~CSlideBar();

  public:
	virtual bool	 Initial() = 0;

  protected:
	void			 OnLButtonDown(UINT nFlags, CPoint point);
	void			 OnLButtonUp(UINT nFlags, CPoint point);
	bool			 OnMouseEnter(UINT nFlags, CPoint point);
	bool			 OnMouseLeave(UINT nFlags, CPoint point);

  protected:
	VECBUTTON		 m_vecButton;
	CPoint			 m_xDragPoint;
};

#endif
