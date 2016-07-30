#ifndef __GradientFill_h__
#define __GradientFill_h__

#include "Fill.h"

class CGradientFill : public CFill {
  protected:
	struct Span {
		int			 nStart;
		int			 nEnd;
		int			 nLength;
		int			 y;
	};

  public:
	CGradientFill();
	virtual ~CGradientFill();

  public:
	void			 SetOrigin(CPoint pt1, CPoint pt2);

  public:
	void			 InitFill();
	void			 FillLine(int x1, int x2, int y);

  private:
	void			 _LineVert(int x1, int x2, int y);
	void			 _LineLean(int x1, int x2, int y);
	void			 _LineBySprite(const Span& span, int nSrcPos);
	void			 _LineByPixel(const Span& span, int nSrcPos);

  private:
	CPoint			 m_xOrigin1;
	CPoint			 m_xOrigin2;
	int				 m_nLength;
	float			 m_db;
	float			 m_dg;
	float			 m_dr;
	CImage			*m_pGradient;
	CSprite			*m_pSprite;
};

inline void CGradientFill::SetOrigin(CPoint pt1, CPoint pt2)
{
	m_xOrigin1 = pt1;
	m_xOrigin2 = pt2;
}

#endif
