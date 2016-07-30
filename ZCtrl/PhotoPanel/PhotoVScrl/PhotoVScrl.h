#ifndef __PhotoVScrl_h__
#define __PhotoVScrl_h__

#include "ScrollBar\VScrollBar.h"

class CPhotoVScrl : public CVScrollBar {
  public:
	CPhotoVScrl(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize);
	virtual ~CPhotoVScrl();

  public:
	bool			 IsAutoScroll() const;
};

inline bool CPhotoVScrl::IsAutoScroll() const
{
	return false;
}

#endif
