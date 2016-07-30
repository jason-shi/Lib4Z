#ifndef __LinScrollWindow_h__
#define __LinScrollWindow_h__

#include "BrowseView.h"

class CBrowseBar;
class CLinBrowseView : public CBrowseView {
  public:
	CLinBrowseView(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CLinBrowseView();

  public:
	bool			 InitBrowseBar(int nCapacity, CImage *pImage, CPoint xSrcPos, CSize xSize, int nDepth);
	void			 ShowBrowseView();
	void			 UpdateView();

  protected:
	virtual CBrowseBar		*CreateBrowseBar(CImage *pImage, CPoint xSrcPos, CSize xSize, int nDepth) = 0;

  public:
	int				 GetFrontWidth() const;
	int				 GetFrontHeight() const;
	int				 GetFieldWidth() const;
	int				 GetFieldHeight() const;
	int				 GetTotalWidth() const;
	int				 GetTotalHeight() const;

  public:
	void			 OnVScrollWindow(int nDelta);
	void			 OnHScrollWindow(int nDelta);
	void			 OnVSlideWindow(int nFront, int nTotal);
	void			 OnHSlideWindow(int nFront, int nTotal);

  public:
	void			 OnAdjustWidth(int nItem, int nWidth);

  protected:
	CBrowseBar	   **m_pBrowseBar;
	int				 m_nCapacity; //要确保浏览条超过View的范围

  protected:
	int				 m_nBrowseLine;
	int				 m_nBrowseItem;
};

#endif
