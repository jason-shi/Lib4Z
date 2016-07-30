#ifndef __NovaCDS_h__
#define __NovaCDS_h__

#include "NewtonCDS.h"

class CNovaCDS : public CNewtonCDS {
  public:
	//z = z - R * f(z) / f'(z) + Z0, f(z) = power(z, n) + C0
	CNovaCDS(int N, const CComplex& C0, const CComplex& R, const CComplex& Z0);
	virtual ~CNovaCDS();

  protected:
	virtual void	 IterFormula(const CComplex& c1, CComplex& c2);

  protected:
	CComplex		 Z0; //起始迭代值
	CComplex		 R; //松弛变量
};

inline void CNovaCDS::IterFormula(const CComplex& c1, CComplex& c2)
{
	CComplex cTemp = cpower(c1, N - 1);
	c2 = c1 - R * (c1 * cTemp + C0) / (N * cTemp) + Z0;
}

#endif
