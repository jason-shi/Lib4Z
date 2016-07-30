#ifndef __NeonFilter_h__
#define __NeonFilter_h__

#include "Filter.h"

class CNeonFilter : public CFilter {
  public:
	CNeonFilter(int nWidth, float fFactor);
	virtual ~CNeonFilter();

  public:
	void			 Filter(CImage *pImage, const CRect& rect);

  protected:
	int				 m_nWidth;
	float			 m_fFactor;
};

#endif
