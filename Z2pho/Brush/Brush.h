#ifndef __Brush_h__
#define __Brush_h__

class CBrush {
  public:
	CBrush();
	virtual ~CBrush();

  public:
	void			 SetDynamics(int nDynamics);
	void			 SetPressure(int nPressure);
	void			 SetColor(COLORREF color);
	void			 SetBkColor(COLORREF color);
	void			 SetPath(int nCount, LPPOINT lpPoints);

  protected:
	virtual void	 InitBrush() = 0;
	virtual void	 Draw(CCanvas *pCanvas) = 0;

  protected:
	int				 m_nDynamics;
	int				 m_nPressure;
	COLORREF		 m_color;
	COLORREF		 m_bkcolor;

  protected:
	int				 m_nCount;
	LPPOINT			 m_lpPoints;
};

inline void CBrush::SetDynamics(int nDynamics)
{
	m_nDynamics = nDynamics;
}

inline void CBrush::SetPressure(int nPressure)
{
	m_nPressure = nPressure;
}

inline void CBrush::SetColor(COLORREF color)
{
	m_color = color;
}

inline void CBrush::SetBkColor(COLORREF color)
{
	m_bkcolor = color;
}

inline void CBrush::SetPath(int nCount, LPPOINT lpPoints)
{
	m_nCount = nCount;
	m_lpPoints = lpPoints;
}

#endif
