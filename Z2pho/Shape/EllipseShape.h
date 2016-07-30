#ifndef __EllipseShape_h__
#define __EllipseShape_h__

#include "Shape.h"

class CEllipseShape : public CShape {
  public:
	CEllipseShape(int xCenter, int yCenter, int Rx, int Ry);
	virtual ~CEllipseShape();

  public:
	void			 Set(int xCenter, int yCenter, int Rx, int Ry);

  private:
	void			 _Draw(CCanvas *pCanvas);
	void			 _Fill(CCanvas *pCanvas);

  private:
	void			 _EllipsePlot(int xCenter, int yCenter, int x, int y);
	void			 _EllipseFill(int xCenter, int yCenter, int x, int y);

  protected:
	int				 xCenter;
	int				 yCenter;
	int				 Rx;
	int				 Ry;
};

inline void CEllipseShape::Set(int xCenter, int yCenter, int Rx, int Ry)
{
	this->xCenter = xCenter;
	this->yCenter = yCenter;
	this->Rx = Rx;
	this->Ry = Ry;
}

#endif
