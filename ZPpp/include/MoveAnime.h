#ifndef __MoveAnime_h__
#define __MoveAnime_h__

#include "Anime.h"

class CMoveAnime : public CAnime {
  public:
	CMoveAnime();
	CMoveAnime(CPoint xSrcPoint, CPoint xDestPoint, int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd = NULL, int *pParam = NULL);
	virtual ~CMoveAnime();

  public:
	void			 SetSrcPoint(CPoint point);
	void			 SetSrcPoint(int x, int y);
	void			 SetDestPoint(CPoint point);
	void			 SetDestPoint(int x, int y);

  protected:
	void			 SetUp();
	void			 Update(int nDelta);

  private:
	CPoint			 m_xSrcPoint; //Æðµã
	CPoint			 m_xDestPoint; //ÖÕµã

  private:
	float			 dx;
	float			 dy;
};

inline void CMoveAnime::SetSrcPoint(CPoint point)
{
	SetSrcPoint(point.x, point.y);
}

inline void CMoveAnime::SetDestPoint(CPoint point)
{
	SetDestPoint(point.x, point.y);
}

#endif
