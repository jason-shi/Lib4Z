#ifndef __SolidFill_h__
#define __SolidFill_h__

#include "Fill.h"

class CSolidFill : public CFill {
	struct Span {
		int				 xLeft;
		int				 xRight;
		int				 y;
		int				 nUpDown;
	};

  public:
	CSolidFill();
	virtual ~CSolidFill();

  public:
	void			 InitFill();
	void			 FillLine(int x1, int x2, int y);

  public:
	void			 SetFlood(int x0, int y0);
	void			 FloodFill(CCanvas *pCanvas);

  private:
	void			 _PushSeed(int x0, int y0);
	void			 _PushSpan(int xLeft, int xRight, int y, int nUpDown);
	bool			 _ScanLine(int y, int nUpDown);

  private:
	int				 x0;
	int				 y0;
	COLORREF		 m_nOldColor;
	COLORREF		 m_nNewColor;
	Span			 m_span;
	std::vector<Span>	 m_stack;
};

inline void CSolidFill::SetFlood(int x0, int y0)
{
	this->x0 = x0;
	this->y0 = y0;
}

inline void CSolidFill::InitFill()
{
}

inline void CSolidFill::FillLine(int x1, int x2, int y)
{
	m_pImage->FillLine(x1, x2, y, m_color);
}

#endif
