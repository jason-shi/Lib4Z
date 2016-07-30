#ifndef __MandelCDS_h__
#define __MandelCDS_h__

class CMandelCDS {
  public:
	CMandelCDS(int N); //z = power(z, N) + c
	virtual ~CMandelCDS();

  public:
	void			 SetIterArgs(int nMaxIter, float fEscDist);
	void			 SetScope(const CComplex& start, const CComplex& end);
	void			 SetRender(COLORREF color, float fFactor);
	void			 Draw(CCanvas *pCanvas);

  protected:
	virtual COLORREF GetColor(CComplex ret, int nIter);

  protected:
	int				 N;
	int				 m_nMaxIter;
	float			 m_fEscDist;
	CComplex		 m_start;
	CComplex		 m_end;
	COLORREF		 m_color;
	float			 m_fFactor; //渐变调节因子
};

inline void CMandelCDS::SetIterArgs(int nMaxIter, float fEscDist)
{
	m_nMaxIter = nMaxIter;
	m_fEscDist = fEscDist;
}

inline void CMandelCDS::SetScope(const CComplex& start, const CComplex& end)
{
	m_start = start;
	m_end = end;
}

inline void CMandelCDS::SetRender(COLORREF color, float fFactor)
{
	m_color = color;
	m_fFactor = fFactor;
}

#endif
