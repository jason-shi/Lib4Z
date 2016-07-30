#ifndef __LianPanel_h__
#define __LianPanel_h__

#include "Panel.h"

class CLianKan;
class CLianPanel : public CPanel {
  public:
	CLianPanel();
	CLianPanel(CImage *pImage, CPoint point, CSize size, int nDepth);
	virtual ~CLianPanel();

  public:
	void			 Select(CLianKan *pLianKan);
	void			 UnSelect();
	void			 OnLClick(CLianKan *pLianKan);
	void			 OnRemoveSame(CLianKan *pObject1, CLianKan *pObject2);

  public:
	bool			 OnMouseEnter(UINT nFlags, CPoint point);
	bool			 OnMouseLeave(UINT nFlags, CPoint point);
	void			 OnLClick(UINT nFlags, CPoint point);

  protected:
	CLianKan		*m_pLianKan;
};

#endif
