#ifndef __Filter_h__
#define __Filter_h__

class CFilter {
  public:
	CFilter();
	virtual ~CFilter();

  protected:
	virtual void	 Filter(CImage *pImage, const CRect& rect) = 0;
};

#endif
