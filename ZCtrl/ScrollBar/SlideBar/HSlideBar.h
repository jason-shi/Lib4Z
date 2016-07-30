#ifndef __HSlideBar_h__
#define __HSlideBar_h__

#include "SlideBar.h"

class CSlideBtn;
class CHSlideBar : public CSlideBar {
  public:
	CHSlideBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize);
	virtual ~CHSlideBar();

  public:
	bool			 Initial();
	void			 Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual void	 SetWidth(int nBarWidth);

  protected:
	CSlideBtn		*m_pLCap;
	CSlideBtn		*m_pLPad;
	CSlideBtn		*m_pCore;
	CSlideBtn		*m_pRPad;
	CSlideBtn		*m_pRCap;
};

#endif
