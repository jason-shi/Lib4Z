#ifndef __TriangleShape_h__
#define __TriangleShape_h__

#include "Shape.h"

class CTriangleShape : public CShape {
  public:
	CTriangleShape(int nCount, LPPOINT lpPoints);
	~CTriangleShape();

  public:
	void			 Set(int nCount, LPPOINT lpPoints);

  private:
	void			 _Draw(CCanvas *pCanvas);
	void			 _Fill(CCanvas *pCanvas);

  protected:
	int				 m_nCount;
	LPPOINT			 lpPoints;
};

inline void CTriangleShape::Set(int nCount, LPPOINT lpPoints)
{
	this->m_nCount = nCount;
	this->lpPoints = lpPoints;
}

#endif
