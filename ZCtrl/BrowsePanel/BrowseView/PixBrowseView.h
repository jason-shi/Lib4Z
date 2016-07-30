#ifndef __PixBrowseView_h__
#define __PixBrowseView_h__

#include "BrowseView.h"

class CPixBrowseView : public CBrowseView {
  public:
	CPixBrowseView(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CPixBrowseView();

  public:
	void			 ShowBrowseView();

  public:
	int				 GetFrontHeight() const;

  public:
	void			 OnVScrollWindow(int nDelta);
	void			 OnHScrollWindow(int nDelta);
	void			 OnVSlideWindow(int nFront, int nTotal);
	void			 OnHSlideWindow(int nFront, int nTotal);
};

#endif
