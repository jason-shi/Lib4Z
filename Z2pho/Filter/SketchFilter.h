#ifndef __SketchFilter_h__
#define __SketchFilter_h__

#include "Filter.h"

class CSketchFilter : public CFilter {
  public:
	CSketchFilter(int nOutline, COLORREF color);
	virtual ~CSketchFilter();

  public:
	void			 Filter(CImage *pImage, const CRect& rect);

  protected:
	int				 m_nOutline;
	COLORREF		 m_color;
};

#endif
