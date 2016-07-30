#ifndef __Fill_h__
#define __Fill_h__

class CCanvas;
class CFill {
  public:
	CFill();
	virtual ~CFill();

  public:
	void			 SetImage(CImage *pImage);
	void			 SetColor(COLORREF color);
	void			 SetBkColor(COLORREF color);

  public:
	virtual void	 InitFill() = 0;
	virtual void	 FillLine(int x1, int x2, int y) = 0; //…®√ËœﬂÃÓ≥‰

  protected:
	CImage			*m_pImage;
	COLORREF		 m_color;
	COLORREF		 m_bkcolor;
};

inline void CFill::SetImage(CImage *pImage)
{
	m_pImage = pImage;
}

inline void CFill::SetColor(COLORREF color)
{
	m_color = color;
}

inline void CFill::SetBkColor(COLORREF color)
{
	m_bkcolor = color;
}

#endif
