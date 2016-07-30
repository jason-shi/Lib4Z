#ifndef __BasicBrush_h__
#define __BasicBrush_h__

#include "Brush.h"

class CBasicBrush : public CBrush {
  public:
	CBasicBrush(int nDiameter);
	virtual ~CBasicBrush();

  public:
	void			 SetDiameter(int nDiameter);

  public:
	void			 InitBrush();
	void			 Draw(CCanvas *pCanvas);

  protected:
	int				 m_nDiameter;
};

inline void CBasicBrush::SetDiameter(int nDiameter)
{
	m_nDiameter = nDiameter;
}

#endif
