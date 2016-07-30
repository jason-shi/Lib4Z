#ifndef __ImageBrush_h__
#define __ImageBrush_h__

#include "Brush.h"

class CImageBrush : public CBrush {
  public:
	CImageBrush(CImage *pImage);
	virtual ~CImageBrush();

  public:
	void			 InitBrush();
	void			 Draw(CCanvas *pCanvas);

  protected:
	CImage			*m_pImage;
};

#endif
