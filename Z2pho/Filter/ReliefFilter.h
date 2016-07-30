#ifndef __ReliefFilter_h__
#define __ReliefFilter_h__

#include "Filter.h"

class CReliefFilter : public CFilter {
  public:
	CReliefFilter(float fAngle);
	virtual ~CReliefFilter();

  public:
	void			 Filter(CImage *pImage, const CRect& rect);

  protected:
	float			 m_fAngle;
};

#endif
