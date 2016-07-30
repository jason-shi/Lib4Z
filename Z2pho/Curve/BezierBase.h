#ifndef __BezierBase_h__
#define __BezierBase_h__

#include "Curve.h"

#define MaxBezierDegree		16

class CBezierBase : public CCurve {
  protected:
	struct Span {
		int				 ptx[MaxBezierDegree];
		int				 pty[MaxBezierDegree];
	};

  public:
	CBezierBase(int nCount, LPPOINT lpPoints);
	virtual ~CBezierBase();

  public:
	virtual void	 Draw(CCanvas *pCanvas);

  protected:
	virtual void	 DeCasteljau(int ptx[], int pty[]) = 0;
	virtual bool	 CheckTermination(int ptx[], int pty[]) = 0;

  protected:
	int				 m_nCount;
	LPPOINT			 m_lpPoints;

  protected:
	Span			 m_span;
	std::vector<Span>	 m_stack;
};

#endif
