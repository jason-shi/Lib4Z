#ifndef __Curve_h__
#define __Curve_h__

class CCurve {
  public:
	CCurve();
	virtual ~CCurve();

  public:
	void			 SetColor(COLORREF color);

  public:
	virtual void	 Draw(CCanvas *pCanvas) = 0;

  protected:
	CImage			*m_pImage;
	COLORREF		 m_color;
};

inline void CCurve::SetColor(COLORREF color)
{
	m_color = color;
}

#endif
