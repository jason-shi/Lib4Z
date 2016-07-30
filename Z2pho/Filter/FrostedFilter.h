#ifndef __FrostedFilter_h__
#define __FrostedFilter_h__

#include "Filter.h"

class CFrostedFilter : public CFilter {
  public:
	CFrostedFilter(int nMinRadius, int nMaxRadius, int nSamples);
	virtual ~CFrostedFilter();

  public:
	void			 Filter(CImage *pImage, const CRect& rect);

  protected:
	COLORREF		 BlendColors(COLORREF colors[], int nLength);

  protected:
	int				 m_nMinRadius;
	int				 m_nMaxRadius;
	int				 m_nSamples;
};

#endif
