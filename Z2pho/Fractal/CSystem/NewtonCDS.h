#ifndef __NewtonCDS_h__
#define __NewtonCDS_h__

class CNewtonCDS {
  public:
	CNewtonCDS(int N, const CComplex& C0); //z = z - f(z) / f'(z), f(z) = power(z, n) + C0
	virtual ~CNewtonCDS();

  public:
	void			 SetIterArgs(int nMaxIter, int nMinIter);
	void			 SetScope(const CComplex& start, const CComplex& end);
	void			 SetRender(COLORREF color, float fFactor);
	int				 IterBrief(CComplex cinit, CComplex& ret, double dL[4]); //迭代过程
	void			 Draw(CCanvas *pCanvas);

  protected:
	virtual void	 IterFormula(const CComplex& c1, CComplex& c2);
	virtual COLORREF GetColor(CComplex cinit);

  protected:
	int				 N;
	CComplex		 C0;
	int				 m_nMaxIter;
	int				 m_nMinIter;
	CComplex		 m_start;
	CComplex		 m_end;
	COLORREF		 m_color;
	float			 m_fFactor; //渐变调节因子
};

inline void CNewtonCDS::SetIterArgs(int nMaxIter, int nMinIter)
{
	m_nMaxIter = nMaxIter;
	m_nMinIter = nMinIter;
}

inline void CNewtonCDS::SetScope(const CComplex& start, const CComplex& end)
{
	m_start = start;
	m_end = end;
}

inline void CNewtonCDS::SetRender(COLORREF color, float fFactor)
{
	m_color = color;
	m_fFactor = fFactor;
}

inline void CNewtonCDS::IterFormula(const CComplex& c1, CComplex& c2)
{
	CComplex cTemp = cpower(c1, N - 1);
	c2 = ((N - 1) * c1 * cTemp + C0) / (N * cTemp);
}

#endif
