#ifndef __Shape_h__
#define __Shape_h__

#define ROUND(x)	((int)((x) + 0.5))

class CFill;
class CShape {
  public:
	CShape();
	virtual ~CShape();

  public:
	void			 SetColor(COLORREF color);
	void			 SetBkColor(COLORREF color);
	void			 SetFill(CFill *pFill);

  public:
	void			 Draw(CCanvas *pCanvas);
	void			 Fill(CCanvas *pCanvas);

  protected:
	virtual void	 _Draw(CCanvas *pCanvas) = 0;
	virtual void	 _Fill(CCanvas *pCanvas) = 0;

  protected:
	void			 FillLine(int x1, int x2, int y);

  protected:
	CImage			*m_pImage;
	CFill			*m_pFill;
	COLORREF		 m_color;
	COLORREF		 m_bkcolor;
};

inline void CShape::SetColor(COLORREF color)
{
	m_color = color;
}

inline void CShape::SetBkColor(COLORREF color)
{
	m_bkcolor = color;
}

inline void CShape::SetFill(CFill *pFill)
{
	m_pFill = pFill;
}

#endif
