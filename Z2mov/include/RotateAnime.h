#ifndef __RotateAnime_h__
#define __RotateAnime_h__

#include "Anime.h"

class CRotateAnime : public CAnime {
  public:
	CRotateAnime();
	CRotateAnime(CPoint xFixPoint, float fAngle, int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd = NULL, int *pParam = NULL);
	virtual ~CRotateAnime();

  public:
	void			 SetFixPoint(CPoint xFixPoint);
	void			 SetAngle(float fAngle);

  protected:
	void			 SetUp();
	void			 Update(int nDelta);

  private:
	CPoint			 m_xFixPoint;
	float			 m_fAngle;
};

inline void CRotateAnime::SetFixPoint(CPoint xFixPoint)
{
	if (IsActive())
		return;
	m_xFixPoint = xFixPoint;
}

inline void CRotateAnime::SetAngle(float fAngle)
{
	if (IsActive())
		return;
	m_fAngle = fAngle;
}

#endif
