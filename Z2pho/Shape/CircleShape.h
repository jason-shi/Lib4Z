#ifndef __CircleShape_h__
#define __CircleShape_h__

#include "Shape.h"

class CCircleShape : public CShape {
  public:
	CCircleShape(int xCenter, int yCenter, int Rc);
	virtual ~CCircleShape();

  public:
	void			 Set(int xCenter, int yCenter, int Rc);

  private:
	void			 _Draw(CCanvas *pCanvas);
	void			 _Fill(CCanvas *pCanvas);

  private:
	void			 _CirclePlot(int xCenter, int yCenter, int x, int y);
	void			 _CircleFill(int xCenter, int yCenter, int x, int y);

  protected:
	int				 xCenter;
	int				 yCenter;
	int				 Rc;
};

inline void CCircleShape::Set(int xCenter, int yCenter, int Rc)
{
	this->xCenter = xCenter;
	this->yCenter = yCenter;
	this->Rc = Rc;
}

#endif
