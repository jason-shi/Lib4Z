#ifndef __LineShape_h__
#define __LineShape_h__

#include "Shape.h"

class CLineShape : public CShape {
  public:
	CLineShape(int x1, int y1, int x2, int y2);
	CLineShape(const CPoint& pt1, const CPoint& pt2);
	virtual ~CLineShape();

  public:
	void			 Set(int x1, int y1, int x2, int y2);
	void			 Set(const CPoint& pt1, const CPoint& pt2);

  private:
	void			 _Draw(CCanvas *pCanvas);
	void			 _Fill(CCanvas *pCanvas);

  protected:
	int				 x1;
	int				 y1;
	int				 x2;
	int				 y2;
};

inline void CLineShape::Set(int x1, int y1, int x2, int y2)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}

inline void CLineShape::Set(const CPoint& pt1, const CPoint& pt2)
{
	Set(pt1.x, pt1.y, pt2.x, pt2.y);
}

#endif
