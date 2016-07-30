#ifndef __SlideBtn_h__
#define __SlideBtn_h__

class CSlideBtn : public CButton {
  public:
	CSlideBtn(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth);
	virtual ~CSlideBtn();

  public:
	void			 SetWidth(int nWidth);
	void			 SetHeight(int nHeight);

  public:
	void			 OnLButtonDown(UINT nFlags, CPoint point);
	bool			 OnMouseEnter(UINT nFlags, CPoint point);
	bool			 OnMouseLeave(UINT nFlags, CPoint point);

  protected:
	int				 GetDragThrs() const;
	bool			 CheckDragMode(UINT nFlags);

  protected:
	void			 OnMousePick(UINT nFlags, CPoint point);
	void			 OnMouseDrag(UINT nFlags, CPoint point);
	void			 OnMouseDrop(UINT nFlags, CPoint point);
};

inline int CSlideBtn::GetDragThrs() const
{
	return 3; //мо╤╞цеоч
}

inline bool CSlideBtn::CheckDragMode(UINT nFlags)
{
	return (nFlags & MK_LBUTTON) != 0;
}

inline void CSlideBtn::OnMousePick(UINT nFlags, CPoint point)
{
	m_pHome->OnMousePick(nFlags, point);
}

inline void CSlideBtn::OnMouseDrag(UINT nFlags, CPoint point)
{
	m_pHome->OnMouseDrag(nFlags, point);
}

inline void CSlideBtn::OnMouseDrop(UINT nFlags, CPoint point)
{
	m_pHome->OnMouseDrop(nFlags, point);
}

#endif
