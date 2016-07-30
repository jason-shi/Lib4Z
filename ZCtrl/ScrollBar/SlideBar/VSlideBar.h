#ifndef __VSlideBar_h__
#define __VSlideBar_h__

#include "SlideBar.h"

class CSlideBtn;
class CVSlideBar : public CSlideBar {
  public:
	CVSlideBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize);
	virtual ~CVSlideBar();

  public:
	bool			 Initial();
	void			 Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual void	 SetHeight(int nBarHeight);

  protected:
	CSlideBtn		*m_pUCap;
	CSlideBtn		*m_pUPad;
	CSlideBtn		*m_pCore;
	CSlideBtn		*m_pDPad;
	CSlideBtn		*m_pDCap;
};

#endif
