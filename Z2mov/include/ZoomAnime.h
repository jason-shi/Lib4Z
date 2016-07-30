#ifndef __ZoomAnime_h__
#define __ZoomAnime_h__

#include "Anime.h"

class CZoomAnime : public CAnime {
  public:
	CZoomAnime();
	CZoomAnime(float fRatioX, float fRatioY, int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd = NULL, int *pParam = NULL);
	virtual ~CZoomAnime();

  public:
	void			 SetRatio(float fRatioX, float fRatioY);

  protected:
	void			 SetUp();
	void			 Update(int nDelta);

  private:
	CRect			 m_xSrcRect;
	CPoint			 m_xSrcPoint; //Æðµã
	CPoint			 m_xDestPoint; //ÖÕµã
	float			 m_fRatioX;
	float			 m_fRatioY;

  private:
	float			 dx;
	float			 dy;
	float			 TmpRatioX;
	float			 TmpRatioY;
};

inline void CZoomAnime::SetRatio(float fRatioX, float fRatioY)
{
	if (IsActive())
		return;
	m_fRatioX = fRatioX;
	m_fRatioY = fRatioY;
}

#endif
